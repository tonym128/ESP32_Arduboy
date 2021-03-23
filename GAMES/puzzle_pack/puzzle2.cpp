#include <Arduino.h>
#include <Arduboy2.h>
#include "puzzle.h"
#include "audio.h"

const SFX_Data sfx_grab = {4, 38 - 32, 0, 7, 0, 16, 0};
const SFX_Data sfx_drop = {24, 31 - 32, 0, 7, 0, 32, 0};
const SFX_Data sfx_cant_drop = {20, 24 - 32, 0, 7, 0, 12, 1};
const SFX_Data sfx_match = {64, 0, 30 - 32, 7, 0, 32, 0};
extern Arduboy2 arduboy;

extern unsigned int game_tick;
extern unsigned int score;
extern byte key_left;
extern byte key_up;
extern byte key_right;
extern byte key_down;
extern byte key_b;

#define PLAYFIELD_WIDTH 7
#define PLAYFIELD_HEIGHT 10
#define BLOCK_WIDTH 18 // 128 / PLAYFIELD_WIDTH

struct Block
{
 byte y = 0;
 byte color = 0;
 byte checked = 0; // TODO this could be merged with color, but it's probably unnecessary
};

struct Game_Data
{
  float delay_amount = 5 * 30;
  byte delay_timer = 0;
  byte match_animation_timer = 0;
  Block playfield[PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT];
  byte selected = 3;
  byte match_count = 0;
  byte picked_block_color = 0;
  byte picked_block_count = 0;
  byte picked_block_y[3];
  byte animation_block_count = 0;
  byte animation_block[PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT]; // not actually possible to be that large but idk what the actual limit is
  byte animation_block_timer = 0;
};

static struct Game_Data * game_data;

static void game_delete()
{
 delete game_data;
}

static void draw_block(byte x, byte y, byte color)
{
 x++;
 switch(color)
 {
  case 1:
   arduboy.fillRect(x, y, BLOCK_WIDTH - 1, 3, 1);
   break;
  case 2:
   arduboy.drawFastHLine(x, y, BLOCK_WIDTH - 1, 1);
   arduboy.drawFastVLine(x + BLOCK_WIDTH - 2, y, 3, 1);
   arduboy.drawFastVLine(x, y, 3, 1);
   break;
  case 3:
   arduboy.drawFastHLine(x, y, BLOCK_WIDTH - 1, 1);
   for(byte a = 0; a < (BLOCK_WIDTH - 1) * 2; a += 2)
   {
    arduboy.drawPixel(x + a % (BLOCK_WIDTH - 1), y + a / (BLOCK_WIDTH - 1) + 1, 1);
   }
   break;
 }
}

static void add_row()
{
 for(byte a = 0; a < PLAYFIELD_WIDTH; a++)
 {
  if(game_data->playfield[a + (PLAYFIELD_HEIGHT - 1) * PLAYFIELD_WIDTH].color) // playfield is full
  {
   set_state_game_over();
   return;
  }
 }
 for(byte a = 0; a < PLAYFIELD_WIDTH; a++)
 {
  for(byte b = PLAYFIELD_HEIGHT - 1; b > 0; b--)
  {
   game_data->playfield[a + b * PLAYFIELD_WIDTH].y = game_data->playfield[a + (b - 1) * PLAYFIELD_WIDTH].y;
   game_data->playfield[a + b * PLAYFIELD_WIDTH].color = game_data->playfield[a + (b - 1) * PLAYFIELD_WIDTH].color;
  }
 }
 for(byte a = 0; a < PLAYFIELD_WIDTH; a++)
 {
  game_data->playfield[a].y = 4;
  game_data->playfield[a].color = random(1, 4);
 }
}

