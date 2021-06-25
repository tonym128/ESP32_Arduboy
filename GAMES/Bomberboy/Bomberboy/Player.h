#ifndef PLAYER_H
#define PLAYER_H

#include "defines.h"
#include <stdint.h>

namespace Bomberboy
{
  
#define PLAYER_UPGRADE_SPEED_2            0x01
#define PLAYER_UPGRADE_SPEED_3            0x02
#define PLAYER_UPGRADE_GO_THROUGH_BOMBS   0x04
#define PLAYER_UPGRADE_GO_THROUGH_WALLS   0x08
#define PLAYER_UPGRADE_NO_BOMB_DAMAGE     0x10
#define PLAYER_UPGRADE_MANUAL_EXPLOSION   0x20

struct Player
{
  uint8_t x;
  uint8_t y;
  uint8_t movement_frame;
  uint8_t flags;  //000LAPoo    live:1 active:1 orientation:2
  uint8_t lives;
  uint8_t upgrade;
  uint8_t bomb_maximum;
  uint8_t bomb_put;
  uint8_t bomb_radius;
  uint8_t bomb_last_cell_x;
  uint8_t bomb_last_cell_y;
  uint8_t invulnerability;

  static void LevelReset(Player* player);
  static void Control(Player* player, uint8_t buttons, uint16_t frame_number);
  static void Draw(Player* player, uint16_t frame_number);
};

}

#endif
