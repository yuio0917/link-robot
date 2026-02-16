#include "../include/LinkRobot.hpp"

static float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// 単点IK: MATLABの solveIK.m と同じアルゴリズム
// 5-bar機構: 左腕(0,0)基準、右腕(d,0)基準
// 戻り値: true=到達可能, false=到達不可
bool LinkRobot::solveIKPoint(float x, float y, float& thetaL, float& thetaR) {
    // --- 左腕の計算 (原点(0,0)基準) ---
    float distL = std::sqrt(x * x + y * y);

    // 到達可能性チェック
    if (distL > (l1 + l2) || distL < std::fabs(l1 - l2)) {
        std::cout << "[IK] Left arm unreachable: dist=" << distL
                  << " target=(" << x << "," << y << ")" << std::endl;
        return false;
    }

    // 余弦定理で内角を計算
    float cosAlphaL = (x * x + y * y + l1 * l1 - l2 * l2) / (2.0f * l1 * distL);
    cosAlphaL = clampf(cosAlphaL, -1.0f, 1.0f);
    float alphaL = std::acos(cosAlphaL);
    float baseAngleL = std::atan2(y, x);

    // 肘が外側（左側）に張り出す: +alpha
    thetaL = baseAngleL + alphaL;

    // --- 右腕の計算 (右モータ(d,0)基準) ---
    float xr = x - d;
    float yr = y;
    float distR = std::sqrt(xr * xr + yr * yr);

    if (distR > (l1 + l2) || distR < std::fabs(l1 - l2)) {
        std::cout << "[IK] Right arm unreachable: dist=" << distR
                  << " target=(" << x << "," << y << ")" << std::endl;
        return false;
    }

    float cosAlphaR = (xr * xr + yr * yr + l1 * l1 - l2 * l2) / (2.0f * l1 * distR);
    cosAlphaR = clampf(cosAlphaR, -1.0f, 1.0f);
    float alphaR = std::acos(cosAlphaR);
    float baseAngleR = std::atan2(yr, xr);

    // 肘が外側（右側）に張り出す: -alpha
    thetaR = baseAngleR - alphaR;

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
