/*
 * Bullet.h : 敵弾
 */
#ifndef _BULLET_H_
#define _BULLET_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// 数
#define BULLET_N                        (0x10)

// 状態
#define BULLET_STATE_NULL               (0x00)
#define BULLET_STATE_SHOOT              (0x10)


/*
 *  型宣言
 */

// 自弾
struct Bullet {

  // 状態
  uint8_t state;

  // 位置
  int16_t positionX;
  int16_t positionY;
  int8_t positionZ;

  // 速度
  int8_t speedX;
  int8_t speedY;

  // 描画
  int8_t drawX;
  int8_t drawY;
  int8_t drawScale;

  // アニメーション
  uint8_t animation;
  
};


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void BulletInitialize(void);
extern void BulletUpdate(void);
extern void BulletRender(void);
extern void BulletDraw(int8_t index);
extern void BulletEntry(int8_t x, int8_t y, int8_t z);
extern bool BulletHit(int8_t *rect, int8_t z);


#endif

