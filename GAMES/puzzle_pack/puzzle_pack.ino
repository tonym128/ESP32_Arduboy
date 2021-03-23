/*

Puzzle Pack by Atomic

*/

#include <Arduino.h>
#include <Arduboy2.h>
#include "puzzle.h"
#include "audio.h"

static PROGMEM const byte title_sprites[64 * 2 * 3] = {
0x00, 0x00, 0xFC, 0xFE, 0x1C, 0x38, 0x70, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x80, 0xE0, 0x6C, 0x0C, 0xC0, 0xC4, 0xCA, 0xD1, 0xCA, 0xC4, 0xC0, 0x80, 0x30, 0x37, 0x3F, 0xF8, 0xF0, 0x30, 0x30, 0x80, 
0xE0, 0x70, 0x30, 0x18, 0x18, 0x18, 0x18, 0x30, 0x70, 0xE0, 0x84, 0x10, 0x02, 0x08, 0xC0, 0xF0, 0xF0, 0xC0, 0xF0, 0xFC, 0xFC, 0x00, 0xE0, 0xF0, 0x38, 0x1C, 0x0C, 0x80, 0xC0, 0xE0, 0x80, 0x00, 
0x00, 0x00, 0x3F, 0x7F, 0x38, 0x1C, 0x0E, 0x07, 0x03, 0x01, 0x18, 0x1E, 0x07, 0x01, 0x18, 0x3C, 0x7C, 0xEC, 0xCC, 0xCC, 0xC0, 0xC0, 0xFF, 0xFF, 0x00, 0x10, 0x60, 0x61, 0x8F, 0x0E, 0x00, 0x07, 
0x1F, 0x38, 0x30, 0x64, 0x65, 0x62, 0x65, 0x31, 0x38, 0x1F, 0x07, 0x30, 0x3C, 0x0F, 0x03, 0x0F, 0x0F, 0x03, 0x00, 0x03, 0x33, 0x30, 0x31, 0x33, 0x37, 0x3E, 0x3C, 0x18, 0x00, 0x01, 0x01, 0x00, 
0x00, 0x00, 0xC0, 0xF0, 0xFD, 0x3F, 0x0F, 0x23, 0x21, 0xF1, 0xF1, 0xD9, 0x0F, 0x0E, 0xC6, 0xF0, 0xFD, 0x3F, 0x2F, 0x23, 0x21, 0xF1, 0xD1, 0xD9, 0x0F, 0x0E, 0x06, 0xC0, 0xF0, 0xFD, 0x3F, 0x0F, 
0x03, 0xF1, 0xFA, 0xFC, 0x0E, 0x06, 0x03, 0x01, 0x01, 0x81, 0x87, 0x86, 0x06, 0x08, 0xC0, 0xF0, 0xFD, 0x3F, 0x2F, 0x23, 0x22, 0xF4, 0xD0, 0xD8, 0x0E, 0x0F, 0x07, 0x01, 0x82, 0x84, 0x68, 0x10, 
0x06, 0x0B, 0x13, 0x23, 0x22, 0x22, 0x22, 0x22, 0x23, 0x21, 0x21, 0x20, 0x16, 0x0B, 0x13, 0x23, 0x22, 0x24, 0x28, 0x30, 0x81, 0xC3, 0xE3, 0xB2, 0x98, 0x0A, 0x83, 0xC3, 0x63, 0x72, 0x58, 0xF8, 
0x00, 0x98, 0xF9, 0x63, 0x33, 0x9A, 0xCA, 0xE2, 0xB3, 0xB9, 0x29, 0x88, 0xCA, 0x6B, 0x73, 0x5B, 0xCA, 0xA8, 0x38, 0x10, 0x01, 0x0B, 0x13, 0x22, 0x24, 0x29, 0x33, 0x2D, 0x00, 0x00, 0x00, 0x00, 
0xFF, 0xFF, 0x63, 0x63, 0x63, 0xB6, 0x7E, 0x5C, 0x40, 0x78, 0x7E, 0xC6, 0x83, 0x83, 0xC3, 0x46, 0x7E, 0x78, 0x40, 0x7F, 0xFF, 0xE3, 0x63, 0x63, 0x36, 0xBE, 0x9C, 0x40, 0x40, 0x40, 0x80, 0x80, 
0x00, 0xC0, 0x40, 0x40, 0x40, 0x40, 0x40, 0x80, 0x80, 0x00, 0x00, 0xF8, 0xFC, 0x0E, 0x02, 0x7C, 0xFE, 0xFF, 0xFF, 0xFF, 0x07, 0x06, 0xFC, 0xF0, 0x00, 0x08, 0x04, 0x02, 0x07, 0x18, 0x60, 0x80, 
0x01, 0x01, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x3E, 0x22, 0x22, 0x9C, 0x80, 0x41, 0xFF, 0x00, 0x00, 0xCE, 0x4A, 0x8A, 0x04, 0x20, 0x51, 0x9E, 0xE3, 0x80, 0x9C, 0x22, 0x22, 0x22, 0x9C, 0x80, 
0x63, 0xFF, 0x00, 0x00, 0xCE, 0x4A, 0x4A, 0x24, 0x20, 0x11, 0x0E, 0x01, 0x07, 0x06, 0x0E, 0x0E, 0x15, 0x3B, 0x3B, 0x37, 0x37, 0x37, 0x1B, 0x4D, 0x80, 0x87, 0x80, 0x80, 0x40, 0x4E, 0x20, 0x1F};

