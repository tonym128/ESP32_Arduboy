#ifndef UNIT_H
#define UNIT_H

#include "defines.h"
#include <stdint.h>

namespace Bomberboy
{

#define UNIT_SPRITE_DIE     0x18
#define UNIT_SPRITE_BONUS   0x20

#define UNIT_SPRITE_BALLOM  0x10
#define UNIT_SPRITE_ONIL    0x28
#define UNIT_SPRITE_DAHL    0x30
#define UNIT_SPRITE_MINVO   0x38
#define UNIT_SPRITE_DORIA   0x40    //8 frames animation, symetry
#define UNIT_SPRITE_OVAPE   0x48
#define UNIT_SPRITE_PONTAN  0x50    //8 frames animation, symetry
#define UNIT_SPRITE_PASS    0x58

#define UNIT_AI_SPEED_MASK  0x03
#define UNIT_AI_PASS_WALS   0x04
#define UNIT_AI_TURN        0x08
#define UNIT_AI_ATTACK      0x10
#define UNIT_AI_8_FRAME     0x20
//#define UNIT_AI_HIDE        0x40

#define UNIT_BALLOM  0
#define UNIT_ONIL    1
#define UNIT_DAHL    2
#define UNIT_MINVO   3
#define UNIT_DORIA   4
#define UNIT_OVAPE   5
#define UNIT_PONTAN  6
#define UNIT_PASS    7

struct Unit
{
  uint8_t x;
  uint8_t y;
  uint8_t flags;  //000LAPoo    live:1 active:1 orientation:2
  uint8_t type;   //

  static int8_t s_orientation_dx[4];
  static int8_t s_orientation_dy[4];

  static void Control(Unit* unit, uint16_t frame_number);
  static void Draw(Unit* unit, uint16_t frame_number);
};

extern const uint8_t s_unit_info[];

}

#endif
