/*
 * Shot.h : 自弾
 */
#ifndef _SHOT_H_
#define _SHOT_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// 数
#define SHOT_N                          (0x04)

// 状態
#define SHOT_STATE_NULL                 (0x00)
#define SHOT_STATE_SHOOT                (0x10)
#define SHOT_STATE_REBOUND              (0x20)

// 速度
#define SHOT_SPEED_SHOOT_Z              (0x01)
#define SHOT_SPEED_REBOUND_X            (0x04)
#define SHOT_SPEED_REBOUND_Y            (0x02)


/*
 *  型宣言
 */

// 自弾
struct Shot {

  // 状態
  uint8_t state;

  // 位置
  int8_t positionX;
  int8_t positionY;
  int8_t positionZ;

  // 描画
  int8_t drawX;
  int8_t drawY;
  int8_t drawScale;
  
};


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void ShotInitialize(void);
extern void ShotUpdate(void);
extern void ShotRender(void);
extern void ShotDraw(int8_t index);
extern void ShotEntry(int8_t x, int8_t y);
extern bool ShotIsState(int8_t index, uint8_t state);
extern void ShotSetState(int8_t index, uint8_t state);
extern bool ShotGetRect(int8_t index, int8_t *rect, int8_t *z);


#endif

