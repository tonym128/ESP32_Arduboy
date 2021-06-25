/*
 * Player.h : プレイヤ
 */
#ifndef _PLAYER_H_
#define _PLAYER_H_

/*
 * 参照ファイル
 */
#include "Condition.h"
#include "Item.h"


/*
 * マクロの定義
 */

// 状態
#define PLAYER_STATE_NULL               (0x00)
#define PLAYER_STATE_STAY               (0x10)
#define PLAYER_STATE_PLAY               (0x20)
#define PLAYER_STATE_DEAD               (0x30)

// 向き
#define PLAYER_DIRECTION_NULL           (-1)
#define PLAYER_DIRECTION_UP             (0x00)
#define PLAYER_DIRECTION_DOWN           (0x01)
#define PLAYER_DIRECTION_LEFT           (0x02)
#define PLAYER_DIRECTION_RIGHT          (0x03)

// 速度
#define PLAYER_SPEED_NORMAL             (0x03)
#define PLAYER_SPEED_SLOW               (0x05)
#define PLAYER_SPEED_FAST               (0x02)

// 体力
#define PLAYER_LIFE                     (0x32)

// ヒット
#define PLAYER_HIT_NORMAL               (0x06)
#define PLAYER_HIT_FAST                 (0x05)

// ダメージ
#define PLAYER_DAMAGE                   (0x08)

// 死亡
#define PLAYER_DEAD                     (0x04)

// 操作
#define PLAYER_CONTROL_UP               (0x00)
#define PLAYER_CONTROL_DOWN             (0x01)
#define PLAYER_CONTROL_LEFT             (0x02)
#define PLAYER_CONTROL_RIGHT            (0x03)
#define PLAYER_CONTROL_SIZE             (0x04)

// 矩形
#define PLAYER_RECT_X                   (-0x04)
#define PLAYER_RECT_Y                   (-0x04)
#define PLAYER_RECT_WIDTH               (0x08)
#define PLAYER_RECT_HEIGHT              (0x08)
#define PLAYER_RECT_TOP                 (-0x04)
#define PLAYER_RECT_BOTTOM              (+0x03)
#define PLAYER_RECT_LEFT                (-0x04)
#define PLAYER_RECT_RIGHT               (+0x03)


/*
 *  型宣言
 */

// プレイヤ
struct Player {

  // 状態
  uint8_t state;

  // ルーム
  int8_t room;

  // 位置
  struct Position {
    int8_t x;
    int8_t y;
  } position;

  // 向き
  int8_t direction;

  // 体力
  int8_t life;

  // 速度
  int8_t speed;

  // ヒット
  int8_t hit;

  // ダメージ
  int8_t damage;

  // コンディション
  uint16_t conditions[CONDITION_SIZE];

  // アイテム
  int8_t items[ITEM_SIZE];

  // 操作
  uint8_t controls[PLAYER_CONTROL_SIZE];

  // アニメーション
  uint8_t animation;

  // マスク
  uint8_t mask;
  
};


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void PlayerInitialize(void);
extern void PlayerUpdate(void);
extern void PlayerRender(void);
extern bool PlayerIsHit(int8_t left, int8_t top, int8_t right, int8_t bottom, int8_t attack, int8_t condition);
extern bool PlayerExitRoom(void);
extern int8_t PlayerGetRoom(void);
extern void PlayerGetPosition(int8_t *x, int8_t *y);
extern bool PlayerIsLive(void);
extern bool PlayerIsDead(void);
extern void PlayerHeal(void);
extern void PlayerHeal(int8_t life);
extern int8_t PlayerGetLife(void);
extern int8_t PlayerGetAttack(void);
extern int8_t PlayerGetDefense(void);
extern bool PlayerIsCondition(int8_t condition);
extern void PlayerSetCondition(int8_t condition);
extern bool PlayerIsItem(int8_t item);


#endif


