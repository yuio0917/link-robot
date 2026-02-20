#include "include/LinkRobot.hpp"

LinkRobot robot;

void setup() {
  Serial.begin(9600);
  robot.begin();
  robot.InitPosition(x, y);
}

void loop() {
  robot.drawString("A");
  delay(5000);
}

// #include <Servo.h>

// Servo servoL;
// Servo servoR;

// const int SERVO_L_PIN = 13;
// const int SERVO_R_PIN = 10;

// void setup() {
//   servoL.attach(SERVO_L_PIN);
//   servoR.attach(SERVO_R_PIN);
// }

// void loop() {
//   // 0 → 180度へゆっくり回転
//   for (int angle = 0; angle <= 180; angle++) {
//     servoL.write(angle);
//     servoR.write(angle);
//     delay(10);
//   }

//   delay(500);

//   // 180 → 0度へ戻す
//   for (int angle = 180; angle >= 0; angle--) {
//     servoL.write(angle);
//     servoR.write(angle);
//     delay(10);
//   }

//   delay(500);
// }
