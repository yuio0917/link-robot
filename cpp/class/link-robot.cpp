#include "LinkRobot.h"

LinkRobot::FLinkBarRobot(int pinL, int pinR, int pinZ) {
    _pinL = pinL;
    _pinR = pinR;
    _pinZ = pinZ;
    
    //リンク長設定
    _d  = 60.0;
    _l1 = 80.0;
    _l2 = 100.0;
    _ext_x = 40.0; 
    _ext_y = 0.0;

    _l_virt = sqrt(pow(_l2 + _ext_x, 2) + pow(_ext_y, 2));
    _phi = atan2(_ext_y, _l2 + _ext_x);
    
    _delay = 20;
}

//初期化
void LinkRobot::begin() {
    servoL.attach(_pinL);
    servoR.attach(_pinR);
    servoZ.attach(_pinZ);
    penUp();
}

void LinkRobot::setSpeed(int delayMs) {
    _delay = delayMs;
}

void LinkRobot::penUp() {
    servoZ.write(90); delay(100);
}

void LinkRobot::penDown() {
    servoZ.write(0); delay(100);
}

//制御
void LinkRobot::moveTo(float x, float y) {
    float degL, degR;
    solveIK(x, y, degL, degR);
    if (isnan(degL) || isnan(degR)) return;
    servoL.write((int)degL);
    servoR.write(180 - (int)degR);
    delay(_delay);
    
    _current_x = x;
    _current_y = y;
}

//逆運動学
void LinkRobot::solveIK(float x, float y, float &degL, float &degR) {
    //運動学をとくプログラムを書く
}

//文字書き部分
void FiveBarRobot::drawChar(char c) {
    // 文字の開始位置
    float startX = _current_x;
    float startY = _current_y;

    //文字の最大サイズを一括管理数変数

    switch(c) {
        case 'A':
            // それぞれの文字書きのロジックを記入
    }
    penUp();
}

void LinkRobot::drawString(String str) {
    for(int i=0; i<str.length(); i++) {
        drawChar(str[i]);
        // 次の文字を書くために位置をずらす
        moveTo(_current_x + 15, _current_y); 
    }
}