const SFX_Data sfx_change_game = {30, 36 - 32, 29 - 32, 7, 0, 16, 1};

Arduboy2 arduboy;

void set_state_menu();
void set_state_play();
void set_state_paused();
void fade_to(void (* f)());

void (* game_state)();

unsigned int tick = 0;
unsigned int game_tick = 0;
unsigned int score = 0;

byte key_left = 0;
byte key_up = 0;
byte key_right = 0;
byte key_down = 0;
byte key_b = 0;
byte key_a = 0;


void intro(){
    arduboy.clear();
    arduboy.setCursor(46, 28);
    arduboy.print("ARDUBOY");
 
  //arduboy.tunes.tone(987, 160); TODO SFX
  //delay(160);
  //arduboy.tunes.tone(1318, 400);
  SFX_Data sfx_intro = {85, 0, 0, 4, 4, 32, 0};
  sfx(&sfx_intro);
  delay(160);
  sfx_intro = {128, 0, 0, 7, 4, 48, 0};
  sfx(&sfx_intro);
  delay(1000);
}


void add_number_to_string(char * string, int number)
{
  for(byte a = 0; a < 255; a++)
  {
    if(string[a] == 0)
    {
      for(unsigned int b = 1; b <= number; a++)
      {
        b *= 10;
      }
      string[a] = 0;
      a--;
      for(unsigned int b = 1; b <= number; a--)
      {
        string[a] = 48 + (number / b) % 10;
        b *= 10;
      }
      return;
    }
  }
}

void add_string_to_string(char * string1, char * string2)
{
  for(byte a = 0; a < 255; a++)
  {
    if(string1[a] == 0)
    {
      for(byte b = 0; a < 255; a++)
      {
        string1[a] = string2[b];
        if(string2[b] == 0) return;
        b++;
      }
      return;
    }
  }
}

byte message_timer = 0;
char message[17];

static void draw_overlay()
{
 if(message_timer)
 {
  message_timer--;
  if(message_timer > 8)
  {
   arduboy.setCursor(0, 0);
  }
  else
  {
   arduboy.setCursor(0, 0 - 8 + message_timer);
  }
  arduboy.print(message);
 }
 else
 {
  arduboy.setCursor(0, 0);
  arduboy.print("SCORE ");
  arduboy.print(score);
 }
}

void send_message(char * string)
{
 message_timer = 30;
 for(byte a = 0; a < 16; a++)
 {
  message[a] = string[a];
  if(!string[a]) break;
 }
}

static byte selected_game = 0;

#define GAME_COUNT 3

Game games[GAME_COUNT];

void draw_game_menu(byte n, int x)
{
  arduboy.setCursor(x * 2, 0);
  arduboy.print("HIGH SCORE ");
  arduboy.print(games[n].high_score);
  arduboy.setCursor(x * 2, 8);
  arduboy.print("TIME PLAYED ");
  arduboy.print((unsigned long)(games[n].time / 60 / 60));
  arduboy.print(":");
  if((unsigned long)(games[n].time / 60 % 60) < 10) arduboy.print("0");
  arduboy.print((unsigned long)(games[n].time / 60 % 60)); 
  
  x = x * 8 / 7;
  byte * buffer = arduboy.getBuffer();
  for(byte a = max(0 - x - 32, 0); a < min(128 - (x + 32), 64); a++)
  {
    for(byte b = 0; b < 2; b++)
    {
      buffer[x + 32 + a + (b + 3) * 128] = pgm_read_byte(title_sprites + n * 64 * 2 + a + b * 64);
    }
  }
}

