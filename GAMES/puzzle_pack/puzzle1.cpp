#include <Arduino.h>
#include <Arduboy2.h>
#include "puzzle.h"
#include "audio.h"

 const byte sine_table[64] = {0, 6, 13, 19, 25, 31, 37, 44, 50, 56, 62, 68, 74, 80, 86, 92, 98, 103, 109, 115, 120, 126, 131, 136, 142, 147, 152, 157, 162, 167, 171, 176, 180, 185, 189, 193, 197, 201, 205, 208, 212, 215, 219, 222, 225, 228, 231, 233, 236, 238, 240, 242, 244, 246, 247, 249, 250, 251, 252, 253, 254, 254, 255, 255};

const SFX_Data sfx_vertical_shift = {32, 34 - 16, 0, 7, 0, 4, 0};
const SFX_Data sfx_horizontal_shift = {32, 28 - 32, 0, 7, 0, 4, 0};
const SFX_Data sfx_cant_shift = {20, 24 - 32, 0, 7, 0, 12, 1};
const SFX_Data sfx_match = {0, 39 - 32, 27 - 32, 7, 0, 23, 1};

extern Arduboy2 arduboy;

extern unsigned int game_tick;
extern unsigned int score;
extern byte key_left;
extern byte key_up;
extern byte key_right;
extern byte key_down;
extern byte key_b;

struct Shape
{
  byte type;
  byte rotation;
  byte x;
  byte y;
};

struct Game_Data
{
  float delay_amount = 60;
  byte delay_timer = 0;
  byte combo = 1;
  byte match_animation_timer = 0;
  Shape playfield[13 * 5];
  byte selected = 2;
  byte b_held = 0;
};

static struct Game_Data * game_data;

static void game_create()
{
 game_data = new Game_Data();
 for(byte a = 0; a < 13 * 5; a++)
 {
  game_data->playfield[a].type = 0;
  if(a % 13 == 6)
  {
    game_data->playfield[a].x = 6 * 9 + 9;
    game_data->playfield[a].y = a / 13 * 11 + 13 + 64;
    //game_data->playfield[a].y = a / 13 * 11 + 13;
    game_data->playfield[a].type = a / 13 + 1;
  }
 }
}

static void game_delete()
{
 delete game_data; 
}

static int fast_sin(byte a, byte size)
{
 if(a < 128)
 {
  if(a >= 64) a = 64 - a + 63;
  return int(pgm_read_byte(sine_table + a)) >> size;
 }
 a = a - 128;
 if(a >= 64) a = 64 - a + 63;
 return 0 - int(pgm_read_byte(sine_table + a)) >> size;
}

static int fast_cos(byte a, byte size)
{
 return fast_sin(a + 64, size);
}

static void draw_shape(Shape shape)
{
  switch(shape.type)
  {
    case 0:
      break;
    case 1:
      // triangle
      arduboy.fillTriangle(shape.x + fast_sin(shape.rotation, 6),
                           shape.y + fast_cos(shape.rotation, 6),
                           shape.x + fast_sin(shape.rotation + 256 / 3, 6),
                           shape.y + fast_cos(shape.rotation + 256 / 3, 6),
                           shape.x + fast_sin(shape.rotation + 256 / 3 * 2, 6),
                           shape.y + fast_cos(shape.rotation + 256 / 3 * 2, 6), 1);
      break;
    case 2:
      // diamond
      arduboy.fillTriangle(shape.x + fast_sin(shape.rotation, 8),
                           shape.y + fast_cos(shape.rotation, 8),
                           shape.x + fast_sin(shape.rotation + 128, 8),
                           shape.y + fast_cos(shape.rotation + 128, 8),
                           shape.x + fast_sin(shape.rotation + 64, 6),
                           shape.y + fast_cos(shape.rotation + 64, 6), 1);
      arduboy.fillTriangle(shape.x + fast_sin(shape.rotation, 8),
                           shape.y + fast_cos(shape.rotation, 8),
                           shape.x + fast_sin(shape.rotation + 128, 8),
                           shape.y + fast_cos(shape.rotation + 128, 8),
                           shape.x + fast_sin(shape.rotation + 192, 6),
                           shape.y + fast_cos(shape.rotation + 192, 6), 1);
      break;
    case 3:
      // square
      for(byte a = 0; a < 4; a++)
      {
       arduboy.drawLine(shape.x + fast_sin(shape.rotation + a * 64, 6),
                        shape.y + fast_cos(shape.rotation + a * 64, 6),
                        shape.x + fast_sin(shape.rotation + (a + 1) * 64, 6),
                        shape.y + fast_cos(shape.rotation + (a + 1) * 64, 6), 1);
      }
      break;
    case 4:
      // star
      for(byte a = 0; a < 5; a++)
      {
       arduboy.drawLine(shape.x,
                        shape.y,
                        shape.x + fast_sin(shape.rotation + a * 256 / 5, 6),
                        shape.y + fast_cos(shape.rotation + a * 256 / 5, 6), 1);
      }
      break;
    case 5:
      // dots
      for(byte a = 0; a < 6; a++)
      {
       arduboy.drawPixel(shape.x + fast_sin(shape.rotation + a * 256 / 6, 6),
                         shape.y + fast_cos(shape.rotation + a * 256 / 6, 6), 1);
      }
      break;
    default:
      // octagon cursor
      for(byte a = 0; a < 8; a++)
      {
       arduboy.drawLine(shape.x + fast_sin(shape.rotation + a * 32, 5),
                        shape.y + fast_cos(shape.rotation + a * 32, 5),
                        shape.x + fast_sin(shape.rotation + (a + 1) * 32, 5),
                        shape.y + fast_cos(shape.rotation + (a + 1) * 32, 5), 1);
      }
 }
}

