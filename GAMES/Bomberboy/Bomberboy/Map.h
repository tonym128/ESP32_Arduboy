#ifndef MAP_H
#define MAP_H

#include "defines.h"
#include <stdint.h>

namespace Bomberboy
{

#define CELL_EMPTY        0x00
#define CELL_SOLID        0x01
#define CELL_BRICK        0x02
#define CELL_BOMB_INITIAL 0x08
#define CELL_BONUS        0x0C
#define CELL_EXIT         0x28
#define CELL_BRICK_CUT_SCENE 0x06

#define CELL_BOMB_EXPLOSION             0x10
#define CELL_BOMB_EXPLOSION_HORIZONTAL  0x18
#define CELL_BOMB_EXPLOSION_VERTICAL    0x20
#define CELL_BOMB_EXPLOSION_LAST        0x27

#define BOMB_FLAG_LEFT  1
#define BOMB_FLAG_RIGHT 2
#define BOMB_FLAG_UP    4
#define BOMB_FLAG_DOWN  8

struct Bomb
{
  uint8_t cell_x;           //5 bits
  uint8_t cell_y;           //4 bits
  uint8_t activation_time;  //8bits
  uint8_t radius;           //5 bits
  uint8_t radius_counter;   //5bits
  uint8_t flags;            //2 bits
};

class Map
{
public:
  static uint8_t m_width;
  static uint8_t m_height;
  static uint8_t m_cell[MAP_WIDTH_MAX*MAP_HEIGHT_MAX];
  
  static Bomb m_bombs[BOMBS_MAX];
  static uint8_t FindUnusedBombIndex();
  static void CleanBombsOnLevel();

  static void Init(uint8_t width, uint8_t height, uint8_t fill_rate);
  static void Control(uint16_t frame_number);
  static void Draw();
};

}

#endif
