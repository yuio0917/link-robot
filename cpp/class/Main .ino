#include "LinkRobot.h"

LinkRobot robot(9, 10, 11);

void setup() {
  Serial.begin(9600);
  robot.begin();
  robot.setSpeed(30);
  
  robot.penUp();
  robot.moveTo(30, 150);
}

void loop() {
  robot.drawString("AB");
  
  // 終了して待機
  while(true);
}