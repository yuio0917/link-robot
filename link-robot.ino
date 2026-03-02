#include "include/LinkRobot.hpp"

LinkRobot robot;

void setup(){
  Serial.begin(9600);
  robot.begin();
}

void loop() {
  robot.InitPosition(10, 180);
  robot.drawString("ACE");
  delay(5000);
}
