/*
 * Spell.cpp : スペル
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Dungeon.h"
#include "Player.h"
#include "Spell.h"


/*
 * 変数の定義
 */

// スペル
static struct Spell spells[SPELL_N];

// 移動
static const int8_t spellMoves[] PROGMEM = {
   0, -1,  0, +1, -1,  0, +1,  0, 
};

// 矩形
static const int8_t spellRects[] PROGMEM = {
  -4,  0,  3,  2, 
  -4, -2,  3,  0, 
   0, -4,  2,  3, 
  -2, -4,  0,  3, 
};


/*
 * 内部関数の宣言
 */


/*
 * スペルを初期化する
 */
void SpellInitialize(void)
{
  // スペルの初期化
  for (int8_t i = 0; i < SPELL_N; i++) {
    spells[i].attack = 0;
  }
}

/*
 * スペルを更新する
 */
void SpellUpdate(void)
{
  // スペルの走査
  for (int8_t i = 0; i < SPELL_N; i++) {

    // スペルの存在
    if (spells[i].attack > 0) {

      // 速度の更新
      if (++spells[i].speed >= SPELL_SPEED) {
  
        // 矩形の取得
        int8_t left = spells[i].position.x + (int8_t)pgm_read_byte(&spellRects[spells[i].direction * 0x04 + 0x00]);
        int8_t top = spells[i].position.y + (int8_t)pgm_read_byte(&spellRects[spells[i].direction * 0x04 + 0x01]);
        int8_t right = spells[i].position.x + (int8_t)pgm_read_byte(&spellRects[spells[i].direction * 0x04 + 0x02]);
        int8_t bottom = spells[i].position.y + (int8_t)pgm_read_byte(&spellRects[spells[i].direction * 0x04 + 0x03]);
        
        // プレイヤとのヒット判定
        if (PlayerIsHit(left, top, right, bottom, spells[i].attack, CONDITION_NULL)) {

          // スペルの削除
          spells[i].attack = 0;
  
        // 移動
        } else {
          
          // 移動量の取得
          int8_t mx = (int8_t)pgm_read_byte(&spellMoves[spells[i].direction * 0x02 + 0x00]);
          int8_t my = (int8_t)pgm_read_byte(&spellMoves[spells[i].direction * 0x02 + 0x01]);
  
          // 壁とのコリジョン判定
          if (DungeonIsCollision(left + mx, top + my, right + mx, bottom + my)) {
            spells[i].attack = 0;
  
          // 移動
          } else {
            spells[i].position.x += mx;
            spells[i].position.y += my;
            spells[i].speed = 0;
          }
        }
      }
    }
  }
}

/*
 * スペルを描画する
 */
void SpellRender(void)
{
  // スペルの走査
  for (int8_t i = 0; i < SPELL_N; i++) {

    // スペルの存在
    if (spells[i].attack > 0) {

      // スペルの描画
      AppDrawPattern(
        0x0084 + spells[i].direction, 
        spells[i].position.x + (int8_t)pgm_read_byte(&spellRects[spells[i].direction * 0x04 + 0x00]), 
        spells[i].position.y + (int8_t)pgm_read_byte(&spellRects[spells[i].direction * 0x04 + 0x01]), 
        0x08, 
        SYSTEM_VIDEO_FLIP_NORMAL
      );
    }
  }
}

/*
 * スペルを削除する
 */
void SpellKill(void)
{
  for (int8_t i = 0; i < SPELL_N; i++) {
    spells[i].attack = 0;
  }
}

/*
 * スペルを撃つ
 */
void SpellCast(int8_t x, int8_t y, int8_t direction, int8_t attack)
{
  // スペルの走査
  for (int8_t i = 0; i < SPELL_N; i++) {

    // 空のスペル
    if (spells[i].attack == 0) {

      // スペルの登録
      spells[i].position.x = x;
      spells[i].position.y = y;
      spells[i].direction = direction;
      spells[i].speed = 0;
      spells[i].attack = attack;
      break;
    }
  }
}


