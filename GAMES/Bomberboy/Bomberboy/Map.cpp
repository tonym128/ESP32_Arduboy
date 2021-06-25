#include "Map.h"
#include "assets.h"
#include "Game.h"
#include "audio.h"

namespace Bomberboy
{
  
static const uint8_t s_block_animation_mask[]  =
{
  0x00, 0x00, 0x00, 0x03, 0x03, 0x03, 0x00, 0x00,
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, //Explosion central
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, //Explosion horizontal
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, //Explosion vertical
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, //Exit
};

static const uint8_t s_block_animation_transform[]  =
{
  0x00, 0x00, 0x00, 0x04, 0x05, 0x00, 0x00, 0x00,
  0x09, 0x0A, 0x0B, 0x08, 0x0D, 0x0E, 0x0F, 0x0C,
  0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x00, //Explosion central
  0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x00, //Explosion horizontal
  0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x00, //Explosion vertical
  0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x28, //Exit
};

uint8_t Map::m_cell[MAP_WIDTH_MAX*MAP_HEIGHT_MAX];
Bomb Map::m_bombs[BOMBS_MAX];
uint8_t Map::m_width = 1;
uint8_t Map::m_height = 1;

void Map::Init(uint8_t width, uint8_t height, uint8_t fill_rate)
{
  m_width = width;
  m_height = height;
  for (uint8_t y = 0; y < height; ++y)
  {
    uint8_t* line = m_cell + y * MAP_WIDTH_MAX;
    for (uint8_t x = 0; x < width; ++x)
    {
      if (x == 0 || x+1 == width || y == 0 || y+1 == height)
        line[x] = CELL_SOLID;
      else if ( uint8_t(x & 1) == 0 && uint8_t(y & 1) == 0)
        line[x] = CELL_SOLID;
      else
      {
        if (random(100) < fill_rate)
          line[x] = CELL_BRICK;
        else
          line[x] = CELL_EMPTY;
      }
    }
  }
  for (uint8_t i = 0; i < BOMBS_MAX; ++i)
    m_bombs[i].radius = 0;
}

uint8_t Map::FindUnusedBombIndex()
{
  for (uint8_t i = 0; i < BOMBS_MAX; ++i)
  {
    if (m_bombs[i].radius == 0)
      return i;
  }
  return 0xFF;
}

void Map::CleanBombsOnLevel()
{
  for (uint8_t i = 0; i < BOMBS_MAX; ++i)
    m_bombs[i].radius = 0;

  for (uint8_t y = 0; y < m_height; ++y)
  {
    uint8_t* line = m_cell + y * MAP_WIDTH_MAX;
    for (uint8_t x = 0; x < m_width; ++x)
    {
      if ( (line[x] >= CELL_BOMB_INITIAL && line[x] < CELL_BOMB_INITIAL + 4) ||
           (line[x] >= CELL_BOMB_EXPLOSION && line[x] <= CELL_BOMB_EXPLOSION_LAST) )
        line[x] = CELL_EMPTY;
    }
  }
}

static void ExplodeBlock(uint8_t cell_x, uint8_t cell_y)
{
  uint8_t cell_type = *(Map::m_cell + cell_y * MAP_WIDTH_MAX + cell_x);
  if (cell_type == CELL_BRICK)
    *(Map::m_cell + cell_y * MAP_WIDTH_MAX + cell_x) = CELL_BRICK+1; //Destroy it
  else if (cell_type >= CELL_BOMB_INITIAL && cell_type < CELL_BOMB_INITIAL+4)
  {
    //Activate bomb
    for (uint8_t i = 0; i < BOMBS_MAX; ++i)
    {
      if (Map::m_bombs[i].radius == 0)
        continue;
      if (Map::m_bombs[i].cell_x == cell_x && Map::m_bombs[i].cell_y == cell_y)
      {
        Map::m_bombs[i].activation_time = 0;
        break;
      }
    }
  }
}

void Map::Control(uint16_t frame_number)
{
  for (uint8_t y = 0; y < m_height; ++y)
  {
    uint8_t* line = m_cell + y * MAP_WIDTH_MAX;
    for (uint8_t x = 0; x < m_width; ++x)
    {
      uint8_t mask = pgm_read_byte(s_block_animation_mask+line[x]);
      if (mask == 0x00)
        continue;
      if ( (uint8_t)(frame_number & mask) == 0 )
      {
        if (line[x] == CELL_BRICK + 0x03 && x == Game::m_bonus_cell_x && y == Game::m_bonus_cell_y)
          line[x] = CELL_BONUS;
        else if (line[x] == CELL_BRICK + 0x03 && x == Game::m_exit_cell_x && y == Game::m_exit_cell_y)
          line[x] = CELL_EXIT;
        else
          line[x] = pgm_read_byte(s_block_animation_transform+line[x]);
      }
    }
  }

  for (uint8_t i = 0; i < BOMBS_MAX; ++i)
  {
    if (m_bombs[i].radius == 0)
      continue;
    if (m_bombs[i].activation_time != 0xFF)
    {
      if (m_bombs[i].activation_time == 0)
      {
        uint16_t cell_index = m_bombs[i].cell_y * MAP_WIDTH_MAX + m_bombs[i].cell_x;
        if (m_bombs[i].radius_counter == 0)
        {
          Game::m_player.bomb_put--;
          *(m_cell + cell_index) = CELL_BOMB_EXPLOSION;
          sound.tones(s_sfx_explosion2);
        } else
        {
          //Explosion Rays
          if ( ((uint8_t)m_bombs[i].flags & BOMB_FLAG_LEFT) != 0)
          {
            if (*(m_cell + cell_index - m_bombs[i].radius_counter) == CELL_EMPTY)
              *(m_cell + cell_index - m_bombs[i].radius_counter) = CELL_BOMB_EXPLOSION_HORIZONTAL;
            else
            {
              ExplodeBlock(m_bombs[i].cell_x - m_bombs[i].radius_counter, m_bombs[i].cell_y);
              m_bombs[i].flags &= ~BOMB_FLAG_LEFT;
            }
          }
          
          if ( ((uint8_t)m_bombs[i].flags & BOMB_FLAG_RIGHT) != 0)
          {
            if (*(m_cell + cell_index + m_bombs[i].radius_counter) == CELL_EMPTY)
              *(m_cell + cell_index + m_bombs[i].radius_counter) = CELL_BOMB_EXPLOSION_HORIZONTAL;
            else
            {
              ExplodeBlock(m_bombs[i].cell_x + m_bombs[i].radius_counter, m_bombs[i].cell_y);
              m_bombs[i].flags &= ~BOMB_FLAG_RIGHT;
            }
          }
          
          if ( ((uint8_t)m_bombs[i].flags & BOMB_FLAG_UP) != 0)
          {
            if (*(m_cell + cell_index - m_bombs[i].radius_counter*MAP_WIDTH_MAX) == CELL_EMPTY)
              *(m_cell + cell_index - m_bombs[i].radius_counter*MAP_WIDTH_MAX) = CELL_BOMB_EXPLOSION_VERTICAL;
            else
            {
              ExplodeBlock(m_bombs[i].cell_x, m_bombs[i].cell_y - m_bombs[i].radius_counter);
              m_bombs[i].flags &= ~BOMB_FLAG_UP;
            }
          }

          if ( ((uint8_t)m_bombs[i].flags & BOMB_FLAG_DOWN) != 0)
          {
            if (*(m_cell + cell_index + m_bombs[i].radius_counter*MAP_WIDTH_MAX) == CELL_EMPTY)
              *(m_cell + cell_index + m_bombs[i].radius_counter*MAP_WIDTH_MAX) = CELL_BOMB_EXPLOSION_VERTICAL;
            else
            {
              ExplodeBlock(m_bombs[i].cell_x, m_bombs[i].cell_y + m_bombs[i].radius_counter);
              m_bombs[i].flags &= ~BOMB_FLAG_DOWN;
            }
          }
          if (m_bombs[i].radius_counter == m_bombs[i].radius)
            m_bombs[i].radius = 0;  //Bomb finish
        }
        m_bombs[i].radius_counter++;
      } else
        m_bombs[i].activation_time --;
    }
  }
}

void Map::Draw()
{
  for (uint8_t y = 0; y < m_height; ++y)
  {
    uint8_t* line = m_cell + y * MAP_WIDTH_MAX;
    for (uint8_t x = 0; x < m_width; ++x)
    {
      Arduboy2::drawBitmap(x*8+Game::m_draw_offset_x, y*8+Game::m_draw_offset_y, s_tiles + ((uint16_t)line[x])*8, 8, 8);
    }
  }
}

}
