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

// ページ
#define TITLE_PAGE_LOGO                 (0x00)
#define TITLE_PAGE_OBJECT               (0x01)
#define TITLE_PAGE_MONSTER_1            (0x02)
#define TITLE_PAGE_MONSTER_2            (0x03)
#define TITLE_PAGE_MONSTER_3            (0x04)
#define TITLE_PAGE_ITEM_1               (0x05)
#define TITLE_PAGE_ITEM_2               (0x06)
#define TITLE_PAGE_CONDITION            (0x07)
#define TITLE_PAGE_SIZE                 (0x08)

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

