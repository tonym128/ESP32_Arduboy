/*
 * Title.h : タイトル
 */
#ifndef _TITLE_H_
#define _TITLE_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// 状態
#define TITLE_STATE_NULL                (0x00)
#define TITLE_STATE_STAY                (0x10)
#define TITLE_STATE_START               (0x20)


/*
 *  型宣言
 */


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void TitleInitialize(void);
extern void TitleUpdate(void);


#endif

