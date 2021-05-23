/*
 * Shot.cpp : 自弾
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Shot.h"


/*
 * 変数の定義
 */

// 自弾
static struct Shot shots[SHOT_N];

// パターン
static const uint8_t shotPatterns[] PROGMEM = {
  0x3a, 0xfc, 0xfc, 0x08, 0x3a, 0xfc, 0xfc, 0x08, 
  0x2a, 0xfc, 0xfc, 0x08, 0x2a, 0xfc, 0xfc, 0x08, 
  0x28, 0xf8, 0xf8, 0x10, 0x38, 0xf8, 0x00, 0x10, 
  0x26, 0xf8, 0xf8, 0x10, 0x36, 0xf8, 0x00, 0x10, 
};

// 矩形
static const uint8_t shotRects[] PROGMEM = {
  0xff, 0x00, 0xff, 0x00, 
  0xfe, 0x01, 0xfe, 0x01, 
  0xfc, 0x03, 0xfc, 0x03, 
  0xfb, 0x04, 0xfb, 0x04, 
};


/*
 * 内部関数の宣言
 */


/*
 * 自弾を初期化する
 */
void ShotInitialize(void)
{
  // 自弾の取得
  struct Shot *shot = shots;
  
  // 自弾の初期化
  for (int8_t i = 0x00; i < SHOT_N; i++) {

    // 状態の設定
    shot->state = SHOT_STATE_NULL;

    // 次へ
    ++shot;
  }
}

/*
 * 自弾を更新する
 */
void ShotUpdate(void)
{
  // 自弾の取得
  struct Shot *shot = shots;
  
  // 自弾の更新
  for (int8_t i = 0x00; i < SHOT_N; i++) {

    // 状態別の処理
    switch (shot->state & 0xf0) {

    // 発射
    case SHOT_STATE_SHOOT:
      {
        shot->positionZ += SHOT_SPEED_SHOOT_Z;
        if (shot->positionZ >= GAME_DISTANCE_FAR) {
          shot->state = SHOT_STATE_NULL;
        }
      }
      break;

    // 跳ね返り
    case SHOT_STATE_REBOUND:
      {
        shot->positionX += shot->positionX >= 0x00 ? SHOT_SPEED_REBOUND_X : -SHOT_SPEED_REBOUND_X;
        shot->positionY += shot->positionY >= 0x00 ? SHOT_SPEED_REBOUND_Y : -SHOT_SPEED_REBOUND_Y;
        if (shot->positionX < GAME_VIEW_LEFT - 0x10 || GAME_VIEW_RIGHT + 0x10 < shot->positionX || shot->positionY < GAME_VIEW_TOP - 0x10 || GAME_VIEW_BOTTOM + 0x10 < shot->positionY) {
          shot->state = SHOT_STATE_NULL;
        }
      }
      break;

    // その他　
    default:
      break;
    }

    // 自弾の存在
    if ((shot->state & 0xf0) != SHOT_STATE_NULL) {

      // 描画の設定
      GameGetDrawPosition(shot->positionX, shot->positionY, shot->positionZ, NULL, &shot->drawY, &shot->drawScale, NULL);
      shot->drawX = shot->positionX;
      GameEntryDraw(GAME_DRAW_SHOT, i, shot->positionZ);
    }

    // 次へ
    ++shot;
  }
}

/*
 * 自弾を描画する
 */
void ShotRender(void)
{
}
void ShotDraw(int8_t index)
{
  struct Shot *shot = &shots[index];
  if ((shot->state & 0xf0) != SHOT_STATE_NULL) {
    uint8_t *p = (uint8_t *)(shotPatterns + (shot->drawScale << 3));
    for (int8_t i = 0x00; i < 0x02; i++) {
      uint8_t pattern = pgm_read_byte(p + 0x00);
      AppClipPattern(
        pattern + 0x00, 
        pattern + 0x20, 
        (int16_t)shot->drawX + pgm_read_byte(p + 0x01) + GAME_VIEW_OX, 
        (int16_t)shot->drawY + pgm_read_byte(p + 0x02) + GAME_VIEW_OY, 
        pgm_read_byte(p + 0x03), 
        SYSTEM_VIDEO_FLIP_NORMAL
      );
      p += 0x04;
    }
  }
}

/*
 * 自弾を登録する
 */
void ShotEntry(int8_t x, int8_t y)
{
  // 自弾の取得
  struct Shot *shot = shots;
  
  // 自弾の走査
  for (int8_t i = 0x00; i < SHOT_N; i++) {

    // 空の自弾
    if (shot->state == SHOT_STATE_NULL) {

      // 自弾の登録
      shot->positionX = x;
      shot->positionY = y;
      shot->positionZ = GAME_DISTANCE_NEAR;
      shot->state = SHOT_STATE_SHOOT;
      break;
    }

    // 次へ
    ++shot;
  }
}

/*
 * 自弾の状態を判定する
 */
bool ShotIsState(int8_t index, uint8_t state)
{
  return (shots[index].state & 0xf0) == state;
}

/*
 * 自弾の状態を設定する
 */
void ShotSetState(int8_t index, uint8_t state)
{
  shots[index].state = state;
}

/*
 * 自弾の矩形を取得する
 */
bool ShotGetRect(int8_t index, int8_t *rect, int8_t *z)
{
  bool result = false;
  if ((shots[index].state & 0xf0) == SHOT_STATE_SHOOT) {
    uint8_t *r = (uint8_t *)(shotRects + (shots[index].drawScale << 2));
    rect[GAME_RECT_LEFT] = shots[index].drawX + (int8_t)pgm_read_byte(r + 0x00);
    rect[GAME_RECT_RIGHT] = shots[index].drawX + (int8_t)pgm_read_byte(r + 0x01);
    rect[GAME_RECT_TOP] = shots[index].drawY + (int8_t)pgm_read_byte(r + 0x02);
    rect[GAME_RECT_BOTTOM] = shots[index].drawY + (int8_t)pgm_read_byte(r + 0x03);
    *z = shots[index].positionZ;
    result = true;
  }
  return result;
}
