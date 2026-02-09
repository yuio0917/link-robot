#include "Config.h"

// ==========================================
// 文字のデザイン設定 (Font Settings)
// ==========================================
const float CHAR_W = 25.0; // 文字の幅 (mm)
const float CHAR_H = 35.0; // 文字の高さ (mm)
const float CHAR_S = 10.0; // 文字同士の間隔 (mm)

// 文字を書く基準のY座標 (Config.hのHOME_Yを使う)
const float BASE_Y = HOME_Y; 

// ==========================================
// 内部変数 (Positions)
// ==========================================
// 現在書こうとしている文字の基準点(左下)
float current_offset_x = 0.0; 
float current_offset_y = 0.0;

// ==========================================
// 補助関数: 相対座標で線を引く
// ==========================================
// 文字の左下を(0,0)、右上を(W,H)とした「相対座標」で指定します
void drawLineRel(float x1, float y1, float x2, float y2) {
  // 始点へ移動(ペン上げ)
  penUp();
  movePenTo(current_offset_x + x1, current_offset_y + y1);
  
  // 終点へ描画(ペン下げ)
  penDown();
  movePenTo(current_offset_x + x2, current_offset_y + y2);
  
  // 書き終わったらペンを上げる
  penUp(); 
}

// 連続した線を引くための関数 (ペンを下ろしたまま移動)
void lineToRel(float x, float y) {
  movePenTo(current_offset_x + x, current_offset_y + y);
}

// ==========================================
// メイン関数: 文字を描画する
// ==========================================
void drawChar(char c) {
  
  // 1. 文字ごとの開始位置(X座標)を決める
  // 画面中央(HOME_X)を中心に、ABCDEを並べる計算
  // A: -2つ分, B: -1つ分, C: 中央, D: +1つ分, E: +2つ分
  
  float centerX = HOME_X - (CHAR_W / 2.0); // 文字の中心補正
  
  switch(c) {
    case 'A': current_offset_x = centerX - (CHAR_W + CHAR_S) * 2; break;
    case 'B': current_offset_x = centerX - (CHAR_W + CHAR_S) * 1; break;
    case 'C': current_offset_x = centerX; break;
    case 'D': current_offset_x = centerX + (CHAR_W + CHAR_S) * 1; break;
    case 'E': current_offset_x = centerX + (CHAR_W + CHAR_S) * 2; break;
    default:  current_offset_x = centerX; break; // その他は真ん中
  }
  
  // Y座標の基準（少し下から書き始める）
  current_offset_y = BASE_Y - (CHAR_H / 2.0);

  // 2. 文字の形を描画 (ストロークデータ)
  // ここで 0~CHAR_W, 0~CHAR_H の範囲で形を作ります
  
  switch(c) {
    case 'A':
      // 左下 -> 頂点 -> 右下 (一筆書き)
      penUp();
      lineToRel(0, 0);       // 左下へ
      penDown();
      lineToRel(CHAR_W/2, CHAR_H); // 頂点へ
      lineToRel(CHAR_W, 0);        // 右下へ
      // 横棒
      drawLineRel(CHAR_W*0.25, CHAR_H*0.4, CHAR_W*0.75, CHAR_H*0.4);
      break;

    case 'B':
      // 縦棒
      drawLineRel(0, 0, 0, CHAR_H);
      // 上の膨らみ
      penUp(); lineToRel(0, CHAR_H);
      penDown();
      lineToRel(CHAR_W*0.8, CHAR_H);
      lineToRel(CHAR_W, CHAR_H*0.75);
      lineToRel(CHAR_W*0.8, CHAR_H*0.5);
      lineToRel(0, CHAR_H*0.5);
      // 下の膨らみ
      penDown(); // そのまま継続
      lineToRel(CHAR_W*0.8, CHAR_H*0.5); // 戻る
      lineToRel(CHAR_W, CHAR_H*0.25);
      lineToRel(CHAR_W*0.8, 0);
      lineToRel(0, 0);
      break;

    case 'C':
      penUp();
      lineToRel(CHAR_W, CHAR_H*0.8);
      penDown();
      lineToRel(CHAR_W*0.5, CHAR_H);    // 上頂点
      lineToRel(0, CHAR_H*0.5);         // 左端
      lineToRel(CHAR_W*0.5, 0);         // 下頂点
      lineToRel(CHAR_W, CHAR_H*0.2);    // 右下
      break;

    case 'D':
      // 縦棒
      drawLineRel(0, 0, 0, CHAR_H);
      // Dのカーブ
      penUp(); lineToRel(0, CHAR_H);
      penDown();
      lineToRel(CHAR_W*0.6, CHAR_H);
      lineToRel(CHAR_W, CHAR_H*0.5);
      lineToRel(CHAR_W*0.6, 0);
      lineToRel(0, 0);
      break;

    case 'E':
      // 外枠 (コの字)
      penUp(); lineToRel(CHAR_W, CHAR_H);
      penDown();
      lineToRel(0, CHAR_H);
      lineToRel(0, 0);
      lineToRel(CHAR_W, 0);
      // 中の横棒
      drawLineRel(0, CHAR_H*0.5, CHAR_W*0.7, CHAR_H*0.5);
      break;
  }
  
  // 書き終わったらペンを上げる
  penUp();
}