#include "LinkRobot.h"

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
    float distL = sqrt(x * x + y * y);

    // 到達可能性チェック
    if (distL > (l1 + l2) || distL < fabs(l1 - l2)) {
        Serial.print(F("[IK] Left arm unreachable: dist="));
        Serial.print(distL);
        Serial.print(F(" target=("));
        Serial.print(x);
        Serial.print(F(","));
        Serial.print(y);
        Serial.println(F(")"));
        return false;
    }

    // 余弦定理で内角を計算
    float cosAlphaL = (x * x + y * y + l1 * l1 - l2 * l2) / (2.0f * l1 * distL);
    cosAlphaL = clampf(cosAlphaL, -1.0f, 1.0f);
    float alphaL = acos(cosAlphaL);
    float baseAngleL = atan2(y, x);

    // 肘が外側（左側）に張り出す: +alpha
    thetaL = baseAngleL + alphaL;

    // --- 右腕の計算 (右モータ(d,0)基準) ---
    float xr = x - d;
    float yr = y;
    float distR = sqrt(xr * xr + yr * yr);

    if (distR > (l1 + l2) || distR < fabs(l1 - l2)) {
        Serial.print(F("[IK] Right arm unreachable: dist="));
        Serial.print(distR);
        Serial.print(F(" target=("));
        Serial.print(x);
        Serial.print(F(","));
        Serial.print(y);
        Serial.println(F(")"));
        return false;
    }

    float cosAlphaR = (xr * xr + yr * yr + l1 * l1 - l2 * l2) / (2.0f * l1 * distR);
    cosAlphaR = clampf(cosAlphaR, -1.0f, 1.0f);
    float alphaR = acos(cosAlphaR);
    float baseAngleR = atan2(yr, xr);

    // 肘が外側（右側）に張り出す: -alpha
    thetaR = baseAngleR - alphaR;

    return true;
}
