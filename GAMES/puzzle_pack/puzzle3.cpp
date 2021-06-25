#include <Arduino.h>
#include <Arduboy2.h>
#include "puzzle.h"
#include "audio.h"

static  const byte sprites[40 * 20] = {
0x3E, 0xC0, 0xA3, 0x17, 0x02, 
0x7F, 0x20, 0x44, 0x88, 0x5C, 
0x43, 0x91, 0xB9, 0x17, 0x14, 
0xBE, 0x13, 0xA8, 0x34, 0x00, 
0xC8, 0x0B, 0x48, 0x08, 0x80, 
0xBE, 0x0B, 0xB8, 0x57, 0x14, 
0x43, 0x8B, 0xA9, 0x34, 0x1C, 
0x7F, 0x05, 0x48, 0x08, 0x00, 
0x7F, 0x18, 0xA4, 0x97, 0x40, 
0x3E, 0xE0, 0x83, 0x02, 0x10, 

0x7C, 0x80, 0x07, 0x0F, 0x02, 
0xEE, 0x42, 0xE8, 0x08, 0x5C, 
0xEE, 0xA2, 0x49, 0xB7, 0x14, 
0x6C, 0x13, 0x78, 0x14, 0x80, 
0x90, 0x13, 0x88, 0x08, 0x00, 
0x6C, 0x0B, 0x68, 0x17, 0x0A, 
0xEE, 0x8E, 0x59, 0x54, 0x0E, 
0xEE, 0x12, 0x98, 0x28, 0x40, 
0xFE, 0x62, 0x48, 0x0F, 0x01, 
0x7C, 0x80, 0x27, 0x05, 0x10};

const SFX_Data sfx_swap_blocks = {16, 44 - 32, 16 - 32, 7, 0, 16, 0};
//const SFX_Data sfx_pop_blocks = {16, 44 - 32, 16 - 32, 7, 0, 25, 0};

extern Arduboy2 arduboy;

extern unsigned int game_tick;
extern unsigned int score;
extern byte key_left;
extern byte key_up;
extern byte key_right;
extern byte key_down;
extern byte key_b;

#define PLAYFIELD_WIDTH 11
#define PLAYFIELD_HEIGHT 5
#define BLOCK_SIZE 11

struct Block
{
 byte x = 0;
 byte y = 0;
 byte falling = 0;
 byte color = 0;
 byte marked = 0;
};

struct Game_Data
{
  float delay_amount = 1 * 30;
  byte delay_timer = 0;
  Block playfield[PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT];
  byte selected_x = 5;
  byte selected_y = PLAYFIELD_HEIGHT - 1;
  byte combo = 1;
  byte combo_timer = 0;
  byte match_color = 0;
  byte animation_block_count = 0;
  byte animation_block[PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT]; // not actually possible to be that large but idk what the actual limit is
  byte animation_block_timer = 0;
};

static struct Game_Data * game_data;

static void game_delete()
{
 delete game_data;
}

void draw_10x10_bitmap(byte y_offset, byte x_offset, byte x, byte y, byte flip) // this function only works with 10x10 sprites in a 40px tall bitmap
{
  byte * buffer = arduboy.getBuffer();
  byte b = 0;
  if(flip) b = 9;
  for(byte a = 0; a < 10; a++)
  {
    byte byte_buffer = pgm_read_byte(sprites + y_offset + a * 5 + x_offset * 50) >> y_offset * 2;
    buffer[x + b + y / 8 * 128] |= byte_buffer << y % 8;
    buffer[x + b + (y / 8 + 1) * 128] |= byte_buffer >> 8 - y % 8;
    byte_buffer = (pgm_read_byte(sprites + y_offset + 1 + a * 5 + x_offset * 50) & 0xff >> 6 - y_offset * 2);
    buffer[x + b + (8 - y_offset * 2 + y) / 8 * 128] |= byte_buffer << (8 - y_offset * 2 + y) % 8;
    buffer[x + b + ((8 - y_offset * 2 + y) / 8 + 1) * 128] |= byte_buffer >> 8 - (8 - y_offset * 2 + y) % 8;
    if(flip) b--;
    else b++;
  }
}

