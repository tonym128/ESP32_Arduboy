/*
 * Player.h : プレイヤ
 */
#ifndef _PLAYER_H_
#define _PLAYER_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// 状態
#define PLAYER_STATE_NULL               (0x00)
#define PLAYER_STATE_STAY               (0x10)
#define PLAYER_STATE_PLAY               (0x20)
#define PLAYER_STATE_DEAD               (0x30)

// 速度
#define PLAYER_SPEED                    (0x01)

// 体力
#define PLAYER_LIFE                     (0x08)

// ダメージ
#define PLAYER_DAMAGE                   (0x30)

// 発射
#define PLAYER_FIRE                     (0x08)

// 矩形
#define PLAYER_RECT_LEFT                (-0x02)
#define PLAYER_RECT_RIGHT               (+0x01)
#define PLAYER_RECT_TOP                 (-0x02)
#define PLAYER_RECT_BOTTOM              (+0x01)


/*
 *  型宣言
 */

// プレイヤ
struct Player {

  // 状態
  uint8_t state;

  // 位置
  int8_t positionX;
  int8_t positionY;

  // 体力
  uint8_t life;

  // ダメージ
  uint8_t damage;

  // 発射
  uint8_t fire;

  // アニメーション
  uint8_t animation;
  
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
extern bool PlayerIsLive(void);
extern bool PlayerIsDead(void);
extern bool PlayerIsState(uint8_t state);
extern void PlayerSetState(uint8_t state);
extern bool PlayerGetPosition(int8_t *x, int8_t *y, int8_t *z);
extern bool PlayerGetRect(int8_t *rect, int8_t *z);
extern uint8_t PlayerGetLife(void);
extern void PlayerDamage(void);


#endif

