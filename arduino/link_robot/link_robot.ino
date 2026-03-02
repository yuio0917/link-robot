#include "LinkRobot.h"

LinkRobot robot;

void setup() {
    Serial.begin(9600);
    robot.begin();
    robot.setSpeed(30);

    robot.penUp();
    robot.moveTo(15, 165);
}

void loop() {
    robot.drawString("ABCDE");
    delay(100);
}
