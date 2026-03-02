// サーボモータ初期化プログラム
// 全サーボを90°にセットして0点合わせを行う
#include <Servo.h>

Servo servoL, servoR, servoZ;

const int pinL = 9;
const int pinR = 10;
const int pinZ = 11;

void setup() {
    Serial.begin(9600);
    servoL.attach(pinL);
    servoR.attach(pinR);
    servoZ.attach(pinZ);

    servoL.write(90);
    servoR.write(90);
    servoZ.write(90);

    Serial.println("=== Servo Init ===");
    Serial.println("All servos set to 90 deg");
}

void loop() {
}
