#include "Menu.h"
#include "assets.h"
#include "Game.h"
#include "Map.h"
#include "Unit.h"
#include "Audio.h"

namespace Bomberboy
{
  
int16_t Menu::m_bomb_speed;
int16_t Menu::m_bomb_pos;
uint8_t Menu::m_logo_pos;
uint8_t Menu::m_button_filter;
uint16_t Menu::m_present_time;
uint8_t Menu::m_difficult_level = 0;
  
void Menu::Init()
{
  Arduboy2Core::setRGBled(RED_LED, 0);
  Arduboy2Core::setRGBled(GREEN_LED, 0);
  Arduboy2Core::setRGBled(BLUE_LED, 0);

  sound.tones(s_music_title);

  m_bomb_speed = 0;
  m_bomb_pos = -16*32;
  m_logo_pos = 128;
  m_button_filter = 0;
  m_present_time = 0;

  //Use bomb to animate particle
  for (uint8_t i = 0; i < BOMBS_MAX; ++i)
    Map::m_bombs[i].activation_time = random(80) + 11;
}

bool Menu::Control(Arduboy2& arduboy, uint8_t buttons, uint16_t frame_number)
{
  m_present_time ++;

  //Animate bomb
  m_bomb_speed += 4;
  m_bomb_pos += m_bomb_speed;
  if ( (m_bomb_pos >> 5) >= 16 && m_bomb_speed > 0)
  {
    m_bomb_pos = 16 << 5;
    if (m_bomb_speed > (3 << 5))
      m_bomb_speed = (3 << 5);
    m_bomb_speed = -m_bomb_speed;
  }

  //Move logo
  m_logo_pos = (m_logo_pos*110 + 12*18) >> 7;

  for (uint8_t i = 0; i < BOMBS_MAX; ++i)
  {
    if (Map::m_bombs[i].activation_time == 0)
    {
      Map::m_bombs[i].cell_x = 32+20;
      Map::m_bombs[i].cell_y = (m_bomb_pos >> 5) + 0;
      Map::m_bombs[i].radius = random(PARTICLE_COUNT);
      Map::m_bombs[i].radius_counter = 0;
      Map::m_bombs[i].activation_time = 255;
    } else if (Map::m_bombs[i].activation_time != 255)
      Map::m_bombs[i].activation_time--;
    else
    {
      Map::m_bombs[i].radius_counter++;
      if (Map::m_bombs[i].radius_counter >= PARTICLE_FRAMES)
      {
        Map::m_bombs[i].cell_x = 32+20;
        Map::m_bombs[i].cell_y = (m_bomb_pos >> 5) + 0;
        Map::m_bombs[i].radius = random(PARTICLE_COUNT);
        Map::m_bombs[i].radius_counter = 0;
      }
    }
  }

  if ( (uint8_t)(buttons & A_BUTTON) != 0 )
    return true;

  if ( ((uint8_t)(buttons & LEFT_BUTTON) != 0 || (uint8_t)(buttons & RIGHT_BUTTON) != 0)
       && m_button_filter == 0)
  {
    m_button_filter = 10;
    m_difficult_level += 1;
    if (m_difficult_level > 1)
      m_difficult_level = 0;
  }

  if ( (uint8_t)(buttons & B_BUTTON) != 0 && m_button_filter == 0)
  {
    m_button_filter = 10;
    if (arduboy.audio.enabled())
      arduboy.audio.off();
    else
      arduboy.audio.on();
  }
  if (m_button_filter != 0)
    m_button_filter--;

  return false;
}

void Menu::Draw(Arduboy2& arduboy)
{
  Arduboy2::drawBitmap(m_logo_pos, 13, s_logo, 112, 32);
  Sprites::drawPlusMask(32, m_bomb_pos >> 5, s_bomb, 0);
  arduboy.setCursor(28 - m_logo_pos, 44);
  arduboy.print(F("Press A to start"));

  arduboy.setCursor(64 - m_logo_pos, 54);
  if (arduboy.audio.enabled())
    arduboy.print(F("B audio ON"));
  else
    arduboy.print(F("B audio OFF"));

  arduboy.setCursor(12 - (int16_t)m_logo_pos, 54);
  if (m_difficult_level == 1)
    arduboy.print(F("<>Hard"));
  else
    arduboy.print(F("<>Easy"));

  for (uint8_t i = 0; i < BOMBS_MAX; ++i)
  {
    if (Map::m_bombs[i].activation_time != 255)
      continue;
    uint16_t index = (uint16_t)Map::m_bombs[i].radius*PARTICLE_FRAMES*2 + (uint16_t)Map::m_bombs[i].radius_counter*2;
    int8_t dx = (int8_t)pgm_read_byte(s_particle + index );
    int8_t dy = (int8_t)pgm_read_byte(s_particle + index + 1 );
    arduboy.drawPixel((int16_t)Map::m_bombs[i].cell_x + dx, (int16_t)Map::m_bombs[i].cell_y + dy);
  }
}


void FinalCutScene::Init()
{
  Menu::m_logo_pos = 64;
  Menu::m_bomb_pos = -8;
  sound.tones(s_music_game_complete);
}

bool FinalCutScene::Control(uint8_t buttons, uint16_t frame_number)
{
  if ((uint8_t)(frame_number & 7) == 0)
  {
    Menu::m_bomb_pos += 1;
    if (Menu::m_bomb_pos >= 120)
      return true;
  }
  if ((uint8_t)(frame_number & 3) == 0)
  {
    if (Menu::m_logo_pos > 0)
      Menu::m_logo_pos --;
  }
  return false;
}

void FinalCutScene::Draw(Arduboy2& arduboy)
{
  arduboy.setCursor((128-90)/2, Menu::m_logo_pos);
  arduboy.print(F("CONGRATULATIONS"));

  arduboy.setCursor((128-108)/2, Menu::m_logo_pos+10);
  arduboy.print(F("BOMBER BOY BECOMES"));
  
  arduboy.setCursor(90, Menu::m_logo_pos+20);
  arduboy.print(F("RUNNER"));
  
  arduboy.setCursor((128-90)/2, Menu::m_logo_pos+30);
  arduboy.print(F("EVGENYKZZ2 2021"));
  
  arduboy.setCursor((128-42)/2, Menu::m_logo_pos+40);
  arduboy.print(F("ARDUBOY"));
  
  arduboy.fillRect(0, 56, 128, 8, BLACK);
  
  for (uint8_t x = 0; x < 16; ++x)
    Arduboy2::drawBitmap(x*8, 56, s_tiles + (CELL_BRICK_CUT_SCENE + (x&1))*8, 8, 8);
  Sprites::drawPlusMask(Menu::m_bomb_pos, 48, s_sprites+(4 + (Menu::m_bomb_pos&3))*18, 0);
}

void GameInfoScene::Init()
{
  Menu::m_bomb_pos = 64;
  Menu::m_present_time = 0;
}

bool GameInfoScene::Control(uint8_t buttons, uint16_t frame_number)
{
  Menu::m_present_time ++;
  if (buttons != 0 || Menu::m_present_time >= GAME_INFO_ACTIVATION_FRAMES)
    return true;
  if ((uint8_t)(frame_number & 1) == 0)
  {
    if (Menu::m_logo_pos > -100)
      Menu::m_bomb_pos --;
  }
  return false;
}

void GameInfoScene::DrawBonusInfo(Arduboy2& arduboy, int16_t x, int16_t &y, int16_t step)
{ 
  uint8_t f = arduboy.frameCount & 3;
  if (y < 64 && y > -8)
  {
    Arduboy2::drawBitmap(x, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
    Sprites::drawPlusMask(x, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_BOMB_AMOUNT)*18, 0);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("One more bomb"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    Arduboy2::drawBitmap(x, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
    Sprites::drawPlusMask(x, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_BOMB_RADIUS)*18, 0);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Radius"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    Arduboy2::drawBitmap(x, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
    Sprites::drawPlusMask(x, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_SPEED)*18, 0);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Speed"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    Arduboy2::drawBitmap(x, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
    Sprites::drawPlusMask(x, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_PROTECTION)*18, 0);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Protection"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    Arduboy2::drawBitmap(x, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
    Sprites::drawPlusMask(x, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_GO_THROUGH_WALLS)*18, 0);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Walk over walls"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    Arduboy2::drawBitmap(x, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
    Sprites::drawPlusMask(x, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_GO_THROUGH_BOMBS)*18, 0);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Walk over bombs"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    Arduboy2::drawBitmap(x, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
    Sprites::drawPlusMask(x, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_FREEZE_TIME)*18, 0);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Freeze ghosts"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    Arduboy2::drawBitmap(x, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
    Sprites::drawPlusMask(x, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_MANUAL_EXPLOSION)*18, 0);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Bomb control"));
  }
  y += step;
}

