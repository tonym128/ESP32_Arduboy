/*
 * Enemy.h : エネミー
 */
#ifndef _ENEMY_H_
#define _ENEMY_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// 数
#define ENEMY_N                         (0x0c)

// 種類
#define ENEMY_TYPE_NULL                 (0x00)
#define ENEMY_TYPE_POLE                 (0x01)
#define ENEMY_TYPE_DORARINFURAS         (0x02)
#define ENEMY_TYPE_JURII                (0x03)
#define ENEMY_TYPE_ONYANMA              (0x04)
#define ENEMY_TYPE_STAMPERON_CORE       (0x05)
#define ENEMY_TYPE_STAMPERON_BODY       (0x06)
#define ENEMY_TYPE_BOMB                 (0x10)

// 状態
#define ENEMY_STATE_NULL                (0x00)

// パラメータ
#define ENEMY_PARAM_0                   (0x00)
#define ENEMY_PARAM_1                   (0x01)
#define ENEMY_PARAM_2                   (0x02)
#define ENEMY_PARAM_3                   (0x03)
#define ENEMY_PARAM_N                   (0x04)

// ヒット
#define ENEMY_HIT_NULL                  (0x00)
#define ENEMY_HIT_DAMAGE                (0x01)
#define ENEMY_HIT_INVINCIBLE            (0x02)
#define ENEMY_HIT_DISTANCE              (0x04)

// 耐久力
#define ENEMY_LIFE_INVINCIBLE           (0xff)

// ジェネレータ
#define ENEMY_GENERATOR_FRAME           (60 * 60)


/*
 *  型宣言
 */

// エネミー
struct Enemy {

  // 参照
  int8_t index;

  // 種類
  uint8_t type;

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
  int8_t drawHeight;

  // 耐久力
  uint8_t life;

  // 発射
  uint8_t fire;

  // パラメータ
  int8_t params[ENEMY_PARAM_N];
  
};

// ジェネレータ
struct EnemyGenerator {

  // 種類
  uint8_t type;

  // 生成するための残りの数
  uint8_t count;

  // 発射
  uint8_t fire;

  // フレーム
  uint16_t frame;

  // ボス
  bool boss;
  
};

/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void EnemyInitialize(void);
extern void EnemyUpdate(void);
extern void EnemyRender(void);
extern void EnemyDraw(int8_t index);
extern uint8_t EnemyHit(int8_t *rect, int8_t z);
extern int8_t EnemyGetCount(void);


#endif

