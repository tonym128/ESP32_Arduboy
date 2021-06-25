/*
 * App.cpp : アプリケーション
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Title.h"
#include "Game.h"
#include "font.h"
#include "pattern.h"


/*
 * 変数の定義
 */

// 状態
static uint8_t appState;

// スコア
static uint8_t appScore;

// 三角関数
static const uint8_t appSins[]  = {
  0x00, 0x06, 0x0c, 0x12, 0x19, 0x1f, 0x25, 0x2b, 0x31, 0x38, 0x3e, 0x44, 0x4a, 0x50, 0x56, 0x5c, 
  0x61, 0x67, 0x6d, 0x73, 0x78, 0x7e, 0x83, 0x88, 0x8e, 0x93, 0x98, 0x9d, 0xa2, 0xa7, 0xab, 0xb0, 
  0xb5, 0xb9, 0xbd, 0xc1, 0xc5, 0xc9, 0xcd, 0xd1, 0xd4, 0xd8, 0xdb, 0xde, 0xe1, 0xe4, 0xe7, 0xea, 
  0xec, 0xee, 0xf1, 0xf3, 0xf4, 0xf6, 0xf8, 0xf9, 0xfb, 0xfc, 0xfd, 0xfe, 0xfe, 0xff, 0xff, 0xff, 
};
static const uint8_t appAtanAngles[]  = {
  0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x40, 0x20, 0x12, 0x0d, 0x09, 0x08, 0x06, 0x05, 
  0x40, 0x2d, 0x20, 0x17, 0x12, 0x0f, 0x0d, 0x0b, 
  0x40, 0x32, 0x28, 0x20, 0x1a, 0x16, 0x12, 0x10, 
  0x40, 0x36, 0x2d, 0x25, 0x20, 0x1b, 0x17, 0x15, 
  0x40, 0x37, 0x30, 0x29, 0x24, 0x20, 0x1c, 0x19, 
  0x40, 0x39, 0x32, 0x2d, 0x28, 0x23, 0x20, 0x1c, 
  0x40, 0x3a, 0x34, 0x2f, 0x2a, 0x26, 0x23, 0x20, 
};
static const uint8_t appAtanOffsets[]  = {
  0x00, 0x80, 0x00, 0x80, 
};


/*
 * 内部関数の宣言
 */


/*
 * アプリケーションを初期化する
 */
void AppInitialize(void)
{
  // スコアの初期化
  appScore = 0x01;
  
  // 状態の初期化
  appState = APP_STATE_TITLE_INITIALIZE;
}

/*
 * アプリケーションを更新する
 */
void AppUpdate(void)
{
  // 状態別の処理
  switch (appState) {

  // タイトルの初期化
  case APP_STATE_TITLE_INITIALIZE:
    TitleInitialize();
    break;

  // タイトルの更新
  case APP_STATE_TITLE_UPDATE:
    TitleUpdate();
    break;

  // ゲームの初期化
  case APP_STATE_GAME_INITIALIZE:
    GameInitialize();
    break;

  // ゲームの更新
  case APP_STATE_GAME_UPDATE:
    GameUpdate();
    break;

  // その他
  default:
    break;
  }

  // 乱数の更新
  SystemGetRandom();
}

/*
 * 状態を設定する
 */
void AppSetState(uint8_t state)
{
  appState = state;
}

/*
 * スコアを設定する
 */
void AppSetScore(uint8_t score)
{
  if (appScore < score) {
    appScore = score;
  }
}

/*
 * スコアを取得する
 */
uint8_t AppGetScore(void)
{
  return appScore;
}

/*
 * パターンを描画する
 */
void AppDrawPattern(int16_t draw, int8_t x, int8_t y, uint8_t width, int8_t flip)
{
  SystemDrawPattern(patternTable + (draw << 3), x, y, width, flip);
}
void AppClipPattern(int16_t clip, int8_t x, int8_t y, uint8_t width, int8_t flip)
{
  SystemClipPattern(patternTable + (clip << 3), x, y, width, flip);
}
void AppClipPattern(int16_t draw, int16_t clip, int8_t x, int8_t y, uint8_t width, int8_t flip)
{
  SystemClipPattern(patternTable + (clip << 3), x, y, width, flip);
  SystemDrawPattern(patternTable + (draw << 3), x, y, width, flip);
}
void AppReversePattern(int16_t draw, int8_t x, int8_t y, uint8_t width, int8_t flip)
{
  SystemReversePattern(patternTable + (draw << 3), x, y, width, flip);
}
void AppMaskPattern(int16_t draw, uint8_t mask, int8_t x, int8_t y, uint8_t width, int8_t flip)
{
  SystemMaskPattern(patternTable + (draw << 3), mask, x, y, width, flip);
}

