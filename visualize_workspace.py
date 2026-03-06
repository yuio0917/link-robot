#!/usr/bin/env python3
"""
パターンBのリンク長でのペン先到達可能範囲（ワークスペース）を描画
サーボ可動範囲内でIKが解ける全ての(x,y)を可視化する
"""

import math
import numpy as np
import matplotlib.pyplot as plt

# ── パターンB パラメータ ──
D = 50.0        # モータ間距離 (mm)
L1 = 85.0      # クランク長 (mm)
L2 = 135.0       # コネクティングロッド長 (mm)
PEN_EXT = 60.0  # ペン延長距離 (mm)
L2_EFF = L2 + PEN_EXT  # 左腕実効リンク長 (145mm)

# サーボ可動範囲 (度)
SERVO_MIN = 30.0
SERVO_MAX = 150.0

# 描画パラメータ
SQUARE_SIZE = 28.0
CHAR_SPACING = 10.0
START_POS_X = 15.0
START_POS_Y = 165.0


def solve_ik(px, py):
    """ペン先位置 (px,py) → (thetaL_deg, thetaR_deg) or None"""
    # 左腕
    dist_l = math.sqrt(px * px + py * py)
    if dist_l > (L1 + L2_EFF) or dist_l < abs(L1 - L2_EFF):
        return None
    cos_a = (dist_l * dist_l + L1 * L1 - L2_EFF * L2_EFF) / (2.0 * L1 * dist_l)
    cos_a = max(-1.0, min(1.0, cos_a))
    theta_l = math.atan2(py, px) + math.acos(cos_a)

    # 合流点
    elbow_lx = L1 * math.cos(theta_l)
    elbow_ly = L1 * math.sin(theta_l)
    dx = px - elbow_lx
    dy = py - elbow_ly
    dist = math.sqrt(dx * dx + dy * dy)
    if dist < 1e-6:
        return None
    ux, uy = dx / dist, dy / dist
    coupler_x = elbow_lx + L2 * ux
    coupler_y = elbow_ly + L2 * uy

    # 右腕
    xr = coupler_x - D
    yr = coupler_y
    dist_r = math.sqrt(xr * xr + yr * yr)
    if dist_r > (L1 + L2) or dist_r < abs(L1 - L2):
        return None
    cos_a = (dist_r * dist_r + L1 * L1 - L2 * L2) / (2.0 * L1 * dist_r)
    cos_a = max(-1.0, min(1.0, cos_a))
    theta_r = math.atan2(yr, xr) - math.acos(cos_a)

    deg_l = math.degrees(theta_l)
    deg_r = math.degrees(theta_r)
    return deg_l, deg_r


def in_workspace(px, py):
    """(px, py)が到達可能かつサーボ範囲内か"""
    result = solve_ik(px, py)
    if result is None:
        return False
    deg_l, deg_r = result
    return (SERVO_MIN <= deg_l <= SERVO_MAX) and (SERVO_MIN <= deg_r <= SERVO_MAX)


def main():
    # スキャン範囲
    x_min, x_max = -100, 200
    y_min, y_max = 0, 300
    resolution = 1.0  # mm単位

    xs = np.arange(x_min, x_max, resolution)
    ys = np.arange(y_min, y_max, resolution)
    workspace = np.zeros((len(ys), len(xs)), dtype=bool)

    for iy, y in enumerate(ys):
        for ix, x in enumerate(xs):
            workspace[iy, ix] = in_workspace(x, y)

    fig, ax = plt.subplots(figsize=(10, 8))

    # ワークスペース描画
    ax.imshow(workspace, origin="lower", extent=[x_min, x_max, y_min, y_max],
              cmap="Blues", alpha=0.5, aspect="equal")

    # ワークスペースの輪郭
    ax.contour(xs, ys, workspace.astype(float), levels=[0.5],
               colors="steelblue", linewidths=2)

    # モータ位置
    ax.plot(0, 0, "ks", markersize=10)
    ax.plot(D, 0, "ks", markersize=10)
    ax.text(0, -10, "Motor L", ha="center", fontsize=9)
    ax.text(D, -10, "Motor R", ha="center", fontsize=9)

    # 文字描画範囲(ABCDE)を表示
    pos_x = START_POS_X
    for i, ch in enumerate("ABCDE"):
        rect = plt.Rectangle((pos_x, START_POS_Y), SQUARE_SIZE, SQUARE_SIZE,
                              linewidth=1.5, edgecolor="red", facecolor="none",
                              linestyle="--")
        ax.add_patch(rect)
        ax.text(pos_x + SQUARE_SIZE / 2, START_POS_Y + SQUARE_SIZE / 2, ch,
                ha="center", va="center", fontsize=10, color="red", fontweight="bold")
        pos_x += SQUARE_SIZE + CHAR_SPACING

    ax.set_title("Pattern B Workspace (d={}, l1={}, l2={}, pe={}, servo {}-{})".format(
        int(D), int(L1), int(L2), int(PEN_EXT), int(SERVO_MIN), int(SERVO_MAX)))
    ax.set_xlabel("X (mm)")
    ax.set_ylabel("Y (mm)")
    ax.grid(True, alpha=0.3)
    ax.set_xlim(x_min, x_max)
    ax.set_ylim(y_min, y_max)

    plt.tight_layout()
    plt.show()


if __name__ == "__main__":
    main()
