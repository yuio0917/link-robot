#include "include/LinkRobot.hpp"

void    LinkRobot::penUp(){
    servoZ.write(90);//停止
    delay(1000);
    servoZ.write(0);//上げ
    delay(1000);
    servoZ.write(90);//上げ状態を停止
}

void    LinkRobot::penDown(){
    servoZ.write(180);//下げ
    delay(1000);
    servoZ.write(90);//下げ状態を停止
}
