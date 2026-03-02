#!/usr/bin/env python3
"""
リンクロボット シミュレーション可視化
5バー平行リンク機構のmatplotlibアニメーション

物理機構:
  左モータ(0,0) ──60mm── 左肘 ──60mm── 合流点 ──50mm── ペン先
  右モータ(D,0) ──60mm── 右肘 ──60mm──┘
  ※ ペン先は左腕のコネクティングロッドの延長上
"""

import sys
import math
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# ── ロボット物理パラメータ ──────────────────────────
D = 50.0        # モータ間距離 (mm)
L1 = 135.0      # クランク長 (mm) — 両腕共通
L2 = 85.0       # コネクティングロッド長 (mm) — 両腕共通
PEN_EXT = 60.0  # ペン延長距離 (mm) — 左腕の合流点から

# 左腕の実効リンク長（クランク先端からペン先まで）
L2_EFF = L2 + PEN_EXT  # 110mm

# ── 描画パラメータ ──────────────────────────────────
N = 20              # 線形補間の分割数
SQUARE_SIZE = 28.0  # 文字サイズ
CHAR_SPACING = 10.0 # 文字間スペース
START_POS_X = 15.0  # 描画開始X
START_POS_Y = 165.0 # 描画開始Y

# ── IK ──────────────────────────────────────────────

def solve_ik_point(px, py):
    """ペン先位置 (px,py) → (thetaL, thetaR) ラジアン

    Step 1: 左腕IK — L1=60, L2_eff=110 で (0,0)→肘→ペン先
    Step 2: 合流点を計算（肘からペン方向に60mm）
    Step 3: 右腕IK — L1=60, L2=60 で (D,0)→肘→合流点
    """
    # ── 左腕 (原点(0,0)基準, L1=60, L2_eff=110) ──
    dist_l = math.sqrt(px * px + py * py)
    if dist_l > (L1 + L2_EFF) or dist_l < abs(L1 - L2_EFF):
        return None

    cos_alpha_l = (dist_l * dist_l + L1 * L1 - L2_EFF * L2_EFF) / (2.0 * L1 * dist_l)
    cos_alpha_l = max(-1.0, min(1.0, cos_alpha_l))
    alpha_l = math.acos(cos_alpha_l)
    base_angle_l = math.atan2(py, px)
    theta_l = base_angle_l + alpha_l  # 肘が外側（左）に張り出す

    # ── 合流点の計算 ──
    elbow_l = (L1 * math.cos(theta_l), L1 * math.sin(theta_l))
    # 肘→ペン方向の単位ベクトル
    dx = px - elbow_l[0]
    dy = py - elbow_l[1]
    dist = math.sqrt(dx * dx + dy * dy)
    if dist < 1e-6:
        return None
    ux, uy = dx / dist, dy / dist
    # 合流点 = 肘から60mm（ペンは110mm先だが合流点は60mm先）
    coupler = (elbow_l[0] + L2 * ux, elbow_l[1] + L2 * uy)

    # ── 右腕 (右モータ(D,0)基準, L1=60, L2=60, ターゲット=合流点) ──
    xr = coupler[0] - D
    yr = coupler[1]
    dist_r = math.sqrt(xr * xr + yr * yr)
    if dist_r > (L1 + L2) or dist_r < abs(L1 - L2):
        return None

    cos_alpha_r = (dist_r * dist_r + L1 * L1 - L2 * L2) / (2.0 * L1 * dist_r)
    cos_alpha_r = max(-1.0, min(1.0, cos_alpha_r))
    alpha_r = math.acos(cos_alpha_r)
    base_angle_r = math.atan2(yr, xr)
    theta_r = base_angle_r - alpha_r  # 肘が外側（右）に張り出す

    return (theta_l, theta_r)


def forward_kinematics(theta_l, theta_r):
    """(thetaL, thetaR) → 肘位置・合流点・ペン先

    Returns: (elbow_l, elbow_r, coupler, pen)
    """
    # 肘位置
    elbow_l = (L1 * math.cos(theta_l), L1 * math.sin(theta_l))
    elbow_r = (D + L1 * math.cos(theta_r), L1 * math.sin(theta_r))

    # 合流点: 左肘・右肘からそれぞれL2(60mm)の円の交点（上側）
    dx = elbow_r[0] - elbow_l[0]
    dy = elbow_r[1] - elbow_l[1]
    dist_e = math.sqrt(dx * dx + dy * dy)

    if dist_e < 1e-6:
        coupler = (elbow_l[0], elbow_l[1] + L2)
    elif dist_e > 2 * L2:
        # 到達不可: 中間点にフォールバック
        ux = dx / dist_e
        uy = dy / dist_e
        coupler = (elbow_l[0] + L2 * ux, elbow_l[1] + L2 * uy)
    else:
        a = (dist_e * dist_e) / (2.0 * dist_e)  # r1==r2 なので a = dist/2
        a = dist_e / 2.0
        h = math.sqrt(max(0, L2 * L2 - a * a))
        mx = elbow_l[0] + a * dx / dist_e
        my = elbow_l[1] + a * dy / dist_e
        coupler = (mx - h * dy / dist_e, my + h * dx / dist_e)

    # ペン先: 合流点から左肘→合流点方向にPEN_EXT延長
    dx_p = coupler[0] - elbow_l[0]
    dy_p = coupler[1] - elbow_l[1]
    dist_p = math.sqrt(dx_p * dx_p + dy_p * dy_p)
    if dist_p < 1e-6:
        pen = (coupler[0], coupler[1] + PEN_EXT)
    else:
        ux = dx_p / dist_p
        uy = dy_p / dist_p
        pen = (coupler[0] + PEN_EXT * ux, coupler[1] + PEN_EXT * uy)

    return elbow_l, elbow_r, coupler, pen