static void add_piece()
{
 byte lowest = 13;
 byte row_list[5];
 byte row_count = 0;
 byte a, b;
 for(a = 0; a < 5; a++)
 {
  byte c = 0;
  for(b = 0; b < 13; b++)
  {
   if(game_data->playfield[a * 13 + b].type > 0) c++;
  }
  if(c < lowest)
  {
   lowest = c;
   row_list[0] = a;
   row_count = 1;
  }
  else if(c == lowest)
  {
   row_list[row_count] = a;
   row_count++;
  }
 }
 if(lowest == 13) // game_data->playfield is full
 {
   set_state_game_over();
   return;
 }
 // pick a row at random
 a = row_list[random(0, row_count)];
 // pick the emptier side
 for(b = 1; b < 7; b++)
 {
  if(game_data->playfield[a * 13 + 6 + b].type == 0)
  {
   b = 6 + b;
   game_data->playfield[a * 13 + b].x = 128;
   break;
  }
  else if(game_data->playfield[a * 13 + 6 - b].type == 0)
  {
   b = 6 - b;
   game_data->playfield[a * 13 + b].x = 0;
   break;
  }
 }
 game_data->playfield[a * 13 + b].type = random(1, 6);
 game_data->playfield[a * 13 + b].y = a * 11 + 13;
}

static void check_match()
{
 byte a, b;
 for(a = 0; a < 3; a++)
 {
  for(b = a; b < 4; b++)
  {
   if(game_data->playfield[6 + b * 13].type != game_data->playfield[6 + (b + 1) * 13].type) break;
  }
  if(b - a > 1)
  {
   game_data->match_animation_timer = 8;
   return;
  }
 }
 game_data->combo = 1;
}

static void clear_match()
{
 byte a = 0;
 byte b = 0;
 byte c = 0;
 for(a = 0; a < 3; a++)
 {
  for(b = a; b < 4; b++)
  {
   if(game_data->playfield[6 + b * 13].type != game_data->playfield[6 + (b + 1) * 13].type) break;
  }
  if(b - a > 1)
  {
   b++;
   score += (b - a) * (b - a) * game_data->combo;
   if(b - a == 5) send_message("FULL COLUMN");
   for(; a < b; a++)
   {
    if(game_data->playfield[a * 13 + 7].type != 0)
    {
     for(c = 6; c < 12; c++)
     {
      game_data->playfield[a * 13 + c] = game_data->playfield[a * 13 + c + 1];
     }
     game_data->playfield[a * 13 + c].type = 0;
    }
    else if(game_data->playfield[a * 13 + 5].type != 0)
    {
     for(c = 6; c > 0; c--)
     {
      game_data->playfield[a * 13 + c] = game_data->playfield[a * 13 + c - 1];
     }
     game_data->playfield[a * 13 + c].type = 0;
    }
    else
    {
     game_data->playfield[a * 13 + 6].type = a + 1;
     game_data->playfield[a * 13 + 6].x = 128;
    }
   }
   if(game_data->combo == 2) send_message("DOUBLE COMBO");
   if(game_data->combo == 3) send_message("TRIPLE COMBO");
   if(game_data->combo == 4) send_message("QUAD COMBO");
   if(game_data->combo >  5) send_message("SUPER COMBO");
   game_data->combo++;
   sfx(&sfx_match);
   check_match();
   return;
  }
 }
}

static void shift_left()
{
 if(game_data->playfield[game_data->selected * 13].type != 0 || game_data->playfield[game_data->selected * 13 + 7].type == 0)
 {
   sfx(&sfx_cant_shift);
   return;
 }
 byte a = 0;
 for(; a < 12; a++)
 {
  game_data->playfield[game_data->selected * 13 + a] = game_data->playfield[game_data->selected * 13 + a + 1];
 }
 game_data->playfield[game_data->selected * 13 + a].type = 0;
 check_match();
 sfx(&sfx_horizontal_shift);
}

