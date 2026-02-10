#include "../include/LinkRobot.h"

LinkRobot::LinkRobot() : CharHandle({&LinkRobot::getAPos, &LinkRobot::getBPos,
        &LinkRobot::getCPos, &LinkRobot::getDPos, &LinkRobot::getEPos,
        &LinkRobot::getCirclePos, &LinkRobot::getCrossPos}){}

LinkRobot::~LinkRobot(){}

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
    solveIK(x, y);
    if (isnan(degL) || isnan(degR)) return;
    servoL.write((int)degL);
    servoR.write(180 - (int)degR);
    delay(_delay);
    
    _current_x = x;
    _current_y = y;
}

//文字書き部分
void LinkRobot::drawChar(std::string c) {
    TextCoords(c);
    solveIK(&charVec);
    moveMotor(&angleVec);
    penUp();
}

void LinkRobot::drawString() {
    for (int i = 0; i < str.length(); i++) {
        drawChar(str[i]);
        moveTo(_current_x + 15, _current_y); 
    }
}