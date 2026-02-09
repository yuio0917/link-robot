#ifndef LINKROBOT_H
#define LINKROBOT_H

#include <Arduino.h>
#include <Servo.h>

class LinkRobot {
public:
    FiveBarRobot(int pinL, int pinR, int pinZ);

    void begin();               // 初期化
    void setSpeed(int delayMs); // スピード調整
    void home();                // ホームへ戻る
    void penUp();               // ペン上げ
    void penDown();             // ペン下げ
    void moveTo(float x, float y); // 指定座標へ移動
    void drawChar(char c);      // 1文字書く
    void drawString(String str); // 文字列を書く

private:
    Servo servoL, servoR, servoZ;
    int _pinL, _pinR, _pinZ;
    float _d, _l1, _l2, _ext_x, _ext_y;
    float _l_virt, _phi; 
    int _delay;
    float _current_x, _current_y;
    void solveIK(float x, float y, float &degL, float &degR);
};

#endif