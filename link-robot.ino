#include "include/LinkRobot.hpp"

LinkRobot robot;

void setup(){
  Serial.begin(9600);
  robot.begin();
  robot.InitPosition(10, 180);
}

void loop() {
  if (Serial.available()){
    Serial.println("文字列を入力してください．");
    String arduinoStr = Serial.readString(); //改行含む
    arduinoStr.trim(); //改行や空白を除去
    std::string str = arduinoStr.c_str(); //arduinoのString型をstd::string型に変更．
    robot.drawString(str);
  }
}