static void DrawEnemy(uint8_t type, int16_t x, int16_t y, uint16_t frame_number)
{
  uint8_t sprite = pgm_read_byte(s_unit_info + type*2);
  uint8_t flags = pgm_read_byte(s_unit_info +type*2+1);

  uint8_t f;
  if ((uint8_t)(flags & UNIT_AI_8_FRAME))
    f = (frame_number / 4) & 7;
  else
  {
    f = (frame_number / 4) & 3;
    f += 4; //Right
  }
  Sprites::drawPlusMask(x, y, s_sprites+(uint16_t)(f+sprite)*18, 0);
}

void GameInfoScene::DrawEnemyInfo(Arduboy2& arduboy, int16_t x, int16_t &y, int16_t step)
{
  if (y < 64 && y > -8)
  {
    DrawEnemy(UNIT_BALLOM, x, y, arduboy.frameCount);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Ballom"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    DrawEnemy(UNIT_ONIL, x, y, arduboy.frameCount);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Onil"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    DrawEnemy(UNIT_DAHL, x, y, arduboy.frameCount);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Dahl"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    DrawEnemy(UNIT_MINVO, x, y, arduboy.frameCount);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Minvo"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    DrawEnemy(UNIT_DORIA, x, y, arduboy.frameCount);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Doria"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    DrawEnemy(UNIT_OVAPE, x, y, arduboy.frameCount);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Ovape"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    DrawEnemy(UNIT_PONTAN, x, y, arduboy.frameCount);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Pontan"));
  }
  y += step;

  if (y < 64 && y > -8)
  {
    DrawEnemy(UNIT_PASS, x, y, arduboy.frameCount);
    arduboy.setCursor(x + 10, y);
    arduboy.print(F("Pass"));
  }
  y += step;
}

