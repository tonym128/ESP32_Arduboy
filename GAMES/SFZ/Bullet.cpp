/*
 * Bullet.cpp : 敵弾
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Player.h"
#include "Bullet.h"


/*
 * 変数の定義
 */

// 敵弾
static struct Bullet bullets[BULLET_N];

// パターン
static const uint8_t bulletPatterns[] PROGMEM = {
  0x3f, 0xfc, 0xfc, 0x08, 0x3f, 0xfc, 0xfc, 0x08, 
  0x2f, 0xfc, 0xfc, 0x08, 0x2f, 0xfc, 0xfc, 0x08, 
  0x2d, 0xf8, 0xf8, 0x10, 0x3d, 0xf8, 0x00, 0x10, 
  0x2b, 0xf8, 0xf8, 0x10, 0x3b, 0xf8, 0x00, 0x10, 
};

// 矩形
static const uint8_t bulletRects[] PROGMEM = {
  0xff, 0x00, 0xff, 0x00, 
  0xfe, 0x01, 0xfe, 0x01, 
  0xfc, 0x03, 0xfc, 0x03, 
  0xfb, 0x04, 0xfb, 0x04, 
};


/*
 * 内部関数の宣言
 */


/*
 * 敵弾を初期化する
 */
void BulletInitialize(void)
{
  // 敵弾の取得
  struct Bullet *bullet = bullets;
  
  // 敵弾の初期化
  for (int8_t i = 0x00; i < BULLET_N; i++) {

    // 状態の設定
    bullet->state = BULLET_STATE_NULL;

    // 次へ
    ++bullet;
  }
}

/*
 * 敵弾を更新する
 */
void BulletUpdate(void)
{
  // 敵弾の取得
  struct Bullet *bullet = bullets;
  
  // 敵弾の更新
  for (int8_t i = 0x00; i < BULLET_N; i++) {

    // 状態別の処理
    switch (bullet->state & 0xf0) {
      
    // 発射
    case BULLET_STATE_SHOOT:
      {
        bullet->positionX += (int16_t)bullet->speedX << 3;
        bullet->positionY += (int16_t)bullet->speedY << 3;
        if ((bullet->animation & 0x01) != 0x00) {
          --bullet->positionZ;
        }
        if (
          bullet->positionZ < GAME_DISTANCE_NEAR ||
          bullet->positionX <= -0x4800 || bullet->positionX >= 0x4800 || 
          bullet->positionY <= -0x2800 || bullet->positionY >= 0x2800
        ) {
          bullet->state = BULLET_STATE_NULL;
        }
        ++bullet->animation;
      }
      break;

    // その他
    default:
      break;
    }

    // 敵弾の存在
    if ((bullet->state & 0xf0) != BULLET_STATE_NULL) {

      // 描画の設定
      GameGetDrawPosition(bullet->positionX >> 8, bullet->positionY >> 8, bullet->positionZ, NULL, &bullet->drawY, &bullet->drawScale, NULL);
      bullet->drawX = bullet->positionX >> 8;
      GameEntryDraw(GAME_DRAW_BULLET, i, bullet->positionZ);
    }

    // 次へ
    ++bullet;
  }
}

/*
 * 敵弾を描画する
 */
void BulletRender(void)
{
}
void BulletDraw(int8_t index)
{
  struct Bullet *bullet = &bullets[index];
  if ((bullet->state & 0xf0) != BULLET_STATE_NULL) {
    uint8_t *p = (uint8_t *)(bulletPatterns + (bullet->drawScale << 3));
    uint8_t flip = (bullet->animation & 0x02) != 0x00 ? SYSTEM_VIDEO_FLIP_NORMAL : SYSTEM_VIDEO_FLIP_REVERSE;
    for (int8_t i = 0x00; i < 0x02; i++) {
      uint8_t pattern = pgm_read_byte(p + 0x00);
      AppClipPattern(
        pattern + 0x00, 
        pattern + 0x20, 
        (int16_t)bullet->drawX + pgm_read_byte(p + 0x01) + GAME_VIEW_OX, 
        (int16_t)bullet->drawY + pgm_read_byte(p + 0x02) + GAME_VIEW_OY, 
        pgm_read_byte(p + 0x03), 
        flip
      );
      p += 0x04;
    }
  }
}

/*
 * 敵弾を登録する
 */
void BulletEntry(int8_t x, int8_t y, int8_t z)
{
  // 一定の距離を離れている
  if (z >= 0x08) {
      
    // 敵弾の取得
    struct Bullet *bullet = bullets;
    
    // 敵弾の走査
    for (int8_t i = 0x00; i < BULLET_N; i++) {
  
      // 空の敵弾
      if (bullet->state == BULLET_STATE_NULL) {
  
        // プレイヤの位置の取得
        int8_t px, py, pz;
        if (z >= 0x08 && PlayerGetPosition(&px, &py, &pz)) {
  
          // 精度を下げる
          px += (SystemGetRandom() & 0x1f) - 0x10;
          py += (SystemGetRandom() & 0x0f) - 0x08;
  
          // 速度の取得
          int16_t sx = (((int16_t)px << 8) - ((int16_t)x << 8)) / (z << 1);
          int16_t sy = (((int16_t)py << 8) - ((int16_t)y << 8)) / (z << 1);
          if (sx < -0x0300) {
            sx = -0x0300;
          } else if (sx > 0x0300) {
            sx = 0x0300;
          }
          if (sy < -0x0300) {
            sy = -0x0300;
          } else if (sy > 0x0300) {
            sy = 0x0300;
          }
          
          // 敵弾の登録
          bullet->positionX = (int16_t)x << 8;
          bullet->positionY = (int16_t)y << 8;
          bullet->positionZ = z;
          bullet->speedX = sx >> 3;
          bullet->speedY = sy >> 3;
          bullet->state = BULLET_STATE_SHOOT;
        }
        break;
      }
  
      // 次へ
      ++bullet;
    }
  }
}

/*
 * 敵弾とのヒット判定を行う
 */
bool BulletHit(int8_t *rect, int8_t z)
{
  bool result = false;
  int8_t br[GAME_RECT_SIZE];
  for (int8_t i = 0x00; i < BULLET_N; i++) {
    if (bullets[i].state != BULLET_STATE_NULL) {
      if (GAME_DISTANCE_NEAR <= bullets[i].positionZ && bullets[i].positionZ <= GAME_DISTANCE_FAR) {
        if (z == bullets[i].positionZ) {
          {
            uint8_t *r = (uint8_t *)(bulletRects + (bullets[i].drawScale << 2));
            br[GAME_RECT_LEFT] = bullets[i].drawX + (int8_t)pgm_read_byte(r + 0x00);
            br[GAME_RECT_RIGHT] = bullets[i].drawX + (int8_t)pgm_read_byte(r + 0x01);
            br[GAME_RECT_TOP] = bullets[i].drawY + (int8_t)pgm_read_byte(r + 0x02);
            br[GAME_RECT_BOTTOM] = bullets[i].drawY + (int8_t)pgm_read_byte(r + 0x03);
          }
          if (
            rect[GAME_RECT_LEFT] > br[GAME_RECT_RIGHT] || 
            rect[GAME_RECT_RIGHT] < br[GAME_RECT_LEFT] || 
            rect[GAME_RECT_TOP] > br[GAME_RECT_BOTTOM] || 
            rect[GAME_RECT_BOTTOM] < br[GAME_RECT_TOP]
          ) {
            ;
          } else {
            result = true;
            break;
          }
        }
      }
    }
  }
  return result;
}
