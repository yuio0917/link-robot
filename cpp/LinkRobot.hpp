#ifndef LINKROBOT_H
#define LINKROBOT_H

#include <Arduino.h>
#include <Servo.h>
#include <numbers>
#include <iostream>

class LinkRobot {
    private:
        //LinkRobotクラスのメンバ関数のアドレスを扱う関数ポインタの宣言
        typedef void (LinkRobot::*HandlePtr)();
        HandlePtr CharHandle[7];

        void    getAPos();
        void    getBPos();
        void    getCPos();
        void    getDPos();
        void    getEPos();
        void    getCirclePos();
        void    getCrossPos();
        void    TextCoords(char c);

        static const int N = 20; //分割数
        static const int squareSize = 20; //描画する文字範囲
        static const float start_pos_x = 40; //描画する初期位置のx座標
        static const float start_pos_y = 120; //描画する初期位置のy座標

        std::vector<std::vector<float>> charVec;

        Servo servoL, servoR, servoZ;
        int _pinL, _pinR, _pinZ;
        float _d, _l1, _l2, _ext_x, _ext_y;
        float _l_virt, _phi; 
        int _delay;
        float _current_x, _current_y;
        void solveIK(float x, float y, float &degL, float &degR);

    public:
        LinkRobot();
        ~LinkRobot();

        FiveBarRobot(int pinL, int pinR, int pinZ);

        void begin();               // 初期化
        void setSpeed(int delayMs); // スピード調整
        void home();                // ホームへ戻る
        void penUp();               // ペン上げ
        void penDown();             // ペン下げ
        void moveTo(float x, float y); // 指定座標へ移動
        void drawChar(char c);      // 1文字書く
        void drawString(); // 文字列を書く
};

#endif