/*
 * Dungeon.h : ダンジョン
 */
#ifndef _DUNGEON_H_
#define _DUNGEON_H_

/*
 * 参照ファイル
 */


/*
 * マクロの定義
 */

// 状態
#define DUNGEON_STATE_NULL              (0x00)
#define DUNGEON_STATE_PLAY              (0x10)

// ルーム
#define DUNGEON_ROOM_SIZE_X             (0x08)
#define DUNGEON_ROOM_SIZE_Y             (0x08)
#define DUNGEON_ROOM_SIZE               (DUNGEON_ROOM_SIZE_X * DUNGEON_ROOM_SIZE_Y)
#define DUNGEON_ROOM_MASK_X             (0x07)
#define DUNGEON_ROOM_MASK_Y             (0x07)
#define DUNGEON_ROOM_MASK               (0x3f)
#define DUNGEON_ROOM_SHIFT              (0x03)
#define DUNGEON_ROOM_FLAG_NULL          (0x00)
#define DUNGEON_ROOM_FLAG_WALL_H        (0x01)
#define DUNGEON_ROOM_FLAG_WALL_V        (0x02)
#define DUNGEON_ROOM_FLAG_ITEM          (0x04)
#define DUNGEON_ROOM_REGION_X           (0x00)
#define DUNGEON_ROOM_REGION_Y           (0x00)
#define DUNGEON_ROOM_REGION_WIDTH       (0x40)
#define DUNGEON_ROOM_REGION_HEIGHT      (0x40)
#define DUNGEON_ROOM_REGION_TOP         (DUNGEON_ROOM_REGION_Y)
#define DUNGEON_ROOM_REGION_BOTTOM      (DUNGEON_ROOM_REGION_Y + DUNGEON_ROOM_REGION_HEIGHT - 1)
#define DUNGEON_ROOM_REGION_LEFT        (DUNGEON_ROOM_REGION_X)
#define DUNGEON_ROOM_REGION_RIGHT       (DUNGEON_ROOM_REGION_X + DUNGEON_ROOM_REGION_WIDTH - 1)
#define DUNGEON_ROOM_WALL_X             (0x04)
#define DUNGEON_ROOM_WALL_Y             (0x04)
#define DUNGEON_ROOM_WALL_WIDTH         (0x38)
#define DUNGEON_ROOM_WALL_HEIGHT        (0x38)
#define DUNGEON_ROOM_WALL_TOP           (DUNGEON_ROOM_WALL_Y)
#define DUNGEON_ROOM_WALL_BOTTOM        (DUNGEON_ROOM_WALL_Y + DUNGEON_ROOM_WALL_HEIGHT - 1)
#define DUNGEON_ROOM_WALL_LEFT          (DUNGEON_ROOM_WALL_X)
#define DUNGEON_ROOM_WALL_RIGHT         (DUNGEON_ROOM_WALL_X + DUNGEON_ROOM_WALL_WIDTH - 1)
#define DUNGEON_ROOM_PATH_TOP           (0x1c)
#define DUNGEON_ROOM_PATH_BOTTOM        (0x23)
#define DUNGEON_ROOM_PATH_LEFT          (0x1c)
#define DUNGEON_ROOM_PATH_RIGHT         (0x23)

// 壁
#define DUNGEON_WALL_NULL               (0x00)
#define DUNGEON_WALL_UP                 (0x01)
#define DUNGEON_WALL_DOWN               (0x02)
#define DUNGEON_WALL_LEFT               (0x04)
#define DUNGEON_WALL_RIGHT              (0x08)

// アイテム
#define DUNGEON_ITEM_RECT_X             (-0x04)
#define DUNGEON_ITEM_RECT_Y             (-0x04)
#define DUNGEON_ITEM_RECT_WIDTH         (0x08)
#define DUNGEON_ITEM_RECT_HEIGHT        (0x08)
#define DUNGEON_ITEM_RECT_TOP           (-0x04)
#define DUNGEON_ITEM_RECT_BOTTOM        (+0x03)
#define DUNGEON_ITEM_RECT_LEFT          (-0x04)
#define DUNGEON_ITEM_RECT_RIGHT         (+0x03)


/*
 *  型宣言
 */

// ダンジョン
struct Dungeon {

  // 状態
  uint8_t state;

  // ルーム
  struct Room {

    // フラグ
    uint8_t flag;

    // 順番
    int8_t order;

    // 距離
    int8_t distance;

    // エネミーの数
    int8_t rest;
    
  } rooms[DUNGEON_ROOM_SIZE_Y][DUNGEON_ROOM_SIZE_X];

  // 開始
  int8_t start;

  // アイテムの位置
  struct Position {
    int8_t x;
    int8_t y;
  } item;
  
};

// ルーム
struct DungeonRoom {

  // エネミーの種類
  int8_t enemyType;

  // エネミーの数
  int8_t enemyRest;

  // アイテム
  int8_t item;
  
};

// 配置
struct DungeonLocation {

  // X 位置
  int8_t x;

  // Y 位置
  int8_t y;
};


/*
 * 外部変数宣言
 */


/*
 * 外部関数宣言
 */
extern void DungeonInitialize(void);
extern void DungeonUpdate(void);
extern void DungeonRender(void);
extern bool DungeonIsCollision(int8_t index, int8_t x, int8_t y);
extern bool DungeonIsCollision(int8_t index, int8_t left, int8_t top, int8_t right, int8_t bottom);
extern bool DungeonIsCollision(int8_t left, int8_t top, int8_t right, int8_t bottom);
extern void DungeonEnterRoom(int8_t index);
extern bool DungeonExitRoom(int8_t *index, int8_t *x, int8_t *y);
extern void DungeonGetStartRoom(int8_t *index, int8_t *x, int8_t *y);
extern int8_t DungeonGetRoomOrder(int8_t index);
extern int8_t DungeonKillEnemy(int8_t index);
extern int8_t DungeonGetEnemyType(int8_t index);
extern int8_t DungeonGetEnemyRest(int8_t index);
extern void DungeonGetEnemyLocation(int8_t order, int8_t rest, int8_t *x, int8_t *y);
extern void DungeonFindItem(int8_t index);
extern int8_t DungeonPickUpItem(int8_t index, int8_t x, int8_t y);

#endif


