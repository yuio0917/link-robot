#include "include/LinkRobot.hpp"


void    LinkRobot::LinearInterpolation(float start_x, float start_y, float end_x, float end_y){
    //補完係数
    float t = 0.0f;
    float px;
    float py;
    
    for (int i = 1; i <= 4; i++){
        t = i / 4.0f;
        px = (end_x - start_x) * t + start_x; //x座標の線形補完の計算
        py = (end_y - start_y) * t + start_y; //y座標の線形補完の計算
        solveIK(px, py);
    }
}

void    LinkRobot::getAPos(){
    //Position2の座標
    double pos2_x = start_pos_x + squareSize / 2;
    double pos2_y = start_pos_y + squareSize;

    //Position3の座標
    double pos3_x = start_pos_x + squareSize;
    double pos3_y = start_pos_y;

    //Position4の座標
    double pos4_x = (start_pos_x + pos2_x) / 2;
    double pos4_y = (start_pos_y + pos2_y) / 2;

    //Position5の座標
    double pos5_x = (pos2_x + pos3_x) / 2;
    double pos5_y = (pos2_y + pos3_y) / 2;
    
    //初期位置からPosition2までの線形補完
    LinearInterpolation(start_pos_x, start_pos_y, pos2_x, pos2_y);
    //Position2からPosition3までの線形補完
    LinearInterpolation(pos2_x, pos2_y, pos3_x, pos3_y);
    //Position3からPosition4までの線形補完
    LinearInterpolation(pos3_x, pos3_y, pos4_x, pos4_y);
    //Position4からPosition5までの線形補完
    LinearInterpolation(pos4_x, pos4_y, pos5_x, pos5_y);
}

// void    LinkRobot::getBPos(std::vector<std::vector<float>> &charVec){

// }

// void    LinkRobot::getCPos(std::vector<std::vector<float>> &charVec){

// }

// void    LinkRobot::getDPos(std::vector<std::vector<float>> &charVec){

// }

// void    LinkRobot::getEPos(std::vector<std::vector<float>> &charVec){

// }

// void    LinkRobot::getCirclePos(std::vector<std::vector<float>> &charVec){

// }

// void    LinkRobot::getCrossPos(std::vector<std::vector<float>> &charVec){

// }

void    LinkRobot::TextCoords(const char c){
    const char Characters[7] = {'A', 'B', 'C', 'D', 'E', '〇', '×'};

    //charVec.clear();
    //charVec.push_back(std::vector<float>{start_pos_x, start_pos_y});

    for (int i = 0; i < 7; i++){
        if (Characters[i] == c){
            (this->*CharHandle[i])();
            return ;
        }
    }
}