#include "include/LinkRobot.hpp"

SerialMock Serial;

LinkRobot robot;

void setup() {
    Serial.begin(9600);
    robot.begin();
    robot.setSpeed(30);

    robot.penUp();
    robot.moveTo(30, 150);
}

void loop() {
    robot.drawString("ABC");
    delay(100);
}

int main() {
    setup();
    loop();
    return 0;
}