static void drop_blocks()
{
 for(byte a = 0; a < PLAYFIELD_WIDTH; a++)
 {
  for(byte b = 0; b < PLAYFIELD_HEIGHT - 1; b++)
  {
   if(game_data->playfield[a + b * PLAYFIELD_WIDTH].color == 0)
   {
    for(byte c = b + 1; c < PLAYFIELD_HEIGHT; c++)
    {
     if(game_data->playfield[a + c * PLAYFIELD_WIDTH].color)
     {
      game_data->playfield[a + b * PLAYFIELD_WIDTH].color = game_data->playfield[a + c * PLAYFIELD_WIDTH].color;
      game_data->playfield[a + b * PLAYFIELD_WIDTH].y = game_data->playfield[a + c * PLAYFIELD_WIDTH].y;
      game_data->playfield[a + c * PLAYFIELD_WIDTH].color = 0;
      break;
     }
    }
   }
  }
 }
}

static void check_match(byte x, byte y)
{
 if(game_data->playfield[x + y * PLAYFIELD_WIDTH].checked == 0 && game_data->playfield[x + y * PLAYFIELD_WIDTH].color == game_data->picked_block_color)
 {
  game_data->match_count++;
  game_data->playfield[x + y * PLAYFIELD_WIDTH].checked = 1;
  if(x > 0) check_match(x - 1, y);
  if(x < PLAYFIELD_WIDTH - 1) check_match(x + 1, y);
  if(y > 0) check_match(x, y - 1);
  if(y < PLAYFIELD_HEIGHT - 1) check_match(x, y + 1);
 }
}

static void clear_checked()
{
 for(byte a = 0; a < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT; a++)
 {
  game_data->playfield[a].checked = 0;
 }
 game_data->match_count = 0;
}

static void clear_match()
{
 game_data->animation_block_count = 0;
 for(byte a = 0; a < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT; a++)
 {
  if(game_data->playfield[a].checked)
  {
   game_data->playfield[a].color = 0;
   game_data->playfield[a].checked = 0;
   game_data->animation_block[game_data->animation_block_count] = a;
   game_data->animation_block_count++;
  }
 }
 game_data->animation_block_timer = 0;
 char buffer[17] = "";
 add_number_to_string(buffer, game_data->match_count);
 add_string_to_string(buffer, " MATCH");
 send_message(buffer);
 score += game_data->match_count * game_data->match_count;
 game_data->match_count = 0;
 drop_blocks();
}

static void game_update()
{
 if(game_tick % (30 * 5) == 0) game_data->delay_amount *= 0.98;
 if(key_left == 1)
 {
  if(game_data->selected > 0) game_data->selected--;
 }
 if(key_right == 1)
 {
  if(game_data->selected < PLAYFIELD_WIDTH - 1) game_data->selected++;
 }
 if(game_data->match_animation_timer)
 {
  game_data->match_animation_timer--;
  if(game_data->match_animation_timer == 0) 
  {
     sfx(&sfx_match);
     clear_match();
  }
 }
 else
 {
  if(key_down == 1)
  {
   if(game_data->picked_block_count < 3)
   {
    for(byte a = PLAYFIELD_HEIGHT; a > 0;)
    {
     a--;
     Block & block = game_data->playfield[game_data->selected + a * PLAYFIELD_WIDTH];
     if(block.color)
     {
      if(game_data->picked_block_count == 0)
      {
       game_data->picked_block_color = block.color;
       game_data->picked_block_y[game_data->picked_block_count] = block.y;
       game_data->picked_block_count = 1;
       block.color = 0;
       sfx(&sfx_grab);
      }
      else if(block.color == game_data->picked_block_color)
      {
       game_data->picked_block_y[game_data->picked_block_count] = block.y;
       game_data->picked_block_count++;
       block.color = 0;
       sfx(&sfx_grab);
      }
      break;
     }
    }
   }
  }
  if(game_tick % 2) game_data->delay_timer++;
  if(game_data->delay_timer > game_data->delay_amount)
  {
   add_row();
   game_data->delay_timer = 0;
  }
  if(key_up == 1)
  {
   if(game_data->picked_block_count)
   {
    byte a = 0;
    for(a = PLAYFIELD_HEIGHT; a > 0;)
    {
    a--;
     if(game_data->playfield[game_data->selected + a * PLAYFIELD_WIDTH].color)
     {
      a++;
      break;
     }
    }
    if(a < PLAYFIELD_HEIGHT)
    {
     for(; a < PLAYFIELD_HEIGHT && game_data->picked_block_count; a++)
     {
      game_data->playfield[game_data->selected + a * PLAYFIELD_WIDTH].color = game_data->picked_block_color;
      game_data->picked_block_count--;
      game_data->playfield[game_data->selected + a * PLAYFIELD_WIDTH].y = game_data->picked_block_y[game_data->picked_block_count];
     }
     sfx(&sfx_drop);
     check_match(game_data->selected, a - 1);
     if(game_data->match_count > 3) game_data->match_animation_timer = 9;
     else clear_checked();
    }
    else
    {
      sfx(&sfx_cant_drop);
    }
   }
  }
 }
}

