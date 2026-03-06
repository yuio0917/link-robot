#!/usr/bin/env python3
"""
リンクロボット リンク長比較シミュレーション
2パターンを左右に並べて表示
"""

import math
import matplotlib.pyplot as plt
import matplotlib.animation as animation

N = 20
CHAR_SPACING = 10.0

# ── パターン定義 ──
CONFIGS = {
    "5chars_22mm_servo40_140": {
        "label": "A: 5文字 22mm servo40-140 (d=40, l1=135, l2=75, pe=55)",
        "D": 50.0, "L1": 85.0, "L2": 135.0, "PEN_EXT": 55.0,
        "SQUARE_SIZE": 22.0,
        "START_POS_X": 10.0, "START_POS_Y": 170.0,
        "text": "ABCDE",
    },
    "5chars_28mm_servo30_150": {
        "label": "B: 5文字 28mm servo30-150 (d=50, l1=135, l2=85, pe=60)",
        "D": 50.0, "L1": 135.0, "L2": 85.0, "PEN_EXT": 60.0,
        "SQUARE_SIZE": 28.0,
        "START_POS_X": 15.0, "START_POS_Y": 165.0,
        "text": "ABCDE",
    },
}

# ── 文字ストローク定義 ──
def get_char_strokes(ch, ox, oy, sq):
    w = h = sq

    def linear_interp(sx, sy, ex, ey):
        pts = []
        for i in range(1, N + 1):
            t = i / N
            pts.append((sx + (ex - sx) * t, sy + (ey - sy) * t))
        return pts

    def draw(norm_pts):
        points = [(ox + norm_pts[0][0] * w, oy + norm_pts[0][1] * h)]
        for i in range(len(norm_pts) - 1):
            sx = ox + norm_pts[i][0] * w;   sy = oy + norm_pts[i][1] * h
            ex = ox + norm_pts[i+1][0] * w; ey = oy + norm_pts[i+1][1] * h
            points.extend(linear_interp(sx, sy, ex, ey))
        return {"pen_down": True, "points": points}

    def move(a, b):
        return {"pen_down": False, "points": [
            (ox + a[0]*w, oy + a[1]*h), (ox + b[0]*w, oy + b[1]*h)]}

    if ch == "A":
        return [draw([(0,0),(0.5,1),(1,0)]), move((1,0),(0.25,0.5)),
                draw([(0.25,0.5),(0.75,0.5)])]
    elif ch == "B":
        return [draw([(0,0),(0,1),(0.8,1),(0.8,0.5),(0,0.5)]),
                move((0,0.5),(0,0.5)),
                draw([(0,0.5),(1,0.5),(1,0),(0,0)])]
    elif ch == "C":
        return [draw([(1,1),(0,1),(0,0),(1,0)])]
    elif ch == "D":
        return [draw([(0,0),(0,1),(0.7,1),(1,0.5),(0.7,0),(0,0)])]
    elif ch == "E":
        return [draw([(1,1),(0,1),(0,0),(1,0)]), move((1,0),(0,0.5)),
                draw([(0,0.5),(0.75,0.5)])]
    elif ch == "O":
        stroke = {"pen_down": True, "points": []}
        for i in range(25):
            a = 2.0 * math.pi * i / 24
            stroke["points"].append((ox+(0.5+0.5*math.cos(a))*w, oy+(0.5+0.5*math.sin(a))*h))
        return [stroke]
    elif ch == "X":
        return [draw([(0,1),(1,0)]), move((1,0),(0,0)), draw([(0,0),(1,1)])]
    return []


