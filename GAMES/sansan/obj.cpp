#include "obj.h"

//Arduboy arduboy;
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);

Obj enemy[ENEMY_NUM];
Obj bullet[BULLET_NUM];

uint8_t i_shot = 0;
uint8_t i_enemy = 0;
uint8_t i_bullet = 0;
uint8_t i_cloud = 0;

uint16_t tm = 0;


/*
int16_t dist(x1, y2, x2, y2){
}
*/

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
  uint16_t tm ){
  obj[*index].stat = stat;
  obj[*index].sp   = sp;
  obj[*index].anim = anim;
  obj[*index].x    = x;
  obj[*index].y    = y;
  obj[*index].vx   = vx;
  obj[*index].vy   = vy;
  obj[*index].tm   = tm;
  obj[*index].opt  = 0;
  *index += 1;
}

void check_range(
  Obj      obj[],
  uint8_t  index,
  int16_t  x1,
  int16_t  y1,
  int16_t  x2,
  int16_t  y2 ){
  for(uint16_t i=0; i<index; i++){
    if(obj[i].stat != 0 && (obj[i].x/256 < x1 || obj[i].y/256 < y1 || obj[i].x/256 > x2 || obj[i].y/256 > y2)){
      obj[i].stat = 0;
    }
  }
}

void remove_obj(
  Obj      obj[],
  uint8_t  *index){
  uint8_t j = 0;
  uint8_t new_index = 0;
  
  for(uint16_t i=0; i<*index; i++){
    if(j < *index){
      while(obj[j].stat == 0) j++;
      if(j < *index){
        new_index++;
        if(i != j){
          obj[i] = obj[j];
        }
        j++;
      }
    } else {
        obj[i].stat = 0;
        obj[i].sp   = 0;
        obj[i].anim = 0;
        obj[i].x    = 0;
        obj[i].y    = 0;
        obj[i].vx   = 0;
        obj[i].vy   = 0;
        obj[i].tm   = 0;
        obj[i].opt  = 0;
    }
  }
  *index = new_index++;
}

void draw_obj(Obj obj[], uint8_t index){
  for(uint8_t i=0; i<index; i++){
      arduboy.drawBitmap(obj[i].x/256-6, obj[i].y/256-6, sb[obj[i].sp+obj[i].anim], 16, 16, BLACK);
      arduboy.drawBitmap(obj[i].x/256-6, obj[i].y/256-6, sw[obj[i].sp+obj[i].anim], 16, 16, WHITE);
  }
}
void draw_obj2(Obj obj[], uint8_t index){
  for(uint8_t i=0; i<index; i++){
      arduboy.drawBitmap(obj[i].x/256-2, obj[i].y/256-2, sb[obj[i].sp+obj[i].anim], 16, 16, BLACK);
      arduboy.drawBitmap(obj[i].x/256-2, obj[i].y/256-2, sw[obj[i].sp+obj[i].anim], 16, 16, WHITE);
  }
}
  // Middle
  //arduboy.drawBitmap(64, 32, bm3, 16, 16, WHITE);
  // Large
  //arduboy.drawBitmap(64, 0, bm4, 32, 32, WHITE);

void add_velocity(Obj obj[], uint8_t index){
  for(uint8_t i=0; i<index; i++){
    obj[i].x += obj[i].vx;
    obj[i].y += obj[i].vy;
  }
}