static void game_draw()
{
 arduboy.clear();
 for(byte a = 0; a < PLAYFIELD_WIDTH; a++)
 {
  for(byte b = 0; b < PLAYFIELD_HEIGHT; b++)
  {
   Block & block = game_data->playfield[a + b * PLAYFIELD_WIDTH];
   if(block.color)
   {
    if(block.y < 8 + 4 * b) block.y++;
    if(block.y > 8 + 4 * b)
    {
     block.y--;
     block.y += (8 + 4 * b - block.y) / 4;
    }
    draw_block(a * BLOCK_WIDTH + 1, block.y, block.color);
   }
   else break;
  }
 }
 for(byte a = 0; a < 128; a++)
 {
  if((a + game_tick / 4) % 4 < 2) arduboy.drawPixel(a, 8 + PLAYFIELD_HEIGHT * 4, 1);
 }
 for(byte a = 0; a < game_data->picked_block_count; a++)
 {
  game_data->picked_block_y[a] += (59 - 4 * a - game_data->picked_block_y[a]) / 4;
  if(game_data->picked_block_y[a] > 59 - 4 * a - 4 && game_data->picked_block_y[a] < 59 - 4 * a) game_data->picked_block_y[a]++;
  draw_block(game_data->selected * BLOCK_WIDTH + 1, game_data->picked_block_y[a], game_data->picked_block_color);
 }
 if(game_data->animation_block_timer < 5)
 {
  for(byte a = 0; a < game_data->animation_block_count; a++)
  {
   arduboy.drawRect(game_data->animation_block[a] % PLAYFIELD_WIDTH * BLOCK_WIDTH + 2 - game_data->animation_block_timer,
                    8 + game_data->animation_block[a] / PLAYFIELD_WIDTH * 4 - game_data->animation_block_timer,
                    BLOCK_WIDTH - 1 + game_data->animation_block_timer * 2, 4 + game_data->animation_block_timer * 2, 1);
  }
  game_data->animation_block_timer++;
 }
 arduboy.drawFastHLine(game_data->selected * BLOCK_WIDTH + 2, 63, BLOCK_WIDTH - 1, 1);
 if(game_tick % 2)
 {
  byte y = 4;
  for(byte a = PLAYFIELD_HEIGHT; a > 0;)
  {
   a--;
   if(game_data->playfield[game_data->selected + a * PLAYFIELD_WIDTH].color)
   {
    y = game_data->playfield[game_data->selected + a * PLAYFIELD_WIDTH].y;
    break;
   }
  }
  arduboy.drawLine(game_data->selected * BLOCK_WIDTH + BLOCK_WIDTH / 2 + 1,
                   y + 4,
                   game_data->selected * BLOCK_WIDTH + BLOCK_WIDTH / 2 - 3,
                   y + 4 + 4, 1);
  arduboy.drawLine(game_data->selected * BLOCK_WIDTH + BLOCK_WIDTH / 2 + 1,
                   y + 4,
                   game_data->selected * BLOCK_WIDTH + BLOCK_WIDTH / 2 + 5,
                   y + 4 + 4, 1);
 }
  game_tick++;
}

static void game_create()
{
 game_data = new Game_Data();
 for(byte a = 0; a < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT; a++)
 {
  game_data->playfield[a].y = 0;
  game_data->playfield[a].color = 0;
  game_data->playfield[a].checked = 0;
 }
 add_row();
 add_row();
 add_row();
}

void create_game2(Game & game)
{
  game._create = game_create;
  game._update = game_update;
  game._draw = game_draw;
  game._delete = game_delete;
}
