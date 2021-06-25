/*
 * Game.h : ゲーム
 */
#ifndef _GAME_H_
#define _GAME_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// 状態
#define GAME_STATE_NULL                 (0x00)
#define GAME_STATE_PLAY                 (0x10)
#define GAME_STATE_OVER                 (0x20)
#define GAME_STATE_CLEAR                (0x30)


/*
 *  型宣言
 */


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void GameInitialize(void);
extern void GameUpdate(void);


#endif

