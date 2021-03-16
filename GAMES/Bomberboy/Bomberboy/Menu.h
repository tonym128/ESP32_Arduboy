#ifndef MENU_H
#define MENU_H

#include "defines.h"
#include <stdint.h>

namespace Bomberboy
{
  
class Menu
{
public:
  static int16_t m_bomb_speed;
  static int16_t m_bomb_pos;
  static uint8_t m_logo_pos;
  static uint8_t m_button_filter;
  static uint16_t m_present_time;
  static uint8_t  m_difficult_level;

  static void Init();
  static bool Control(Arduboy2& arduboy, uint8_t buttons, uint16_t frame_number);
  static void Draw(Arduboy2& arduboy);
};

class FinalCutScene
{
public:
  static void Init();
  static bool Control(uint8_t buttons, uint16_t frame_number);
  static void Draw(Arduboy2& arduboy);
};

class GameInfoScene
{
public:
  static void DrawBonusInfo(Arduboy2& arduboy, int16_t x, int16_t &y, int16_t step);
  static void DrawEnemyInfo(Arduboy2& arduboy, int16_t x, int16_t &y, int16_t step);
  static void Init();
  static bool Control(uint8_t buttons, uint16_t frame_number);
  static void Draw(Arduboy2& arduboy);
};

}

#endif
