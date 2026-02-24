#ifndef LINKROBOT_H
#define LINKROBOT_H

#include <Arduino.h>
#include <Servo.h>

class LinkRobot {
private:
    // 分割数・描画サイズ
    static constexpr int N = 20;
    static constexpr float squareSize = 20.0f;
    static constexpr float charSpacing = 10.0f;

    // ロボット物理パラメータ (MATLABシミュレーションと一致)
    static constexpr float d  = 60.0f;   // モータ間距離
    static constexpr float l1 = 80.0f;   // モータ側リンク長
    static constexpr float l2 = 100.0f;  // ペン側リンク長

    // サーボ可動範囲 (度)
    static constexpr float servoMin = 10.0f;
    static constexpr float servoMax = 170.0f;

    // 描画初期位置
    float startPosX;
    float startPosY;

    // サーボ・ピン
    Servo servoL, servoR, servoPen;
    int _pinL, _pinR, _pinZ;

    int _delay;
    float _currentX, _currentY;

    // IK: 単点
    bool solveIKPoint(float x, float y, float& thetaL, float& thetaR);

    // 2点間をN分割補間し、各点でIK→サーボ移動を即座に実行
    void drawSegment(float sx, float sy, float ex, float ey);

    // 正規化座標のポリラインを描画（ペン制御は呼び出し側で行う）
    void drawNormPolyline(const float pts[][2], int count,
                          float ox, float oy, float w, float h);

    // 各文字の描画
    void drawA();
    void drawB();
    void drawC();
    void drawD();
    void drawE();
    void drawF();
    void drawG();
    void drawH();
    void drawI();
    void drawJ();
    void drawK();
    void drawL();
    void drawM();
    void drawN();
    void drawCircle();  // O
    void drawP();
    void drawQ();
    void drawR();
    void drawS();
    void drawT();
    void drawU();
    void drawV();
    void drawW();
    void drawCross();   // X
    void drawY();
    void drawZ();

public:
    LinkRobot();

    void begin();
    void setSpeed(int delayMs);
    void home();
    void penUp();
    void penDown();
    void moveTo(float x, float y);
    void drawChar(char c);
    void drawString(const char* str);
};

#endif
