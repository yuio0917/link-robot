#include "include/LinkRobot.hpp"

LinkRobot::LinkRobot() : CharHandle({&LinkRobot::getAPos}){}

LinkRobot::~LinkRobot(){}

//初期化
void LinkRobot::begin() {
<<<<<<< HEAD
    servoL.attach(9);
    servoR.attach(10);
}

void   LinkRobot::InitPosition(const float x, const float y){
    solveIK(x, y);
=======
    servoL.attach(_pinL);
    servoR.attach(_pinR);
    // servoZ.attach(_pinZ);
>>>>>>> main
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
    float degL = theta1 * (180.0f / (float)M_PI);
    float degR = theta2 * (180.0f / (float)M_PI);
    Serial.println(degL);
    Serial.println(degR);
    servoL.write((int)degL);
    servoR.write((int)degR);
    delay(500);
}

//文字書き部分
void LinkRobot::drawChar(const char c) {
    TextCoords(c);
    // penUp();
}

void LinkRobot::drawString(const std::string &str) {
<<<<<<< HEAD
=======
    // servoR.write(30);
    // servoL.write(30);
    // delay(5000);

>>>>>>> main
    for (int i = 0; i < str.length(); i++) {
        drawChar(str[i]);
        break;
        // moveTo(_current_x + 15, _current_y);
    }
}
