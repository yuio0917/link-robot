#include "LinkRobot.h"

LinkRobot robot;

void setup() {
    Serial.begin(9600);
    robot.begin();
    robot.setSpeed(30);

    robot.penUp();
    robot.moveTo(30, 150);
}

void loop() {
    robot.drawString("AB");
    delay(100);
}