void allow_mute()
{
  if(key_up == 1 && arduboy.audio.enabled() == 0)
  {
    arduboy.audio.on();
    SFX_Data sfx_unmute = {32, 34 - 32, 0, 7, 0, 12, 0};
    sfx(&sfx_unmute);
  }
  if(key_down == 1)
  {
    arduboy.audio.off();
  }
}

extern void create_game1(Game & game);
extern void create_game2(Game & game);
extern void create_game3(Game & game);

#define GAME_SAVE_SIZE 6 // int + long

#define EEPROM_SAVE_START 27

static char game_name[] = "Puzzle Pack";

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.display();
  create_game1(games[0]);
  create_game2(games[1]);
  create_game3(games[2]);
  byte save_found = 1;
  for(byte a = 0; game_name[a] != 0; a++)
  {
    if(EEPROM.read(EEPROM_STORAGE_SPACE_START + a) != game_name[a])
    {
      EEPROM.write(EEPROM_STORAGE_SPACE_START + a, game_name[a]);
      save_found = 0;
    }
  }
  if(save_found)
  {
    for(byte a = 0; a < GAME_COUNT; a++)
    {
      games[a].high_score = EEPROM.read(EEPROM_SAVE_START + a * GAME_SAVE_SIZE);
      games[a].high_score <<= 8;
      games[a].high_score += EEPROM.read(EEPROM_SAVE_START + a * GAME_SAVE_SIZE + 1);
      games[a].time = 0;
      for(byte b = 0; b < 4; b++)
      {
        games[a].time <<= 8;
        games[a].time += EEPROM.read(EEPROM_SAVE_START + a * GAME_SAVE_SIZE + 2 + b);
      }
    }
  }
  else
  {
    for(byte a = 0; a < GAME_COUNT * GAME_SAVE_SIZE; a++)
    {
      EEPROM.write(EEPROM_SAVE_START + a, 0);
    }
  }
  fade_to(set_state_menu);
  EEPROM.commit();
}

static void new_game()
{
  message_timer = 0;
  game_tick = 0;
  score = 0;
  games[selected_game]._create();
  arduboy.initRandomSeed();
  set_state_play();
}

static void state_play()
{
  if(key_a == 1) set_state_paused();
  else
  {
    games[selected_game]._update();
    games[selected_game]._draw();
    draw_overlay();
  }
}

static void state_game_over()
{
  games[selected_game]._draw();
  draw_overlay();
  arduboy.setCursor(64 - 9 * 6 / 2, 32);
  arduboy.print("GAME OVER");
  if(tick > 30 && key_b) 
  {
    games[selected_game]._delete();
    fade_to(set_state_menu);
  }
}

static void state_paused()
{
  arduboy.fillRect(0, 0, 128, 8, 0);
  arduboy.setCursor(128 - (tick + 128) % 256, 0);
  arduboy.print("SCORE ");
  arduboy.print(score);
  arduboy.print(" ");
  arduboy.setCursor(128 - tick % 256, 0);
  arduboy.print("HIGH SCORE ");
  arduboy.print(games[selected_game].high_score);
  arduboy.print(" ");
   byte * buffer = arduboy.getBuffer();
   for(byte a = tick % 4; a < minVal(128, tick); a += 4)
   {
    byte rollover1 = 0;
    byte rollover2 = 0;
    if((tick - a) % 32 < 16)
    {
      for(byte b = 1; b < 8; b++)
      {
       rollover1 = buffer[a + b * 128] >> 7;
       buffer[a + b * 128] = buffer[a + b * 128] << 1;
       buffer[a + b * 128] |= rollover2;
       rollover2 = rollover1;
      }
      buffer[a + 128] |= rollover2;
    }
    else
    {
      for(byte b = 7; b > 0; b--)
      {
       rollover1 = buffer[a + b * 128] << 7;
       buffer[a + b * 128] = buffer[a + b * 128] >> 1;
       buffer[a + b * 128] |= rollover2;
       rollover2 = rollover1;
      }
      buffer[a + 7 * 128] |= rollover2;
    }
   }
   if(key_a == 1)
   {
     set_state_play();
   }
   allow_mute();
}

#define STAR_COUNT 64
byte * stars;

