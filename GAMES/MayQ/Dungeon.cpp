/*
 * Dungeon.cpp : ダンジョン
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Item.h"
#include "Dungeon.h"
#include "Player.h"
#include "Enemy.h"


/*
 * 変数の定義
 */

// ダンジョン
static struct Dungeon dungeon;

// ルーム
static const struct DungeonRoom dungeonRooms[DUNGEON_ROOM_SIZE] PROGMEM = {
  /* 00 */ {ENEMY_TYPE_CRYSTAL,  1, ITEM_DOOR,     }, // Crystal Door
  /* 01 */ {ENEMY_TYPE_ORC,      1, ITEM_NULL,     }, // Lv1
  /* 02 */ {ENEMY_TYPE_BAT,      1, ITEM_HAMMER,   }, // Lv1*Hammer
  /* 03 */ {ENEMY_TYPE_SKELTON,  1, ITEM_SWORD,    }, // Lv1 Sword
  /* 04 */ {ENEMY_TYPE_BAT,      2, ITEM_HAMMER,   }, // Lv1*Hammer
  /* 05 */ {ENEMY_TYPE_ORC,      2, ITEM_SHIELD,   }, // Lv1 Shield
  /* 06 */ {ENEMY_TYPE_BAT,      4, ITEM_HAMMER,   }, // Lv1*Hammer
  /* 07 */ {ENEMY_TYPE_SKELTON,  2, ITEM_NULL,     }, // Lv1
  /* 08 */ {ENEMY_TYPE_BAT,      5, ITEM_HAMMER,   }, // Lv1*Hammer
  /* 09 */ {ENEMY_TYPE_GARGOYLE, 1, ITEM_POTION,   }, // Lv1 Boss Potion
  /* 10 */ {ENEMY_TYPE_NULL,     0, ITEM_NULL,     }, // ---
  /* 11 */ {ENEMY_TYPE_ROGUE,    2, ITEM_BOOTS,    }, // Lv2*Boots
  /* 12 */ {ENEMY_TYPE_GHOST,    2, ITEM_KEY,      }, // Lv2*Key
  /* 13 */ {ENEMY_TYPE_SKELTON,  4, ITEM_SWORD,    }, // Lv2 Sword
  /* 14 */ {ENEMY_TYPE_SPIDER,   2, ITEM_NULL,     }, // Lv2
  /* 15 */ {ENEMY_TYPE_ZORN,     1, ITEM_CANDLE,   }, // Lv2*Candle
  /* 16 */ {ENEMY_TYPE_ROGUE,    4, ITEM_BOOTS,    }, // Lv2*Boots
  /* 17 */ {ENEMY_TYPE_ORC,      4, ITEM_SHIELD,   }, // Lv2 Shield
  /* 18 */ {ENEMY_TYPE_SPIDER,   4, ITEM_NULL,     }, // Lv2
  /* 19 */ {ENEMY_TYPE_SKELTON,  5, ITEM_NULL,     }, // Lv2
  /* 20 */ {ENEMY_TYPE_GHOST,    4, ITEM_KEY,      }, // Lv2*Key
  /* 21 */ {ENEMY_TYPE_ORC,      5, ITEM_NULL,     }, // Lv2
  /* 22 */ {ENEMY_TYPE_ZORN,     2, ITEM_CANDLE,   }, // Lv2*Candle
  /* 23 */ {ENEMY_TYPE_REAPER,   1, ITEM_POTION,   }, // Lv2 Boss Potion
  /* 24 */ {ENEMY_TYPE_NULL,     0, ITEM_NULL,     }, // ---
  /* 25 */ {ENEMY_TYPE_ROGUE,    4, ITEM_BOOTS,    }, // Lv3*Boots
  /* 30 */ {ENEMY_TYPE_LIZARD,   4, ITEM_SWORD,    }, // Lv3 Sword
  /* 27 */ {ENEMY_TYPE_ZORN,     4, ITEM_CANDLE,   }, // Lv3*Candle
  /* 28 */ {ENEMY_TYPE_MAGE,     1, ITEM_MIRROR,   }, // Lv3*Mirror
  /* 29 */ {ENEMY_TYPE_ROGUE,    5, ITEM_BOOTS,    }, // Lv3*Boots
  /* 26 */ {ENEMY_TYPE_GOLEM,    4, ITEM_SHIELD,   }, // Lv3 Shield
  /* 31 */ {ENEMY_TYPE_GHOST,    5, ITEM_KEY,      }, // Lv3*Key
  /* 32 */ {ENEMY_TYPE_SQUID,    4, ITEM_NULL,     }, // Lv3
  /* 33 */ {ENEMY_TYPE_MAGE,     2, ITEM_MIRROR,   }, // Lv3*Mirror
  /* 34 */ {ENEMY_TYPE_PYTHON,   4, ITEM_NULL,     }, // Lv3
  /* 37 */ {ENEMY_TYPE_LIZARD,   5, ITEM_SWORD,    }, // Lv3 Sword
  /* 36 */ {ENEMY_TYPE_SPIDER,   5, ITEM_NULL,     }, // Lv3
  /* 35 */ {ENEMY_TYPE_GOLEM,    5, ITEM_SHIELD,   }, // Lv3 Shield
  /* 38 */ {ENEMY_TYPE_MAGE,     4, ITEM_MIRROR,   }, // Lv3*Mirror
  /* 39 */ {ENEMY_TYPE_SQUID,    5, ITEM_NULL,     }, // Lv3
  /* 40 */ {ENEMY_TYPE_ZORN,     5, ITEM_CANDLE,   }, // Lv3*Candle
  /* 41 */ {ENEMY_TYPE_SHADOW,   1, ITEM_POTION,   }, // Lv3 Boss Potion
  /* 42 */ {ENEMY_TYPE_NULL,     0, ITEM_NULL,     }, // ---
  /* 43 */ {ENEMY_TYPE_KNIGHT,   4, ITEM_SWORD,    }, // Lv4 Sword
  /* 44 */ {ENEMY_TYPE_MAGE,     5, ITEM_MIRROR,   }, // Lv4*Mirror
  /* 45 */ {ENEMY_TYPE_SPIDER,   8, ITEM_NULL,     }, // Lv4
  /* 46 */ {ENEMY_TYPE_LICH,     4, ITEM_NULL,     }, // Lv4
  /* 47 */ {ENEMY_TYPE_PYTHON,   5, ITEM_NULL,     }, // Lv4
  /* 48 */ {ENEMY_TYPE_GAZER,    4, ITEM_RING,     }, // Lv4*Ring
  /* 49 */ {ENEMY_TYPE_GOLEM,    8, ITEM_NULL,     }, // Lv4
  /* 50 */ {ENEMY_TYPE_DAEMON,   4, ITEM_SHIELD,   }, // Lv4 Shield
  /* 51 */ {ENEMY_TYPE_KNIGHT,   5, ITEM_SWORD,    }, // Lv4 Sword
  /* 52 */ {ENEMY_TYPE_LICH,     4, ITEM_NULL,     }, // Lv4
  /* 53 */ {ENEMY_TYPE_GAZER,    4, ITEM_RING,     }, // Lv4*Ring
  /* 54 */ {ENEMY_TYPE_DAEMON,   4, ITEM_SHIELD,   }, // Lv4 Shield
  /* 55 */ {ENEMY_TYPE_GHOST,    8, ITEM_KEY,      }, // Lv4*Key
  /* 56 */ {ENEMY_TYPE_PYTHON,   8, ITEM_NULL,     }, // Lv4
  /* 57 */ {ENEMY_TYPE_SQUID,    8, ITEM_NULL,     }, // Lv4
  /* 58 */ {ENEMY_TYPE_LIZARD,   8, ITEM_NULL,     }, // Lv4
  /* 59 */ {ENEMY_TYPE_KNIGHT,   8, ITEM_NULL,     }, // Lv4
  /* 60 */ {ENEMY_TYPE_LICH,     5, ITEM_NULL,     }, // Lv4
  /* 61 */ {ENEMY_TYPE_GAZER,    5, ITEM_RING,     }, // Lv4*Ring
  /* 62 */ {ENEMY_TYPE_DAEMON,   5, ITEM_NULL,     }, // Lv4
  /* 63 */ {ENEMY_TYPE_DRAGON,   1, ITEM_CRYSTAL,  }, // Lv4 Boss Crystal
};

