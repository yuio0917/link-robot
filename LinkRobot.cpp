#include "include/LinkRobot.hpp"

LinkRobot::LinkRobot() : CharHandle({&LinkRobot::drawA, &LinkRobot::drawB, &LinkRobot::drawC,
    &LinkRobot::drawD, &LinkRobot::drawE, &LinkRobot::drawCircle, &LinkRobot::drawCross}){}

LinkRobot::~LinkRobot(){}

void LinkRobot::begin() {
    servoL.attach(_pinL);
    servoR.attach(_pinR);
    servoZ.attach(_pinZ);
}

void   LinkRobot::InitPosition(const float x, const float y){
    penUp();
    start_pos_x = x;
    start_pos_y = y;
    solveIK(start_pos_x, start_pos_y);
}

void LinkRobot::moveTo() {
    penUp();
    start_pos_x += squareSize + gap;
}

void LinkRobot::moveMotor(const float &theta1, const float &theta2) {
    float degL = theta1 * (180.0f / (float)M_PI);
    float degR = theta2 * (180.0f / (float)M_PI);
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