static void state_menu()
{
  static int x = 0;
  arduboy.clear();
  arduboy.setCursor(0 - tick % 128, 56);
  arduboy.print("SELECT GAME");
  arduboy.setCursor(0 - tick % 128 + 128, 56);
  arduboy.print("SELECT GAME");
  if(key_left == 1)
  {
    selected_game--;
    if(selected_game >= GAME_COUNT) selected_game = GAME_COUNT - 1;
    x -= 64;
    sfx(&sfx_change_game);
  }
  if(key_right == 1)
  {
    selected_game++;
    selected_game %= GAME_COUNT;
    x += 64;
    sfx(&sfx_change_game);
  }
  x /= 2;
  if(x < 2 && x > -2) x = 0;
  draw_game_menu(selected_game, x);
  draw_game_menu((selected_game + GAME_COUNT - 1) % GAME_COUNT, x - 64);
  draw_game_menu((selected_game + 1) % GAME_COUNT, x + 64);
  stars[STAR_COUNT - 1 - tick % STAR_COUNT] = random(0, 256);
  for(byte a = tick & 1; a < STAR_COUNT; a += 2)
  {
    float z = 1 - float((a + tick) % STAR_COUNT) / 64;
    arduboy.drawPixel((float(stars[a] >> 4) - 8) * 2 / z + 64,
                      (float(stars[a] & 15) - 8) * 2 / z + 32, 1);
  };
  if(key_b == 1) 
  {
    free(stars);
    fade_to(new_game);
    SFX_Data sfx_start_game = {32, 33 - 32, 0, 6, 1, 128, 0};
    sfx(&sfx_start_game);
  }
  allow_mute();
}

static void (* fade_to_function)();

static void state_fade()
{
  byte * buffer = arduboy.getBuffer();
  for(byte a = 0; a < 128; a++)
  {
   byte rollover1 = 0;
   byte rollover2 = 0;
   for(byte b = 0; b < 8; b++)
   {
    rollover1 = buffer[a + b * 128] >> 7 << 6;
    buffer[a + b * 128] = buffer[a + b * 128] << 1;
    buffer[a + b * 128] |= rollover2;
    rollover2 = rollover1;
   }
  }
  if(tick > 28) fade_to_function();
}

void fade_to(void (* f)())
{
  tick = 0;
  fade_to_function = f;
  game_state = state_fade;
}

void set_state_menu()
{
  stars = (byte*)malloc(sizeof(byte) * STAR_COUNT);
  for(byte a = 0; a < STAR_COUNT; a++)
  {
    stars[a] = B10001000;
  }
  game_state = state_menu;
}

void set_state_play()
{
  game_state = state_play;
//  arduboy.audio.save_on_off();
}

void set_state_paused()
{
  tick = 0;
  arduboy.setCursor(64 - 6 * 6 / 2, 32);
  arduboy.print("PAUSED");
  game_state = state_paused;
}

void set_state_game_over()
{
  if(score > games[selected_game].high_score)
  {
    EEPROM.write(EEPROM_SAVE_START + selected_game * GAME_SAVE_SIZE, score >> 8);
    EEPROM.write(EEPROM_SAVE_START + selected_game * GAME_SAVE_SIZE + 1, score);
    games[selected_game].high_score = score;
  }
  games[selected_game].time += game_tick / 30;
  for(byte b = 0; b < 4; b++)
  {
    EEPROM.write(EEPROM_SAVE_START + selected_game * GAME_SAVE_SIZE + 2 + b, games[selected_game].time >> (3 - b) * 8);
  }
  tick = 0;
  game_state = state_game_over;
  SFX_Data sfx_game_over = {128, 31 - 32, 0, 7, 4, 256, 0};
  sfx(&sfx_game_over);
  EEPROM.commit();
}

void loop() {
  if (!(arduboy.nextFrame())) return;
  if(arduboy.pressed(LEFT_BUTTON)) key_left++;
  else key_left = 0;
  if(arduboy.pressed(UP_BUTTON)) key_up++;
  else key_up = 0;
  if(arduboy.pressed(RIGHT_BUTTON)) key_right++;
  else key_right = 0;
  if(arduboy.pressed(DOWN_BUTTON)) key_down++;
  else key_down = 0;
  if(arduboy.pressed(A_BUTTON)) key_a++;
  else key_a = 0;
  if(arduboy.pressed(B_BUTTON)) key_b++;
  else key_b = 0;
  game_state();
  tick++;
  if(arduboy.audio.enabled() == 0)
  {
    arduboy.setCursor(128 - 4 * 6, 0);
    arduboy.print("MUTE");
  }
  arduboy.display();
}