// 順番
static int8_t dungeonOrders[DUNGEON_ROOM_SIZE];

// 配置
static const struct DungeonLocation dungeonEnemyLocations_1[] PROGMEM = {
  {0x20, 0x20, }, 
};
static const struct DungeonLocation dungeonEnemyLocations_4[] PROGMEM = {
  {0x1a, 0x1a, }, 
  {0x26, 0x26, }, 
  {0x26, 0x1a, }, 
  {0x1a, 0x26, }, 
};
static const struct DungeonLocation dungeonEnemyLocations_5[] PROGMEM = {
  {0x20, 0x20, }, 
  {0x18, 0x18, }, 
  {0x28, 0x28, }, 
  {0x28, 0x18, }, 
  {0x18, 0x28, }, 
};
static const struct DungeonLocation dungeonEnemyLocations_9[] PROGMEM = {
  {0x18, 0x18, }, 
  {0x28, 0x28, }, 
  {0x28, 0x18, }, 
  {0x18, 0x28, }, 
  {0x20, 0x18, }, 
  {0x20, 0x28, }, 
  {0x18, 0x20, }, 
  {0x28, 0x20, }, 
  {0x20, 0x20, }, 
};

// パターン
static const uint8_t dungeonPatterns[][8] PROGMEM = {
  {0x10, 0x11, 0x11, 0x12, 0x13, 0x11, 0x11, 0x14, }, 
  {0x10, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x14, }, 
  {0x1b, 0x1c, 0x1c, 0x1d, 0x1e, 0x1c, 0x1c, 0x1f, }, 
  {0x1b, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1c, 0x1f, }, 
  {0x15, 0x15, 0x17, 0x19, 0x15, 0x15, 0x00, 0x00, }, 
  {0x15, 0x15, 0x15, 0x15, 0x15, 0x15, 0x00, 0x00, }, 
  {0x16, 0x16, 0x18, 0x1a, 0x16, 0x16, 0x00, 0x00, }, 
  {0x16, 0x16, 0x16, 0x16, 0x16, 0x16, 0x00, 0x00, }, 
};


