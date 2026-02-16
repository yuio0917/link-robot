#include "../include/LinkRobot.hpp"

LinkRobot::LinkRobot() : CharHandle({&LinkRobot::getAPos, &LinkRobot::getBPos,
        &LinkRobot::getCPos, &LinkRobot::getDPos, &LinkRobot::getEPos,
        &LinkRobot::getCirclePos, &LinkRobot::getCrossPos}){}

LinkRobot::~LinkRobot(){}

//初期化
void LinkRobot::begin() {
    servoL.attach(13);
    // servoR.attach(_pinR);
    // servoZ.attach(_pinZ);
}

// void LinkRobot::setSpeed(int delayMs) {
//     _delay = delayMs;
// }

// void LinkRobot::penUp() {
//     servoPen.write(90); delay(100);
// }

// void LinkRobot::penDown() {
//     servoPen.write(0); delay(100);
// }

//制御
// void LinkRobot::moveTo(float x, float y) {

// }

void LinkRobot::moveMotor(std::vector<std::vector<double>> &angleVec) {
    for (int i = 0; i < angleVec[0].size(); i++){
        servoL.write((int)angleVec[0][i]);
        servoR.write((int)angleVec[1][i]);
        delay(100);
    }
}

//文字書き部分
void LinkRobot::drawChar(const std::string c) {
    TextCoords(c);
    solveIK(charVec);
    moveMotor(angleVec);
    // penUp();
}

void LinkRobot::drawString() {
    for (int i = 0; i < str.length(); i++) {
        drawChar(str[i]);
        moveTo(_current_x + 15, _current_y); 
    }
}