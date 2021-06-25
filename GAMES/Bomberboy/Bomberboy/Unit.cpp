#include "Unit.h"
#include "Map.h"
#include "Game.h"
#include "assets.h"
#include "defines.h"

namespace Bomberboy
{
  
const uint8_t s_unit_info[]  =
{
  //SPRITE              //SPEED+AI
  UNIT_SPRITE_BALLOM,   0x01,
  UNIT_SPRITE_ONIL,     0x02 | UNIT_AI_TURN,
  UNIT_SPRITE_DAHL,     0x02 | UNIT_AI_TURN | UNIT_AI_ATTACK,
  UNIT_SPRITE_MINVO,    0x03,
  UNIT_SPRITE_DORIA,    0x00 | UNIT_AI_TURN | UNIT_AI_PASS_WALS | UNIT_AI_ATTACK | UNIT_AI_8_FRAME,
  UNIT_SPRITE_OVAPE,    0x01 | UNIT_AI_TURN | UNIT_AI_PASS_WALS | UNIT_AI_ATTACK,
  UNIT_SPRITE_PONTAN,   0x02 | UNIT_AI_TURN | UNIT_AI_PASS_WALS | UNIT_AI_ATTACK | UNIT_AI_8_FRAME,
  UNIT_SPRITE_PASS,     0x03 | UNIT_AI_TURN | UNIT_AI_PASS_WALS | UNIT_AI_ATTACK,
};

//                                  L   R    U   D
int8_t Unit::s_orientation_dx[4] = {-1, +1,  0,  0};
int8_t Unit::s_orientation_dy[4] = { 0,  0, -1, +1};

static bool CanPassCell(uint8_t flags, uint8_t cell_x, uint8_t cell_y)
{
  uint8_t cell = Map::m_cell[cell_x + cell_y*MAP_WIDTH_MAX];
  if (cell == CELL_EMPTY || ( (uint8_t)(flags & UNIT_AI_PASS_WALS) != 0 && cell == CELL_BRICK ))
    return true;
  else
    return false;
}
  
void Unit::Control(Unit* unit, uint16_t frame_number)
{
  //Die test
  if ( (uint8_t)(unit->flags & UNIT_FLAG_ALIVE) != 0 )
  {
    uint8_t cell_x0 = unit->x / 8;
    uint8_t cell_x1 = (unit->x+7) / 8;
    uint8_t cell_y0 = unit->y / 8;
    uint8_t cell_y1 = (unit->y+7) / 8;
    if ( (Map::m_cell[cell_x0 + cell_y0*MAP_WIDTH_MAX] >= CELL_BOMB_EXPLOSION && Map::m_cell[cell_x0 + cell_y0*MAP_WIDTH_MAX] <= CELL_BOMB_EXPLOSION_LAST) ||
         (Map::m_cell[cell_x1 + cell_y0*MAP_WIDTH_MAX] >= CELL_BOMB_EXPLOSION && Map::m_cell[cell_x1 + cell_y0*MAP_WIDTH_MAX] <= CELL_BOMB_EXPLOSION_LAST) ||
         (Map::m_cell[cell_x0 + cell_y1*MAP_WIDTH_MAX] >= CELL_BOMB_EXPLOSION && Map::m_cell[cell_x0 + cell_y1*MAP_WIDTH_MAX] <= CELL_BOMB_EXPLOSION_LAST) ||
         (Map::m_cell[cell_x1 + cell_y1*MAP_WIDTH_MAX] >= CELL_BOMB_EXPLOSION && Map::m_cell[cell_x1 + cell_y1*MAP_WIDTH_MAX] <= CELL_BOMB_EXPLOSION_LAST) )
    {
      unit->flags = unit->flags & (~UNIT_FLAG_ALIVE);
      Game::m_ghost_left --;
      unit->type = 0; //Use type as frame counter
      return;
    }
  } else
  {
    unit->type ++;
    if (unit->type >= 8*4)
      unit->flags = 0;
    return;
  }

  if (Game::m_ghost_freeze != 0)
    return;

  uint8_t flags = pgm_read_byte(s_unit_info + unit->type*2+1);
  uint8_t speed = flags & UNIT_AI_SPEED_MASK;

  if (speed == 0)
  {
    if ((uint8_t)(frame_number & 7) != 0)
      return;
  } else if (speed == 1)
  {
    if ((uint8_t)(frame_number & 3) != 0)
      return;
  } else if (speed == 0)
  {
    if ((uint8_t)(frame_number & 1) != 0)
      return;
  } else if (speed == 3)
  {
    //No limits
  }

  uint8_t orientation = unit->flags & ORIENTATION_MASK;  
  bool can_walk = false;
  switch (orientation)
  {
    case ORIENTATION_LEFT:
      if (CanPassCell(flags, (unit->x-1)/8, (unit->y  ) / 8) &&
          CanPassCell(flags, (unit->x-1)/8, (unit->y+7) / 8))
          can_walk = true;
    break;
    case ORIENTATION_RIGHT:
      if (CanPassCell(flags, (unit->x+8)/8, (unit->y  ) / 8) &&
          CanPassCell(flags, (unit->x+8)/8, (unit->y+7) / 8))
          can_walk = true;
    break;
    case ORIENTATION_UP:
      if (CanPassCell(flags, (unit->x  )/8, (unit->y-1) / 8) &&
          CanPassCell(flags, (unit->x+7)/8, (unit->y-1) / 8))
          can_walk = true;
    break;
    case ORIENTATION_DOWN:
      if (CanPassCell(flags, (unit->x  )/8, (unit->y+8) / 8) &&
          CanPassCell(flags, (unit->x+7)/8, (unit->y+8) / 8))
          can_walk = true;
    break;
  }
  if (can_walk)
  {
    unit->x += s_orientation_dx[orientation];
    unit->y += s_orientation_dy[orientation];
    if ( (uint8_t)(flags & UNIT_AI_TURN) &&
         (uint8_t)(unit->x & 7) == 0 &&
         (uint8_t)(unit->y & 7) == 0 &&
         random(10) == 0) //10%
    {
      unit->flags = (unit->flags & (~ORIENTATION_MASK)) | (random(4) & ORIENTATION_MASK);
    }
    
    //bed logic
    /*if ( (uint8_t)(flags & UNIT_AI_ATTACK) )
    {
      uint8_t player_cell_x = (Game::m_player.x+4)/8;
      uint8_t player_cell_y = (Game::m_player.y+4)/8;
      uint8_t unit_cell_x = unit->x/8;
      uint8_t unit_cell_y = unit->y/8;
      if (player_cell_y == unit_cell_y)
      {
        if (player_cell_x <= unit_cell_x && player_cell_x+4 >= unit_cell_x)
          unit->flags = (unit->flags & (~ORIENTATION_MASK)) | (ORIENTATION_LEFT);
        else if (player_cell_x >= unit_cell_x && player_cell_x <= unit_cell_x+4)
          unit->flags = (unit->flags & (~ORIENTATION_MASK)) | (ORIENTATION_RIGHT);
      } else if (player_cell_x == unit_cell_x)
      {
        if (player_cell_y <= unit_cell_y && player_cell_y+4 >= unit_cell_y)
          unit->flags = (unit->flags & (~ORIENTATION_MASK)) | (ORIENTATION_UP);
        else if (player_cell_y >= unit_cell_y && player_cell_y <= unit_cell_y+4)
          unit->flags = (unit->flags & (~ORIENTATION_MASK)) | (ORIENTATION_DOWN);
      }
    }*/
  } else
    unit->flags = (unit->flags & (~ORIENTATION_MASK)) | (random(4) & ORIENTATION_MASK);
}

void Unit::Draw(Unit* unit, uint16_t frame_number)
{
  if ( (uint8_t)(unit->flags & UNIT_FLAG_ALIVE) != 0 )
  {
    uint8_t sprite = pgm_read_byte(s_unit_info + unit->type*2);
    uint8_t flags = pgm_read_byte(s_unit_info + unit->type*2+1);

    uint8_t f;
    if ((uint8_t)(flags & UNIT_AI_8_FRAME))
      f = (frame_number / 4) & 7;
    else
    {
      f = (frame_number / 4) & 3;
        uint8_t orientation = unit->flags & ORIENTATION_MASK;
      if (orientation == ORIENTATION_RIGHT || orientation == ORIENTATION_DOWN)
        f += 4;
    }
    Sprites::drawPlusMask(unit->x+Game::m_draw_offset_x, unit->y+Game::m_draw_offset_y, s_sprites+(uint16_t)(f+sprite)*18, 0);
  } else
  {
    uint16_t f = UNIT_SPRITE_DIE + (unit->type / 4);
    Sprites::drawPlusMask(unit->x+Game::m_draw_offset_x, unit->y+Game::m_draw_offset_y, s_sprites+f*18, 0);
  }
}

}
