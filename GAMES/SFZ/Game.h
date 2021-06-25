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
#define GAME_STATE_START                (0x10)
#define GAME_STATE_PLAY                 (0x20)
#define GAME_STATE_OVER                 (0x30)
#define GAME_STATE_HISCORE              (0x40)

// 距離
#define GAME_DISTANCE_NEAR              (0x00)
#define GAME_DISTANCE_FAR               (0x2f)

// 表示
#define GAME_VIEW_OX                    (0x40)
#define GAME_VIEW_OY                    (0x20)
#define GAME_VIEW_LEFT                  (-0x40)
#define GAME_VIEW_RIGHT                 (+0x3f)
#define GAME_VIEW_TOP                   (-0x20)
#define GAME_VIEW_BOTTOM                (+0x1f)

// 描画
#define GAME_DRAW_N                     (0x20)
#define GAME_DRAW_SHOT                  (0x4000)
#define GAME_DRAW_ENEMY                 (0x8000)
#define GAME_DRAW_BULLET                (0xc000)

// 矩形
#define GAME_RECT_LEFT                  (0x00)
#define GAME_RECT_RIGHT                 (0x01)
#define GAME_RECT_TOP                   (0x02)
#define GAME_RECT_BOTTOM                (0x03)
#define GAME_RECT_SIZE                  (0x04)


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
extern void GameGetDrawPosition(int8_t positionX, int8_t positionY, int8_t positionZ, int8_t *drawX, int8_t *drawY, int8_t *drawScale, int8_t *drawHeight);
extern void GameEntryDraw(uint16_t draw, int8_t index, int8_t z);
extern void GameAddScore(uint16_t score);


#endif