# ── 文字ストローク定義 ──────────────────────────────

def linear_interp(sx, sy, ex, ey, n):
    """2点間をn分割で線形補間"""
    pts = []
    for i in range(1, n + 1):
        t = i / n
        pts.append((sx + (ex - sx) * t, sy + (ey - sy) * t))
    return pts


def make_draw_stroke(normalized_pts, ox, oy, w, h, n):
    """正規化座標→ワールド座標変換 + 補間"""
    points = []
    first = normalized_pts[0]
    points.append((ox + first[0] * w, oy + first[1] * h))
    for i in range(len(normalized_pts) - 1):
        sx = ox + normalized_pts[i][0] * w
        sy = oy + normalized_pts[i][1] * h
        ex = ox + normalized_pts[i + 1][0] * w
        ey = oy + normalized_pts[i + 1][1] * h
        points.extend(linear_interp(sx, sy, ex, ey, n))
    return {"pen_down": True, "points": points}


def make_move_stroke(from_pt, to_pt):
    """ペンアップ移動ストローク"""
    return {"pen_down": False, "points": [from_pt, to_pt]}


def get_char_strokes(ch, ox, oy):
    """文字のストローク一覧を取得"""
    w = SQUARE_SIZE
    h = SQUARE_SIZE

    def draw(pts):
        return make_draw_stroke(pts, ox, oy, w, h, N)

    def move(last_norm, next_norm):
        return make_move_stroke(
            (ox + last_norm[0] * w, oy + last_norm[1] * h),
            (ox + next_norm[0] * w, oy + next_norm[1] * h),
        )

    if ch == "A":
        return [
            draw([(0,0), (0.5,1), (1,0)]),
            move((1,0), (0.25,0.5)),
            draw([(0.25,0.5), (0.75,0.5)]),
        ]
    elif ch == "B":
        return [
            draw([(0,0), (0,1), (0.8,1), (0.8,0.5), (0,0.5)]),
            move((0,0.5), (0,0.5)),
            draw([(0,0.5), (1,0.5), (1,0), (0,0)]),
        ]
    elif ch == "C":
        return [
            draw([(1,1), (0,1), (0,0), (1,0)]),
        ]
    elif ch == "D":
        return [
            draw([(0,0), (0,1), (0.7,1), (1,0.5), (0.7,0), (0,0)]),
        ]
    elif ch == "E":
        return [
            draw([(1,1), (0,1), (0,0), (1,0)]),
            move((1,0), (0,0.5)),
            draw([(0,0.5), (0.75,0.5)]),
        ]
    elif ch == "O":
        # 24点の円近似
        stroke = {"pen_down": True, "points": []}
        cx, cy, r = 0.5, 0.5, 0.5
        num_pts = 24
        for i in range(num_pts + 1):
            angle = 2.0 * math.pi * i / num_pts
            nx = cx + r * math.cos(angle)
            ny = cy + r * math.sin(angle)
            stroke["points"].append((ox + nx * w, oy + ny * h))
        return [stroke]
    elif ch == "X":
        return [
            draw([(0,1), (1,0)]),
            move((1,0), (0,0)),
            draw([(0,0), (1,1)]),
        ]
    else:
        print(f"[TextCoords] Unknown character: {ch}")
        return []


# ── 全フレームの事前計算 ──────────────────────────────

def build_frames(text):
    """
    描画テキストから全フレームのデータを生成
    各フレーム: (theta_l, theta_r, pen_down)
    ペン位置はFKで算出
    """
    frames = []
    pos_x = START_POS_X

    for ch in text:
        strokes = get_char_strokes(ch, pos_x, START_POS_Y)

        for stroke in strokes:
            if not stroke["points"]:
                continue

            if stroke["pen_down"]:
                # ペンアップで最初の点へ移動
                pt = stroke["points"][0]
                ik = solve_ik_point(pt[0], pt[1])
                if ik:
                    frames.append((ik[0], ik[1], False))

                # ペンダウンで描画
                for pt in stroke["points"]:
                    ik = solve_ik_point(pt[0], pt[1])
                    if ik:
                        frames.append((ik[0], ik[1], True))
            else:
                # 移動ストローク（ペンアップ）
                for pt in stroke["points"]:
                    ik = solve_ik_point(pt[0], pt[1])
                    if ik:
                        frames.append((ik[0], ik[1], False))

        pos_x += SQUARE_SIZE + CHAR_SPACING

    return frames


