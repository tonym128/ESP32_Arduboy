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

// エネミー
#define ENEMY_N                         (0x09)

// 種類
#define ENEMY_TYPE_NULL                 (0x00)
#define ENEMY_TYPE_ORC                  (0x01)
#define ENEMY_TYPE_GOLEM                (0x02)
#define ENEMY_TYPE_SKELTON              (0x03)
#define ENEMY_TYPE_LICH                 (0x04)
#define ENEMY_TYPE_GHOST                (0x05)
#define ENEMY_TYPE_ROGUE                (0x06)
#define ENEMY_TYPE_KNIGHT               (0x07)
#define ENEMY_TYPE_MAGE                 (0x08)
#define ENEMY_TYPE_BAT                  (0x09)
#define ENEMY_TYPE_PYTHON               (0x0a)
#define ENEMY_TYPE_SPIDER               (0x0b)
#define ENEMY_TYPE_ZORN                 (0x0c)
#define ENEMY_TYPE_SQUID                (0x0d)
#define ENEMY_TYPE_GAZER                (0x0e)
#define ENEMY_TYPE_LIZARD               (0x0f)
#define ENEMY_TYPE_DAEMON               (0x10)
#define ENEMY_TYPE_GARGOYLE             (0x11)
#define ENEMY_TYPE_REAPER               (0x12)
#define ENEMY_TYPE_SHADOW               (0x13)
#define ENEMY_TYPE_DRAGON               (0x14)
#define ENEMY_TYPE_CRYSTAL              (0x15)
#define ENEMY_TYPE_SIZE                 (0x16)

// 状態
#define ENEMY_STATE_NULL                (0x00)
#define ENEMY_STATE_STAY                (0x10)
#define ENEMY_STATE_PLAY                (0x20)
#define ENEMY_STATE_DEAD                (0x30)

// フラグ
#define ENEMY_FLAG_NULL                 (0x00)
#define ENEMY_FLAG_HIDE                 (0x01)

// 向き
#define ENEMY_DIRECTION_UP              (0x00)
#define ENEMY_DIRECTION_DOWN            (0x01)
#define ENEMY_DIRECTION_LEFT            (0x02)
#define ENEMY_DIRECTION_RIGHT           (0x03)

// ダメージ
#define ENEMY_DAMAGE                    (0x08)

// 死亡
#define ENEMY_DEAD                      (0x04)

// パラメータ
#define ENEMY_PARAM_0                   (0x00)
#define ENEMY_PARAM_1                   (0x01)
#define ENEMY_PARAM_2                   (0x02)
#define ENEMY_PARAM_3                   (0x03)
#define ENEMY_PARAM_N                   (0x04)

// スペル
#define ENEMY_CAST_1                    (0x01)
#define ENEMY_CAST_2                    (0x02)
#define ENEMY_CAST_2_RANDOM             (0x82)


/*
 *  型宣言
 */

// エネミー
struct Enemy {

  // 種類
  int8_t type;

  // 状態
  uint8_t state;

  // フラグ
  uint8_t flag;

  // 位置
  struct Position {
    int8_t x;
    int8_t y;
  } position;

  // 向き
  int8_t direction;

  // 体力
  int16_t life;

  // 速度
  int8_t speed;

  // ヒット
  int8_t hit;

  // ダメージ
  int8_t damage;

  // アニメーション
  uint8_t animation;

  // マスク
  uint8_t mask;

  // パラメータ
  uint8_t params[ENEMY_PARAM_N];
};

// エネミーデータ
struct EnemyData {

  // 体力
  int16_t life;

  // 速度
  int8_t speed;

  // ヒット
  int8_t hit;

  // 攻撃力
  int8_t attack;

  // スペル
  int8_t spell;

  // コンディション
  int8_t condition;

  // 矩形
  struct Rect {
    int8_t left;
    int8_t top;
    int8_t right;
    int8_t bottom;
  } rect;

  // パターン
  uint8_t pattern;

  // サイズ
  struct Size {
    int8_t x;
    int8_t y;
  } size;
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
extern void EnemyEncount(void);
extern bool EnemyIsCollision(int8_t left, int8_t top, int8_t right, int8_t bottom);
extern bool EnemyIsHit(int8_t left, int8_t top, int8_t right, int8_t bottom, int8_t attack);


#endif


