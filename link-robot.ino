#include "include/LinkRobot.hpp"

LinkRobot robot;

void setup() {
  Serial.begin(9600);
  robot.begin();
  robot.InitPosition(120, 90);
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
//   pinMode(LED_BUILTIN, OUTPUT);
// }

// void loop() {
//   digitalWrite(LED_BUILTIN, HIGH);
//   delay(500);
//   digitalWrite(LED_BUILTIN, LOW);
//   delay(500);
// }