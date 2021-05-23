/*
   App.h : アプリケーション
*/
#ifndef _APP_H_
#define _APP_H_

/*
   参照ファイル
*/


/*
   マクロの定義
*/

// 状態
#define APP_STATE_NULL                  (0x00)
#define APP_STATE_TITLE_INITIALIZE      (0x01)
#define APP_STATE_TITLE_UPDATE          (0x02)
#define APP_STATE_GAME_INITIALIZE       (0x03)
#define APP_STATE_GAME_UPDATE           (0x04)

// スコア
#define APP_SCORE_MAX                   (9999)


/*
    型宣言
*/


/*
   外部変数宣言
*/


/*
   外部関数宣言
*/
extern void AppInitialize(void);
extern void AppUpdate(void);
extern void AppSetState(uint8_t state);
extern void AppSetScore(uint8_t score);
extern uint8_t AppGetScore(void);
extern void AppDrawPattern(int16_t draw, int8_t x, int8_t y, uint8_t width, int8_t flip);
extern void AppClipPattern(int16_t clip, int8_t x, int8_t y, uint8_t width, int8_t flip);
extern void AppClipPattern(int16_t draw, int16_t clip, int8_t x, int8_t y, uint8_t width, int8_t flip);
extern void AppMaskPattern(int16_t draw, uint8_t mask, int8_t x, int8_t y, uint8_t width, int8_t flip);
extern void AppDrawChar(char c, int8_t x, int8_t y);
extern void AppDrawString(const char *string, int8_t x, int8_t y);
extern void AppDrawString(const char *string, int8_t x, int8_t y, int8_t length);
extern void AppClipString(const char *string, int8_t x, int8_t y);
extern void AppClipString(const char *string, int8_t x, int8_t y, int8_t length);
extern void AppDrawNumber(uint16_t number, int8_t x, int8_t y, int8_t length);
extern void AppClipNumber(uint16_t number, int8_t x, int8_t y, int8_t length);
extern int16_t AppGetSin(uint8_t angle);
extern int16_t AppGetCos(uint8_t angle);
extern uint8_t AppGetAtan(int16_t y, int16_t x);


#endif

