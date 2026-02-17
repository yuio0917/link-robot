#include "include/LinkRobot.hpp"

LinkRobot::LinkRobot() : CharHandle({&LinkRobot::getAPos}){}

LinkRobot::~LinkRobot(){}

//初期化
void LinkRobot::begin() {
    servoL.attach(13);
    servoR.attach(10);
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
        delay(500);
    }
}

//文字書き部分
void LinkRobot::drawChar(const char c) {
    TextCoords(c);
    solveIK(charVec);
    moveMotor(angleVec);
    // penUp();
}

void LinkRobot::drawString(const std::string &str) {
    servoR.write(30);
    servoL.write(30);
    delay(5000);

    for (int i = 0; i < str.length(); i++) {
        drawChar(str[i]);
        break;
        // moveTo(_current_x + 15, _current_y);
    }
}

void    LinkRobot::getInputString(){
    std::cout << "描く文字列を入力してください" << std::endl;
    std::getline(std::cin, _str);
}