/*
 * 内部関数の宣言
 */
static void DungeonMakeMaze(void);
static void DungeonCalcMazeDistance(int8_t x, int8_t y);
static uint8_t DungeonGetRoomWall(int8_t x, int8_t y);
static uint8_t DungeonGetPoleWall(int8_t x, int8_t y);
static Dungeon::Room *DungeonGetRoom(int8_t index);
static int8_t DungeonGetItem(int8_t index);


/*
 * ダンジョンを初期化する
 */
void DungeonInitialize(void)
{
  // 迷路の作成
  DungeonMakeMaze();

  // エネミーの配置
  for (int8_t y = 0; y < DUNGEON_ROOM_SIZE_Y; y++) {
    for (int8_t x = 0; x < DUNGEON_ROOM_SIZE_X; x++) {
      dungeon.rooms[y][x].rest = (int8_t)pgm_read_byte(&dungeonRooms[dungeon.rooms[y][x].order].enemyRest);
    }
  }

  // 状態の設定
  dungeon.state = DUNGEON_STATE_PLAY;
}

/*
 * ダンジョンを更新する
 */
void DungeonUpdate(void)
{
  // 状態別の処理
  switch (dungeon.state & 0xf0) {
    
  // プレイ
  case DUNGEON_STATE_PLAY:
    {
    }
    break;

  // その他
  default:
    break;
  }
}

/*
 * ダンジョンを描画する
 */