# ── アニメーション ────────────────────────────────────

def run_animation(text):
    frames = build_frames(text)
    if not frames:
        print("No frames to draw.")
        return

    fig, ax = plt.subplots(1, 1, figsize=(10, 8))
    ax.set_aspect("equal")
    ax.set_title(f"Link Robot: \"{text}\"")
    ax.set_xlabel("X (mm)")
    ax.set_ylabel("Y (mm)")

    # 描画範囲の計算（FK のペン位置から）
    all_pen = [forward_kinematics(f[0], f[1]) for f in frames]
    all_px = [p[3][0] for p in all_pen]
    all_py = [p[3][1] for p in all_pen]
    margin = 30
    x_min = min(min(all_px), 0) - margin
    x_max = max(max(all_px), D) + margin
    y_min = min(min(all_py), 0) - margin
    y_max = max(all_py) + margin
    ax.set_xlim(x_min, x_max)
    ax.set_ylim(y_min, y_max)

    # モーター位置 (固定点)
    ax.plot(0, 0, "ks", markersize=10, label="Motor L")
    ax.plot(D, 0, "ks", markersize=10, label="Motor R")

    # リンク描画（モーター→肘→合流点）
    link_left, = ax.plot([], [], "b-o", linewidth=2.5, markersize=5)
    link_right, = ax.plot([], [], "r-o", linewidth=2.5, markersize=5)
    # ペン延長線（合流点→ペン先）
    pen_ext, = ax.plot([], [], "g-", linewidth=2, alpha=0.8)
    pen_marker, = ax.plot([], [], "gv", markersize=10)

    # ペン軌跡
    current_trail_x = []
    current_trail_y = []
    trail_artists = []

    def init():
        link_left.set_data([], [])
        link_right.set_data([], [])
        pen_ext.set_data([], [])
        pen_marker.set_data([], [])
        return [link_left, link_right, pen_ext, pen_marker]

    def update(frame_idx):
        nonlocal current_trail_x, current_trail_y

        theta_l, theta_r, pen_down = frames[frame_idx]

        # FK で全ジョイント位置を計算
        elbow_l, elbow_r, coupler, pen_pos = forward_kinematics(theta_l, theta_r)
        px, py = pen_pos

        # リンク描画: モーター → 肘 → 合流点
        link_left.set_data([0, elbow_l[0], coupler[0]], [0, elbow_l[1], coupler[1]])
        link_right.set_data([D, elbow_r[0], coupler[0]], [0, elbow_r[1], coupler[1]])
        # ペン延長線: 合流点 → ペン先
        pen_ext.set_data([coupler[0], px], [coupler[1], py])
        pen_marker.set_data([px], [py])

        # ペン軌跡
        if pen_down:
            current_trail_x.append(px)
            current_trail_y.append(py)
        else:
            if len(current_trail_x) > 1:
                line, = ax.plot(current_trail_x, current_trail_y, "k-", linewidth=1.5)
                trail_artists.append(line)
            current_trail_x = []
            current_trail_y = []

        # 現在描画中の軌跡を一時表示
        artists = [link_left, link_right, pen_ext, pen_marker] + trail_artists
        if len(current_trail_x) > 1:
            if hasattr(update, "_temp_line") and update._temp_line is not None:
                update._temp_line.set_data(current_trail_x, current_trail_y)
            else:
                update._temp_line, = ax.plot(current_trail_x, current_trail_y, "k-", linewidth=1.5)
            artists.append(update._temp_line)
        elif hasattr(update, "_temp_line") and update._temp_line is not None:
            update._temp_line.set_data([], [])

        return artists

    update._temp_line = None

    # フレーム数に応じてインターバル調整
    interval = max(10, 3000 // len(frames))

    ani = animation.FuncAnimation(
        fig, update, frames=len(frames),
        init_func=init, interval=interval,
        blit=False, repeat=False,
    )

    ax.legend(loc="upper right")
    ax.grid(True, alpha=0.3)
    plt.tight_layout()
    plt.show()


# ── メイン ────────────────────────────────────────────

if __name__ == "__main__":
    text = sys.argv[1] if len(sys.argv) > 1 else "ABC"

    # サポートする文字チェック
    supported = set("ABCDEOX")
    for ch in text:
        if ch not in supported:
            print(f"Warning: '{ch}' is not supported. Supported: {', '.join(sorted(supported))}")

    run_animation(text)