/*
 * 文字を描画する
 */
void AppDrawChar(char c, int8_t x, int8_t y)
{
  SystemDrawPattern(fontTable + ((c - ' ') << 2), x, y, 0x04, SYSTEM_VIDEO_FLIP_NORMAL);
}

/*
 * 文字列を描画する
 */
void AppDrawString(const char *string, int8_t x, int8_t y)
{
  char c;
  while ((c = pgm_read_byte(string)) != '\0') {
    SystemDrawPattern(fontTable + ((c - ' ') << 2), x, y, 0x04, SYSTEM_VIDEO_FLIP_NORMAL);
    x += 0x04;
    ++string;
  }
}
void AppDrawString(const char *string, int8_t x, int8_t y, int8_t length)
{
  char c;
  while ((c = pgm_read_byte(string)) != '\0' && length > 0x00) {
    SystemDrawPattern(fontTable + ((c - ' ') << 2), x, y, 0x04, SYSTEM_VIDEO_FLIP_NORMAL);
    x += 0x04;
    ++string;
    --length;
  }
}
void AppClipString(const char *string, int8_t x, int8_t y)
{
  char c;
  while ((c = pgm_read_byte(string)) != '\0') {
    SystemClipPattern(fontTable + ((c - ' ') << 2), x, y, 0x04, SYSTEM_VIDEO_FLIP_NORMAL);
    x += 0x04;
    ++string;
  }
}
void AppClipString(const char *string, int8_t x, int8_t y, int8_t length)
{
  char c;
  while ((c = pgm_read_byte(string)) != '\0' && length > 0x00) {
    SystemClipPattern(fontTable + ((c - ' ') << 2), x, y, 0x04, SYSTEM_VIDEO_FLIP_NORMAL);
    x += 0x04;
    ++string;
    --length;
  }
}

/*
 * 数値を描画する
 */
void AppDrawNumber(uint16_t number, int8_t x, int8_t y, int8_t length)
{
  x += (length - 1) << 2;
  do {
    SystemDrawPattern(fontTable + 0x40 + ((number % 10) << 2), x, y, 0x04, SYSTEM_VIDEO_FLIP_NORMAL);
    x -= 0x04;
    number /= 10;
  } while (number > 0x00);
}
void AppClipNumber(uint16_t number, int8_t x, int8_t y, int8_t length)
{
  x += (length - 1) << 2;
  do {
    SystemClipPattern(fontTable + 0x40 + ((number % 10) << 2), x, y, 0x04, SYSTEM_VIDEO_FLIP_NORMAL);
    x -= 0x04;
    number /= 10;
  } while (number > 0x00);
}

/*
 * 三角関数を取得する
 */
int16_t AppGetSin(uint8_t angle)
{
  int16_t result = 0x0100;
  uint8_t n = angle & 0x7f;
  if (n < 0x40) {
    result = pgm_read_byte(appSins + n);
  } else if (n > 0x40) {
    result = pgm_read_byte(appSins + 0x80 - n);
  }
  if (angle > 0x80) {
    result = -result;
  }
  return result;
}
int16_t AppGetCos(uint8_t angle)
{
  return AppGetSin(angle + 0x40);
}
uint8_t AppGetAtan(int16_t y, int16_t x)
{
  // 正数に補正
  int8_t offset = 0x00;
  if (x < 0x0000) {
    x = -x;
    if (x < 0x0000) {
      x = 0x7fff;
    }
    offset |= 0x01;
  }
  if (y < 0x0000) {
    y = -y;
    if (y < 0x0000) {
      y = 0x7fff;
    }
    offset |= 0x02;
  }

  // 参照の取得
  int8_t n = (y >= x) ? y : x;
  while (n >= 0x08) {
    n >>= 1;
    x >>= 1;
    y >>= 1;
  }

  // 角度の取得
  uint8_t angle = pgm_read_byte(appAtanAngles + (y << 3) + x);
  if (((offset + 0x01) & 0x02) != 0x00) {
    angle = -angle; 
  }
  angle += pgm_read_byte(appAtanOffsets + offset);

  // 終了
  return angle;
}

