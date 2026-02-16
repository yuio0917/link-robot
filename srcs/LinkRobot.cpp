#include "../include/LinkRobot.hpp"

LinkRobot::LinkRobot()
    : CharHandle{&LinkRobot::getAPos, &LinkRobot::getBPos,
                 &LinkRobot::getCPos, &LinkRobot::getDPos,
                 &LinkRobot::getEPos, &LinkRobot::getCirclePos,
                 &LinkRobot::getCrossPos} {}

LinkRobot::~LinkRobot() {}

void LinkRobot::begin() {
    servoL.attach(_pinL);
    servoR.attach(_pinR);
    servoPen.attach(_pinZ);
    penUp();
    std::cout << "[Robot] Initialized" << std::endl;
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
    std::cout << "[Robot] Home position" << std::endl;
}

void LinkRobot::moveTo(float x, float y) {
    float thetaL = 0, thetaR = 0;
    if (solveIKPoint(x, y, thetaL, thetaR)) {
        constexpr float RAD2DEG = 180.0f / static_cast<float>(M_PI);
        int degL = static_cast<int>(thetaL * RAD2DEG);
        int degR = static_cast<int>(thetaR * RAD2DEG);
        servoL.write(degL);
        servoR.write(degR);
        delay(_delay);
        _currentX = x;
        _currentY = y;
    }
}

void LinkRobot::moveMotor(const std::vector<std::vector<float>>& angles) {
    for (const auto& a : angles) {
        servoL.write(static_cast<int>(a[0]));
        servoR.write(static_cast<int>(a[1]));
        delay(_delay);
    }
}

void LinkRobot::drawChar(const std::string& c) {
    std::vector<Stroke> strokes;
    TextCoords(c, strokes);

    std::cout << "[Robot] Drawing '" << c << "' (" << strokes.size() << " strokes)" << std::endl;

    for (const auto& stroke : strokes) {
        if (stroke.points.empty()) continue;

        // ストロークの最初の点へ移動（ペンアップ状態）
        if (!stroke.penDown) {
            penUp();
            // 移動ストロークの各点を通過
            auto angles = solveIK(stroke.points);
            moveMotor(angles);
        } else {
            // 描画ストロークの最初の点へペンアップで移動
            penUp();
            moveTo(stroke.points[0][0], stroke.points[0][1]);
            // ペンダウンして描画
            penDown();
            auto angles = solveIK(stroke.points);
            moveMotor(angles);
        }
    }
    penUp();

    // 現在位置を文字の右端に更新
    _currentX = startPosX + squareSize;
    _currentY = startPosY;
}

void LinkRobot::drawString(const std::string& str) {
    // 描画開始位置を保存
    float origX = startPosX;

    std::cout << "[Robot] Drawing string: " << str << std::endl;

    size_t i = 0;
    while (i < str.size()) {
        std::string ch;
        unsigned char c = static_cast<unsigned char>(str[i]);

        // UTF-8マルチバイト文字対応
        if (c < 0x80) {
            ch = str.substr(i, 1);
            i += 1;
        } else if ((c & 0xE0) == 0xC0) {
            ch = str.substr(i, 2);
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            ch = str.substr(i, 3);
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            ch = str.substr(i, 4);
            i += 4;
        } else {
            i += 1;
            continue;
        }

        drawChar(ch);

        // 次の文字の描画位置を更新
        startPosX += squareSize + charSpacing;
    }

    // 描画位置を元に戻す
    startPosX = origX;
    penUp();
    home();
}
