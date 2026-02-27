#ifndef LINKROBOT_H
#define LINKROBOT_H

#include <Arduino.h>
#include <Servo.h>
#include <vector>  
#include <iostream>
#include <cmath>

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
        void    TextCoords(const char c);

        //マクロ(#define)ではなく定数として扱う
        
        static const int N = 20; //分割数
        static const int r = 25; //円の半径
        static const int squareSize = 50; //描画する文字範囲
        static constexpr float start_pos_x = 10; //描画する初期位置のx座標
        static constexpr float start_pos_y = 180; //描画する初期位置のy座標

        //std::vector<std::vector<float>> charVec; //描画する文字のx, y座標情報を格納
        std::vector<std::vector<double>> angleVec; //motor1とmotor2の角度情報を格納

        // float   dist_joint1_to_pen; //joint1からペン先までの距離
        // float   dist_joint2_to_pen; //joint2からペン先までの距離

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

        double   theta_1; //motor1の回転角度
        double   theta_2; //motor2の回転角度

        std::string _str; //ユーザーから入力される文字列を格納する変数

        Servo servoL, servoR, servoZ, servoPen;

        int _pinL = 9;
        int _pinR = 10;
        // int _pinZ;

        float _d, _l1, _l2, _ext_x, _ext_y;
        float _l_virt, _phi; 
        int _delay;
        float _current_x, _current_y;
        void    solveIK(const float x, const float y);
        void    moveMotor(const float theta1, const float theta2);
        void    LinearInterpolation(float start_x, float start_y, float end_x, float end_y);

    public:
        LinkRobot();
        ~LinkRobot();

        void begin();               // 初期化
        // void setSpeed(int delayMs); // スピード調整
        // void home();                // ホームへ戻る
        // void penUp();               // ペン上げ
        // void penDown();             // ペン下げ
        // void moveTo(float x, float y); // 指定座標へ移動
        void drawChar(const char c);      // 1文字書く
        void drawString(const std::string &str); // 文字列を書く
        void    InitPosition(const float x, const float y);
};

#endif