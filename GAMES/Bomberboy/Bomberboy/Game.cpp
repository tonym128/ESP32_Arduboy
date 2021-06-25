#include "Game.h"
#include "Map.h"
#include "Menu.h"
#include "assets.h"
#include "audio.h"

namespace Bomberboy
{

Unit Game::m_units[UNITS_MAX];
Player Game::m_player;
int16_t Game::m_draw_offset_x = 0;
int16_t Game::m_draw_offset_y = 0;
uint8_t Game::m_level;
uint8_t Game::m_splash_level;
uint8_t Game::m_pause_toggle;
uint8_t Game::m_pause_mode;
uint8_t Game::m_ghost_left;
uint8_t Game::m_ghost_freeze;

uint8_t Game::m_bonus_cell_x;
uint8_t Game::m_bonus_cell_y;
uint8_t Game::m_bonus_type;
uint8_t Game::m_exit_cell_x;
uint8_t Game::m_exit_cell_y;

uint8_t Game::m_flags;
uint8_t Game::m_game_pause_music;

static const uint8_t s_level_info[]  =
{
  //W H   Bonus                   fill  enemy 1            enemy 2
  17, 9,  BONUS_BOMB_AMOUNT,      20,   UNIT_BALLOM,  2,   UNIT_BALLOM, 0,    // 1
  17, 9,  BONUS_BOMB_RADIUS,      21,   UNIT_BALLOM,  3,   UNIT_BALLOM, 0,    // 2
  17, 9,  BONUS_SPEED,            22,   UNIT_BALLOM,  4,   UNIT_BALLOM, 0,    // 3
  21, 9,  BONUS_BOMB_RADIUS,      23,   UNIT_BALLOM,  4,   UNIT_ONIL,   1,    // 4
  21, 9,  BONUS_MANUAL_EXPLOSION, 24,   UNIT_BALLOM,  4,   UNIT_ONIL,   2,    // 5
  23, 9,  BONUS_BOMB_AMOUNT,      25,   UNIT_BALLOM,  4,   UNIT_ONIL,   2,    // 6
  17,15,  BONUS_BOMB_RADIUS,      26,   UNIT_ONIL,    4,   UNIT_DAHL,   1,    // 7
  17,15,  BONUS_GO_THROUGH_BOMBS, 27,   UNIT_ONIL,    4,   UNIT_DAHL,   2,    // 8
  21,15,  BONUS_PROTECTION,       28,   UNIT_ONIL,    4,   UNIT_DAHL,   3,    // 9
  
  23,15,  BONUS_GO_THROUGH_WALLS, 30,   UNIT_DAHL,    6,   UNIT_MINVO,  2,    // 10
  23,15,  BONUS_MANUAL_EXPLOSION, 30,   UNIT_DAHL,    6,   UNIT_MINVO,  2,    // 11
  23,15,  BONUS_BOMB_AMOUNT,      30,   UNIT_DAHL,    5,   UNIT_MINVO,  3,    // 12
  23,15,  BONUS_BOMB_RADIUS,      30,   UNIT_DAHL,    5,   UNIT_MINVO,  3,    // 13
  23,15,  BONUS_BOMB_RADIUS,      30,   UNIT_DAHL,    4,   UNIT_MINVO,  4,    // 14
  23,15,  BONUS_PROTECTION,       30,   UNIT_DAHL,    4,   UNIT_MINVO,  4,    // 15
  23,15,  BONUS_MANUAL_EXPLOSION, 30,   UNIT_DAHL,    4,   UNIT_MINVO,  4,    // 16
  23,15,  BONUS_BOMB_AMOUNT,      30,   UNIT_DAHL,    4,   UNIT_MINVO,  4,    // 17
  23,15,  BONUS_SPEED,            30,   UNIT_DAHL,    4,   UNIT_MINVO,  4,    // 18
  23,15,  BONUS_BOMB_RADIUS,      30,   UNIT_DAHL,    4,   UNIT_MINVO,  4,    // 19
  
  27,15,  BONUS_BOMB_RADIUS,      40,   UNIT_MINVO,   7,   UNIT_DORIA,  2,    // 20
  27,15,  BONUS_MANUAL_EXPLOSION, 40,   UNIT_MINVO,   7,   UNIT_DORIA,  2,    // 21
  27,15,  BONUS_BOMB_AMOUNT,      40,   UNIT_MINVO,   7,   UNIT_DORIA,  2,    // 22
  27,15,  BONUS_BOMB_RADIUS,      40,   UNIT_MINVO,   7,   UNIT_DORIA,  2,    // 23
  27,15,  BONUS_GO_THROUGH_WALLS, 40,   UNIT_MINVO,   6,   UNIT_DORIA,  3,    // 24
  27,15,  BONUS_BOMB_RADIUS,      40,   UNIT_MINVO,   6,   UNIT_DORIA,  3,    // 25
  27,15,  BONUS_MANUAL_EXPLOSION, 40,   UNIT_MINVO,   6,   UNIT_DORIA,  3,    // 26
  27,15,  BONUS_BOMB_AMOUNT,      40,   UNIT_MINVO,   6,   UNIT_DORIA,  3,    // 27
  27,15,  BONUS_BOMB_RADIUS,      40,   UNIT_MINVO,   5,   UNIT_DORIA,  4,    // 28
  27,15,  BONUS_PROTECTION,       40,   UNIT_MINVO,   5,   UNIT_DORIA,  4,    // 29
  
  29,15,  BONUS_BOMB_RADIUS,      50,   UNIT_OVAPE,  10,   UNIT_PONTAN, 2,    // 30
  29,15,  BONUS_MANUAL_EXPLOSION, 50,   UNIT_OVAPE,  10,   UNIT_PONTAN, 2,    // 31
  29,15,  BONUS_BOMB_AMOUNT,      50,   UNIT_OVAPE,  10,   UNIT_PONTAN, 2,    // 32
  29,15,  BONUS_BOMB_RADIUS,      50,   UNIT_OVAPE,  10,   UNIT_PONTAN, 2,    // 33
  29,15,  BONUS_GO_THROUGH_BOMBS, 50,   UNIT_OVAPE,   9,   UNIT_PONTAN, 3,    // 34
  29,15,  BONUS_BOMB_RADIUS,      50,   UNIT_OVAPE,   9,   UNIT_PONTAN, 3,    // 35
  29,15,  BONUS_MANUAL_EXPLOSION, 50,   UNIT_OVAPE,   9,   UNIT_PONTAN, 3,    // 36
  29,15,  BONUS_BOMB_AMOUNT,      50,   UNIT_OVAPE,   9,   UNIT_PONTAN, 3,    // 37
  29,15,  BONUS_BOMB_RADIUS,      50,   UNIT_OVAPE,   8,   UNIT_PONTAN, 4,    // 38
  29,15,  BONUS_SPEED,            50,   UNIT_OVAPE,   8,   UNIT_PONTAN, 4,    // 39
  
  31,15,  BONUS_BOMB_RADIUS,      60,   UNIT_PONTAN, 14,   UNIT_PASS,   1,    // 40  
  31,15,  BONUS_MANUAL_EXPLOSION, 60,   UNIT_PONTAN, 14,   UNIT_PASS,   1,    // 41
  31,15,  BONUS_GO_THROUGH_WALLS, 60,   UNIT_PONTAN, 13,   UNIT_PASS,   2,    // 42
  31,15,  BONUS_BOMB_RADIUS,      60,   UNIT_PONTAN, 12,   UNIT_PASS,   3,    // 43
  31,15,  BONUS_PROTECTION,       60,   UNIT_PONTAN, 11,   UNIT_PASS,   4,    // 44
  31,15,  BONUS_BOMB_RADIUS,      60,   UNIT_PONTAN, 10,   UNIT_PASS,   5,    // 45
  31,15,  BONUS_MANUAL_EXPLOSION, 60,   UNIT_PASS,   16,   UNIT_DAHL,   0,    // 46
  31,15,  BONUS_BOMB_AMOUNT,      62,   UNIT_PASS,   16,   UNIT_DAHL,   0,    // 47
  31,15,  BONUS_GO_THROUGH_BOMBS, 64,   UNIT_PASS,   16,   UNIT_DAHL,   0,    // 48
  31,15,  BONUS_BOMB_RADIUS,      66,   UNIT_PASS,   16,   UNIT_DAHL,   0,    // 49

  31,15,  BONUS_BOMB_RADIUS,      70,   UNIT_PASS,   16,   UNIT_DAHL,   0,    // 50
};

void Game::Init()
{
  Arduboy2Core::setRGBled(RED_LED, 0);
  Arduboy2Core::setRGBled(GREEN_LED, 0);
  Arduboy2Core::setRGBled(BLUE_LED, 0);

  m_flags = 0;
  m_game_pause_music = 0;

  //Clear player
  m_player.upgrade = 0;
  m_player.flags = 0;
  m_player.lives = 3;
  m_player.bomb_maximum = 1;
  m_player.bomb_put = 0;
  m_player.bomb_radius = 1;
  m_level = 1;
  m_pause_toggle = 0;
  m_ghost_left = 0;

  if (Menu::m_difficult_level == 0)
  {
    m_player.lives = 5;
    m_player.bomb_maximum = 2;
    m_player.bomb_radius = 2;
    m_player.upgrade = PLAYER_UPGRADE_SPEED_2;
  }

  //m_player.bomb_maximum = 4;
  //m_player.bomb_radius = 4;
  //m_player.upgrade = PLAYER_UPGRADE_SPEED_2 | PLAYER_UPGRADE_NO_BOMB_DAMAGE;

  //m_player.bomb_maximum = BOMBS_MAX;
  //m_player.bomb_radius = RADIUS_MAX;
  //m_player.upgrade = PLAYER_UPGRADE_SPEED_3 | PLAYER_UPGRADE_GO_THROUGH_BOMBS | PLAYER_UPGRADE_NO_BOMB_DAMAGE | PLAYER_UPGRADE_GO_THROUGH_WALLS | PLAYER_UPGRADE_MANUAL_EXPLOSION;
  //m_level = 40;
}

void Game::PutEmenies(uint8_t unit_type, uint8_t amount)
{
  if (amount == 0)
    return;
  if (m_ghost_left >= UNITS_MAX)
    return;
  for (uint8_t tries = 0; tries < 254; tries++)
  {
    uint8_t x = random(Map::m_width);
    uint8_t y = random(Map::m_height);
    //Cell is busy?
    if (*(Map::m_cell + y * MAP_WIDTH_MAX + x) != CELL_EMPTY)
      continue;
    //Too close to the player?
    if (abs(x - STARTUP_CELL_X) <= 2 && abs(y - STARTUP_CELL_Y) <= 2)
      continue;
    m_units[m_ghost_left].flags =
              (random(4) & ORIENTATION_MASK) //Orientation
              | UNIT_FLAG_ACTIVE | UNIT_FLAG_ALIVE;
    m_units[m_ghost_left].x = x*8;
    m_units[m_ghost_left].y = y*8;
    m_units[m_ghost_left].type = unit_type;
    m_ghost_left ++;
    amount--;
    if (m_ghost_left >= UNITS_MAX || amount == 0)
      break;
  }
}

void Game::StartLevel()
{
  m_splash_level = LEVEL_TITLE_DURATION_FRAMES;
  sound.tones(s_music_level_start);
  m_game_pause_music = 0;
  m_ghost_freeze = 0;

  uint16_t level_offset = ((uint16_t)(m_level-1) * 8);
  
  //Map::Init(31, 15);  //Maximum
  Map::Init(pgm_read_byte(s_level_info + level_offset + 0), pgm_read_byte(s_level_info + level_offset + 1), pgm_read_byte(s_level_info + level_offset + 3));  //Minimum

  //Prepare player
  Player::LevelReset(&m_player);
  //Get some free space for player
  *(Map::m_cell + STARTUP_CELL_Y * MAP_WIDTH_MAX + STARTUP_CELL_X) = CELL_EMPTY;
  *(Map::m_cell + STARTUP_CELL_Y * MAP_WIDTH_MAX + STARTUP_CELL_X+1) = CELL_EMPTY;
  *(Map::m_cell + (STARTUP_CELL_Y+1) * MAP_WIDTH_MAX + STARTUP_CELL_X) = CELL_EMPTY;
  
  //Clear enemies
  for (uint8_t i = 0; i < UNITS_MAX; ++i)
    m_units[i].flags = 0;

  //Put emenies
  m_ghost_left = 0;
  uint8_t enemy_type =   pgm_read_byte(s_level_info + level_offset + 4);
  uint8_t enemy_amount = pgm_read_byte(s_level_info + level_offset + 5);
  PutEmenies(enemy_type, enemy_amount);

  enemy_type =   pgm_read_byte(s_level_info + level_offset + 6);
  enemy_amount = pgm_read_byte(s_level_info + level_offset + 7);
  PutEmenies(enemy_type, enemy_amount);

  //But bonus
  while (true)
  {
    uint8_t x = random(Map::m_width);
    uint8_t y = random(Map::m_height);
    
    //Cell is not brick?
    if (*(Map::m_cell + y * MAP_WIDTH_MAX + x) != CELL_BRICK)
      continue;
    m_bonus_cell_x = x;
    m_bonus_cell_y = y;
    m_bonus_type = pgm_read_byte(s_level_info + level_offset + 2);

    //Get something usefull
    for (uint8_t i = 0; i < 220; ++i)
    {
      if (m_bonus_type == BONUS_BOMB_AMOUNT && m_player.bomb_maximum < BOMBS_MAX)
        break;
      if (m_bonus_type == BONUS_BOMB_RADIUS && m_player.bomb_radius < RADIUS_MAX)
        break;
      if (m_bonus_type == BONUS_SPEED && (uint8_t)(m_player.upgrade & PLAYER_UPGRADE_SPEED_3 ) == 0)
        break;
      if (m_bonus_type == BONUS_PROTECTION && (uint8_t)(m_player.upgrade & PLAYER_UPGRADE_NO_BOMB_DAMAGE ) == 0)
        break;
      if (m_bonus_type == BONUS_GO_THROUGH_WALLS && (uint8_t)(m_player.upgrade & PLAYER_UPGRADE_GO_THROUGH_WALLS ) == 0)
        break;
      if (m_bonus_type == BONUS_GO_THROUGH_BOMBS && (uint8_t)(m_player.upgrade & PLAYER_UPGRADE_GO_THROUGH_BOMBS ) == 0)
        break;
      if (m_bonus_type == BONUS_MANUAL_EXPLOSION && (uint8_t)(m_player.upgrade & PLAYER_UPGRADE_MANUAL_EXPLOSION ) == 0)
        break;
      if (m_bonus_type == BONUS_FREEZE_TIME)
        break;
      m_bonus_type = random(8);
    }
    break;
  }

  //Put exit
  while (true)
  {
    uint8_t x = random(Map::m_width);
    uint8_t y = random(Map::m_height);
    //Cell is not brick?
    if (*(Map::m_cell + y * MAP_WIDTH_MAX + x) != CELL_BRICK)
      continue;
    if (x == m_bonus_cell_x && y == m_bonus_cell_y)
      continue;
    m_exit_cell_x = x;
    m_exit_cell_y = y;
    break;
  }
}

bool Game::Control(uint8_t buttons, uint16_t frame_number)
{
  if ( (uint8_t)(m_flags & GAME_FLAG_LEVEL_DONE) )
  {
    m_flags &= (~GAME_FLAG_LEVEL_DONE);
    m_flags |= GAME_FLAG_PAUSE_LEVEL_DONE;
    m_game_pause_music = 90;
    sound.tones(s_music_level_done);
  }
   
  if ( (uint8_t)(m_flags & GAME_FLAG_PLAYER_DIE) )
  {
    m_flags &= (~GAME_FLAG_PLAYER_DIE);
    m_game_pause_music = 60;
    m_flags |= GAME_FLAG_PAUSE_DEATH;
    sound.tones(s_music_level_die);
  }
  
  if (m_game_pause_music > 0)
  {
    m_game_pause_music --;
    if (m_game_pause_music == 0)
    {
      if ( (uint8_t)(m_flags & GAME_FLAG_PAUSE_DEATH) )
      {
        m_flags &= (~GAME_FLAG_PAUSE_DEATH);
        if (m_player.lives != 0)
        {
          m_player.lives--;
          m_player.bomb_put = 0;
          m_player.bomb_last_cell_x = 0;
          m_player.bomb_last_cell_y = 0;
          m_player.movement_frame = 0;
          m_player.invulnerability = INVULNERABILITY;
          m_player.flags = UNIT_FLAG_ACTIVE | UNIT_FLAG_ALIVE;
      
          //Show level name
          m_splash_level = LEVEL_TITLE_DURATION_FRAMES;
          sound.tones(s_music_level_start);
          //Remove all bombs
          Map::CleanBombsOnLevel();
        } else
        {
          //This is over
          Game::m_flags |= GAME_FLAG_GAME_OVER;
          sound.tones(s_music_game_over);
          m_splash_level = LEVEL_GAME_OVER_DURATION_FRAMES;
        }
        return false;
      }
      if ( (uint8_t)(m_flags & GAME_FLAG_PAUSE_LEVEL_DONE) )
      {
        m_flags &= (~GAME_FLAG_PAUSE_LEVEL_DONE);
        m_level++;
        if (m_level > 50)
          return true;
        StartLevel();
        m_splash_level = LEVEL_TITLE_DURATION_FRAMES;
      }
    }
  }
  
  if (m_splash_level != 0)
    m_splash_level --;

  if (m_splash_level != 0)
    return false;

  //Enter to pause screen
  if ( (uint8_t)(buttons & B_BUTTON) != 0 )
  {
    m_pause_toggle ++;
    if (m_pause_toggle >= PAUSE_PRESS_DURATION)
    {
      m_pause_toggle = 0;
      if (m_pause_mode != 0)
        m_pause_mode = 0;
      else
        m_pause_mode = PAUSE_MODE_STATS;
      Menu::m_logo_pos = 0;   //A counter, used for memory saving
      Menu::m_bomb_pos = 0;
      return false;
    }
  } else
    m_pause_toggle = 0;

  if (m_pause_mode)
  {
    if ( (uint8_t)(buttons & A_BUTTON) != 0 && Menu::m_logo_pos == 0)
    {
      if (m_pause_mode == PAUSE_MODE_STATS)
        m_pause_mode = PAUSE_MODE_INFO;
      else
        m_pause_mode = PAUSE_MODE_STATS;
      Menu::m_logo_pos = 15;
    }

    if ((uint8_t)(buttons & UP_BUTTON) != 0)
    {
      if (Menu::m_bomb_pos > 0)
        Menu::m_bomb_pos --;
    }
    
    if ((uint8_t)(buttons & DOWN_BUTTON) != 0)
    {
      if (Menu::m_bomb_pos < 80-64)
        Menu::m_bomb_pos ++;
    }
    
    if (Menu::m_logo_pos > 0)
      Menu::m_logo_pos --;
    return false;
  }

  Map::Control(frame_number);
  if ( (uint8_t)(m_player.flags & UNIT_FLAG_ACTIVE) != 0 && (uint8_t)(Game::m_flags & GAME_FLAG_PAUSE_LEVEL_DONE) == 0)
    Player::Control(&m_player, buttons, frame_number);

  if ( (uint8_t)(m_player.flags & UNIT_FLAG_ALIVE) == 0)
    Arduboy2Core::setRGBled(RED_LED, 255);
  else
    Arduboy2Core::setRGBled(RED_LED, 0);

  if (m_ghost_freeze > 0)
    m_ghost_freeze --;
  for (uint8_t i = 0; i < UNITS_MAX; ++i)
  {
    if ((uint8_t)(m_units[i].flags & UNIT_FLAG_ACTIVE) != 0)
      Unit::Control(m_units+i, frame_number+i);
  }
  if (m_ghost_left == 0)
    Arduboy2Core::setRGBled(GREEN_LED, 255);
  else
    Arduboy2Core::setRGBled(GREEN_LED, 0);

  if (m_bonus_cell_x == 0)
    Arduboy2Core::setRGBled(BLUE_LED, 255);
  else
    Arduboy2Core::setRGBled(BLUE_LED, 0);

  if ( (uint8_t)(m_flags & GAME_FLAG_GAME_OVER) != 0 && m_splash_level == 0 )
    return true;

  return false;
}

void Game::Draw(Arduboy2& arduboy)
{
  if (m_splash_level != 0)
  {
    if ( (uint8_t)(m_flags & GAME_FLAG_GAME_OVER) )
    {
      arduboy.setCursor(38, 32);
      arduboy.print(F("GAME OVER"));
    } else
    {
      arduboy.setCursor(42, 32);
      arduboy.print(F("Level "));
      arduboy.print(m_level);
      arduboy.setCursor(42, 56);
      arduboy.print(F("Lives "));
      arduboy.print(m_player.lives);
    }
  } else if (m_pause_mode)
  {
    if (m_pause_mode == PAUSE_MODE_STATS)
    {
      arduboy.setCursor(92, 56);
      arduboy.print(F("A-info"));
        
      int16_t bomb_x = 82;
      int16_t bomb_y = 24;
      Sprites::drawPlusMask(bomb_x, bomb_y, s_bomb, 0);
      for (uint8_t i = 0; i < PARTICLE_COUNT; ++i)
      {
        uint16_t index = (uint16_t)i*PARTICLE_FRAMES*2 + ((arduboy.frameCount + i*3) & (PARTICLE_FRAMES-1))*2;
        int8_t dx = (int8_t)pgm_read_byte(s_particle + index );
        int8_t dy = (int8_t)pgm_read_byte(s_particle + index + 1 );
        arduboy.drawPixel(bomb_x + dx + 20, bomb_y + dy + 0);
      }
  
      arduboy.setCursor(8,  8); arduboy.print(F("Level  ")); arduboy.print(m_level);
      arduboy.setCursor(8, 16); arduboy.print(F("Ghost  ")); arduboy.print(m_ghost_left);
      arduboy.setCursor(8, 24); arduboy.print(F("Lives  ")); arduboy.print(m_player.lives);
      arduboy.setCursor(8, 32); arduboy.print(F("Bombs  ")); arduboy.print(m_player.bomb_maximum);
      arduboy.setCursor(8, 40); arduboy.print(F("Radius ")); arduboy.print(m_player.bomb_radius);
  
      int16_t pos = 8;
      static const int16_t s = 10;
      static const int16_t y = 52;
      uint8_t f = arduboy.frameCount & 3;
      if ( (uint8_t)(m_player.upgrade & PLAYER_UPGRADE_SPEED_3))
      {
        Arduboy2::drawBitmap(pos, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
        Sprites::drawPlusMask(pos, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_SPEED)*18, 0);
        pos += s;
        Arduboy2::drawBitmap(pos, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
        Sprites::drawPlusMask(pos, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_SPEED)*18, 0);
        pos += s;
      } else if ( (uint8_t)(m_player.upgrade & PLAYER_UPGRADE_SPEED_2))
      {
        Arduboy2::drawBitmap(pos, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
        Sprites::drawPlusMask(pos, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_SPEED)*18, 0);
        pos += s;
      }
      if ( (uint8_t)(m_player.upgrade & PLAYER_UPGRADE_GO_THROUGH_BOMBS))
      {
        Arduboy2::drawBitmap(pos, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
        Sprites::drawPlusMask(pos, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_GO_THROUGH_BOMBS)*18, 0);
        pos += s;
      }
      if ( (uint8_t)(m_player.upgrade & PLAYER_UPGRADE_GO_THROUGH_WALLS))
      {
        Arduboy2::drawBitmap(pos, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
        Sprites::drawPlusMask(pos, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_GO_THROUGH_WALLS)*18, 0);
        pos += s;
      }
      if ( (uint8_t)(m_player.upgrade & PLAYER_UPGRADE_NO_BOMB_DAMAGE))
      {
        Arduboy2::drawBitmap(pos, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
        Sprites::drawPlusMask(pos, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_PROTECTION)*18, 0);
        pos += s;
      }
      if ( (uint8_t)(m_player.upgrade & PLAYER_UPGRADE_MANUAL_EXPLOSION))
      {
        Arduboy2::drawBitmap(pos, y, s_tiles + (CELL_BONUS + f)*8, 8, 8);
        Sprites::drawPlusMask(pos, y, s_sprites+(UNIT_SPRITE_BONUS + BONUS_MANUAL_EXPLOSION)*18, 0);
        pos += s;
      }
    } else
    {
      int16_t pos = -Menu::m_bomb_pos;
      GameInfoScene::DrawBonusInfo(arduboy, 8, pos, 10);
      
      //Progress bar
      arduboy.fillRect(128-7, 0, 6, 64, WHITE);
      arduboy.fillRect(128-6, 1, 4, 62, BLACK);

      int16_t p = (60-8) * Menu::m_bomb_pos / (80-64);
      arduboy.fillRect(128-5, 2+p, 2, 8, WHITE);
    }
  } else
  {
    if (Map::m_width == 17)
      m_draw_offset_x = -4;
    else
    {
      m_draw_offset_x = (64-4)-(int16_t)m_player.x;
      if (Map::m_width*8+m_draw_offset_x < 128)
        m_draw_offset_x = 128-(int16_t)Map::m_width*8;
      if (m_draw_offset_x > 0)
        m_draw_offset_x = 0;
    }
  
    if (Map::m_height == 9)
      m_draw_offset_y= -4;
    else
    {
      m_draw_offset_y = (32-4)-(int16_t)m_player.y;
      if (Map::m_height*8+m_draw_offset_y < 64)
      m_draw_offset_y = 64-(int16_t)Map::m_height*8;
      if (m_draw_offset_y > 0)
        m_draw_offset_y = 0;
    }
      
    Map::Draw();

    uint8_t bonus_cell = *(Map::m_cell + m_bonus_cell_y*MAP_WIDTH_MAX + m_bonus_cell_x);
    if (bonus_cell >= CELL_BONUS && bonus_cell < CELL_BONUS+4)
      Sprites::drawPlusMask(m_bonus_cell_x*8+Game::m_draw_offset_x, m_bonus_cell_y*8 + Game::m_draw_offset_y, s_sprites+(UNIT_SPRITE_BONUS + m_bonus_type)*18, 0);

    if ( (uint8_t)(m_player.flags & UNIT_FLAG_ACTIVE) != 0 )
      Player::Draw(&m_player, arduboy.frameCount);

    for (uint8_t i = 0; i < UNITS_MAX; ++i)
    {
      if ((uint8_t)(m_units[i].flags & UNIT_FLAG_ACTIVE) != 0)
        Unit::Draw(m_units+i, (m_ghost_freeze == 0) ? (arduboy.frameCount+i) : i);
    }
  }
}

}
