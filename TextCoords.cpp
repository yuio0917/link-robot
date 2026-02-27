#include "include/LinkRobot.hpp"


void    LinkRobot::LinearInterpolation(float start_x, float start_y, float end_x, float end_y){
    //補完係数
    float t = 0.0f;
    float px;
    float py;
    
    for (int i = 1; i <= 15.0f; i++){
        t = i / 15.0f;
        px = (end_x - start_x) * t + start_x; //x座標の線形補完の計算
        py = (end_y - start_y) * t + start_y; //y座標の線形補完の計算
        solveIK(px, py);
    }
}

void    LinkRobot::getAPos(){
    //Position2の座標
    double pos2_x = start_pos_x + squareSize / 2;
    double pos2_y = start_pos_y + squareSize;

    //Position3の座標
    double pos3_x = start_pos_x + squareSize;
    double pos3_y = start_pos_y;

    //Position4の座標
    double pos4_x = (start_pos_x + pos2_x) / 2;
    double pos4_y = (start_pos_y + pos2_y) / 2;

    //Position5の座標
    double pos5_x = (pos2_x + pos3_x) / 2;
    double pos5_y = (pos2_y + pos3_y) / 2;
    
    //初期位置からPosition2までの線形補完
    LinearInterpolation(start_pos_x, start_pos_y, pos2_x, pos2_y);
    //Position2からPosition3までの線形補完
    LinearInterpolation(pos2_x, pos2_y, pos3_x, pos3_y);
    //Position3からPosition4までの線形補完
    LinearInterpolation(pos3_x, pos3_y, pos4_x, pos4_y);
    //Position4からPosition5までの線形補完
    LinearInterpolation(pos4_x, pos4_y, pos5_x, pos5_y);
}

void    LinkRobot::getBPos(){
    const double semiMajor = squareSize;  //楕円の長軸
    const double semiMinor = squareSize / 4;   //楕円の短軸
    
    //p2の座標
    double p2_x = start_pos_x;
    double p2_y = start_pos_y + squareSize;
    
    //上楕円の中心座標
    double cx_1 = start_pos_x;
    double cy_1 = (p2_y + (start_pos_y + p2_y) / 2) / 2;
    
    //下楕円の中心座標
    double cx_2 = start_pos_x;
    double cy_2 = (start_pos_y + (start_pos_y + p2_y) / 2) / 2;
    
    //p1からp2への線形補完
    LinearInterpolation(start_pos_x, start_pos_y, p2_x, p2_y);
    
    //上楕円の描画
    for (int i = 0; i < N; i++) {
        double theta = M_PI / 2.0 - (i / (N - 1.0)) * M_PI;  //pi/2から-pi/2への角度
        double px1 = cx_1 + semiMajor * cos(theta);
        double py1 = cy_1 + semiMinor * sin(theta);
        solveIK(px1, py1);
    }
    
    //下楕円の描画
    for (int i = 0; i < N; i++) {
        double theta = M_PI / 2.0 - (i / (N - 1.0)) * M_PI;  //pi/2から-pi/2への角度
        double px2 = cx_2 + semiMajor * cos(theta);
        double py2 = cy_2 + semiMinor * sin(theta);
        solveIK(px2, py2);
    }
}

void    LinkRobot::getCPos(){
    double cx = start_pos_x + (squareSize / 2); //Cを描く円の中心x座標
    double cy = start_pos_y + (squareSize / 2); //Cを描く円の中心y座標

    const double semiMajor = squareSize / 2;  //楕円の長軸
    const double semiMinor = squareSize / 2;   //楕円の短軸
    
    //π/4から7π/4までの角度で円弧を描画
    for (int i = 0; i < 20; i++) {
        double theta = M_PI / 4.0 + (i / (N - 1.0)) * (7.0 * M_PI / 4.0 - M_PI / 4.0);
        double px = cx + semiMajor * cos(theta);
        double py = cy + semiMinor * sin(theta);
        solveIK(px, py);
    }
}

