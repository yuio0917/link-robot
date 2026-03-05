#include "include/LinkRobot.hpp"

void    LinkRobot::penUp(){
    servoZ.write(89); //停止
    delay(1000);
    servoZ.write(93); //上げ方向に回転
    delay(1000);
    servoZ.write(89); //停止
}

void    LinkRobot::penDown(){
    servoZ.write(89); //停止
    delay(1000);
    servoZ.write(78); //下げ
    delay(1000);
    servoZ.write(89); //停止
}
