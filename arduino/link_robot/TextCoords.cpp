#include "LinkRobot.h"

// A: 2ストローク
void LinkRobot::drawA() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // S1: (0,0)→(0.5,1)→(1,0) 左脚→頂点→右脚
    const float s1[][2] = {{0, 0}, {0.5f, 1}, {1, 0}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 3, ox, oy, w, h);

    // ペンアップで横棒の開始点へ移動
    penUp();
    moveTo(ox + 0.25f * w, oy + 0.5f * h);

    // S2: (0.25,0.5)→(0.75,0.5) 横棒
    const float s2[][2] = {{0.25f, 0.5f}, {0.75f, 0.5f}};
    penDown();
    drawNormPolyline(s2, 2, ox, oy, w, h);
}

// B: 2ストローク
void LinkRobot::drawB() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // S1: (0,0)→(0,1)→(0.8,1)→(0.8,0.5)→(0,0.5) 上半分
    const float s1[][2] = {{0, 0}, {0, 1}, {0.8f, 1}, {0.8f, 0.5f}, {0, 0.5f}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 5, ox, oy, w, h);

    // ペンアップ移動
    penUp();
    moveTo(ox + 0.0f * w, oy + 0.5f * h);

    // S2: (0,0.5)→(1,0.5)→(1,0)→(0,0) 下半分
    const float s2[][2] = {{0, 0.5f}, {1, 0.5f}, {1, 0}, {0, 0}};
    penDown();
    drawNormPolyline(s2, 4, ox, oy, w, h);
}

// C: 1ストローク
void LinkRobot::drawC() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // (1,1)→(0,1)→(0,0)→(1,0)
    const float s1[][2] = {{1, 1}, {0, 1}, {0, 0}, {1, 0}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 4, ox, oy, w, h);
}

// D: 1ストローク
void LinkRobot::drawD() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // (0,0)→(0,1)→(0.7,1)→(1,0.5)→(0.7,0)→(0,0)
    const float s1[][2] = {{0, 0}, {0, 1}, {0.7f, 1}, {1, 0.5f}, {0.7f, 0}, {0, 0}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 6, ox, oy, w, h);
}

// E: 2ストローク
void LinkRobot::drawE() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // S1: (1,1)→(0,1)→(0,0)→(1,0) コの字
    const float s1[][2] = {{1, 1}, {0, 1}, {0, 0}, {1, 0}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 4, ox, oy, w, h);

    // ペンアップで中央横棒の開始点へ移動
    penUp();
    moveTo(ox + 0.0f * w, oy + 0.5f * h);

    // S2: (0,0.5)→(0.75,0.5) 中央横棒
    const float s2[][2] = {{0, 0.5f}, {0.75f, 0.5f}};
    penDown();
    drawNormPolyline(s2, 2, ox, oy, w, h);
}

// F: 2ストローク
void LinkRobot::drawF() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // S1: (1,1)→(0,1)→(0,0) 上辺→左縦線
    const float s1[][2] = {{1, 1}, {0, 1}, {0, 0}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 3, ox, oy, w, h);

    // S2: (0,0.5)→(0.75,0.5) 中央横棒
    penUp();
    moveTo(ox + 0.0f * w, oy + 0.5f * h);
    const float s2[][2] = {{0, 0.5f}, {0.75f, 0.5f}};
    penDown();
    drawNormPolyline(s2, 2, ox, oy, w, h);
}

// G: 1ストローク
void LinkRobot::drawG() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // (1,1)→(0,1)→(0,0)→(1,0)→(1,0.5)→(0.5,0.5)
    const float s1[][2] = {{1, 1}, {0, 1}, {0, 0}, {1, 0}, {1, 0.5f}, {0.5f, 0.5f}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 6, ox, oy, w, h);
}

// H: 3ストローク
void LinkRobot::drawH() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // S1: (0,1)→(0,0) 左縦線
    const float s1[][2] = {{0, 1}, {0, 0}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 2, ox, oy, w, h);

    // S2: (0,0.5)→(1,0.5) 横棒
    penUp();
    moveTo(ox + 0.0f * w, oy + 0.5f * h);
    const float s2[][2] = {{0, 0.5f}, {1, 0.5f}};
    penDown();
    drawNormPolyline(s2, 2, ox, oy, w, h);

    // S3: (1,1)→(1,0) 右縦線
    penUp();
    moveTo(ox + 1.0f * w, oy + 1.0f * h);
    const float s3[][2] = {{1, 1}, {1, 0}};
    penDown();
    drawNormPolyline(s3, 2, ox, oy, w, h);
}

