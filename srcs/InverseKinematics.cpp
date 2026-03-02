#include "../include/LinkRobot.hpp"

static float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// 単点IK: ペン先位置(x,y) → (thetaL, thetaR)
// 5-bar機構 + ペン延長: 左腕 l1=60 + l2Eff=110, 右腕 l1=60 + l2=60
// Step 1: 左腕IKでペン先に到達 → thetaL
// Step 2: 合流点を計算（左肘からl2=60mm先）
// Step 3: 右腕IKで合流点に到達 → thetaR
bool LinkRobot::solveIKPoint(float x, float y, float& thetaL, float& thetaR) {
    // --- 左腕の計算 (原点(0,0)基準, l1 + l2Eff) ---
    float distL = std::sqrt(x * x + y * y);

    if (distL > (l1 + l2Eff) || distL < std::fabs(l1 - l2Eff)) {
        std::cout << "[IK] Left arm unreachable: dist=" << distL
                  << " target=(" << x << "," << y << ")" << std::endl;
        return false;
    }

    float cosAlphaL = (distL * distL + l1 * l1 - l2Eff * l2Eff) / (2.0f * l1 * distL);
    cosAlphaL = clampf(cosAlphaL, -1.0f, 1.0f);
    float alphaL = std::acos(cosAlphaL);
    float baseAngleL = std::atan2(y, x);
    thetaL = baseAngleL + alphaL;

    // --- 合流点の計算 ---
    float elbowLx = l1 * std::cos(thetaL);
    float elbowLy = l1 * std::sin(thetaL);
    float dx = x - elbowLx;
    float dy = y - elbowLy;
    float dist = std::sqrt(dx * dx + dy * dy);
    if (dist < 1e-6f) return false;
    float ux = dx / dist;
    float uy = dy / dist;
    float couplerX = elbowLx + l2 * ux;
    float couplerY = elbowLy + l2 * uy;

    // --- 右腕の計算 (右モータ(d,0)基準, l1 + l2, ターゲット=合流点) ---
    float xr = couplerX - d;
    float yr = couplerY;
    float distR = std::sqrt(xr * xr + yr * yr);

    if (distR > (l1 + l2) || distR < std::fabs(l1 - l2)) {
        std::cout << "[IK] Right arm unreachable: dist=" << distR
                  << " coupler=(" << couplerX << "," << couplerY << ")" << std::endl;
        return false;
    }

    float cosAlphaR = (distR * distR + l1 * l1 - l2 * l2) / (2.0f * l1 * distR);
    cosAlphaR = clampf(cosAlphaR, -1.0f, 1.0f);
    float alphaR = std::acos(cosAlphaR);
    float baseAngleR = std::atan2(yr, xr);
    thetaR = baseAngleR - alphaR;  // 肘を外側（右）に配置

    return true;
}

// バッチIK: point-array [[x,y],...] → angle-array [[thetaL_deg, thetaR_deg],...]
std::vector<std::vector<float>> LinkRobot::solveIK(const std::vector<std::vector<float>>& points) {
    std::vector<std::vector<float>> angles;
    constexpr float RAD2DEG = 180.0f / static_cast<float>(M_PI);

    for (const auto& pt : points) {
        float thetaL = 0, thetaR = 0;
        if (solveIKPoint(pt[0], pt[1], thetaL, thetaR)) {
            // ラジアン→度に変換
            float degL = thetaL * RAD2DEG;
            float degR = thetaR * RAD2DEG;
            // サーボ範囲にクランプ
            degL = clampf(degL, servoMin, servoMax);
            degR = clampf(degR, servoMin, servoMax);
            angles.push_back({degL, degR});
        } else {
            std::cout << "[IK] Skipping unreachable point ("
                      << pt[0] << "," << pt[1] << ")" << std::endl;
        }
    }
    return angles;
}