void GameInfoScene::Draw(Arduboy2& arduboy)
{
  for (int16_t i = -8; i < 16; ++i)
  {
     Arduboy2::drawBitmap(  0, i*8 + Menu::m_bomb_pos/4, s_tiles + (CELL_BRICK)*8, 8, 8);
     Arduboy2::drawBitmap(120, i*8 + Menu::m_bomb_pos/4, s_tiles + (CELL_BRICK)*8, 8, 8);
  }
  static const int16_t step = 10;
  static const int16_t x = 16;
  int16_t y = Menu::m_bomb_pos;

  arduboy.setCursor(x, y);
  arduboy.print(F("Bomberboy "));
  arduboy.print(F(VERSION));
  y += step;
  
  arduboy.setCursor(x, y);
  arduboy.print(F("Evgenykz 2021"));
  y += step;
  y += step;
  
  if (y < 64 && y > -8)
  {
    arduboy.setCursor(x, y);
    arduboy.print(F("----Bonuses----"));
  }
  y += step;
  DrawBonusInfo(arduboy, x, y, step);

  y += step;
  y += step;
  if (y < 64 && y > -8)
  {
    arduboy.setCursor(x, y);
    arduboy.print(F("----Ghosts----"));
  }
  y += step;
  DrawEnemyInfo(arduboy, x, y, step);
}

}
