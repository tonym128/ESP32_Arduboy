/*
 * Field.cpp : フィールド
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Field.h"


/*
 * 変数の定義
 */

// フィールド
static struct Field field;

// 地面
static const uint32_t fieldGroundScrolls[]  = {
  0b000000001111110000111001, 
  0b000000011111110000111001, 
  0b000000111111100001111001, 
  0b000001111111000001110001, 
  0b000011111111000011110011, 
  0b000111111110000011110011, 
  0b001111111100000111100011, 
  0b011111111000000111100011, 
};


/*
 * 内部関数の宣言
 */


/*
 * フィールドを初期化する
 */
void FieldInitialize(void)
{
  // 地面の設定
  field.groundScroll = 0x00;
  field.groundSpeed = 0x00;
  
  // 状態の設定
  field.state = FIELD_STATE_PLAY;
}

/*
 * フィールドを更新する
 */
void FieldUpdate(void)
{
  // 状態別の処理
  switch (field.state & 0xf0) {
    
  // プレイ
  case FIELD_STATE_PLAY:
    {
      // 地面の更新
      if (++field.groundSpeed >= FIELD_GROUND_SPEED) {
        ++field.groundScroll;
        field.groundSpeed = 0x00;
      }
    }
    break;

  // その他
  default:
    break;
  }
}

/*
 * フィールドを描画する
 */
void FieldRender(void)
{
  // 遠景の描画
  {
    for (int8_t i = 0x00; i < 0x05; i++) {
      AppDrawPattern(0x0100 + (i << 4), 0x00, 0x00 + (i << 3), 0x80, SYSTEM_VIDEO_FLIP_NORMAL);
    }  
  }
  
  // 地面の描画
  {
    AppDrawPattern(0x0150, 0x00, 0x28, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawPattern(0x0150, 0x40, 0x28, 0x40, SYSTEM_VIDEO_FLIP_REVERSE);
    AppDrawPattern(0x0160, 0x00, 0x30, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawPattern(0x0160, 0x40, 0x30, 0x40, SYSTEM_VIDEO_FLIP_REVERSE);
    AppDrawPattern(0x0170, 0x00, 0x38, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    AppDrawPattern(0x0170, 0x40, 0x38, 0x40, SYSTEM_VIDEO_FLIP_REVERSE);
    {
      uint8_t *v = SystemGetVideoBuffer() + 0x0280;
      uint32_t mask = pgm_read_dword(fieldGroundScrolls + (field.groundScroll & 0x07));
      if ((field.groundScroll & 0x08) != 0x00) {
        mask = ~mask;
      }
      for (uint8_t y = 0x00; y < 0x03; y++) {
        uint8_t m = (uint8_t)mask;
        for (uint8_t x = 0x00; x < WIDTH; x++) {
          *v++ ^= m;
        }
        mask >>= 8;
      }
    }
  }
}


