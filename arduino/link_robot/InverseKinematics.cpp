#include "LinkRobot.h"

static float clampf(float v, float lo, float hi) {
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}

// 単点IK: ペン先位置(x,y) → (thetaL, thetaR)
// 5-bar機構 + ペン延長: 左腕 l1=135 + l2Eff=145, 右腕 l1=135 + l2=85
// Step 1: 左腕IKでペン先に到達 → thetaL
// Step 2: 合流点を計算（左肘からl2=85mm先）
// Step 3: 右腕IKで合流点に到達 → thetaR
bool LinkRobot::solveIKPoint(float x, float y, float& thetaL, float& thetaR) {
    // --- 左腕の計算 (原点(0,0)基準, l1 + l2Eff) ---
    float distL = sqrt(x * x + y * y);

    if (distL > (l1 + l2Eff) || distL < fabs(l1 - l2Eff)) {
        Serial.print(F("[IK] Left arm unreachable: dist="));
        Serial.print(distL);
        Serial.print(F(" target=("));
        Serial.print(x);
        Serial.print(F(","));
        Serial.print(y);
        Serial.println(F(")"));
        return false;
    }

    float cosAlphaL = (distL * distL + l1 * l1 - l2Eff * l2Eff) / (2.0f * l1 * distL);
    cosAlphaL = clampf(cosAlphaL, -1.0f, 1.0f);
    float alphaL = acos(cosAlphaL);
    float baseAngleL = atan2(y, x);
    thetaL = baseAngleL + alphaL;

    // --- 合流点の計算 ---
    float elbowLx = l1 * cos(thetaL);
    float elbowLy = l1 * sin(thetaL);
    float dx = x - elbowLx;
    float dy = y - elbowLy;
    float dist = sqrt(dx * dx + dy * dy);
    if (dist < 1e-6f) return false;
    float ux = dx / dist;
    float uy = dy / dist;
    // 合流点 = 左肘からl2(85mm)先
    float couplerX = elbowLx + l2 * ux;
    float couplerY = elbowLy + l2 * uy;

    // --- 右腕の計算 (右モータ(d,0)基準, l1 + l2, ターゲット=合流点) ---
    float xr = couplerX - d;
    float yr = couplerY;
    float distR = sqrt(xr * xr + yr * yr);

    if (distR > (l1 + l2) || distR < fabs(l1 - l2)) {
        Serial.print(F("[IK] Right arm unreachable: dist="));
        Serial.print(distR);
        Serial.print(F(" coupler=("));
        Serial.print(couplerX);
        Serial.print(F(","));
        Serial.print(couplerY);
        Serial.println(F(")"));
        return false;
    }

    float cosAlphaR = (distR * distR + l1 * l1 - l2 * l2) / (2.0f * l1 * distR);
    cosAlphaR = clampf(cosAlphaR, -1.0f, 1.0f);
    float alphaR = acos(cosAlphaR);
    float baseAngleR = atan2(yr, xr);
    thetaR = baseAngleR - alphaR;  // 肘を外側（右）に配置（安定配置）

    return true;
}