static void drop_blocks()
{
 for(byte a = 0; a < PLAYFIELD_WIDTH; a++)
 {
  for(byte b = PLAYFIELD_HEIGHT; b > 0;)
  {
   b--;
   if(game_data->playfield[a + b * PLAYFIELD_WIDTH].color == 0)
   {
    for(byte c = b; c > 0;)
    {
     c--;
     if(game_data->playfield[a + c * PLAYFIELD_WIDTH].color)
     {
      Block & block = game_data->playfield[a + b * PLAYFIELD_WIDTH];
      Block & old_block = game_data->playfield[a + c * PLAYFIELD_WIDTH];
      block.color = old_block.color;
      block.x = old_block.x;
      block.y = old_block.y;
      block.falling = 1;
      old_block.color = 0;
      break;
     }
    }
   }
  }
 }
}

static void add_block_at(byte x)
{
 byte colors[5];
 for(byte a = 0; a < 5; a++) colors[a] = 0;
 byte y = 0;
 for(; y < PLAYFIELD_HEIGHT; y++)
 {
   if(game_data->playfield[x + y * PLAYFIELD_WIDTH].color)
   {
    colors[game_data->playfield[x + y * PLAYFIELD_WIDTH].color] = 1;
    break;
   }
 }
 y--;
 if(x > 0) colors[game_data->playfield[x - 1 + y * PLAYFIELD_WIDTH].color] = 1;
 if(x < PLAYFIELD_WIDTH - 1) colors[game_data->playfield[x + 1 + y * PLAYFIELD_WIDTH].color] = 1;
 byte color_count = 0;
 for(byte a = 1; a < 5; a++)
 {
  if(colors[a]) color_count++;
 }
 byte random_color = random(0, 4 - color_count);
 byte selected_color = 1;
 while(random_color)
 {
  while(colors[selected_color] > 0) selected_color++;
  selected_color++;
  random_color--;
 }
 while(colors[selected_color] > 0) selected_color++;
 Block & block = game_data->playfield[x + y * PLAYFIELD_WIDTH];
 block.x = x * BLOCK_SIZE;
 block.y = 0;
 block.falling = 1;
 block.color = selected_color;
}

static void add_block()
{
 byte empty_columns = 0;
 for(byte a = 0; a < PLAYFIELD_WIDTH; a++)
 {
  if(game_data->playfield[a].color == 0)
  {
   empty_columns++;
  }
 }
 if(empty_columns == 0)
 {
  set_state_game_over();
 }
 else
 {
  byte random_column = random(0, empty_columns);
  byte selected_column = 0;
  while(random_column)
  {
   while(game_data->playfield[selected_column].color > 0)
   {
    selected_column++;
   }
   selected_column++;
   random_column--;
  }
  while(game_data->playfield[selected_column].color > 0) selected_column++;
  add_block_at(selected_column);
 }
}

static void mark_matches(byte x, byte y)
{
 if(game_data->playfield[x + y * PLAYFIELD_WIDTH].marked == 0 && game_data->playfield[x + y * PLAYFIELD_WIDTH].color == game_data->match_color)
 {
  game_data->playfield[x + y * PLAYFIELD_WIDTH].marked = 1;
  if(x > 0) mark_matches(x - 1, y);
  if(x < PLAYFIELD_WIDTH - 1) mark_matches(x + 1, y);
  if(y > 0) mark_matches(x, y - 1);
  if(y < PLAYFIELD_HEIGHT - 1) mark_matches(x, y + 1);
 }
}

static void clear_matches()
{
 byte match_count = 0;
 for(byte a = 0; a < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT; a++)
 {
  if(game_data->playfield[a].marked)
  {
   match_count++;
   game_data->playfield[a].color = 0;
   game_data->playfield[a].marked = 0;
   game_data->animation_block[game_data->animation_block_count] = a;
   game_data->animation_block_count++;
  }
 }
 if(match_count)
 {
  game_data->animation_block_timer = 0;
  char buffer[17] = "";
  add_number_to_string(buffer, match_count);
  if(game_data->combo > 1)
  {
    add_string_to_string(buffer, " X ");
    add_number_to_string(buffer, game_data->combo);
    add_string_to_string(buffer, " COMBO");
  }
  else
  {
    add_string_to_string(buffer, " MATCH");
  }
  SFX_Data sfx_pop_blocks = {16, 44 - 32, 17 - 32 - game_data->combo_timer, 7, 0, 25, 0};
  sfx(&sfx_pop_blocks);
  send_message(buffer);
  score += game_data->combo * match_count * match_count;
  drop_blocks();
  game_data->combo_timer = 16;
  game_data->combo++;
 }
}

