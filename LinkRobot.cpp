#include "include/LinkRobot.hpp"

LinkRobot::LinkRobot() : CharHandle({&LinkRobot::getAPos, &LinkRobot::getBPos, &LinkRobot::getCPos,
    &LinkRobot::getDPos, &LinkRobot::getEPos, &LinkRobot::getCirclePos, &LinkRobot::getCrossPos}){}

LinkRobot::~LinkRobot(){}

//初期化
void LinkRobot::begin() {
    servoL.attach(_pinL);
    servoR.attach(_pinR);
    servoZ.attach(_pinZ);
    servoZ.write(90);
}

void   LinkRobot::InitPosition(const float x, const float y){
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
    for (int i = 0; i < str.length(); i++) {
        drawChar(str[i]);
        break;
        // moveTo(_current_x + 15, _current_y);
    }
}
