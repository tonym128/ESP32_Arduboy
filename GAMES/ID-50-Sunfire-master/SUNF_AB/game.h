#ifndef GAME_H
#define GAME_H

#include <Arduino.h>
#include "globals.h"
#include "elements.h"
#include "sound_data.h"

signed char hand_state = 1;

#define V_CENTER 32
#define H_CENTER 64

signed char backSectionY[16];
signed char pitch = 0;
signed char roll = 0;
signed char x_change = 0;
signed char x_drift = 0;

void set_background_pitch() {
  signed char offset = pitch;
  offset += 6;
  for (signed char i = 0; i < 16; i++) {
    backSectionY[i] = pgm_read_byte_near (PITCH_DATA + (offset * 16) + i);
  }
}

void set_background_roll() {
  for (signed char i = 0; i < 16; i++) {
    backSectionY[i] += roll;
  }
}

//8 * 32 sections
void draw_background_section (unsigned char strip,signed  char y) {
  unsigned char start = (strip * 4);

  for (signed char i = 0; i < 4; i++) {
    sprites.drawOverwrite((strip * 8) + x_drift, y + (i * 8), IMG_BACKDROP, start + i);
  }
}

void stateGamePlaying() {

  if (arduboy.everyXFrames(3)) blinking = !blinking;

  // arduboy.drawPixel(H_CENTER,V_CENTER,WHITE);
  x_change = 0;

  hand_state = 1;
  if (arduboy.pressed(RIGHT_BUTTON)) {
    hand_state = 2;
    if (pitch < 6) {
      pitch++;
    } else {
      if (x_drift > -16) x_drift--;
    }
  }

  if (arduboy.pressed(LEFT_BUTTON)) {
    hand_state = 0;
    if (pitch > -6)
    {
      pitch--;
    } else {
      if (x_drift < 16) x_drift++;
    }
  }

  if (arduboy.pressed(DOWN_BUTTON)) {
    hand_state = 3;
    if (roll > -8) {
      roll--;
      x_change = -1;
    }

  }
  if (arduboy.pressed(UP_BUTTON)) {
    hand_state = 4;
    if (roll < 16) {
      roll++;
      x_change = 1;
    }
  }

  if (arduboy.pressed(B_BUTTON)) {
    LevelElement bullet = level_element_get(0);

    if (bullet.state == STATE_HIDDEN) {
      bullet.state = BULLET_L_MOVE;
      bullet.x = 32;
      bullet.y = 32;
      level_element_update(0, bullet);

      bullet = level_element_get(1);
      bullet.state = BULLET_R_MOVE;
      bullet.x = 96;
      bullet.y = 32;
      level_element_update(1, bullet);
    }
  }

  if (arduboy.pressed(A_BUTTON)) {
    LevelElement plasma = level_element_get(2);

    if (plasma.state == STATE_HIDDEN) {
      plasma.step = 0;
      plasma.counter = 32;
      plasma.state = PLASMA_NEAR;
      plasma.x = 64;
      plasma.y = 24;
      level_element_update(2, plasma);
    }
  }

  set_background_pitch();
  set_background_roll();
  for (signed char i = 0; i < 16; i++) draw_background_section (i, backSectionY[i]);
  level_element_handle(pitch, x_change);

  sprites.drawSelfMasked(48, 18, IMG_HUD, 0);
  sprites.drawSelfMasked(80, 18, IMG_HUD, 1);
  sprites.drawSelfMasked(48, 34, IMG_HUD, 2);
  sprites.drawSelfMasked(80, 34, IMG_HUD, 3);
  sprites.drawSelfMasked(63, 25, IMG_HUD, 4);

  sprites.drawOverwrite(0, 43, IMG_COCKPIT, 0);
  sprites.drawOverwrite(56, 48, IMG_HAND, hand_state);

  if (danger) sprites.drawOverwrite(12, 54, IMG_DANGER, blinking);
  else
  {
    sprites.drawOverwrite(12, 54, IMG_DISPLAY, 0);
    level_element_draw_display();
  }

  //print_number(92, 54, score);

};

void stateMenuPlay()
{
  //ATM.stop();
  danger = false;
  //bullets
  level_element_add(TYPE_BULLET, 0, 0, STATE_HIDDEN, 2, 0);
  level_element_add(TYPE_BULLET, 0, 0, STATE_HIDDEN, 2, 0);
  //plasma
  level_element_add(TYPE_PLASMA, 0, 0, STATE_HIDDEN, 2, 0);
  //missle
  level_element_add(TYPE_MISSILE, 0, 0, STATE_HIDDEN, 2, 1);
  //debris animation
  level_element_add(TYPE_DEBRIS, H_CENTER, V_CENTER, STATE_DEBRIS_TL_MOVE, 2, 1);
  //enemies
  level_element_add(TYPE_ENEMY_REAR, 0, 0, STATE_ENEMY_REAR_IN_TL, 1, 1);
  level_element_add(TYPE_ENEMY_FRONT, 32, 18, STATE_ENEMY_FRONT_IN_LF, 1, 1);

  level_element_add(TYPE_ENEMY_CARRIER, 32, 32, STATE_ENEMY_CARRIER_SM, 1, 100);
  //background
  for (signed char i = 0; i < 16; i++) backSectionY[i] = 8;

  gameState = STATE_GAME_PLAYING;
};

void waveComplete() {
  //ATM.play(playing);
  gameState = STATE_WAVE_COMPLETE;
}

void stateWaveComplete() {
  sprites.drawSelfMasked(20, 8, IMG_WAVE_COMPLETE, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) stateMenuPlay();
}


void stateGameOver() {
  sprites.drawSelfMasked(20, 8, IMG_DEATH, 0);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) {
    memset(levelElements, 0, sizeof(levelElements));
    element_count = 0;
    gameState = STATE_MENU_MAIN;
    //ATM.play(titleSong);
  }
}



//-----------

#endif
