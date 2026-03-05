#ifndef LINKROBOT_H
#define LINKROBOT_H

#include <Arduino.h>
#include <Servo.h>
#include <vector>  
#include <iostream>
#include <cmath>
#include <string>

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
        void    penUp();
        void    penDown();
        //マクロ(#define)ではなく定数として扱う
        
        static const int N = 20; //分割数
        static const int r = 25; //円の半径
        static const int squareSize = 50; //描画する文字範囲
        static constexpr float start_pos_x = 10; //描画する初期位置のx座標
        static constexpr float start_pos_y = 180; //描画する初期位置のy座標

        //各リンクの長さやjoinの座標
        double  joint1_x = 0;
        double  joint1_y = 0;
        double  joint2_x = l1;
        double  joint2_y = 0;
        
        static const int    l1 = 60;
        static const int    l2 = 135;
        static const int    l3 = 135;
        static const int    l4 = 85;
        static const int    l5 = 85;
        static const int    l6 = 50;

        static const int gap = 10; //文字間隔(mm)

        double   theta_1; //motor1の回転角度
        double   theta_2; //motor2の回転角度

        Servo servoL, servoR, servoZ;

        int _pinL = 9;
        int _pinR = 10;
        int _pinZ = 11;

        void    solveIK(const float x, const float y);
        void    moveMotor(const float theta1, const float theta2);
        void    LinearInterpolation(float start_x, float start_y, float end_x, float end_y);


    public:
        LinkRobot();
        ~LinkRobot();

        void begin();               // 初期化
        void moveTo(); // 指定座標へ移動
        void drawChar(const char c);      // 1文字書く
        void drawString(const std::string &str); // 文字列を書く
        void    InitPosition(const float x, const float y);
};

#endif
