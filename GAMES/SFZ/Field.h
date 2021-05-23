/*
 * Field.h : フィールド
 */
#ifndef _FIELD_H_
#define _FIELD_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// 状態
#define FIELD_STATE_NULL                (0x00)
#define FIELD_STATE_PLAY                (0x10)

// 地面
#define FIELD_GROUND_SPEED              (0x02)


/*
 *  型宣言
 */

// フィールド
struct Field {

  // 状態
  uint8_t state;

  // 地面
  uint8_t groundScroll;
  uint8_t groundSpeed;
  
};


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void FieldInitialize(void);
extern void FieldUpdate(void);
extern void FieldRender(void);


#endif


