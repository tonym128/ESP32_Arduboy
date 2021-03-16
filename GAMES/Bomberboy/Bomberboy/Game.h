#ifndef GAME_H
#define GAME_H

#include "Unit.h"
#include "Player.h"

#define BONUS_BOMB_AMOUNT       0
#define BONUS_BOMB_RADIUS       1
#define BONUS_SPEED             2
#define BONUS_PROTECTION        3
#define BONUS_GO_THROUGH_WALLS  4
#define BONUS_GO_THROUGH_BOMBS  5
#define BONUS_FREEZE_TIME       6
#define BONUS_MANUAL_EXPLOSION  7

#define GAME_FLAG_GAME_OVER     1
#define GAME_FLAG_LEVEL_DONE    2
#define GAME_FLAG_PLAYER_DIE    4
#define GAME_FLAG_PAUSE_DEATH   8
#define GAME_FLAG_PAUSE_LEVEL_DONE    16

#define PAUSE_MODE_STATS 1
#define PAUSE_MODE_INFO  2

namespace Bomberboy
{

class Game
{
public:
  static Unit m_units[UNITS_MAX];
  static Player m_player;
  static int16_t m_draw_offset_x;
  static int16_t m_draw_offset_y;
  static uint8_t m_level;
  static uint8_t m_splash_level;
  static uint8_t m_pause_toggle;
  static uint8_t m_pause_mode;
  static uint8_t m_ghost_left;
  static uint8_t m_ghost_freeze;

  static uint8_t m_bonus_cell_x;
  static uint8_t m_bonus_cell_y;
  static uint8_t m_bonus_type;
  static uint8_t m_exit_cell_x;
  static uint8_t m_exit_cell_y;
  static uint8_t m_flags;
  static uint8_t m_game_pause_music;

  static void Init();

  static void StartLevel();
  static void PutEmenies(uint8_t unit_type, uint8_t amount);
  
  static bool Control(uint8_t buttons, uint16_t frame_number);
  static void Draw(Arduboy2& arduboy);
};

}

#endif
