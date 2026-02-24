#include "LinkRobot.h"

LinkRobot::LinkRobot()
    : startPosX(-20.0f), startPosY(120.0f),
      _pinL(9), _pinR(10), _pinZ(11),
      _delay(30), _currentX(-20.0f), _currentY(120.0f) {}

void LinkRobot::begin() {
    servoL.attach(_pinL);
    servoR.attach(_pinR);
    servoPen.attach(_pinZ);
    penUp();
    Serial.println(F("[Robot] Initialized"));
}

void LinkRobot::setSpeed(int delayMs) {
    _delay = delayMs;
}

void LinkRobot::penUp() {
    servoPen.write(90);
    delay(100);
}

void LinkRobot::penDown() {
    servoPen.write(0);
    delay(100);
}

void LinkRobot::home() {
    penUp();
    moveTo(d / 2.0f, 120.0f);
    Serial.println(F("[Robot] Home position"));
}

void LinkRobot::moveTo(float x, float y) {
    float thetaL = 0, thetaR = 0;
    if (solveIKPoint(x, y, thetaL, thetaR)) {
        float degL = thetaL * (180.0f / (float)M_PI);
        float degR = thetaR * (180.0f / (float)M_PI);
        degL = constrain(degL, servoMin, servoMax);
        degR = constrain(degR, servoMin, servoMax);
        servoL.write((int)degL);
        servoR.write((int)degR);
        delay(_delay);
        _currentX = x;
        _currentY = y;
    }
}

// 2点間をN分割補間し、各点でIK→サーボ移動を即座に実行（メモリ不使用）
void LinkRobot::drawSegment(float sx, float sy, float ex, float ey) {
    for (int i = 1; i <= N; i++) {
        float t = (float)i / (float)N;
        float px = (ex - sx) * t + sx;
        float py = (ey - sy) * t + sy;
        moveTo(px, py);
    }
}

// 正規化座標のポリラインを描画（連続するセグメントを逐次処理）
void LinkRobot::drawNormPolyline(const float pts[][2], int count,
                                  float ox, float oy, float w, float h) {
    for (int i = 0; i < count - 1; i++) {
        float sx = ox + pts[i][0] * w;
        float sy = oy + pts[i][1] * h;
        float ex = ox + pts[i + 1][0] * w;
        float ey = oy + pts[i + 1][1] * h;
        drawSegment(sx, sy, ex, ey);
    }
}

void LinkRobot::drawChar(char c) {
    Serial.print(F("[Robot] Drawing '"));
    Serial.print(c);
    Serial.println(F("'"));

    switch (c) {
        case 'A': drawA(); break;
        case 'B': drawB(); break;
        case 'C': drawC(); break;
        case 'D': drawD(); break;
        case 'E': drawE(); break;
        case 'F': drawF(); break;
        case 'G': drawG(); break;
        case 'H': drawH(); break;
        case 'I': drawI(); break;
        case 'J': drawJ(); break;
        case 'K': drawK(); break;
        case 'L': drawL(); break;
        case 'M': drawM(); break;
        case 'N': drawN(); break;
        case 'O': drawCircle(); break;
        case 'P': drawP(); break;
        case 'Q': drawQ(); break;
        case 'R': drawR(); break;
        case 'S': drawS(); break;
        case 'T': drawT(); break;
        case 'U': drawU(); break;
        case 'V': drawV(); break;
        case 'W': drawW(); break;
        case 'X': drawCross(); break;
        case 'Y': drawY(); break;
        case 'Z': drawZ(); break;
        default:
            Serial.print(F("[TextCoords] Unknown character: "));
            Serial.println(c);
            return;
    }

    penUp();
    _currentX = startPosX + squareSize;
    _currentY = startPosY;
}

void LinkRobot::drawString(const char* str) {
    float origX = startPosX;

    Serial.print(F("[Robot] Drawing string: "));
    Serial.println(str);

    for (int i = 0; str[i] != '\0'; i++) {
        drawChar(str[i]);
        startPosX += squareSize + charSpacing;
    }

    startPosX = origX;
    penUp();
    home();
}