static byte is_matched(byte x, byte y)
{
  if(game_data->playfield[x + y * PLAYFIELD_WIDTH].color == 0 || game_data->playfield[x + y * PLAYFIELD_WIDTH].falling) return 0;
  if(x > 0 && x < PLAYFIELD_WIDTH - 1 &&
     game_data->playfield[x + 1 + y * PLAYFIELD_WIDTH].falling == 0 && game_data->playfield[x - 1 + y * PLAYFIELD_WIDTH].falling == 0 &&
     game_data->playfield[x + y * PLAYFIELD_WIDTH].color == game_data->playfield[x + 1 + y * PLAYFIELD_WIDTH].color &&
     game_data->playfield[x + y * PLAYFIELD_WIDTH].color == game_data->playfield[x - 1 + y * PLAYFIELD_WIDTH].color) return 1;
  if(y > 0 && y < PLAYFIELD_HEIGHT - 1 &&
     game_data->playfield[x + (y + 1) * PLAYFIELD_WIDTH].falling == 0 && game_data->playfield[x + (y - 1) * PLAYFIELD_WIDTH].falling == 0 &&
     game_data->playfield[x + y * PLAYFIELD_WIDTH].color == game_data->playfield[x + (y + 1) * PLAYFIELD_WIDTH].color &&
     game_data->playfield[x + y * PLAYFIELD_WIDTH].color == game_data->playfield[x + (y - 1) * PLAYFIELD_WIDTH].color) return 1;
  return 0;
}

static void find_matches()
{
 for(byte a = 0; a < PLAYFIELD_WIDTH; a++)
 {
  for(byte b = 0; b < PLAYFIELD_HEIGHT; b++)
  {
   if(is_matched(a, b))
   {
    game_data->animation_block_count = 0;
    game_data->match_color = game_data->playfield[a + b * PLAYFIELD_WIDTH].color;
    mark_matches(a, b);
   }
  }
 }
 clear_matches();
}

static void swap_blocks(signed char x, signed char y)
{
 Block temp_block = game_data->playfield[game_data->selected_x + game_data->selected_y * PLAYFIELD_WIDTH];
 game_data->playfield[game_data->selected_x + game_data->selected_y * PLAYFIELD_WIDTH] = game_data->playfield[game_data->selected_x + x + (game_data->selected_y + y) * PLAYFIELD_WIDTH];
 game_data->playfield[game_data->selected_x + x + (game_data->selected_y + y) * PLAYFIELD_WIDTH] = temp_block;
}

static void game_update()
{
 if(game_tick % (30 * 5) == 0) game_data->delay_amount *= 0.98;
 if(key_b && game_data->playfield[game_data->selected_x + game_data->selected_y * PLAYFIELD_WIDTH].color)
 {
  if(key_left == 1)
  {
   if(game_data->selected_x > 0)
   {
    game_data->selected_x--;
    swap_blocks(1, 0);
    if(game_data->playfield[game_data->selected_x + 1 + game_data->selected_y * PLAYFIELD_WIDTH].color == 0)
    {
     drop_blocks();
    }
    sfx(&sfx_swap_blocks);
   }
  }
  if(key_right == 1)
  {
   if(game_data->selected_x < PLAYFIELD_WIDTH - 1)
   {
    game_data->selected_x++;
    swap_blocks(-1, 0);
    if(game_data->playfield[game_data->selected_x - 1 + game_data->selected_y * PLAYFIELD_WIDTH].color == 0)
    {
     drop_blocks();
    }
    sfx(&sfx_swap_blocks);
   }
  }
  while(game_data->selected_y < PLAYFIELD_HEIGHT - 1)
  {
   if(game_data->playfield[game_data->selected_x + game_data->selected_y * PLAYFIELD_WIDTH].color) break;
   game_data->selected_y++;
  }
  if(key_up == 1)
  {
   if(game_data->selected_y > 0 && game_data->playfield[game_data->selected_x + (game_data->selected_y - 1) * PLAYFIELD_WIDTH].color)
   {
    game_data->selected_y--;
    swap_blocks(0, 1);
    sfx(&sfx_swap_blocks);
   }
  }
  if(key_down == 1)
  {
   if(game_data->selected_y < PLAYFIELD_HEIGHT - 1)
   {
    game_data->selected_y++;
    swap_blocks(0, -1);
    sfx(&sfx_swap_blocks);
   }
  }
 }
 else
 {
  if(key_left == 1)
  {
   if(game_data->selected_x > 0) game_data->selected_x--;
  }
  if(key_right == 1)
  {
   if(game_data->selected_x < PLAYFIELD_WIDTH - 1) game_data->selected_x++;
  }
  if(key_up == 1)
  {
   if(game_data->selected_y > 0) game_data->selected_y--;
  }
  if(key_down == 1)
  {
   if(game_data->selected_y < PLAYFIELD_HEIGHT - 1) game_data->selected_y++;
  }
 }
 find_matches();
 if(game_data->combo_timer) game_data->combo_timer--;
 else game_data->combo = 1;
 if(game_tick < PLAYFIELD_WIDTH * 3)
 {
  add_block_at(game_tick % PLAYFIELD_WIDTH);
 }
 else
 {
  game_data->delay_timer++;
  if(game_data->delay_timer > game_data->delay_amount)
  {
   add_block();
   game_data->delay_timer = 0;
  }
 }
}

