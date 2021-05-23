/*
 * System.h : システムライブラリ
 */
#ifndef _SYSTEM_H_
#define _SYSTEM_H_

/*
 * 参照ファイル
 */
#include "Arduboy2.h"


/*
 * マクロの定義
 */

// デバッグ
#define SYSTEM_DEBUG                    (0x00)

// ビデオ
#define SYSTEM_VIDEO_FLIP_NORMAL        (+0x01)
#define SYSTEM_VIDEO_FLIP_REVERSE       (-0x01)

// シリアル
#define SYSTEM_SERIAL_STRING_LENGTH     (0x08)


/*
 *  型宣言
 */

// キー入力
struct Input {

  // 押された瞬間のボタン
  uint8_t edge;

  // 押されているボタン
  uint8_t push;

};

// ビデオ
struct Video {

  // バッファ
  uint8_t *buffer;

};

// サウンド
struct Sound {

  // 再生データ
  const char *head;
  const char *play;

  // 繰り返し
  bool loop;

  // テンポ
  uint8_t tempo;

  // オクターブ
  uint8_t octave;

  // 音長
  uint8_t length;

  // 休符
  uint8_t rest;

  // フレーム
  uint16_t frame;
  
};


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void SystemInitialize(uint8_t frameRate);
extern bool SystemUpdate(void);
extern Arduboy2 &SystemGetArduboy(void);
extern bool SystemIsInputEdge(uint8_t button);
extern bool SystemIsInputPush(uint8_t button);
extern uint8_t *SystemGetVideoBuffer(void);
extern void SystemClearVideo(uint8_t color);
extern void SystemDisplayVideo(void);
extern void SystemDrawPattern(const uint8_t *pattern, int8_t x, int8_t y, uint8_t width, int8_t flip);
extern void SystemClipPattern(const uint8_t *pattern, int8_t x, int8_t y, uint8_t width, int8_t flip);
extern void SystemMaskPattern(const uint8_t *pattern, uint8_t mask, int8_t x, int8_t y, uint8_t width, int8_t flip);
extern void SystemDrawByte(uint8_t draw, int8_t x, int8_t y, uint8_t width);
extern void SystemClipByte(uint8_t clip, int8_t x, int8_t y, uint8_t width);
extern bool SystemIsSoundEnable(void);
extern void SystemSwitchSound(void);
extern void SystemSwitchSound(bool enable);
extern void SystemRequestSound(const char *mml, bool loop);
extern void SystemStopSound(void);
extern bool SystemIsSoundPlaying(void);
extern uint8_t SystemGetRandom(void);


#endif

