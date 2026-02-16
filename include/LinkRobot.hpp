#ifndef LINKROBOT_H
#define LINKROBOT_H

#include <Arduino.h>
#include <Servo.h>
#include <vector>
#include <string>
#include <cmath>
#include <iostream>

struct Stroke {
    std::vector<std::vector<float>> points; // [[x,y], [x,y], ...]
    bool penDown;
};

class LinkRobot {
private:
    // 文字座標取得用メンバ関数ポインタ
    typedef void (LinkRobot::*HandlePtr)(std::vector<Stroke>&);
    HandlePtr CharHandle[7];

    void getAPos(std::vector<Stroke>& strokes);
    void getBPos(std::vector<Stroke>& strokes);
    void getCPos(std::vector<Stroke>& strokes);
    void getDPos(std::vector<Stroke>& strokes);
    void getEPos(std::vector<Stroke>& strokes);
    void getCirclePos(std::vector<Stroke>& strokes);
    void getCrossPos(std::vector<Stroke>& strokes);
    void TextCoords(const std::string& c, std::vector<Stroke>& strokes);

    // 分割数・描画サイズ
    static constexpr int N = 20;
    static constexpr float squareSize = 20.0f;
    static constexpr float charSpacing = 10.0f;

    // ロボット物理パラメータ (MATLABシミュレーションと一致)
    static constexpr float d  = 60.0f;  // モータ間距離
    static constexpr float l1 = 80.0f;  // モータ側リンク長
    static constexpr float l2 = 100.0f; // ペン側リンク長

    // サーボ可動範囲 (度)
    static constexpr float servoMin = 10.0f;
    static constexpr float servoMax = 170.0f;

    // 描画初期位置
    float startPosX = 30.0f; // d/2 = 左右中央
    float startPosY = 120.0f;

    // サーボ・ピン
    Servo servoL, servoR, servoPen;
    int _pinL = 9, _pinR = 10, _pinZ = 11;

    int _delay = 30;
    float _currentX = 30.0f;
    float _currentY = 120.0f;

    // IK: 単点
    bool solveIKPoint(float x, float y, float& thetaL, float& thetaR);
    // IK: バッチ (point-array → angle-array)
    std::vector<std::vector<float>> solveIK(const std::vector<std::vector<float>>& points);

    void moveMotor(const std::vector<std::vector<float>>& angles);

public:
    LinkRobot();
    ~LinkRobot();

    void begin();
    void setSpeed(int delayMs);
    void home();
    void penUp();
    void penDown();
    void moveTo(float x, float y);
    void drawChar(const std::string& c);
    void drawString(const std::string& str);
};

#endif
