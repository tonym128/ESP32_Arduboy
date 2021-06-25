/*
** Pocket Fighter - An FTG for Arduboy
** V 1.0.2
**
** For the latest info, see https://github.com/pocket-arduino/pocket_fighter/
**
** Copyright (C) 2017 Wang Renxin. All rights reserved.
*/

#include <Arduboy2.h>
#include <ESPboyPlaytune.h>

/**< Time related. */
#define FPS 60 // Frames per second.
#define APS 60 // Actions per second.
#define APT (FPS / APS) // Actions per tick.
#define AFPS 6 // Frames of animation per second.
#define AFPT (FPS / AFPS) // Frames of animation per tick.
#define JMP_TICKS 18 // Ticks of jumping to peak.
#define HDK_TICKS 19 // Life ticks of hadouken.
/**< For compact storage. */
#define ACT_BITS 3 // For action types storage.
#define TMS_BITS 13 // For action timestamp storage.
/**< The borders of the scene. */
#define BORDER_LEFT 3
#define BORDER_RIGHT 123
#define BORDER_TOP 7
#define BORDER_BOTTOM 54
#define SCREEN_LEFT 0
#define SCREEN_RIGHT 127
/**< Game states. */
#define GS_MENU 0
#define GS_FIGHT 1
#define GS_WIN 2
#define GS_LOSE 3
#define GS_NEXT 4
#define GS_GAMEOVER 5
/**< The save location of the best survived level. */
#define SAVE_LOCATION (EEPROM_STORAGE_SPACE_START + 888)
/**< Some tones. */
#define TONE_PLAYER_MOVE 350
#define TONE_PLAYER_JUMP 370
#define TONE_PLAYER_ATK 380
#define TONE_CPU_MOVE 250
#define TONE_CPU_JUMP 270
#define TONE_CPU_ATK 280

#ifndef countof
# define countof(__a) (sizeof(__a) / sizeof(*(__a)))
#endif /* countof */

#ifndef static_assert
# define _static_assert_impl(cond, msg) typedef char static_assertion_##msg[(!!(cond)) * 2 - 1]
# define _compile_time_assert3(x, l) _static_assert_impl(x, static_assertion_at_line_##l)
# define _compile_time_assert2(x, l) _compile_time_assert3(x, l)
# define static_assert(x) _compile_time_assert2(x, __LINE__)
#endif /* static_assert */

typedef enum Actions {
  ACT_NONE,
  ACT_LEFT,
  ACT_RIGHT,
  ACT_IDLE,
  ACT_PUNCH,
  ACT_SHOOT,
  ACT_JUMP,
  ACT_RESERVED,
  ACT_COUNT
} Actions;

typedef struct Record {
  Actions action : ACT_BITS;
  uint16_t timeStamp : TMS_BITS;
} Record;

typedef struct Fighter {
  uint8_t hp : 2;
  uint8_t dir : 1;
  uint8_t mirror : 1;
  uint8_t x : 7, y : 6;
  uint8_t dx : 3, dy : 3;
  uint8_t bx : 7, by : 6;
  uint8_t dbx : 1;
  uint8_t hadoukenLife : 5;
  uint8_t tookPunch : 1;
  uint8_t jumpTicks : 6;
  Actions action : ACT_BITS;
  Actions actionCache : ACT_BITS;
  uint8_t frameTicks : 5;
  uint8_t frame : 3;
} Fighter;

