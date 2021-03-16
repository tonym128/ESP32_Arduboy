#include "Player.h"
#include "Map.h"
#include "Game.h"
#include "defines.h"
#include "assets.h"
#include "audio.h"

namespace Bomberboy
{

void Player::LevelReset(Player* player)
{
  player->x = STARTUP_CELL_X*8;
  player->y = STARTUP_CELL_Y*8;
  player->flags = UNIT_FLAG_ACTIVE | UNIT_FLAG_ALIVE;
  player->bomb_put = 0;
  player->bomb_last_cell_x = 0;
  player->bomb_last_cell_y = 0;
  player->invulnerability = 0;
}

static bool CanPassCell(Player* player, uint8_t cell_x, uint8_t cell_y)
{
  if (cell_x == player->bomb_last_cell_x && cell_y == player->bomb_last_cell_y)
    return true;
  uint8_t cell = Map::m_cell[cell_x + cell_y*MAP_WIDTH_MAX];
  if ( cell == CELL_EMPTY )
    return true;
  if ( cell >= CELL_BONUS && cell < CELL_BONUS+4 )
    return true;
  if ( cell >= CELL_BONUS && cell < CELL_EXIT+8 )
    return true;
  if ( cell >= CELL_BOMB_EXPLOSION && cell <= CELL_BOMB_EXPLOSION_LAST )
    return true;
  if ( (uint8_t)(player->upgrade & PLAYER_UPGRADE_GO_THROUGH_BOMBS) != 0 &&
       cell >= CELL_BOMB_INITIAL && cell < CELL_BOMB_INITIAL + 4 )
    return true;
  if ( (uint8_t)(player->upgrade & PLAYER_UPGRADE_GO_THROUGH_WALLS) != 0 &&
       cell >= CELL_BRICK && cell < CELL_BRICK + 4 )
    return true;

  return false;
}

static void AlignVertical(Player* player)
{
  uint8_t rest = player->y & 7;
  if (rest <= 2)
    player->y = player->y & (~7);
  else if (rest >= 5)
    player->y = (player->y & (~7)) + 8;
}

static void AlignHorizontal(Player* player)
{
  uint8_t rest = player->x & 7;
  if (rest <= 2)
    player->x = player->x & (~7);
  else if (rest >= 5)
    player->x = (player->x & (~7)) + 8;
}

void Player::Control(Player* player, uint8_t buttons, uint16_t frame_number)
{
  //Dead can't control unit
  if ( (uint8_t)(player->flags & UNIT_FLAG_ALIVE) == 0)
  {
    player->movement_frame ++;
    if (player->movement_frame >= 4*8)
    {
      //Revive player
      if (player->flags != 0)
      {
        player->flags = 0;
        Game::m_flags |= GAME_FLAG_PLAYER_DIE;
      }
    }
    return;
  }

  if (player->invulnerability > 0)
    player->invulnerability --;

  //Test for death
  if ( (uint8_t)(player->upgrade & PLAYER_UPGRADE_NO_BOMB_DAMAGE) == 0 && player->invulnerability == 0) //Die from bomb
  {
    uint8_t cell_x0 = player->x / 8;
    uint8_t cell_x1 = (player->x+7) / 8;
    uint8_t cell_y0 = player->y / 8;
    uint8_t cell_y1 = (player->y+7) / 8;
    if ( (Map::m_cell[cell_x0 + cell_y0*MAP_WIDTH_MAX] >= CELL_BOMB_EXPLOSION && Map::m_cell[cell_x0 + cell_y0*MAP_WIDTH_MAX] <= CELL_BOMB_EXPLOSION_LAST) ||
         (Map::m_cell[cell_x1 + cell_y0*MAP_WIDTH_MAX] >= CELL_BOMB_EXPLOSION && Map::m_cell[cell_x1 + cell_y0*MAP_WIDTH_MAX] <= CELL_BOMB_EXPLOSION_LAST) ||
         (Map::m_cell[cell_x0 + cell_y1*MAP_WIDTH_MAX] >= CELL_BOMB_EXPLOSION && Map::m_cell[cell_x0 + cell_y1*MAP_WIDTH_MAX] <= CELL_BOMB_EXPLOSION_LAST) ||
         (Map::m_cell[cell_x1 + cell_y1*MAP_WIDTH_MAX] >= CELL_BOMB_EXPLOSION && Map::m_cell[cell_x1 + cell_y1*MAP_WIDTH_MAX] <= CELL_BOMB_EXPLOSION_LAST) )
    {
      player->flags = player->flags & (~UNIT_FLAG_ALIVE);
      player->lives --;
      player->movement_frame = 0;
      sound.tones(s_sfx_die);
      return;
    }
  }

  //Are ghost eat bomber?
  if (player->invulnerability == 0)
  {
    for (uint8_t i = 0; i < UNITS_MAX; ++i)
    {
      if ((uint8_t)(Game::m_units[i].flags & UNIT_FLAG_ALIVE) == 0)
        continue;
      if (   (player->x+2 >= Game::m_units[i].x && player->x+2 < Game::m_units[i].x + 8 && player->y+2 >= Game::m_units[i].y && player->y+2 < Game::m_units[i].y + 8)
          || (player->x+5 >= Game::m_units[i].x && player->x+5 < Game::m_units[i].x + 8 && player->y+2 >= Game::m_units[i].y && player->y+2 < Game::m_units[i].y + 8)
          || (player->x+2 >= Game::m_units[i].x && player->x+2 < Game::m_units[i].x + 8 && player->y+5 >= Game::m_units[i].y && player->y+5 < Game::m_units[i].y + 8)
          || (player->x+5 >= Game::m_units[i].x && player->x+5 < Game::m_units[i].x + 8 && player->y+5 >= Game::m_units[i].y && player->y+5 < Game::m_units[i].y + 8)
          )
      {
        player->flags = player->flags & (~UNIT_FLAG_ALIVE);
        player->lives --;
        player->movement_frame = 0;
        sound.tones(s_sfx_die);
        return;
      }
    }
  }

  //Test for bonus
  if (Game::m_bonus_cell_x != 0)
  {
    uint8_t cell_x = (player->x+4) / 8;
    uint8_t cell_y = (player->y+4) / 8;
    uint8_t cell = Map::m_cell[cell_x + cell_y*MAP_WIDTH_MAX];
    if (cell >= CELL_BONUS && cell < CELL_BONUS+4)
    {
      sound.tones(s_sfx_bonus);
      Map::m_cell[cell_x + cell_y*MAP_WIDTH_MAX] = CELL_EMPTY;
      Game::m_bonus_cell_x = 0;
      switch(Game::m_bonus_type)
      {
        case BONUS_BOMB_AMOUNT:
          if (player->bomb_maximum < BOMBS_MAX)
            player->bomb_maximum++;
        break;
        case BONUS_BOMB_RADIUS:
          if (player->bomb_radius < RADIUS_MAX)
            player->bomb_radius++;
        break;
        case BONUS_SPEED:
          if ( (uint8_t)(player->upgrade & PLAYER_UPGRADE_SPEED_3) != 0 )
          {
            //Nothing to change
          } else if ( (uint8_t)(player->upgrade & PLAYER_UPGRADE_SPEED_2) != 0 )
            player->upgrade |= PLAYER_UPGRADE_SPEED_3;
          else
            player->upgrade |= PLAYER_UPGRADE_SPEED_2;
        break;
        case BONUS_PROTECTION:
            player->upgrade |= PLAYER_UPGRADE_NO_BOMB_DAMAGE;
        break;
        case BONUS_GO_THROUGH_WALLS:
            player->upgrade |= PLAYER_UPGRADE_GO_THROUGH_WALLS;
        break;
        case BONUS_GO_THROUGH_BOMBS:
            player->upgrade |= PLAYER_UPGRADE_GO_THROUGH_BOMBS;
        break;
        case BONUS_FREEZE_TIME:
            Game::m_ghost_freeze = GHOST_FREEZE_TIME;
        break;
        case BONUS_MANUAL_EXPLOSION:
            player->upgrade |= PLAYER_UPGRADE_MANUAL_EXPLOSION;
        break;
      }
    }
  }

  //Test for exit
  if (Game::m_ghost_left == 0)
  {
    uint8_t cell_x = (player->x+4) / 8;
    uint8_t cell_y = (player->y+4) / 8;
    uint8_t cell = Map::m_cell[cell_x + cell_y*MAP_WIDTH_MAX];
    if (cell >= CELL_EXIT && cell < CELL_EXIT+8)
    {
      //Level complete !!
      Game::m_flags |= GAME_FLAG_LEVEL_DONE;
      return;
    }
  }
  
  if ( (uint8_t)(player->upgrade & PLAYER_UPGRADE_SPEED_3) != 0 )
  {
    //No speed limit
  } else if ( (uint8_t)(player->upgrade & PLAYER_UPGRADE_SPEED_2) != 0 )
  {
    if ( (uint8_t)(frame_number & 1) != 0)
      return;
  } else
  {
    if ( (uint8_t)(frame_number & 3) != 0)
      return;
  }
  
  if (buttons == 0)
    return;

  if ( (uint8_t)(buttons & A_BUTTON) != 0 && player->bomb_put < player->bomb_maximum )
  {
    uint8_t orientation = player->flags & ORIENTATION_MASK;  
    uint8_t cell_x = (player->x + 4) / 8;
    uint8_t cell_y = (player->y + 4) / 8;
    if ( *(Map::m_cell + cell_y * MAP_WIDTH_MAX + cell_x) == CELL_EMPTY )
    {
      uint8_t index = Map::FindUnusedBombIndex();
      if (index != 0xFF)
      {
        sound.tones(s_sfx_bomb_put);
        *(Map::m_cell + cell_y * MAP_WIDTH_MAX + cell_x) = CELL_BOMB_INITIAL;
        player->bomb_put ++;
        player->bomb_last_cell_x = cell_x;
        player->bomb_last_cell_y = cell_y;
        Map::m_bombs[index].cell_x = cell_x;
        Map::m_bombs[index].cell_y = cell_y;
        Map::m_bombs[index].radius = player->bomb_radius;
        if ( (uint8_t)(player->upgrade & PLAYER_UPGRADE_MANUAL_EXPLOSION) != 0 )
          Map::m_bombs[index].activation_time = 255;
        else
          Map::m_bombs[index].activation_time = BOMBS_ACTIVATION_FRAMES;
        Map::m_bombs[index].radius_counter = 0;
        Map::m_bombs[index].flags = BOMB_FLAG_LEFT | BOMB_FLAG_RIGHT | BOMB_FLAG_UP | BOMB_FLAG_DOWN;
      }
    }
  }

  if ( (uint8_t)(buttons & B_BUTTON) != 0 && (uint8_t)(player->upgrade & PLAYER_UPGRADE_MANUAL_EXPLOSION) != 0 )
  {
    for (uint8_t i = 0; i < BOMBS_MAX; ++i)
    {
      if (Map::m_bombs[i].radius == 0)
        continue;
      //Activate only one bomb
      if (Map::m_bombs[i].activation_time != 0)
      {
        Map::m_bombs[i].activation_time = 0;
        break;
      }
    }
  }

  if ( (uint8_t)(buttons & (LEFT_BUTTON | RIGHT_BUTTON)) != 0)  //Do vertical align
    AlignVertical(player);

  if ( (uint8_t)(buttons & (UP_BUTTON | DOWN_BUTTON)) != 0) //Do horizontal align
    AlignHorizontal(player);

  //Movements
  if (player->bomb_last_cell_x != 0)
  {
    //control last bomb position
    uint8_t cell_x = (player->x + 4) / 8;
    uint8_t cell_y = (player->y + 4) / 8;
    if (cell_x != player->bomb_last_cell_x || cell_y != player->bomb_last_cell_y)
    {
      player->bomb_last_cell_x = 0;
      player->bomb_last_cell_y = 0;
    }
  }
  
  if ((uint8_t)(buttons & LEFT_BUTTON) != 0)
  {
    player->flags = (player->flags & (~ORIENTATION_MASK)) | ORIENTATION_LEFT;
    uint8_t cell_x = (player->x - 1)/8;
    uint8_t cell_y0 = player->y/8;
    uint8_t cell_y1 = (player->y + 7)/8;
    if (CanPassCell(player, cell_x, cell_y0) && CanPassCell(player, cell_x, cell_y1))
    {
      player->movement_frame++;
      player->x --;
    }
  }

  if ((uint8_t)(buttons & RIGHT_BUTTON) != 0)
  {
    player->flags = (player->flags & (~ORIENTATION_MASK)) | ORIENTATION_RIGHT;
    uint8_t cell_x = (player->x + 8)/8;
    uint8_t cell_y0 = player->y/8;
    uint8_t cell_y1 = (player->y + 7)/8;
    if (CanPassCell(player, cell_x, cell_y0) && CanPassCell(player, cell_x, cell_y1))
    {
      player->movement_frame++;
      player->x ++;
    }
  }

  if ((uint8_t)(buttons & UP_BUTTON) != 0)
  {
    player->flags = (player->flags & (~ORIENTATION_MASK)) | ORIENTATION_UP;
    uint8_t cell_y = (player->y - 1)/8;
    uint8_t cell_x0 = player->x/8;
    uint8_t cell_x1 = (player->x + 7)/8;
    if (CanPassCell(player, cell_x0, cell_y) && CanPassCell(player, cell_x1, cell_y))
    {
      player->movement_frame++;
      player->y --;
    }
  }

  if ((uint8_t)(buttons & DOWN_BUTTON) != 0)
  {
    player->flags = (player->flags & (~ORIENTATION_MASK)) | ORIENTATION_DOWN;
    uint8_t cell_y = (player->y + 8)/8;
    uint8_t cell_x0 = player->x/8;
    uint8_t cell_x1 = (player->x + 7)/8;
    if (CanPassCell(player, cell_x0, cell_y) && CanPassCell(player, cell_x1, cell_y))
    {
      player->movement_frame++;
      player->y ++;
    }
  }
}

void Player::Draw(Player* player, uint16_t frame_number)
{
  if ( (uint8_t)(player->flags & UNIT_FLAG_ALIVE) != 0 )
  {
    uint8_t f = player->movement_frame & 3;
    //f += 12;  //Die
    uint8_t orientation = player->flags & ORIENTATION_MASK;
    if (orientation == ORIENTATION_RIGHT)
      f += 4;
    if (orientation == ORIENTATION_DOWN || orientation == ORIENTATION_UP)
      f += 8;
    if (player->invulnerability > 0)
    {
      if ((uint8_t)(frame_number & 3) <= 1)
        Sprites::drawPlusMask(player->x+Game::m_draw_offset_x, player->y+Game::m_draw_offset_y, s_sprites+f*18, 0);
    } else
      Sprites::drawPlusMask(player->x+Game::m_draw_offset_x, player->y+Game::m_draw_offset_y, s_sprites+f*18, 0);
  } else
  {
    uint8_t f = player->movement_frame / 8;
    f += 12;
    Sprites::drawPlusMask(player->x+Game::m_draw_offset_x, player->y+Game::m_draw_offset_y, s_sprites+f*18, 0);
  }
}

}
