//#include <Arduboy.h>
#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "bitmaps.h"

#define ENEMY_NUM 20
#define BULLET_NUM 8

extern Arduboy2 arduboy;
extern ArduboyTones sound;

typedef struct {
  uint8_t stat, sp, anim;
  int16_t x, y, vx, vy;
  uint16_t tm;
  int8_t opt;
} Obj;

extern Obj enemy[ENEMY_NUM];
extern Obj bullet[BULLET_NUM];
extern uint8_t i_enemy;
extern uint8_t i_bullet;

extern uint16_t tm;

void set_obj(
  Obj      obj[],
  uint8_t  *index,
  uint8_t  stat,
  uint8_t  sp,
  uint8_t  anim,
  int16_t  x,
  int16_t  y,
  int16_t  vx,
  int16_t  vy,
  uint16_t tm );
void check_range(
  Obj      obj[],
  uint8_t  index,
  int16_t  x1,
  int16_t  y1,
  int16_t  x2,
  int16_t  y2 );
void remove_obj(
  Obj      obj[],
  uint8_t  *index);
void draw_obj(Obj obj[], uint8_t index);
void draw_obj2(Obj obj[], uint8_t index);
void add_velocity(Obj obj[], uint8_t index);