void    LinkRobot::getDPos(){
    const double semiMajor = squareSize / 1.5;  //楕円の長軸
    const double semiMinor = squareSize / 2;   //楕円の短軸
    
    //p2の座標
    double p2_x = start_pos_x;
    double p2_y = start_pos_y + squareSize;
    
    //縦線を描画
    for (int i = 0; i < N; i++) {
        double px = start_pos_x + (i / (N - 1.0)) * (p2_x - start_pos_x);
        double py = start_pos_y + (i / (N - 1.0)) * (p2_y - start_pos_y);
        solveIK(px, py);
    }
    
    //楕円の中心座標
    double cx = start_pos_x;
    double cy = start_pos_y + (squareSize / 2);
    
    //右半円を描画
    for (int i = 0; i < N; i++) {
        double theta = M_PI / 2.0 - (i / (N - 1.0)) * M_PI;  //pi/2から-pi/2への角度
        double px = cx + semiMajor * cos(theta);
        double py = cy + semiMinor * sin(theta);
        solveIK(px, py);
    }
}

void    LinkRobot::getEPos(){
    //m1の座標
    double m1_x = start_pos_x + squareSize;
    double m1_y = start_pos_y + squareSize;
    
    //m2の座標
    double m2_x = m1_x - squareSize;
    double m2_y = m1_y;
    
    //m3の座標
    double m3_x = m1_x - squareSize;
    double m3_y = m1_y - (1.5 * squareSize);
    
    //m4の座標
    double m4_x = m1_x;
    double m4_y = m3_y;
    
    //m5の座標（中点）
    double m5_x = (m2_x + m3_x) / 2.0;
    double m5_y = (m2_y + m3_y) / 2.0;
    
    //m6の座標（中点）
    double m6_x = (m4_x + m1_x) / 2.0;
    double m6_y = (m4_y + m1_y) / 2.0;
    
    //m1からm2の線分
    for (int i = 0; i < N; i++) {
        double px = m1_x + (i / (N - 1.0)) * (m2_x - m1_x);
        double py = m1_y + (i / (N - 1.0)) * (m2_y - m1_y);
        solveIK(px, py);
    }
    
    //m2からm3の線分
    for (int i = 0; i < N; i++) {
        double px = m2_x + (i / (N - 1.0)) * (m3_x - m2_x);
        double py = m2_y + (i / (N - 1.0)) * (m3_y - m2_y);
        solveIK(px, py);
    }
    
    //m3からm4の線分
    for (int i = 0; i < N; i++) {
        double px = m3_x + (i / (N - 1.0)) * (m4_x - m3_x);
        double py = m3_y + (i / (N - 1.0)) * (m4_y - m3_y);
        solveIK(px, py);
    }
    
    //m5からm6の線分
    for (int i = 0; i < N; i++) {
        double px = m5_x + (i / (N - 1.0)) * (m6_x - m5_x);
        double py = m5_y + (i / (N - 1.0)) * (m6_y - m5_y);
        solveIK(px, py);
    }
}

void    LinkRobot::getCirclePos(){
    double xc = start_pos_x + (squareSize / 2);
    double yc = start_pos_y + (squareSize / 2);

    double semiMajor = 25;
    double semiMinor = 37.5;

    //各セグメントで円を描画
    for (int i = 0; i <= N; i++) {
        double theta1 = (2.0 * M_PI * i) / N;
        double theta2 = (2.0 * M_PI * (i + 1)) / N;
        double x1 = xc + semiMajor * cos(theta1);
        double y1 = yc + semiMinor * sin(theta1);
        double x2 = xc + semiMajor * cos(theta2);
        double y2 = yc + semiMinor * sin(theta2);

        LinearInterpolation(x1, y1, x2, y2);
    }
}

void    LinkRobot::getCrossPos(){
    //左上座標
    double x_left_top = start_pos_x;
    double y_left_top = start_pos_y + 50;
    
    //右下座標
    double x_right_bottom = start_pos_x + 30;
    double y_right_bottom = start_pos_y;
    
    //右上座標
    double x_right_top = start_pos_x + 30;
    double y_right_top = start_pos_y + 50;
    
    //左下座標
    double x_left_bottom = start_pos_x;
    double y_left_bottom = start_pos_y;
    
    LinearInterpolation(x_left_top, y_left_top, x_right_bottom, y_right_bottom);
    penUp();
    solveIK(x_right_top, y_right_top);
    delay(500);
    penDown();
    LinearInterpolation(x_right_top, y_right_top, x_left_bottom, y_left_bottom);
}

void    LinkRobot::drawChar(const char c){
    const char Characters[7] = {'A', 'B', 'C', 'D', 'E', 'O', 'X'};

    for (int i = 0; i < 7; i++){
        if (Characters[i] == c){
            (this->*CharHandle[i])();
            return ;
        }
    }
}