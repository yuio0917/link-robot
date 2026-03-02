#include "include/LinkRobot.hpp"

LinkRobot robot;

void setup(){
  Serial.begin(9600);
  robot.begin();
  robot.InitPosition(10, 180);
  robot.drawString("AB");
}

void loop() {
}
