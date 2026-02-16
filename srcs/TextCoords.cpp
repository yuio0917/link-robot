#include "../include/LinkRobot.hpp"

// ヘルパー: 2点間をN分割で線形補間し、結果をpointsに追加
static void LinearInterpolation(float sx, float sy, float ex, float ey,
                                int n, std::vector<std::vector<float>>& points) {
    for (int i = 1; i <= n; i++) {
        float t = static_cast<float>(i) / static_cast<float>(n);
        float px = (ex - sx) * t + sx;
        float py = (ey - sy) * t + sy;
        points.push_back({px, py});
    }
}

// ヘルパー: 描画ストローク追加（正規化座標→ワールド座標変換）
static void addDrawStroke(const std::vector<std::vector<float>>& normalizedPts,
                          float offsetX, float offsetY, float w, float h,
                          int interpN, std::vector<Stroke>& strokes) {
    Stroke s;
    s.penDown = true;
    // 最初の点を追加
    float firstX = offsetX + normalizedPts[0][0] * w;
    float firstY = offsetY + normalizedPts[0][1] * h;
    s.points.push_back({firstX, firstY});
    // 各セグメントを補間
    for (size_t i = 0; i + 1 < normalizedPts.size(); i++) {
        float sx = offsetX + normalizedPts[i][0] * w;
        float sy = offsetY + normalizedPts[i][1] * h;
        float ex = offsetX + normalizedPts[i + 1][0] * w;
        float ey = offsetY + normalizedPts[i + 1][1] * h;
        LinearInterpolation(sx, sy, ex, ey, interpN, s.points);
    }
    strokes.push_back(s);
}

// ヘルパー: 移動ストローク追加（ペンアップで移動）
static void addMoveStroke(float fromX, float fromY, float toX, float toY,
                          std::vector<Stroke>& strokes) {
    Stroke s;
    s.penDown = false;
    s.points.push_back({fromX, fromY});
    s.points.push_back({toX, toY});
    strokes.push_back(s);
}

// A: 2ストローク
void LinkRobot::getAPos(std::vector<Stroke>& strokes) {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // S1: (0,0)→(0.5,1)→(1,0) 左脚→頂点→右脚
    addDrawStroke({{0,0},{0.5f,1},{1,0}}, ox, oy, w, h, N, strokes);

    // ペンアップで横棒の開始点へ移動
    float lastX = ox + 1.0f * w, lastY = oy + 0.0f * h;
    float nextX = ox + 0.25f * w, nextY = oy + 0.5f * h;
    addMoveStroke(lastX, lastY, nextX, nextY, strokes);

    // S2: (0.25,0.5)→(0.75,0.5) 横棒
    addDrawStroke({{0.25f,0.5f},{0.75f,0.5f}}, ox, oy, w, h, N, strokes);
}

// B: 2ストローク
void LinkRobot::getBPos(std::vector<Stroke>& strokes) {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // S1: (0,0)→(0,1)→(0.8,1)→(0.8,0.5)→(0,0.5) 上半分
    addDrawStroke({{0,0},{0,1},{0.8f,1},{0.8f,0.5f},{0,0.5f}}, ox, oy, w, h, N, strokes);

    // ペンアップ移動
    float lastX = ox + 0.0f * w, lastY = oy + 0.5f * h;
    float nextX = ox + 0.0f * w, nextY = oy + 0.5f * h;
    addMoveStroke(lastX, lastY, nextX, nextY, strokes);

    // S2: (0,0.5)→(1,0.5)→(1,0)→(0,0) 下半分
    addDrawStroke({{0,0.5f},{1,0.5f},{1,0},{0,0}}, ox, oy, w, h, N, strokes);
}

// C: 1ストローク
void LinkRobot::getCPos(std::vector<Stroke>& strokes) {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // (1,1)→(0,1)→(0,0)→(1,0)
    addDrawStroke({{1,1},{0,1},{0,0},{1,0}}, ox, oy, w, h, N, strokes);
}

// D: 1ストローク
void LinkRobot::getDPos(std::vector<Stroke>& strokes) {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // (0,0)→(0,1)→(0.7,1)→(1,0.5)→(0.7,0)→(0,0)
    addDrawStroke({{0,0},{0,1},{0.7f,1},{1,0.5f},{0.7f,0},{0,0}}, ox, oy, w, h, N, strokes);
}

// E: 2ストローク
void LinkRobot::getEPos(std::vector<Stroke>& strokes) {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // S1: (1,1)→(0,1)→(0,0)→(1,0) コの字
    addDrawStroke({{1,1},{0,1},{0,0},{1,0}}, ox, oy, w, h, N, strokes);

    // ペンアップ移動
    float lastX = ox + 1.0f * w, lastY = oy + 0.0f * h;
    float nextX = ox + 0.0f * w, nextY = oy + 0.5f * h;
    addMoveStroke(lastX, lastY, nextX, nextY, strokes);

    // S2: (0,0.5)→(0.75,0.5) 中央横棒
    addDrawStroke({{0,0.5f},{0.75f,0.5f}}, ox, oy, w, h, N, strokes);
}

// 〇: 1ストローク（24点の円近似ポリゴン）
void LinkRobot::getCirclePos(std::vector<Stroke>& strokes) {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;
    float cx = 0.5f, cy = 0.5f, r = 0.5f;
    int numPoints = 24;

    Stroke s;
    s.penDown = true;
    for (int i = 0; i <= numPoints; i++) {
        float angle = 2.0f * static_cast<float>(M_PI) * static_cast<float>(i) / static_cast<float>(numPoints);
        float nx = cx + r * std::cos(angle);
        float ny = cy + r * std::sin(angle);
        float wx = ox + nx * w;
        float wy = oy + ny * h;
        s.points.push_back({wx, wy});
    }
    strokes.push_back(s);
}

// ×: 2ストローク
void LinkRobot::getCrossPos(std::vector<Stroke>& strokes) {
    float ox = startPosX, oy = startPosY;
    float w = squareSize, h = squareSize;

    // S1: (0,1)→(1,0) 左上→右下
    addDrawStroke({{0,1},{1,0}}, ox, oy, w, h, N, strokes);

    // ペンアップ移動
    float lastX = ox + 1.0f * w, lastY = oy + 0.0f * h;
    float nextX = ox + 0.0f * w, nextY = oy + 0.0f * h;
    addMoveStroke(lastX, lastY, nextX, nextY, strokes);

    // S2: (0,0)→(1,1) 左下→右上
    addDrawStroke({{0,0},{1,1}}, ox, oy, w, h, N, strokes);
}

// 文字座標マッピング
void LinkRobot::TextCoords(const std::string& c, std::vector<Stroke>& strokes) {
    const std::string Characters[7] = {"A", "B", "C", "D", "E", "〇", "×"};

    for (int i = 0; i < 7; i++) {
        if (Characters[i] == c) {
            (this->*CharHandle[i])(strokes);
            return;
        }
    }
    std::cout << "[TextCoords] Unknown character: " << c << std::endl;
}
