#include "include/LinkRobot.hpp"

LinkRobot::LinkRobot() : CharHandle({&LinkRobot::getAPos, &LinkRobot::getBPos, &LinkRobot::getCPos,
    &LinkRobot::getDPos, &LinkRobot::getEPos, &LinkRobot::getCirclePos, &LinkRobot::getCrossPos}){}

LinkRobot::~LinkRobot(){}

//初期化
void LinkRobot::begin() {
    servoL.attach(_pinL);
    servoR.attach(_pinR);
    servoZ.attach(_pinZ);
    servoZ.write(89);
}

void   LinkRobot::InitPosition(const float x, const float y){
    start_pos_x = x;
    start_pos_y = y;
    solveIK(start_pos_x, start_pos_y);
}

//制御
void LinkRobot::moveTo() {
    start_pos_x += squareSize + gap;
}

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
    if (str.size() == 1)
        drawChar(str[0]);
    else{
        for (int i = 0; i < str.length(); i++) {
            drawChar(str[i]);
            moveTo();
        }
    }
}