static void game_draw()
{
 arduboy.clear();
 for(byte a = 0; a < PLAYFIELD_WIDTH; a++)
 {
  for(byte b = PLAYFIELD_HEIGHT; b > 0;)
  {
   b--;
   Block & block = game_data->playfield[a + b * PLAYFIELD_WIDTH];
   if(block.color)
   {
    if(block.falling)
    {
     block.y += block.falling >> 1;
     block.falling++;
     if(block.y > b * BLOCK_SIZE + 8)
     {
      block.y = b * BLOCK_SIZE + 8 - 1;
      block.falling = 0;
     }
    }
    else
    {
     block.y += (b * BLOCK_SIZE + 8 - block.y) / 4;
     if((b * BLOCK_SIZE + 8 - block.y) / 4 == 0) block.y = b * BLOCK_SIZE + 8;
    }
    block.x += (a * BLOCK_SIZE - block.x) / 4;
    if((a * BLOCK_SIZE - block.x) / 4 == 0) block.x = a * BLOCK_SIZE;
    byte flip = 0;
    if((game_tick / 4 + block.x + block.y) % 6 > 2) flip = 1;
    byte frame = 0;
    if(((game_tick + 1) / 4 + block.x + block.y) % 3 == 0) frame = 1;
    draw_10x10_bitmap(block.color - 1, frame, 3 + block.x, block.y, flip);
   }
  }
 }
 if(game_data->animation_block_timer < BLOCK_SIZE)
 {
  for(byte a = 0; a < game_data->animation_block_count; a++)
  {
   byte x = 3 + game_data->animation_block[a] % PLAYFIELD_WIDTH * BLOCK_SIZE;
   byte y = 8 + game_data->animation_block[a] / PLAYFIELD_WIDTH * BLOCK_SIZE;
   byte positive_offset = game_data->animation_block_timer + (game_data->animation_block_timer >> 1);
   byte negative_offset = game_data->animation_block_timer >> 1;
   byte size = BLOCK_SIZE - game_data->animation_block_timer;
   arduboy.drawRect(x - negative_offset,
                    y - negative_offset, size, size, 1);
   arduboy.drawRect(x + positive_offset,
                    y - negative_offset, size, size, 1);
   arduboy.drawRect(x + positive_offset,
                    y + positive_offset, size, size, 1);
   arduboy.drawRect(x - negative_offset,
                    y + positive_offset, size, size, 1);
  }
  game_data->animation_block_timer++;
 }
 if(game_tick % 2) arduboy.drawRect(2 + game_data->selected_x * BLOCK_SIZE, 7 + game_data->selected_y * BLOCK_SIZE, BLOCK_SIZE + 1, BLOCK_SIZE + 1, 1);
 game_tick++;
}

static void game_create()
{
 game_data = new Game_Data();
 for(byte a = 0; a < PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT; a++)
 {
  game_data->playfield[a].color = 0;
 }
}

void create_game3(Game & game)
{
  game._create = game_create;
  game._update = game_update;
  game._draw = game_draw;
  game._delete = game_delete;
}
