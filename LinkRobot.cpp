#include "include/LinkRobot.hpp"

LinkRobot::LinkRobot() : CharHandle({&LinkRobot::getAPos, &LinkRobot::getBPos, &LinkRobot::getCPos,
    &LinkRobot::getDPos, &LinkRobot::getEPos, &LinkRobot::getCirclePos, &LinkRobot::getCrossPos}){}

LinkRobot::~LinkRobot(){}

//初期化
void LinkRobot::begin() {
    servoL.attach(_pinL);
    servoR.attach(_pinR);
    servoZ.attach(_pinZ);
    servoL.write(90);
    servoR.write(90);
    servoZ.write(90);
}

void   LinkRobot::InitPosition(const float x, const float y){
   start_pos_x = x;
   start_pos_y = y;
   solveIK(x, y);
}

//制御
// void LinkRobot::moveTo(float x, float y) {

// }

void LinkRobot::moveMotor(const float theta1, const float theta2) {
    float degL = theta1 * (180.0f / (float)M_PI);
    float degR = theta2 * (180.0f / (float)M_PI);
    Serial.println(degL);
    Serial.println(degR);
    servoL.write((int)degL);
    servoR.write((int)degR);
    delay(500);
}

void LinkRobot::drawString(const std::string &str) {
    static constexpr float gap = 5; //文字間の隙間(mm)
    for (int i = 0; i < str.length(); i++) {
        drawChar(str[i]);
        // 最後の文字以外は次の文字の開始位置へ移動
        if (i < (int)str.length() - 1) {
            penUp();
            start_pos_x += squareSize + gap;
            solveIK(start_pos_x, start_pos_y);
            penDown();
        }
    }
}