# ── IK / FK ──
def solve_ik_point(px, py, D, L1, L2, PEN_EXT):
    L2_EFF = L2 + PEN_EXT
    dist_l = math.sqrt(px*px + py*py)
    if dist_l > (L1+L2_EFF) or dist_l < abs(L1-L2_EFF): return None
    cos_a = (dist_l*dist_l + L1*L1 - L2_EFF*L2_EFF) / (2*L1*dist_l)
    cos_a = max(-1, min(1, cos_a))
    theta_l = math.atan2(py, px) + math.acos(cos_a)
    elbow_l = (L1*math.cos(theta_l), L1*math.sin(theta_l))
    dx = px-elbow_l[0]; dy = py-elbow_l[1]
    dist = math.sqrt(dx*dx+dy*dy)
    if dist < 1e-6: return None
    ux, uy = dx/dist, dy/dist
    coupler = (elbow_l[0]+L2*ux, elbow_l[1]+L2*uy)
    xr = coupler[0]-D; yr = coupler[1]
    dist_r = math.sqrt(xr*xr+yr*yr)
    if dist_r > (L1+L2) or dist_r < abs(L1-L2): return None
    cos_a = (dist_r*dist_r+L1*L1-L2*L2) / (2*L1*dist_r)
    cos_a = max(-1, min(1, cos_a))
    theta_r = math.atan2(yr, xr) - math.acos(cos_a)
    return (theta_l, theta_r)


def forward_kinematics(theta_l, theta_r, D, L1, L2, PEN_EXT):
    elbow_l = (L1*math.cos(theta_l), L1*math.sin(theta_l))
    elbow_r = (D + L1*math.cos(theta_r), L1*math.sin(theta_r))
    dx = elbow_r[0]-elbow_l[0]; dy = elbow_r[1]-elbow_l[1]
    dist_e = math.sqrt(dx*dx+dy*dy)
    if dist_e < 1e-6:
        coupler = (elbow_l[0], elbow_l[1]+L2)
    elif dist_e > 2*L2:
        ux = dx/dist_e; uy = dy/dist_e
        coupler = (elbow_l[0]+L2*ux, elbow_l[1]+L2*uy)
    else:
        a = dist_e/2.0
        h = math.sqrt(max(0, L2*L2 - a*a))
        mx = elbow_l[0] + a*dx/dist_e
        my = elbow_l[1] + a*dy/dist_e
        coupler = (mx - h*dy/dist_e, my + h*dx/dist_e)
    dx_p = coupler[0]-elbow_l[0]; dy_p = coupler[1]-elbow_l[1]
    dist_p = math.sqrt(dx_p*dx_p+dy_p*dy_p)
    if dist_p < 1e-6:
        pen = (coupler[0], coupler[1]+PEN_EXT)
    else:
        ux = dx_p/dist_p; uy = dy_p/dist_p
        pen = (coupler[0]+PEN_EXT*ux, coupler[1]+PEN_EXT*uy)
    return elbow_l, elbow_r, coupler, pen


def build_frames(cfg):
    D = cfg["D"]; L1 = cfg["L1"]; L2 = cfg["L2"]; PE = cfg["PEN_EXT"]
    sq = cfg["SQUARE_SIZE"]; spx = cfg["START_POS_X"]; spy = cfg["START_POS_Y"]
    frames = []
    pos_x = spx
    for ch in cfg["text"]:
        strokes = get_char_strokes(ch, pos_x, spy, sq)
        for stroke in strokes:
            if not stroke["points"]: continue
            if stroke["pen_down"]:
                pt = stroke["points"][0]
                ik = solve_ik_point(pt[0], pt[1], D, L1, L2, PE)
                if ik: frames.append((ik[0], ik[1], False))
                for pt in stroke["points"]:
                    ik = solve_ik_point(pt[0], pt[1], D, L1, L2, PE)
                    if ik: frames.append((ik[0], ik[1], True))
            else:
                for pt in stroke["points"]:
                    ik = solve_ik_point(pt[0], pt[1], D, L1, L2, PE)
                    if ik: frames.append((ik[0], ik[1], False))
        pos_x += sq + CHAR_SPACING
    return frames


