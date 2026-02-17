#include "include/LinkRobot.hpp"

LinkRobot robot;

void  InitPosition(){
  for (int i = 0; i <= 90; i += 10){
    servoL.write(i);
    servoR.write(i);
    delay(100);
  }
}

void setup() {
  Serial.begin(9600);
  robot.begin();
}

void loop() {
  InitPosition();
  getInputString();
  robot.drawString(str);
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