typedef void (* AttackAnimationFinishedEventHandler)(Fighter*, bool);

 const uint8_t SPRITE_HEART[] = { 0x00, 0x1c, 0x3e, 0x7e, 0xfc, 0x5e, 0x26, 0x1c };
 const uint8_t SPRITE_HEAD[][8] = {
  { 0x7e, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7e },
  { 0x7e, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0x7e }
};
 const uint8_t SPRITE_BODY_IDLE[][8] = {
  { 0x8c, 0x8c, 0x80, 0x00, 0x00, 0x80, 0x8c, 0x8c },
  { 0x98, 0x98, 0x80, 0x00, 0x00, 0x80, 0x98, 0x98 }
};
 const uint8_t SPRITE_BODY_MOVE[][8] = {
  { 0x48, 0x48, 0x40, 0x00, 0x00, 0x80, 0x8c, 0x8c },
  { 0x8c, 0x8c, 0x80, 0x00, 0x00, 0x40, 0x48, 0x48 }
};
 const uint8_t SPRITE_BODY_PUNCH0[][11] = {
  { 0x8c, 0x8c, 0x80, 0x00, 0x00, 0x80, 0x8c, 0x8c, 0x00, 0x00, 0x00 },
  { 0x8c, 0x8c, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x0c, 0x0c, 0x00 },
  { 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x8c, 0x8c, 0x00, 0x0c, 0x0c }
};
 const uint8_t SPRITE_BODY_PUNCH1[][11] = {
  { 0x00, 0x00, 0x00, 0x8c, 0x8c, 0x80, 0x00, 0x00, 0x80, 0x8c, 0x8c },
  { 0x00, 0x0c, 0x0c, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x8c, 0x8c },
  { 0x0c, 0x0c, 0x00, 0x8c, 0x8c, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80 }
};
 const uint8_t SPRITE_BODY_SHOOT0[][11] = {
  { 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x36, 0x36, 0x00 },
  { 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80, 0x36, 0x36, 0x00 }
};
 const uint8_t SPRITE_BODY_SHOOT1[][11] = {
  { 0x00, 0x36, 0x36, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80 },
  { 0x00, 0x36, 0x36, 0x80, 0x80, 0x80, 0x00, 0x00, 0x80, 0x80, 0x80 }
};
 const uint8_t SPRITE_BULLET[] = { 0x03, 0x03 };
 const uint8_t SPRITE_ARROW[] = { 0xfe, 0x7c, 0x38, 0x10, 0x00, 0x00, 0x00, 0x00 };
 const uint8_t SPRITE_SOUND_ON[] = { 0x38, 0x38, 0x44, 0x82, 0xfe, 0x00, 0x54, 0x92 };
 const uint8_t SPRITE_SOUND_OFF[] = { 0x38, 0x38, 0x44, 0x82, 0xfe, 0x00, 0x00, 0x00 };
 const uint8_t SPRITE_GO[] = { 0x7c, 0x82, 0xe4, 0x00, 0x7c, 0x82, 0x7c, 0x00, 0xbe };
 const uint8_t SPRITE_COPYRIGHT[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x24, 0x00, 0x18, 0x24, 0x24, 0x00, 0x24, 0x18,
  0x00, 0x00, 0x1c, 0x20, 0x1c, 0x20, 0x1c, 0x00, 0x10, 0x28, 0x38, 0x00, 0x38, 0x08, 0x30, 0x00,
  0x48, 0x54, 0x28, 0x00, 0x3c, 0x14, 0x28, 0x00, 0x18, 0x34, 0x28, 0x00, 0x38, 0x08, 0x30, 0x00,
  0x28, 0x10, 0x28, 0x00, 0x34, 0x00, 0x38, 0x08, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x20,
  0x1c, 0x00, 0x3e, 0x00, 0x20, 0x00, 0x1c, 0x22, 0x1c, 0x00, 0x20, 0x00, 0x32, 0x2a, 0x24, 0x00
};
 const uint8_t SPRITE_TITLE[] = {
  0x00, 0x00, 0x00, 0xfc, 0x02, 0x02, 0x00, 0xfe, 0x00, 0x02, 0xfc, 0x00, 0xfc, 0x00, 0x02, 0xfc,
  0x00, 0xfc, 0x00, 0x02, 0x04, 0x00, 0xfe, 0x00, 0x90, 0x0e, 0x00, 0xfe, 0x00, 0x02, 0x02, 0x00,
  0x02, 0x00, 0xfe, 0x02, 0x02, 0x00, 0x02, 0x02, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0xfc, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x01, 0x00, 0x00, 0x3f, 0x00, 0x40, 0x3f,
  0x00, 0x3f, 0x00, 0x40, 0x20, 0x00, 0x7f, 0x00, 0x03, 0x7c, 0x00, 0x7f, 0x00, 0x41, 0x41, 0x00,
  0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x82, 0x82, 0x00, 0x02, 0x00, 0xfe, 0x02, 0x00, 0xfc,
  0x00, 0x82, 0x84, 0x00, 0xfe, 0x00, 0x80, 0xfe, 0x00, 0x02, 0x00, 0xfe, 0x02, 0x02, 0x00, 0xfe,
  0x00, 0x02, 0x02, 0x00, 0xfe, 0x00, 0x02, 0xfc, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3f, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40,
  0x40, 0x40, 0x40, 0x40, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x7f, 0x40, 0x00, 0x3f,
  0x00, 0x40, 0x7f, 0x00, 0x7f, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x7f,
  0x00, 0x41, 0x41, 0x00, 0x7f, 0x00, 0x01, 0x7e, 0x00, 0x40, 0x40, 0x3f, 0x00, 0x00, 0x00, 0x00
};
 const uint8_t SPRITE_NEXT[] = {
  0x00, 0x00, 0x00, 0xfc, 0x02, 0x02, 0x00, 0xfe, 0x00, 0xe0, 0x00, 0xfe, 0x00, 0xfe, 0x00, 0x02,
  0x02, 0x00, 0x0e, 0x10, 0x80, 0x70, 0x0e, 0x00, 0x02, 0x00, 0xfe, 0x02, 0x02, 0x00, 0x00, 0xfe,
  0x00, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x02, 0x02, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xfe, 0x00, 0xfe,
  0x00, 0x02, 0x02, 0x00, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x02, 0x02, 0xfc, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x07, 0x7f, 0x00, 0x7f, 0x00, 0x41,
  0x41, 0x00, 0x78, 0x04, 0x00, 0x07, 0x78, 0x00, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x7f,
  0x00, 0x40, 0x40, 0x00, 0x7f, 0x00, 0x41, 0x41, 0x00, 0x07, 0x00, 0x40, 0x38, 0x07, 0x00, 0x7f,
  0x00, 0x41, 0x41, 0x00, 0x7f, 0x00, 0x40, 0x40, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0xfe, 0x00, 0x02, 0xfc, 0x00, 0xfe, 0x00, 0x02, 0xfc,
  0x00, 0xfe, 0x00, 0x02, 0x02, 0x00, 0x7c, 0x00, 0x02, 0x04, 0x00, 0x7c, 0x00, 0x02, 0x04, 0x00,
  0x00, 0x00, 0x00, 0xfe, 0x00, 0x02, 0xfc, 0x00, 0xfc, 0x00, 0x02, 0xfc, 0x00, 0xfe, 0x00, 0xfe,
  0x00, 0xfe, 0x00, 0xfe, 0x00, 0xe0, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3f, 0x40, 0x40, 0x00, 0x7f, 0x00, 0x01, 0x00, 0x00, 0x7f, 0x00, 0x01, 0x7e,
  0x00, 0x7f, 0x00, 0x41, 0x41, 0x00, 0x20, 0x00, 0x41, 0x3e, 0x00, 0x20, 0x00, 0x41, 0x3e, 0x00,
  0x00, 0x00, 0x00, 0x7f, 0x00, 0x40, 0x3f, 0x00, 0x3f, 0x00, 0x40, 0x3f, 0x00, 0x3f, 0x00, 0x3f,
  0x40, 0x3f, 0x00, 0x7f, 0x00, 0x00, 0x07, 0x7f, 0x00, 0x40, 0x40, 0x3f, 0x00, 0x00, 0x00, 0x00
};
 const uint8_t SPRITE_OVER[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0xc0, 0x60, 0x20, 0xe0, 0x20, 0x60, 0xc0, 0x00, 0xfc, 0x00, 0x82, 0x84, 0x00, 0x00, 0x00, 0x06,
  0x78, 0x80, 0x00, 0xfe, 0x00, 0x80, 0x7c, 0xfe, 0x00, 0xfe, 0x00, 0x02, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xfc, 0x00, 0x02, 0xfc, 0x00, 0xfe, 0x00, 0x00, 0x00, 0xfe, 0x00, 0xfe,
  0x00, 0x02, 0x02, 0x00, 0xfe, 0x00, 0x02, 0xfc, 0x00, 0xc0, 0x60, 0x20, 0xe0, 0x20, 0x60, 0xc0,
  0x01, 0x03, 0x01, 0x03, 0x01, 0x03, 0x01, 0x00, 0x3f, 0x00, 0x40, 0x7f, 0x00, 0x7f, 0x00, 0x04,
  0x04, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x00, 0x41, 0x41, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x40, 0x3f, 0x00, 0x07, 0x00, 0x40, 0x38, 0x07, 0x00, 0x7f,
  0x00, 0x41, 0x41, 0x00, 0x7f, 0x00, 0x01, 0x7e, 0x00, 0x01, 0x03, 0x01, 0x03, 0x01, 0x03, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
  0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
  0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04,
  0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static Arduboy2 A; // The Arduboy instance.
static ESPboyPlaytune N(A.audio.enabled); // For tune playing.

static Record R[300]; // Action records.
static uint8_t C0 = 0; // Cursor of the Player's action records.
static uint8_t C1 = 0; // Cursor of the CPU's action records.
static uint8_t T0 = 0; // Ticks of the Player's current action record.
static uint16_t T1 = 0; // Ticks of the CPU's current action record.
static_assert((countof(R) % 2) == 0);

static Fighter F[2]; // Fighter objects. F[0] is your Player, F[1] is the CPU.

static uint8_t GS = GS_MENU; // The game state.

static uint8_t L = 0; // Current level.
static uint8_t B = 1; // Best level.

static void sound(uint16_t pitch, long duration) {
  if (A.audio.enabled())
    N.tone(pitch, duration);
}

static void prepare(void) {
  // New best level?
  if (++L > B) {
    B = L;
    EEPROM.put(SAVE_LOCATION, B);
    EEPROM.commit();
  }
  // Fighter states.
  uint8_t i = 0, j = 1;
  if ((L % 2) == 0) { i = 1; j = 0; }
  F[i].dir = 0;
  F[i].mirror = 0;
  F[i].x = BORDER_LEFT + 16;
  F[i].y = BORDER_BOTTOM;
  F[i].dx = F[i].dy = 0;
  F[i].bx = F[i].by = F[i].dbx = 0;
  F[i].hadoukenLife = 0;
  F[i].tookPunch = 0;
  F[i].jumpTicks = 0;
  F[i].action = ACT_IDLE;
  F[i].actionCache = ACT_NONE;
  F[i].frameTicks = 0;
  F[i].frame = 0;
  F[j].dir = 1;
  F[j].mirror = 0;
  F[j].x = BORDER_RIGHT - 16;
  F[j].y = BORDER_BOTTOM;
  F[j].dx = F[j].dy = 0;
  F[j].bx = F[j].by = F[j].dbx = 0;
  F[j].hadoukenLife = 0;
  F[j].tookPunch = 0;
  F[j].jumpTicks = 0;
  F[j].action = ACT_IDLE;
  F[j].actionCache = ACT_NONE;
  F[j].frameTicks = 0;
  F[j].frame = 0;
  // Adds one point of HP to the Player per level passed.
  if (F[0].hp < 3) ++F[0].hp;
  // Refills the opponent's HP.
  F[1].hp = 3;
  // Moves the action records from the buffer of Player to the one of CPU, and clears the buffer of Player.
  C0 = 0;
  C1 = countof(R) / 2;
  T0 = T1 = 0;
  memmove(R + countof(R) / 2, R, sizeof(R) / 2);
  memset(R, 0, sizeof(R) / 2);
}

static void addPlayerRecord(Actions act);

static void setFighterAction(Fighter* f, Actions act) {
  f->action = act;
  if (f->actionCache == act) f->actionCache = ACT_NONE;
  f->frameTicks = 0;
  f->frame = 0;
}
static bool isFighterAction(Fighter* f, Actions act) {
  return f->action == act;
}
static void setFighterProperAction(Fighter* f, bool isSelf) {
  if (f->dy == 0) {
    if (isSelf && f->actionCache == ACT_LEFT) {
      addPlayerRecord(ACT_LEFT);
      setFighterAction(f, ACT_LEFT);
    } else if (isSelf && f->actionCache == ACT_RIGHT) {
      addPlayerRecord(ACT_RIGHT);
      setFighterAction(f, ACT_RIGHT);
    } else {
      f->action = ACT_IDLE;
    }
  } else {
    f->action = ACT_JUMP;
  }
  f->frameTicks = 0;
}
static void setFighterActionCache(Fighter* f, Actions act) {
  f->actionCache = act;
}

static bool isFighterShooting(Fighter* f) {
  return f->hadoukenLife != 0;
}
static void fighterShoot(Fighter* f) {
  f->hadoukenLife = HDK_TICKS;
  if (f->dir == 0) {
    f->bx = f->x + 3;
    f->by = f->y - 6;
  } else {
    f->bx = f->x - 3;
    f->by = f->y - 6;
  }
  f->dbx = f->dir;
}
static void finishFighterShoot(Fighter* f) {
  f->hadoukenLife = 0;
}
static void tickFighterShoot(Fighter* f) {
  if (!isFighterShooting(f)) return;
  if (--f->hadoukenLife == 0) return;
  if (f->dbx == 0) {
    if (++f->bx >= SCREEN_RIGHT) finishFighterShoot(f);
    else if (++f->bx >= SCREEN_RIGHT) finishFighterShoot(f);
  } else {
    if (f->bx <= SCREEN_LEFT) finishFighterShoot(f);
    else if (--f->bx <= SCREEN_LEFT) finishFighterShoot(f);
    else if (--f->bx <= SCREEN_LEFT) finishFighterShoot(f);
  }
}

static bool isFighterJumping(Fighter* f) {
  return f->dy != 0;
}
static void setFighterJump(Fighter* f) {
  uint8_t dy = f->dy + 2;
  if (dy > 5) dy = 5;
  f->dy = dy;
  f->jumpTicks = 0;
}
static void tickFighterJump(Fighter* f) {
  if (f->dy != 0) {
    if (++f->jumpTicks < JMP_TICKS) {
      f->x += f->dx; f->y -= f->dy;
      if (f->y < BORDER_TOP) {
        f->y = BORDER_TOP;
      }
    } else if (++f->jumpTicks >= JMP_TICKS) {
      f->x += f->dx; f->y += f->dy;
      if (f->y >= BORDER_BOTTOM) {
        f->y = BORDER_BOTTOM;
        f->jumpTicks = 0;
        f->dx = f->dy = 0;
        if (f->action == ACT_JUMP) {
          setFighterAction(f, ACT_IDLE);
        }
      }
    }
  }
}

static void addPlayerRecord(Actions act) {
  tickPlayerRecord(true);
  if (++C0 >= countof(R) / 2) C0 = 0;
  R[C0].action = act;
  R[C0].timeStamp = 0;
  T0 = 0;
}
static Record* getPlayerRecord(void) {
  return &R[C0];
}
static void tickPlayerRecord(bool force) {
  if (++T0 >= APT || force) {
    T0 = 0;
    uint16_t ts = getPlayerRecord()->timeStamp++;
    if (getPlayerRecord()->timeStamp < ts)
      getPlayerRecord()->timeStamp = ts;
  }
}
static Record* getCpuRecord(void) {
  return &R[C1];
}
static void nextCpuRecord(void) {
  if (++C1 >= countof(R)) C1 = countof(R) / 2;
  else if (R[C1].action == ACT_NONE) C1 = countof(R) / 2;
  T1 = 0;
}

static void flipCpu(Fighter* f) {
  f->dir = f->dir == 0 ? 1 : 0;
  f->mirror = f->mirror == 0 ? 1 : 0;
}

static void detectCollision(Fighter* fl, Fighter* fr) {
  bool collided = false;
  if (isFighterShooting(fl)) {
    if (A.collide(Point { fl->bx, fl->by }, Rect { fr->x - 3, fr->y - 14, 8, 15 })) {
      collided = true;
      if (fr->hp > 0) --fr->hp;
      finishFighterShoot(fl);
    }
  }
  if (collided) return;
  if (fl->tookPunch == 0 && isFighterAction(fl, ACT_PUNCH)) {
    if (A.collide(Rect { fl->dir == 0 ? fl->x + 3 : fl->x - 2, fl->y - 6, 5, 4 }, Rect { fr->x - 3, fr->y - 14, 8, 15 })) {
      if (fr->hp > 0) --fr->hp;
      fl->tookPunch = 1;
    }
  }
}

static void drawFighter(Fighter* f, bool isSelf, AttackAnimationFinishedEventHandler atkFin) {
  A.drawBitmap(f->x - 3, f->y - 14, isSelf ? SPRITE_HEAD[0] : SPRITE_HEAD[1], 8, 8, WHITE);
  switch (f->action) {
    case ACT_LEFT:
    case ACT_RIGHT:
    case ACT_JUMP:
      if (f->frame >= countof(SPRITE_BODY_MOVE))
        f->frame = 0;
      A.drawBitmap(f->x - 3, f->y - 7, SPRITE_BODY_MOVE[f->frame], 8, 8, WHITE);
      break;
    case ACT_IDLE:
      if (f->frame >= countof(SPRITE_BODY_IDLE))
        f->frame = 0;
      A.drawBitmap(f->x - 3, f->y - 7, SPRITE_BODY_IDLE[f->frame], 8, 8, WHITE);
      break;
    case ACT_PUNCH:
      if (f->dir == 0) {
        if (f->frame >= countof(SPRITE_BODY_PUNCH0)) {
          f->frame = 0;
          f->tookPunch = 0;
          if (atkFin) atkFin(f, isSelf);
        }
        A.drawBitmap(f->x - 3, f->y - 7, SPRITE_BODY_PUNCH0[f->frame], 11, 8, WHITE);
      } else {
        if (f->frame >= countof(SPRITE_BODY_PUNCH1)) {
          f->frame = 0;
          f->tookPunch = 0;
          if (atkFin) atkFin(f, isSelf);
        }
        A.drawBitmap(f->x - 6, f->y - 7, SPRITE_BODY_PUNCH1[f->frame], 11, 8, WHITE);
      }
      break;
    case ACT_SHOOT:
      if (f->dir == 0) {
        if (f->frame >= countof(SPRITE_BODY_SHOOT0)) {
          f->frame = 0;
          if (atkFin) atkFin(f, isSelf);
        }
        A.drawBitmap(f->x - 3, f->y - 7, SPRITE_BODY_SHOOT0[f->frame], 11, 8, WHITE);
      } else {
        if (f->frame >= countof(SPRITE_BODY_SHOOT1)) {
          f->frame = 0;
          if (atkFin) atkFin(f, isSelf);
        }
        A.drawBitmap(f->x - 6, f->y - 7, SPRITE_BODY_SHOOT1[f->frame], 11, 8, WHITE);
      }
      break;
  }
  if (isFighterShooting(f)) {
    A.drawBitmap(f->bx, f->by, SPRITE_BULLET, 2, 8, WHITE);
  }
}
static void drawProgress(void) {
  A.setCursor(8, BORDER_BOTTOM + 3);
  A.print("LEVEL ");
  A.print(L);
  A.print(" BEST ");
  A.print(B);
}
static void drawHUD(Fighter* l, Fighter* r) {
  for (uint8_t i = 0; i < l->hp; ++i) {
    A.drawBitmap(8 + 8 * i, 0, SPRITE_HEART, 8, 8, WHITE);
  }
  for (uint8_t i = 0; i < r->hp; ++i) {
    A.drawBitmap(110 - 8 * i, 0, SPRITE_HEART, 8, 8, WHITE);
  }
  A.drawLine(0, BORDER_BOTTOM + 1, 127, BORDER_BOTTOM + 1, WHITE);
  drawProgress();
}

static void menu(void) {
  if (A.justPressed(UP_BUTTON | DOWN_BUTTON)) {
    if (T1 == 0) T1 = 1;
    else if (T1 == 1) T1 = 0;
    sound(500, 100);
  } else if (A.justReleased(A_BUTTON | B_BUTTON | LEFT_BUTTON | RIGHT_BUTTON)) {
    if (T1 == 0) {
      if (!!C0 != !!A.audio.enabled())
        A.audio.saveOnOff();
      prepare();
      GS = GS_FIGHT;
    } else {
      if (A.audio.enabled()) A.audio.off();
      else A.audio.on();
    }
    sound(500, 100);
  }
  ++T0;
  A.drawBitmap(16, 16, SPRITE_TITLE, 64, 32, WHITE);
  A.drawBitmap(65 - 3, 30 - 14, SPRITE_HEAD[0], 8, 8, WHITE);
  A.drawBitmap(65 - 3, 30 - 7, SPRITE_BODY_IDLE[(T0 % (AFPT * 2)) < AFPT ? 0 : 1], 8, 8, WHITE);
  A.drawBitmap(90, 30 + T1 * 8, SPRITE_ARROW, 8, 8, WHITE);
  A.drawBitmap(98, 30, SPRITE_GO, 9, 8, WHITE);
  A.drawBitmap(98, 38, A.audio.enabled() ? SPRITE_SOUND_ON : SPRITE_SOUND_OFF, 8, 8, WHITE);
  A.drawBitmap(0, 55, SPRITE_COPYRIGHT, 128, 8, WHITE);
}
static void fight(void) {
  // Player control.
  if (A.justPressed(A_BUTTON) && !isFighterAction(&F[0], ACT_PUNCH) && !isFighterAction(&F[0], ACT_SHOOT)) {
    sound(TONE_PLAYER_ATK, 80);
    addPlayerRecord(ACT_PUNCH);
    setFighterAction(&F[0], ACT_PUNCH);
    if (A.pressed(LEFT_BUTTON)) setFighterActionCache(&F[0], ACT_LEFT);
    else if (A.pressed(RIGHT_BUTTON)) setFighterActionCache(&F[0], ACT_RIGHT);
  } else if (A.justPressed(B_BUTTON) && !isFighterAction(&F[0], ACT_PUNCH) && !isFighterAction(&F[0], ACT_SHOOT) && !isFighterShooting(&F[0])) {
    sound(TONE_PLAYER_ATK, 80);
    addPlayerRecord(ACT_SHOOT);
    setFighterAction(&F[0], ACT_SHOOT);
    fighterShoot(&F[0]);
    if (A.pressed(LEFT_BUTTON)) setFighterActionCache(&F[0], ACT_LEFT);
    else if (A.pressed(RIGHT_BUTTON)) setFighterActionCache(&F[0], ACT_RIGHT);
  } else if (A.justPressed(LEFT_BUTTON)) {
    sound(TONE_PLAYER_MOVE, 80);
    _left:
    addPlayerRecord(ACT_LEFT);
    setFighterAction(&F[0], ACT_LEFT);
  } else if (A.justPressed(RIGHT_BUTTON)) {
    sound(TONE_PLAYER_MOVE, 80);
    _right:
    addPlayerRecord(ACT_RIGHT);
    setFighterAction(&F[0], ACT_RIGHT);
  } else if (A.justPressed(UP_BUTTON)) {
    if (!isFighterJumping(&F[0])) {
      sound(TONE_PLAYER_JUMP, 80);
      addPlayerRecord(ACT_JUMP);
      setFighterAction(&F[0], ACT_JUMP);
      setFighterJump(&F[0]);
      if (A.pressed(LEFT_BUTTON)) goto _left;
      else if (A.pressed(RIGHT_BUTTON)) goto _right;
    }
  }
  if (A.justReleased(LEFT_BUTTON)) {
    if (isFighterAction(&F[0], ACT_LEFT)) {
      addPlayerRecord(ACT_IDLE);
      setFighterAction(&F[0], ACT_IDLE);
    }
    setFighterActionCache(&F[0], ACT_NONE);
  } else if (A.justReleased(RIGHT_BUTTON)) {
    if (isFighterAction(&F[0], ACT_RIGHT)) {
      addPlayerRecord(ACT_IDLE);
      setFighterAction(&F[0], ACT_IDLE);
    }
    setFighterActionCache(&F[0], ACT_NONE);
  }
  tickPlayerRecord(false);
  // Plays the records of CPU.
  if (T1 == 0) {
    ++T1;
    Actions act = getCpuRecord()->action;
    switch (act) {
      case ACT_NONE:
        nextCpuRecord();
        break;
      case ACT_LEFT: /* Fall through. */
      case ACT_RIGHT:
        sound(TONE_CPU_MOVE, 80);
        setFighterAction(&F[1], act);
        break;
      case ACT_IDLE:
        setFighterAction(&F[1], act);
        break;
      case ACT_PUNCH:
        sound(TONE_CPU_ATK, 80);
        setFighterAction(&F[1], act);
        break;
      case ACT_SHOOT:
        sound(TONE_CPU_ATK, 80);
        if (isFighterShooting(&F[1])) {
          T1 = 0;
        } else {
          setFighterAction(&F[1], act);
          fighterShoot(&F[1]);
        }
        break;
      case ACT_JUMP:
        sound(TONE_CPU_JUMP, 80);
        setFighterAction(&F[1], act);
        setFighterJump(&F[1]);
        break;
    }
  } else if (++T1 >= APT * getCpuRecord()->timeStamp) {
    T1 = 0;
    nextCpuRecord();
  }
  // Updates movement of fighters.
  Fighter* f = &F[0];
  if (isFighterAction(f, ACT_LEFT)) {
    f->dir = 1;
    if (--f->x < BORDER_LEFT) f->x = BORDER_LEFT;
  } else if (isFighterAction(f, ACT_RIGHT)) {
    f->dir = 0;
    if (++f->x > BORDER_RIGHT) f->x = BORDER_RIGHT;
  }
  f = &F[1];
  bool moving = false;
  if (isFighterAction(f, ACT_LEFT)) {
    moving = true;
    if (f->mirror) f->dir = 0;
    else f->dir = 1;
  } else if (isFighterAction(f, ACT_RIGHT)) {
    moving = true;
    if (f->mirror) f->dir = 1;
    else f->dir = 0;
  }
  if (moving) {
    if (f->dir == 1) {
      if (--f->x < BORDER_LEFT) {
        f->x = BORDER_LEFT;
        flipCpu(f);
      }
    } else {
      if (++f->x > BORDER_RIGHT) {
        f->x = BORDER_RIGHT;
        flipCpu(f);
      }
    }
  }
  // Updates states of fighters.
  for (uint8_t i = 0; i < 2; ++i) {
    tickFighterShoot(&F[i]);
    tickFighterJump(&F[i]);
  }
  // Detects collision, and checks competition state.
  detectCollision(&F[0], &F[1]);
  detectCollision(&F[1], &F[0]);
  if (F[1].hp == 0) {
    GS = GS_WIN;
    T0 = 0;
  } else if (F[0].hp == 0) {
    GS = GS_LOSE;
    T0 = 0;
  }
  // Updates animations and draws objects.
  for (uint8_t i = 0; i < 2; ++i) {
    if (++F[i].frameTicks >= AFPT) {
      F[i].frameTicks = 0;
      ++F[i].frame;
    }
    drawFighter(&F[i], i == 0, setFighterProperAction);
  }
  drawHUD(&F[0], &F[1]);
}
static void win(void) {
  if (++T0 >= FPS * 2 / 3) {
    GS = GS_NEXT;
    T0 = T1 = 0;
    return;
  }
  for (uint8_t i = 0; i < 2; ++i) {
    if (++F[i].frameTicks >= AFPT) {
      F[i].frameTicks = 0;
      ++F[i].frame;
    }
    drawFighter(&F[i], i == 0, NULL);
  }
  drawHUD(&F[0], &F[1]);
  sound(600 + T0, 20);
}
static void lose(void) {
  if (++T0 >= FPS * 2 / 3) {
    GS = GS_GAMEOVER;
    T0 = T1 = 0;
    return;
  }
  for (uint8_t i = 0; i < 2; ++i) {
    if (++F[i].frameTicks >= AFPT) {
      F[i].frameTicks = 0;
      ++F[i].frame;
    }
    drawFighter(&F[i], i == 0, NULL);
  }
  drawHUD(&F[0], &F[1]);
  sound(300 - T0, 20);
}
static void next(void) {
  if (T1 < FPS) {
    if (T1 == 0) { A.setRGBled(0, 15, 0); sound(800, 233); }
    else if (T1 == FPS / 3) { A.setRGBled(0, 31, 0); sound(1000, 233); }
    else if (T1 == FPS * 2 / 3) { A.setRGBled(0, 63, 0); sound(1200, 333); }
    if (++T1 == FPS) A.setRGBled(0, 0, 0);
  } else {
    if (A.justReleased(DOWN_BUTTON | A_BUTTON | B_BUTTON)) {
      prepare();
      GS = GS_FIGHT;
    }
  }
  A.drawBitmap(32, 16, SPRITE_NEXT, 64, 32, WHITE);
}
static void over(void) {
  if (T1 < FPS) {
    if (T1 == 0) { A.setRGBled(63, 0, 0); sound(300, 233); }
    else if (T1 == FPS / 3) { A.setRGBled(31, 0, 0); sound(300, 233); }
    else if (T1 == FPS * 2 / 3) { A.setRGBled(15, 0, 0); sound(200, 333); }
    if (++T1 == FPS) A.setRGBled(0, 0, 0);
  } else {
    if (A.justReleased(DOWN_BUTTON | A_BUTTON | B_BUTTON)) {
      memset(R, 0, sizeof(R));
      C0 = A.audio.enabled();
      T0 = T1 = 0;
      F[0].hp = F[1].hp = 3;
      GS = GS_MENU;
      L = 0;
    }
  }
  A.drawBitmap(32, 16, SPRITE_OVER, 64, 32, WHITE);
  drawProgress();
}

void inogamesetup() {
  Serial.begin(9600);
  A.begin();
  A.setFrameRate(FPS);
  N.initChannel(PIN_SPEAKER_1);
#ifndef AB_DEVKIT
  N.initChannel(PIN_SPEAKER_2);
#else
  N.initChannel(PIN_SPEAKER_1);
  N.toneMutesScore(true);
#endif
  memset(R, 0, sizeof(R));
  F[0].hp = F[1].hp = 3;
  EEPROM.get(SAVE_LOCATION, B);
}

void inogameloop() {
  delay(0);
  if (!(A.nextFrame()))
    return;
  A.clear();
  A.pollButtons();
  switch (GS) {
    case GS_MENU:
      menu();
      break;
    case GS_FIGHT:
      fight();
      break;
    case GS_WIN:
      win();
      break;
    case GS_LOSE:
      lose();
      break;
    case GS_NEXT:
      next();
      break;
    case GS_GAMEOVER:
      over();
      break;
  }
  Serial.write(A.getBuffer(), 128 * 64 / 8);
  A.display();
}
void gameLogicLoop(void *)
{
  for (;;) {
    inogameloop(); 
    // ArduinoOTA.handle();
  }
}

void setup() {
  inogamesetup();
  xTaskCreatePinnedToCore(gameLogicLoop, "g", 4096, nullptr, 0, nullptr, 0);
}

void loop() {
	delay(60000);
}