# ── メイン: 2パターン並列表示 ──
def run_compare():
    cfgs = list(CONFIGS.values())
    all_frames = [build_frames(c) for c in cfgs]
    max_len = max(len(f) for f in all_frames)

    fig, axes = plt.subplots(1, 2, figsize=(16, 8))
    fig.suptitle("Link Robot: Parameter Comparison", fontsize=14)

    artists_list = []
    trail_data = []

    for i, (cfg, frames, ax) in enumerate(zip(cfgs, all_frames, axes)):
        D = cfg["D"]; L1 = cfg["L1"]; L2 = cfg["L2"]; PE = cfg["PEN_EXT"]
        ax.set_aspect("equal")
        ax.set_title(cfg["label"], fontsize=11)
        ax.set_xlabel("X (mm)")
        ax.set_ylabel("Y (mm)")

        # 描画範囲
        all_pen = [forward_kinematics(f[0], f[1], D, L1, L2, PE) for f in frames]
        all_px = [p[3][0] for p in all_pen]
        all_py = [p[3][1] for p in all_pen]
        margin = 30
        ax.set_xlim(min(min(all_px), 0) - margin, max(max(all_px), D) + margin)
        ax.set_ylim(min(min(all_py), 0) - margin, max(all_py) + margin)

        # ホワイトボード領域を表示
        board = plt.Rectangle((0, 0), 300, 200, linewidth=2,
                               edgecolor='gray', facecolor='lightyellow', alpha=0.3)
        ax.add_patch(board)

        ax.plot(0, 0, "ks", markersize=10)
        ax.plot(D, 0, "ks", markersize=10)
        ax.text(0, -8, "Motor L", ha="center", fontsize=8)
        ax.text(D, -8, "Motor R", ha="center", fontsize=8)

        link_l, = ax.plot([], [], "b-o", linewidth=2.5, markersize=5)
        link_r, = ax.plot([], [], "r-o", linewidth=2.5, markersize=5)
        pen_line, = ax.plot([], [], "g-", linewidth=2, alpha=0.8)
        pen_mk, = ax.plot([], [], "gv", markersize=10)

        artists_list.append({"link_l": link_l, "link_r": link_r,
                             "pen_line": pen_line, "pen_mk": pen_mk,
                             "ax": ax, "trail_artists": [], "temp_line": None})
        trail_data.append({"x": [], "y": []})

        ax.grid(True, alpha=0.3)

    def update(frame_idx):
        result = []
        for i, (cfg, frames) in enumerate(zip(cfgs, all_frames)):
            D = cfg["D"]; L1 = cfg["L1"]; L2 = cfg["L2"]; PE = cfg["PEN_EXT"]
            a = artists_list[i]; td = trail_data[i]

            if frame_idx >= len(frames):
                # アニメーション終了済み
                result.extend([a["link_l"], a["link_r"], a["pen_line"], a["pen_mk"]])
                continue

            tl, tr, pen_down = frames[frame_idx]
            el, er, cp, pen = forward_kinematics(tl, tr, D, L1, L2, PE)

            a["link_l"].set_data([0, el[0], cp[0]], [0, el[1], cp[1]])
            a["link_r"].set_data([D, er[0], cp[0]], [0, er[1], cp[1]])
            a["pen_line"].set_data([cp[0], pen[0]], [cp[1], pen[1]])
            a["pen_mk"].set_data([pen[0]], [pen[1]])

            if pen_down:
                td["x"].append(pen[0]); td["y"].append(pen[1])
            else:
                if len(td["x"]) > 1:
                    line, = a["ax"].plot(td["x"], td["y"], "k-", linewidth=1.5)
                    a["trail_artists"].append(line)
                td["x"] = []; td["y"] = []

            if len(td["x"]) > 1:
                if a["temp_line"] is None:
                    a["temp_line"], = a["ax"].plot(td["x"], td["y"], "k-", linewidth=1.5)
                else:
                    a["temp_line"].set_data(td["x"], td["y"])
            elif a["temp_line"] is not None:
                a["temp_line"].set_data([], [])

            result.extend([a["link_l"], a["link_r"], a["pen_line"], a["pen_mk"]])
        return result

    interval = max(10, 3000 // max_len)
    ani = animation.FuncAnimation(fig, update, frames=max_len,
                                  interval=interval, blit=False, repeat=False)
    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    run_compare()
