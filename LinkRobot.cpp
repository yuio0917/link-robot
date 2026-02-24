#include "include/LinkRobot.hpp"

LinkRobot::LinkRobot() : CharHandle({&LinkRobot::getAPos}){}

LinkRobot::~LinkRobot(){}

//初期化
void LinkRobot::begin() {
    servoL.attach(13);
    servoR.attach(10);
}

void   LinkRobot::InitPosition(const float x, const float y){
    solveIK(x, y);
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

void LinkRobot::moveMotor(const float theta1, const float theta2) {
    Serial.println(theta1);
    Serial.println(theta2);
    servoL.write((int)theta1);
    servoR.write((int)theta2);
    delay(500);
}

//文字書き部分
void LinkRobot::drawChar(const char c) {
    TextCoords(c);
    // penUp();
}

void LinkRobot::drawString(const std::string &str) {
    for (int i = 0; i < str.length(); i++) {
        drawChar(str[i]);
        break;
        // moveTo(_current_x + 15, _current_y);
    }
}