// I: 3ストローク
void LinkRobot::drawI() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // S1: (0.25,1)→(0.75,1) 上辺
    const float s1[][2] = {{0.25f, 1}, {0.75f, 1}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 2, ox, oy, w, h);

    // S2: (0.5,1)→(0.5,0) 縦線
    penUp();
    moveTo(ox + 0.5f * w, oy + 1.0f * h);
    const float s2[][2] = {{0.5f, 1}, {0.5f, 0}};
    penDown();
    drawNormPolyline(s2, 2, ox, oy, w, h);

    // S3: (0.25,0)→(0.75,0) 下辺
    penUp();
    moveTo(ox + 0.25f * w, oy + 0.0f * h);
    const float s3[][2] = {{0.25f, 0}, {0.75f, 0}};
    penDown();
    drawNormPolyline(s3, 2, ox, oy, w, h);
}

// J: 2ストローク
void LinkRobot::drawJ() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // S1: (0.25,1)→(0.75,1) 上辺
    const float s1[][2] = {{0.25f, 1}, {0.75f, 1}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 2, ox, oy, w, h);

    // S2: (0.5,1)→(0.5,0.15)→(0.25,0)→(0,0.2) 縦線→フック
    penUp();
    moveTo(ox + 0.5f * w, oy + 1.0f * h);
    const float s2[][2] = {{0.5f, 1}, {0.5f, 0.15f}, {0.25f, 0}, {0, 0.2f}};
    penDown();
    drawNormPolyline(s2, 4, ox, oy, w, h);
}

// K: 2ストローク
void LinkRobot::drawK() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // S1: (0,1)→(0,0) 縦線
    const float s1[][2] = {{0, 1}, {0, 0}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 2, ox, oy, w, h);

    // S2: (1,1)→(0,0.5)→(1,0) 斜め線
    penUp();
    moveTo(ox + 1.0f * w, oy + 1.0f * h);
    const float s2[][2] = {{1, 1}, {0, 0.5f}, {1, 0}};
    penDown();
    drawNormPolyline(s2, 3, ox, oy, w, h);
}

// L: 1ストローク
void LinkRobot::drawL() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // (0,1)→(0,0)→(1,0)
    const float s1[][2] = {{0, 1}, {0, 0}, {1, 0}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 3, ox, oy, w, h);
}

// M: 1ストローク
void LinkRobot::drawM() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // (0,0)→(0,1)→(0.5,0.5)→(1,1)→(1,0)
    const float s1[][2] = {{0, 0}, {0, 1}, {0.5f, 0.5f}, {1, 1}, {1, 0}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 5, ox, oy, w, h);
}

// N: 1ストローク
void LinkRobot::drawN() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // (0,0)→(0,1)→(1,0)→(1,1)
    const float s1[][2] = {{0, 0}, {0, 1}, {1, 0}, {1, 1}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 4, ox, oy, w, h);
}

// 〇 → 'O': 1ストローク（24点の円近似ポリゴン）
void LinkRobot::drawCircle() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;
    float cx = 0.5f, cy = 0.5f, r = 0.5f;
    int numPoints = 24;

    // 最初の点へ移動
    float firstX = ox + (cx + r * cos(0.0f)) * w;
    float firstY = oy + (cy + r * sin(0.0f)) * h;
    penUp();
    moveTo(firstX, firstY);
    penDown();

    // 円周上の各点へ逐次移動
    for (int i = 1; i <= numPoints; i++) {
        float angle = 2.0f * (float)M_PI * (float)i / (float)numPoints;
        float nx = cx + r * cos(angle);
        float ny = cy + r * sin(angle);
        float wx = ox + nx * w;
        float wy = oy + ny * h;
        moveTo(wx, wy);
    }
}

// × → 'X': 2ストローク
void LinkRobot::drawCross() {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // S1: (0,1)→(1,0) 左上→右下
    const float s1[][2] = {{0, 1}, {1, 0}};
    penUp();
    moveTo(ox + s1[0][0] * w, oy + s1[0][1] * h);
    penDown();
    drawNormPolyline(s1, 2, ox, oy, w, h);

    // ペンアップで移動
    penUp();
    moveTo(ox + 0.0f * w, oy + 0.0f * h);

    // S2: (0,0)→(1,1) 左下→右上
    const float s2[][2] = {{0, 0}, {1, 1}};
    penDown();
    drawNormPolyline(s2, 2, ox, oy, w, h);
}
