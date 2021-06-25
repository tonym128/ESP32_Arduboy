/*
 * Spell.h : スペル
 */
#ifndef _SPELL_H_
#define _SPELL_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// スペル
#define SPELL_N                         (0x08)

// 向き
#define SPELL_DIRECTION_UP              (0x00)
#define SPELL_DIRECTION_DOWN            (0x01)
#define SPELL_DIRECTION_LEFT            (0x02)
#define SPELL_DIRECTION_RIGHT           (0x03)

// 速度
#define SPELL_SPEED                     (0x01)


/*
 *  型宣言
 */

// スペル
struct Spell {

  // 位置
  struct Position {
    int8_t x;
    int8_t y;
  } position;

  // 向き
  int8_t direction;

  // 速度
  int8_t speed;

  // 攻撃力
  int8_t attack;
};


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void SpellInitialize(void);
extern void SpellUpdate(void);
extern void SpellRender(void);
extern void SpellKill(void);
extern void SpellCast(int8_t x, int8_t y, int8_t direction, int8_t attack);


#endif