static void shift_right()
{
 if(game_data->playfield[game_data->selected * 13 + 12].type != 0 || game_data->playfield[game_data->selected * 13 + 5].type == 0)
 {
   sfx(&sfx_cant_shift);
   return;
 }
 byte a = 12;
 for(; a > 0; a--)
 {
  game_data->playfield[game_data->selected * 13 + a] = game_data->playfield[game_data->selected * 13 + a - 1];
 }
 game_data->playfield[game_data->selected * 13 + a].type = 0;
 check_match();
 sfx(&sfx_horizontal_shift);
}

static void shift_up()
{
 Shape temp_shape = game_data->playfield[6];
 temp_shape.y = 11 * 5 + 13;
 byte a;
 for(a = 0; a < 4; a++)
 {
  game_data->playfield[6 + 13 * a] = game_data->playfield[6 + 13 * (a + 1)];
 }
 game_data->playfield[6 + 13 * a] = temp_shape;
 check_match();
 sfx(&sfx_vertical_shift);
}

static void shift_down()
{
 Shape temp_shape = game_data->playfield[6 + 4 * 13];
 temp_shape.y = 13 - 11;
 byte a;
 for(a = 4; a > 0; a--)
 {
  game_data->playfield[6 + 13 * a] = game_data->playfield[6 + 13 * (a - 1)];
 }
 game_data->playfield[6 + 13 * a] = temp_shape;
 check_match();
 sfx(&sfx_vertical_shift);
}

static void game_update()
{
 if(game_tick % 128 == 0) game_data->delay_amount = game_data->delay_amount * 0.98;
 if(game_data->match_animation_timer)
 {
  game_data->match_animation_timer--;
  if(game_data->match_animation_timer == 0) clear_match();
 }
 else
 {
  if(key_left == 1)
  {
   shift_left();
  }
  if(key_right == 1)
  {
   shift_right();
  }
  if(key_b)
  {
   game_data->b_held = 1;
   if(key_up == 1)
   {
    shift_up();
   }
   if(key_down == 1)
   {
    shift_down();
   }
   if(key_b == 1) sfx(&sfx_vertical_shift);
  }
  game_data->delay_timer++;
  if(game_data->delay_timer > game_data->delay_amount)
  {
   game_data->delay_timer = 0;
   add_piece();
  }
 }
 if(!key_b)
 {
  if(game_data->b_held)
  {
    sfx(&sfx_horizontal_shift);
    game_data->b_held = 0;
  }
  if(key_up == 1)
  {
   game_data->selected--;
   if(game_data->selected > 4) game_data->selected = 4;
  }
  if(key_down == 1)
  {
   game_data->selected++;
   if(game_data->selected > 4) game_data->selected = 0;
  }
 }
}

static void game_draw()
{
  arduboy.clear();
  if(game_data->match_animation_timer == 0 && key_b)
  {
    arduboy.drawFastVLine(9 * 7 - 8, 8, 63, 1);
    arduboy.drawFastVLine(9 * 7 + 7, 8, 63, 1);
  }
  else
  {
    for(byte a = 0; a < 64 - 8; a++)
    {
      if((a + game_tick / 4) % 4 < 2)
      {
        arduboy.drawPixel(9 * 7 - 8, 8 + a, 1);
        arduboy.drawPixel(9 * 7 + 7, 63 - a, 1);
      }
    }
  }
  draw_shape((Shape){11, game_tick, 9 * 7, game_data->selected * 11 + 13});
  for(byte a = 0; a < 5; a++)
  {
    for(byte b = 0; b < 13; b++)
    {
      if(game_data->playfield[a * 13 + b].type > 0)
      {
        Shape & shape = game_data->playfield[a * 13 + b];
        byte x;
        shape.rotation = 256 - shape.x * 2 + shape.y * 4 + game_tick / 2;
        if(b < 6) x = b * 9 + 4;
        if(b == 6) x = b * 9 + 9;
        if(b > 6) x = b * 9 + 14;
        shape.x = shape.x + (x - shape.x) / 4;
        if(shape.x - x > 0) shape.x--;
        if(shape.x - x < 0) shape.x++;
        byte y = a * 11 + 13;
        shape.y = shape.y + (y - shape.y) / 4;
        if(shape.y - y > 0) shape.y--;
        if(shape.y - y < 0) shape.y++;
        draw_shape(shape);
      }
    }
  }
  game_tick++;
}

void create_game1(Game & game)
{
  game._create = game_create;
  game._update = game_update;
  game._draw = game_draw;
  game._delete = game_delete;
}