void DungeonRender(void)
{
  // ルームの取得
  int8_t index = PlayerGetRoom();
  
  // 壁の取得
  uint8_t wall = DungeonGetRoomWall(index & DUNGEON_ROOM_MASK_X, (index >> DUNGEON_ROOM_SHIFT) & DUNGEON_ROOM_MASK_Y);

  // 外周の描画
  {
    uint8_t *p = (uint8_t *)dungeonPatterns[(wall & DUNGEON_WALL_UP) == 0 ? 0x00 : 0x01];
    for (int8_t x = 0; x < DUNGEON_ROOM_SIZE_X; x++) {
      AppDrawPattern((int16_t)pgm_read_byte(p), x * 0x08, 0x00, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
      ++p;
    }
  }
  {
    uint8_t *p = (uint8_t *)dungeonPatterns[(wall & DUNGEON_WALL_DOWN) == 0 ? 0x02 : 0x03];
    for (int8_t x = 0; x < DUNGEON_ROOM_SIZE_X; x++) {
      AppDrawPattern((int16_t)pgm_read_byte(p), x * 0x08, 0x38, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
      ++p;
    }
  }
  {
    uint8_t *p = (uint8_t *)dungeonPatterns[(wall & DUNGEON_WALL_LEFT) == 0 ? 0x04 : 0x05];
    for (int8_t y = 1; y < DUNGEON_ROOM_SIZE_Y - 1; y++) {
      AppDrawPattern((int16_t)pgm_read_byte(p), 0x00, y * 0x08, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
      ++p;
    }
  }
  {
    uint8_t *p = (uint8_t *)dungeonPatterns[(wall & DUNGEON_WALL_RIGHT) == 0 ? 0x06 : 0x07];
    for (int8_t y = 1; y < DUNGEON_ROOM_SIZE_Y - 1; y++) {
      AppDrawPattern((int16_t)pgm_read_byte(p), 0x38, y * 0x08, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
      ++p;
    }
  }
  /*
  {
    for (int8_t y = 1; y < DUNGEON_ROOM_SIZE_Y - 1; y++) {
      for (int8_t x = 1; x < DUNGEON_ROOM_SIZE_X - 1; x++) {
        AppDrawPattern(0x0001, x * 0x08, y * 0x08, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
      }
    }
  }
  */

  /*
  // 全体マップ
  for (int8_t y = 0; y < DUNGEON_ROOM_SIZE_Y; y++) {
    for (int8_t x = 0; x < DUNGEON_ROOM_SIZE_X; x++) {
      AppDrawPattern(0x20 + DungeonGetRoomWall(x, y), x * 0x08 + 0x40, y * 0x08, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
    }
  }
  */

  // アイテムの描画
  {
    int8_t item = DungeonGetItem(index);
    if (item != ITEM_NULL) {
      if (item == ITEM_DOOR) {
        if (PlayerIsItem(ITEM_KEY)) {
          int8_t px, py;
          PlayerGetPosition(&px, &py);
          if (abs(px - dungeon.item.x) <= (DUNGEON_ITEM_RECT_WIDTH + PLAYER_RECT_WIDTH) && abs(py - dungeon.item.y) <= (DUNGEON_ITEM_RECT_HEIGHT + PLAYER_RECT_HEIGHT)) {
            item = ITEM_EXIT;
          }
        }
      }
      AppDrawPattern(0x0090 + (item - 1), dungeon.item.x + DUNGEON_ITEM_RECT_X, dungeon.item.y + DUNGEON_ITEM_RECT_Y, DUNGEON_ITEM_RECT_WIDTH, SYSTEM_VIDEO_FLIP_NORMAL);
    }
  }
}

/*
 * 迷路を作成する
 */
static void DungeonMakeMaze(void)
{
  // 壁の初期化
  for (int8_t y = 0; y < DUNGEON_ROOM_SIZE_Y; y++) {
    for (int8_t x = 0; x < DUNGEON_ROOM_SIZE_X; x++) {
      dungeon.rooms[y][x].flag = DUNGEON_ROOM_FLAG_NULL;
    }
  }
  for (int8_t x = 0; x < DUNGEON_ROOM_SIZE_X; x++) {
    dungeon.rooms[0][x].flag |= DUNGEON_ROOM_FLAG_WALL_H;
  }
  for (int8_t y = 0; y < DUNGEON_ROOM_SIZE_Y; y++) {
    dungeon.rooms[y][0].flag |= DUNGEON_ROOM_FLAG_WALL_V;
  }

  // 迷路の作成
  for (int8_t y = 1; y < DUNGEON_ROOM_SIZE_Y; y++) {
    for (int8_t x = 1; x < DUNGEON_ROOM_SIZE_X; x++) {
      int8_t px = x;
      int8_t py = y;
      uint8_t wall = DungeonGetPoleWall(px, py);
      uint8_t mask = DUNGEON_WALL_UP | DUNGEON_WALL_DOWN | DUNGEON_WALL_LEFT | DUNGEON_WALL_RIGHT;
      while ((wall & mask) == 0) {
        int8_t r = (SystemGetRandom() >> 3) & 0x03;
        while ((wall & (1 << r)) != 0) {
          r = (r + 1) & 0x03;
        }
        r = 1 << r;
        if (r == DUNGEON_WALL_UP) {
          py = (py - 1) & DUNGEON_ROOM_MASK_Y;
          dungeon.rooms[py][px].flag |= DUNGEON_ROOM_FLAG_WALL_V;
          mask = DUNGEON_WALL_UP | DUNGEON_WALL_LEFT | DUNGEON_WALL_RIGHT;
        } else if (r == DUNGEON_WALL_DOWN) {
          dungeon.rooms[py][px].flag |= DUNGEON_ROOM_FLAG_WALL_V;
          py = (py + 1) & DUNGEON_ROOM_MASK_Y;
          mask = DUNGEON_WALL_DOWN | DUNGEON_WALL_LEFT | DUNGEON_WALL_RIGHT;
        } else if (r == DUNGEON_WALL_LEFT) {
          px = (px - 1) & DUNGEON_ROOM_MASK_X;
          dungeon.rooms[py][px].flag |= DUNGEON_ROOM_FLAG_WALL_H;
          mask = DUNGEON_WALL_UP | DUNGEON_WALL_DOWN | DUNGEON_WALL_LEFT;
        } else {
          dungeon.rooms[py][px].flag |= DUNGEON_ROOM_FLAG_WALL_H;
          px = (px + 1) & DUNGEON_ROOM_MASK_X;
          mask = DUNGEON_WALL_UP | DUNGEON_WALL_DOWN | DUNGEON_WALL_RIGHT;
        }
        wall = DungeonGetPoleWall(px, py);
      }
    }
  }

  // 距離の計測
  for (int8_t y = 0; y < DUNGEON_ROOM_SIZE_Y; y++) {
    for (int8_t x = 0; x < DUNGEON_ROOM_SIZE_X; x++) {
      dungeon.rooms[y][x].distance = -1;
    }
  }
  {
    int8_t x = (SystemGetRandom() >> 2) & DUNGEON_ROOM_MASK_X;
    int8_t y = (SystemGetRandom() >> 2) & DUNGEON_ROOM_MASK_Y;
    dungeon.rooms[y][x].distance = 0;
    dungeon.start = (y << DUNGEON_ROOM_SHIFT) | x;
    DungeonCalcMazeDistance(x, y);
  }

  // 外周の調整
  for (int8_t x = 0; x < DUNGEON_ROOM_SIZE_X; x++) {
    if (((SystemGetRandom() >> 4) & 0x07) < 0x03) {
      dungeon.rooms[0][x].flag ^= DUNGEON_ROOM_FLAG_WALL_H;
      if (DungeonGetPoleWall(x, 0) == 0) {
        dungeon.rooms[0][x].flag |= DUNGEON_ROOM_FLAG_WALL_H;
      }
    }
  }
  for (int8_t y = 0; y < DUNGEON_ROOM_SIZE_Y; y++) {
    if (((SystemGetRandom() >> 4) & 0x07) < 0x03) {
      dungeon.rooms[y][0].flag ^= DUNGEON_ROOM_FLAG_WALL_V;
      if (DungeonGetPoleWall(0, y) == 0) {
        dungeon.rooms[y][0].flag |= DUNGEON_ROOM_FLAG_WALL_V;
      }
    }
  }
  
  // 距離順に並べる
  for (int8_t i = 0; i < DUNGEON_ROOM_SIZE; i++) {
    dungeonOrders[i] = i;
  }
  for (int8_t i = 1; i < DUNGEON_ROOM_SIZE; i++) {
    int8_t t = dungeonOrders[i];
    int8_t d = dungeon.rooms[(t >> DUNGEON_ROOM_SHIFT) & DUNGEON_ROOM_MASK_Y][t & DUNGEON_ROOM_MASK_X].distance;
    if (dungeon.rooms[(dungeonOrders[i - 1] >> DUNGEON_ROOM_SHIFT) & DUNGEON_ROOM_MASK_Y][dungeonOrders[i - 1] & DUNGEON_ROOM_MASK_X].distance > d) {
      int8_t j = i;
      do {
        dungeonOrders[j] = dungeonOrders[j - 1];
        --j;
      } while (j > 0 && dungeon.rooms[(dungeonOrders[j - 1] >> DUNGEON_ROOM_SHIFT) & DUNGEON_ROOM_MASK_Y][dungeonOrders[j - 1] & DUNGEON_ROOM_MASK_X].distance > d);
      dungeonOrders[j] = t;
    }
  }
  for (int8_t i = 0; i < DUNGEON_ROOM_SIZE; i++) {
    int8_t j = dungeonOrders[i];
    dungeon.rooms[(j >> DUNGEON_ROOM_SHIFT) & DUNGEON_ROOM_MASK_Y][j & DUNGEON_ROOM_MASK_X].order = i;
  }
}

/*
 * 迷路の距離を計測する
 */
static void DungeonCalcMazeDistance(int8_t x, int8_t y)
{
  int8_t d_0 = dungeon.rooms[y][x].distance;
  uint8_t wall = DungeonGetRoomWall(x, y);
  if ((wall & DUNGEON_WALL_UP) == 0) {
    int8_t d_1 = dungeon.rooms[y - 1][x].distance;
    if (d_1 < 0 || d_1 >= d_0 + 1) {
      dungeon.rooms[y - 1][x].distance = d_0 + 1;
      DungeonCalcMazeDistance(x, y - 1);
    }
  }
  if ((wall & DUNGEON_WALL_DOWN) == 0) {
    int8_t d_1 = dungeon.rooms[y + 1][x].distance;
    if (d_1 < 0 || d_1 >= d_0 + 1) {
      dungeon.rooms[y + 1][x].distance = d_0 + 1;
      DungeonCalcMazeDistance(x, y + 1);
    }
  }
  if ((wall & DUNGEON_WALL_LEFT) == 0) {
    int8_t d_1 = dungeon.rooms[y][x - 1].distance;
    if (d_1 < 0 || d_1 >= d_0 + 1) {
      dungeon.rooms[y][x - 1].distance = d_0 + 1;
      DungeonCalcMazeDistance(x - 1, y);
    }
  }
  if ((wall & DUNGEON_WALL_RIGHT) == 0) {
    int8_t d_1 = dungeon.rooms[y][x + 1].distance;
    if (d_1 < 0 || d_1 >= d_0 + 1) {
      dungeon.rooms[y][x + 1].distance = d_0 + 1;
      DungeonCalcMazeDistance(x + 1, y);
    }
  }
}

/*
 * ルームの壁を取得する
 */
static uint8_t DungeonGetRoomWall(int8_t x, int8_t y)
{
  uint8_t wall = DUNGEON_WALL_NULL;
  x = x & DUNGEON_ROOM_MASK_X;
  y = y & DUNGEON_ROOM_MASK_Y;
  if ((dungeon.rooms[y][x].flag & DUNGEON_ROOM_FLAG_WALL_H) != 0) {
    wall |= DUNGEON_WALL_UP;
  }
  if ((dungeon.rooms[(y + 1) & DUNGEON_ROOM_MASK_Y][x].flag & DUNGEON_ROOM_FLAG_WALL_H) != 0) {
    wall |= DUNGEON_WALL_DOWN;
  }
  if ((dungeon.rooms[y][x].flag & DUNGEON_ROOM_FLAG_WALL_V) != 0) {
    wall |= DUNGEON_WALL_LEFT;
  }
  if ((dungeon.rooms[y][(x + 1) & DUNGEON_ROOM_MASK_X].flag & DUNGEON_ROOM_FLAG_WALL_V) != 0) {
    wall |= DUNGEON_WALL_RIGHT;
  }
  return wall;
}

/*
 * 柱の壁を取得する
 */
static uint8_t DungeonGetPoleWall(int8_t x, int8_t y)
{
  uint8_t wall = DUNGEON_WALL_NULL;
  x = x & DUNGEON_ROOM_MASK_X;
  y = y & DUNGEON_ROOM_MASK_Y;
  if ((dungeon.rooms[(y - 1) & DUNGEON_ROOM_MASK_Y][x].flag & DUNGEON_ROOM_FLAG_WALL_V) != 0) {
    wall |= DUNGEON_WALL_UP;
  }
  if ((dungeon.rooms[y][x].flag & DUNGEON_ROOM_FLAG_WALL_V) != 0) {
    wall |= DUNGEON_WALL_DOWN;
  }
  if ((dungeon.rooms[y][(x - 1) & DUNGEON_ROOM_MASK_X].flag & DUNGEON_ROOM_FLAG_WALL_H) != 0) {
    wall |= DUNGEON_WALL_LEFT;
  }
  if ((dungeon.rooms[y][x].flag & DUNGEON_ROOM_FLAG_WALL_H) != 0) {
    wall |= DUNGEON_WALL_RIGHT;
  }
  return wall;
}

/*
 * ルームとのコリジョンを判定する
 */
bool DungeonIsCollision(int8_t index, int8_t x, int8_t y)
{
  bool result = false;
  uint8_t wall = DungeonGetRoomWall(index & DUNGEON_ROOM_MASK_X, (index >> DUNGEON_ROOM_SHIFT) & DUNGEON_ROOM_MASK_Y);
  if (y < DUNGEON_ROOM_WALL_TOP) {
    if ((wall & DUNGEON_WALL_UP) != 0 || x < DUNGEON_ROOM_PATH_LEFT || x > DUNGEON_ROOM_PATH_RIGHT) {
      result = true;
    }
  }
  if (y > DUNGEON_ROOM_WALL_BOTTOM) {
    if ((wall & DUNGEON_WALL_DOWN) != 0 || x < DUNGEON_ROOM_PATH_LEFT || x > DUNGEON_ROOM_PATH_RIGHT) {
      result = true;
    }
  }
  if (x < DUNGEON_ROOM_WALL_LEFT) {
    if ((wall & DUNGEON_WALL_LEFT) != 0 || y < DUNGEON_ROOM_PATH_TOP || y > DUNGEON_ROOM_PATH_BOTTOM) {
      result = true;
    }
  }
  if (x > DUNGEON_ROOM_WALL_RIGHT) {
    if ((wall & DUNGEON_WALL_RIGHT) != 0 || y < DUNGEON_ROOM_PATH_TOP || y > DUNGEON_ROOM_PATH_BOTTOM) {
      result = true;
    }
  }
  return result;
}
bool DungeonIsCollision(int8_t index, int8_t left, int8_t top, int8_t right, int8_t bottom)
{
  bool result = false;
  uint8_t wall = DungeonGetRoomWall(index & DUNGEON_ROOM_MASK_X, (index >> DUNGEON_ROOM_SHIFT) & DUNGEON_ROOM_MASK_Y);
  if (top < DUNGEON_ROOM_WALL_TOP) {
    if ((wall & DUNGEON_WALL_UP) != 0 || left < DUNGEON_ROOM_PATH_LEFT || right > DUNGEON_ROOM_PATH_RIGHT) {
      result = true;
    }
  }
  if (bottom > DUNGEON_ROOM_WALL_BOTTOM) {
    if ((wall & DUNGEON_WALL_DOWN) != 0 || left < DUNGEON_ROOM_PATH_LEFT || right > DUNGEON_ROOM_PATH_RIGHT) {
      result = true;
    }
  }
  if (left < DUNGEON_ROOM_WALL_LEFT) {
    if ((wall & DUNGEON_WALL_LEFT) != 0 || top < DUNGEON_ROOM_PATH_TOP || bottom > DUNGEON_ROOM_PATH_BOTTOM) {
      result = true;
    }
  }
  if (right > DUNGEON_ROOM_WALL_RIGHT) {
    if ((wall & DUNGEON_WALL_RIGHT) != 0 || top < DUNGEON_ROOM_PATH_TOP || bottom > DUNGEON_ROOM_PATH_BOTTOM) {
      result = true;
    }
  }
  return result;
}
bool DungeonIsCollision(int8_t left, int8_t top, int8_t right, int8_t bottom)
{
  return (top < DUNGEON_ROOM_WALL_TOP || bottom > DUNGEON_ROOM_WALL_BOTTOM || left < DUNGEON_ROOM_WALL_LEFT || right > DUNGEON_ROOM_WALL_RIGHT) ? true : false;
}

/*
 * ルームに入る
 */
void DungeonEnterRoom(int8_t index)
{
  // アイテムの出現
  if (DungeonGetItem(index) != ITEM_NULL) {
    dungeon.item.x = DUNGEON_ROOM_REGION_X + DUNGEON_ROOM_REGION_WIDTH / 2;
    dungeon.item.y = DUNGEON_ROOM_REGION_Y + DUNGEON_ROOM_REGION_HEIGHT / 2;
  }
}

/*
 * ルームから出る
 */
bool DungeonExitRoom(int8_t *index, int8_t *x, int8_t *y)
{
  bool result = false;
  int8_t ix = *index & DUNGEON_ROOM_MASK_X;
  int8_t iy = (*index >> DUNGEON_ROOM_SHIFT) & DUNGEON_ROOM_MASK_Y;
  if (*y < DUNGEON_ROOM_REGION_TOP) {
    *y = DUNGEON_ROOM_REGION_BOTTOM;
    iy = iy - 1;
    result = true;
  } else if (*y > DUNGEON_ROOM_REGION_BOTTOM) {
    *y = DUNGEON_ROOM_REGION_TOP;
    iy = iy + 1;
    result = true;
  } else if (*x < DUNGEON_ROOM_REGION_LEFT) {
    *x = DUNGEON_ROOM_REGION_RIGHT;
    ix = ix - 1;
    result = true;
  } else if (*x > DUNGEON_ROOM_REGION_RIGHT) {
    *x = DUNGEON_ROOM_REGION_LEFT;
    ix = ix + 1;
    result = true;
  }
  *index = ((iy & DUNGEON_ROOM_MASK_Y) << DUNGEON_ROOM_SHIFT) | (ix & DUNGEON_ROOM_MASK_X);
  return result;
}

/*
 * ルームを取得する
 */
static Dungeon::Room *DungeonGetRoom(int8_t index)
{
  return &dungeon.rooms[(index >> DUNGEON_ROOM_SHIFT) & DUNGEON_ROOM_MASK_Y][index & DUNGEON_ROOM_MASK_X];
}

/*
 * 距離が０のルームを取得する
 */
void DungeonGetStartRoom(int8_t *index, int8_t *x, int8_t *y)
{
  *index = dungeon.start;
  *x = DUNGEON_ROOM_REGION_WIDTH / 2 + DUNGEON_ROOM_REGION_X;
  *y = DUNGEON_ROOM_REGION_HEIGHT * 3 / 4 + DUNGEON_ROOM_REGION_Y;
}

/*
 * ルームの順番を取得する
 */
int8_t DungeonGetRoomOrder(int8_t index)
{
  return DungeonGetRoom(index)->order;
}

/*
 * エネミーを１体殺す
 */
int8_t DungeonKillEnemy(int8_t index)
{
  struct Dungeon::Room *room = DungeonGetRoom(index);
  if (room->rest > 0) {
    int8_t type = DungeonGetEnemyType(index);
    if (type == ENEMY_TYPE_GARGOYLE) {
      if (PlayerIsItem(ITEM_HAMMER)) {
        --room->rest;
      }
    } else if (type == ENEMY_TYPE_REAPER) {
      if (PlayerIsItem(ITEM_CANDLE)) {
        --room->rest;
      }
    } else if (type == ENEMY_TYPE_SHADOW) {
      if (PlayerIsItem(ITEM_MIRROR)) {
        --room->rest;
      }
    } else {
      --room->rest;
    }
  }
  return room->rest;
}

/*
 * エネミーの種類を取得する
 */
int8_t DungeonGetEnemyType(int8_t index)
{
  return (int8_t)pgm_read_byte(&dungeonRooms[DungeonGetRoomOrder(index)].enemyType);
}

/*
 * エネミーの数を取得する
 */
int8_t DungeonGetEnemyRest(int8_t index)
{
  return DungeonGetRoom(index)->rest;
}

/*
 * エネミーの位置を取得する
 */
void DungeonGetEnemyLocation(int8_t order, int8_t rest, int8_t *x, int8_t *y)
{
  struct DungeonLocation *location = NULL;
  if (rest > 5) {
    location = (DungeonLocation *)dungeonEnemyLocations_9;
  } else if (rest == 5) {
    location = (DungeonLocation *)dungeonEnemyLocations_5;
  } else if (rest > 1) {
    location = (DungeonLocation *)dungeonEnemyLocations_4;
  } else {
    location = (DungeonLocation *)dungeonEnemyLocations_1;
  }
  *x = (int8_t)pgm_read_byte(&location[order].x);
  *y = (int8_t)pgm_read_byte(&location[order].y);
}

/*
 * アイテムを発見する
 */
void DungeonFindItem(int8_t index)
{
  // ルームの取得
  struct Dungeon::Room *room = DungeonGetRoom(index);

  // アイテムの取得
  int8_t item = (int8_t)pgm_read_byte(&dungeonRooms[room->order].item);
  if (ITEM_BOOTS <= item && item <= ITEM_KEY) {
    int8_t type = DungeonGetEnemyType(index);
    for (int8_t i = 0; i < DUNGEON_ROOM_SIZE; i++) {
      if (DungeonGetEnemyType(i) == type && DungeonGetEnemyRest(i) > 0) {
        item = ITEM_NULL;
        break;
      }
    }
  }
  
  // アイテムの存在
  if (item != ITEM_NULL) {
    
    // プレイヤの位置の取得
    int8_t px, py;
    PlayerGetPosition(&px, &py);
  
    // アイテムの位置の取得
    dungeon.item.x = DUNGEON_ROOM_REGION_X + DUNGEON_ROOM_REGION_WIDTH / 4;
    if (px < DUNGEON_ROOM_REGION_X + DUNGEON_ROOM_REGION_WIDTH / 2) {
      dungeon.item.x += DUNGEON_ROOM_REGION_WIDTH / 2;
    }
    dungeon.item.y = DUNGEON_ROOM_REGION_Y + DUNGEON_ROOM_REGION_HEIGHT / 4;
    if (py < DUNGEON_ROOM_REGION_Y + DUNGEON_ROOM_REGION_HEIGHT / 2) {
      dungeon.item.y += DUNGEON_ROOM_REGION_HEIGHT / 2;
    }

    // アイテムの出現
    room->flag |= DUNGEON_ROOM_FLAG_ITEM;
  }
}

/*
 * アイテムを拾う
 */
int8_t DungeonPickUpItem(int8_t index, int8_t x, int8_t y)
{
  int8_t item = DungeonGetItem(index);
  if (item != ITEM_NULL) {
    if (x >= dungeon.item.x + DUNGEON_ITEM_RECT_LEFT && x <= dungeon.item.x + DUNGEON_ITEM_RECT_RIGHT && y >= dungeon.item.y + DUNGEON_ITEM_RECT_TOP && y <= dungeon.item.y + DUNGEON_ITEM_RECT_BOTTOM) {
      if (item == ITEM_DOOR) {
        if (PlayerIsItem(ITEM_KEY)) {
          item = ITEM_EXIT;
        }
      } else {
        DungeonGetRoom(index)->flag ^= DUNGEON_ROOM_FLAG_ITEM;
      }
    } else {
      item = ITEM_NULL;
    }
  }
  return item;
}

/*
 * アイテムを取得する
 */
static int8_t DungeonGetItem(int8_t index)
{
  struct Dungeon::Room *room = DungeonGetRoom(index);
  int8_t item = ITEM_NULL;
  if ((room->flag & DUNGEON_ROOM_FLAG_ITEM) != 0) {
    item = (int8_t)pgm_read_byte(&dungeonRooms[room->order].item);
  }
  return item;
}
