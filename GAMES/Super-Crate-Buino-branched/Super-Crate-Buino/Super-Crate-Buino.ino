 /*
 * (C) Copyright 2014 Aurélien Rodot. All rights reserved.
 *
 * You can redistribute this program and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

//#include <EEPROM.h>
#include "pitches.h"
#include "Arduboy2Gamebuino.h"
#include <pgmspace.h>

Arduboy2Gamebuino gb;

const byte logo[] PROGMEM = {64,30,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xFF,0xFF,0xFF,0xFF,0xF0,0x0,0x0,0x0,0x81,0x32,0x4,0x8,0x10,0x0,0x0,0x0,0x9F,0x32,0x64,0xF9,0x90,0x0,0x0,0x0,0x81,0x32,0x4,0x38,0x30,0x0,0x0,0x0,0xF9,0x32,0x7C,0xF9,0x90,0x0,0x0,0x0,0x81,0x2,0x7C,0x9,0x90,0x0,0x0,0x0,0xFF,0xFF,0xFF,0xFF,0xF0,0x0,0x0,0x0,0x81,0x2,0x4,0x8,0x10,0x0,0x0,0x0,0x9F,0x32,0x67,0x39,0xF0,0x0,0x0,0x0,0x9F,0x6,0x7,0x38,0x70,0x0,0x0,0x0,0x9F,0x32,0x67,0x39,0xF0,0x0,0x0,0x0,0x81,0x32,0x67,0x38,0x10,0x0,0x0,0x0,0xFF,0xFF,0xFF,0xFF,0xF0,0x0,0x0,0x0,0x60,0x4C,0x93,0x20,0x60,0x60,0x0,0x0,0x26,0x4C,0x91,0x26,0x40,0x1,0xF8,0x0,0x20,0xCC,0x92,0x26,0x40,0xD,0xF8,0x20,0xA6,0x4C,0x93,0x26,0x40,0x81,0xF,0xE4,0x20,0x40,0x93,0x20,0x40,0x1,0xBF,0x80,0x3F,0xFF,0xFF,0xFF,0xC0,0x1,0xF8,0x0,0x7,0xFF,0xFF,0xFE,0x0,0x1,0xF8,0x0,0x0,0xFF,0xFF,0xF0,0x0,0x1,0xF8,0x0,0x0,0x1F,0xFF,0x80,0x0,0x1,0x98,0x0,0x0,0x3,0xFC,0x0,0x0,0xFF,0xFF,0xF0,0x0,0x0,0x60,0x0,0x0,0x0,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x0,0x0,0x0,0x0,0x0,0xC3,0xC,0x8,0x0,0x0,0x0,0x0,0x0,0x34,0xD3,0x48,0x0,0x0,0x0,0x0,0x0,0xCB,0x2C,0xB8,0x0,0x0,0x0,0x0,0x0,};

// for Gamebuino button functions
#define GB_BTN_RIGHT     0
#define GB_BTN_LEFT      1
#define GB_BTN_DOWN      2
#define GB_BTN_UP        3
#define GB_BTN_A         4
#define GB_BTN_B         5

//EEPROM offsets
#define EEPROM_TOKEN 0xAA
#define EEPROM_TOKEN_OFFSET   EEPROM_STORAGE_SPACE_START+0x0050
#define EEPROM_WEAPONS_OFFSET EEPROM_STORAGE_SPACE_START+0x0051
#define EEPROM_MAPS_OFFSET    EEPROM_STORAGE_SPACE_START+0x0053
#define EEPROM_SCORE_OFFSET   EEPROM_STORAGE_SPACE_START+0x0053

//Weapons and bullets subtypes
#define NUMWEAPONS 13
#define W_PISTOL 0
#define W_RIFLE 1
#define W_SHOTGUN 2
#define W_ROCKET 3
#define W_CLUB 4
#define W_REVOLVER 5
#define W_MINE 6
#define W_SNIPER 7
#define W_MACHINEGUN 8
#define W_GRENADE 9
#define W_AKIMBO 10
#define W_DISK 11
#define W_LASER 12
//can't be held by the player as a weapon :
#define W_EXPLOSION 13
#define W_SHELL 14

byte unlockedWeapons = 0;
byte unlockedMaps = 0;

#define SCORETHRESHOLD_1 4
#define SCORETHRESHOLD_2 8
#define SCORETHRESHOLD_3 12
#define SCORETHRESHOLD_4 14
#define SCORETHRESHOLD_5 16
const uint8_t scoreThresholds[] = {SCORETHRESHOLD_1, SCORETHRESHOLD_2, SCORETHRESHOLD_3, SCORETHRESHOLD_4, SCORETHRESHOLD_5};

//Enemy types
#define E_SMALL 0
#define E_BIG 1

//there is a scale between the world cccordinates and the screen coordinates
//to allow sub pixel accuracy without the use of floats
#define SCALE 8

int cameraX, cameraY, shakeTimeLeft, shakeAmplitude;
byte popupTimeLeft;
const __FlashStringHelper* popupText;

int toScreenX(int x) {
  return x / SCALE - cameraX;
}

int toScreenY(int y) {
  return y / SCALE - cameraY;
}

///////////////////////////////////////////// SOUNDS
const uint16_t player_damage_sound[] PROGMEM = {NOTE_B4, 250, TONES_END};
const uint16_t revolver_sound[] PROGMEM = {NOTE_F5, 50, NOTE_A3, 50, NOTE_GS3, 50, NOTE_G3, 50, NOTE_FS3, 50, TONES_END};
const uint16_t grenade_sound[] PROGMEM = {NOTE_A3, 50, TONES_END};
const uint16_t machinegun_sound[] PROGMEM = {NOTE_D4, 50, NOTE_A3, 100, NOTE_GS3, 100, NOTE_G3, 50, TONES_END};
const uint16_t rocket_sound[] PROGMEM = {NOTE_A4, 3000, TONES_END};
const uint16_t blast_sound[] PROGMEM = {NOTE_GS3, 50, NOTE_G3, 50, NOTE_FS3, 50, NOTE_F3, 50, NOTE_E3, 50, NOTE_DS3, 50, NOTE_D3, 50, NOTE_CS3, 50, NOTE_C3, 50, NOTE_B2, 50, TONES_END};
const uint16_t power_up_sound[] PROGMEM = {NOTE_D4, 50, NOTE_FS4, 50, NOTE_A4, 50, NOTE_D5, 50, NOTE_FS5, 50, NOTE_CS5, 50, NOTE_G4, 50, NOTE_AS4, 50, NOTE_DS5, 50, NOTE_G5, 50, NOTE_F4, 50, NOTE_A4, 50, NOTE_C5, 50, NOTE_F5, 50, NOTE_A5, 50, TONES_END};
const uint16_t enemy_death_sound[] PROGMEM = {NOTE_G5, 50, TONES_END};
const uint16_t jump_sound[] PROGMEM = {NOTE_G4, 50, NOTE_GS4, 50, NOTE_A4, 50, TONES_END};
const uint16_t enemy_felt_sound[] PROGMEM = {NOTE_FS3, 750, TONES_END};
const uint16_t shotgun_sound[] PROGMEM = {NOTE_B3, 150, TONES_END};
const uint16_t laser_sound[] PROGMEM = {NOTE_D5, 50, NOTE_CS5, 50, NOTE_C5, 50, NOTE_B4, 50, NOTE_AS4, 50, NOTE_A4, 50, TONES_END};
const uint16_t club_sound[] PROGMEM = {NOTE_E3, 50, NOTE_DS3, 50, NOTE_D3, 50, TONES_END};

const uint16_t playOK[] PROGMEM = {NOTE_C4, 50, NOTE_C5, 50, TONES_END};
const uint16_t playTick[] PROGMEM = {NOTE_C5, 50, TONES_END};

///////////////////////////////////////////// WORLD
#define SPRITE_SIZE 6

#define BG_BLOCK 1
#define BG_FENCE 2
#define BG_TREE 3
#define BG_BLOCKTOP 4
#define BG_BLOCKCORNER 5
#define BG_TREEMIDDLE 6
#define BG_TREEBOTTOM 7

//maps are encoded like bitmaps
const byte map0[] PROGMEM = {
  16, 10,
  B11111110, B01111111,
  B10000000, B00000001,
  B10000000, B00000001,
  B10001111, B11110001,
  B10000000, B00000001,
  B10000000, B00000001,
  B11111100, B00111111,
  B10000000, B00000001,
  B10000000, B00000001,
  B10011111, B11111001,
};

const byte map1[] PROGMEM = {
  24, 16,
  B11111111, B11100111, B11111111,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B10000011, B11111111, B11000001,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B11111100, B00000000, B00111111,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B10000111, B11111111, B11100001,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B11111000, B00000000, B00011111,
  B11111111, B11000011, B11111111,
};

const byte map2[] PROGMEM = {
  24, 16,
  B11111111, B11100111, B11111111,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B10000000, B00111100, B00000001,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B10000111, B11100111, B11100001,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B11111100, B00011000, B00111111,
  B11100000, B00000000, B00000111,
  B11100000, B00000000, B00000111,
  B11100000, B11111111, B00000111,
  B11100000, B00000000, B00000111,
  B11100000, B00000000, B00000111,
  B11111111, B11000011, B11111111,
};

const byte map3[] PROGMEM = {
  24, 16,
  B11111111, B11100111, B11111111,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B10000011, B11111111, B11000001,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B11111000, B00000000, B00011111,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B10001111, B11000011, B11110001,
  B10000000, B00000000, B00000001,
  B10000000, B00000000, B00000001,
  B11110000, B00111100, B00001111,
  B11110011, B11111111, B11001111,
  B11110011, B11111111, B11001111,
};

const byte map4[] PROGMEM = {
  32, 24,
  B11111111, B11111110, B01111111, B11111111,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B11110000, B00000111, B11100000, B00001111,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B10000111, B11111100, B00111111, B11100001,
  B10000000, B00000000, B00000000, B00000001,
  B10000000, B00000000, B00000000, B00000001,
  B11110000, B00000011, B11000000, B00000001,
  B10000000, B00000011, B11000001, B11111111,
  B10000000, B00000011, B11000000, B00111111,
  B10001111, B11111111, B11000000, B00000011,
  B10000000, B00000011, B11111100, B00000011,
  B10000000, B00000000, B00000000, B00000011,
  B11111100, B00000000, B00000000, B00000011,
  B11000000, B00001100, B00000001, B11111111,
  B11000000, B00001100, B00000000, B00111111,
  B11001111, B11111100, B00000011, B00001111,
  B11000000, B00001111, B00000011, B00001111,
  B11000000, B00000011, B11000000, B00001111,
  B11111100, B00000000, B00000000, B00001111,
  B11111111, B11000000, B00000000, B00001111,
  B11111111, B11111111, B11111111, B00001111,
};

#define NUMMAPS 5
const byte* const maps[NUMMAPS] = {
  map0, map1, map2, map3, map4
};

uint8_t score[NUMMAPS];

const byte bricks[] PROGMEM = {
  8, 6, 0xFC, 0x24, 0xFC, 0x90, 0xFC, 0x48
};
const byte grass[] PROGMEM = {
  8, 6, 0xFC, 0x0, 0x0, 0x18, 0xA4, 0x58
};
const byte grass_edge[] PROGMEM = {
  8, 6, 0x7C, 0x80, 0x80, 0x80, 0x94, 0xE8
};
const byte beam[] PROGMEM = {
  8, 6, 0xFC, 0x10, 0x28, 0x44, 0x80, 0xFC
};
const byte roundPlatform[] PROGMEM = {
  8, 6, 0xFC, 0x0, 0x0, 0x0, 0xFC, 0xFC
};
const byte roundPlatform_edge[] PROGMEM = {
  8, 6, 0x7C, 0xE0, 0xC0, 0xE0, 0xFC, 0x7C,
};
const byte largeChecker[] PROGMEM = {
  8, 6, 0x1C, 0x1C, 0x1C, 0xE0, 0xE0, 0xE0,
};
const byte blackWall[] PROGMEM = {
  8, 6, 0xF4, 0xFC, 0xBC, 0xF8, 0xFC, 0xDC,
};

class World {
  public:
    const byte* tiles;
    const byte* wall; //pointer to the used tilemap
    const byte* platform;
    const byte* edge;
    byte mapNumber;

    int getWidth() {
      return SPRITE_SIZE * pgm_read_byte(tiles) * SCALE;
    }
    int getHeight() {
      return SPRITE_SIZE * pgm_read_byte(tiles + 1) * SCALE;
    }

    byte tileAtPosition(int x, int y) {
      int tile_x = (x - SCALE / 2) / SPRITE_SIZE / SCALE;
      int tile_y = (y - SCALE / 2) / SPRITE_SIZE / SCALE;

      int w = pgm_read_byte(tiles);
      int h = pgm_read_byte(tiles + 1);

      // Offmap is made of air
      if (tile_x < 0 || tile_x >= w || tile_y < 0 || tile_y >= h)
        return 0;

      //maps are encoded like bitmaps so we can use getBitmapPixel
      return gb.getBitmapPixel(tiles, tile_x, tile_y) > 0 ? 1 : 0;
    }

    boolean solidCollisionAtPosition(int16_t x, int16_t y, int16_t w, int16_t h) {
      if (tileAtPosition(x, y + h))
        return true;
      if (tileAtPosition(x + w, y + h))
        return true;
      if (tileAtPosition(x + w, y))
        return true;
      if (tileAtPosition(x, y))
        return true;
      //also check at half the length for boxes larger than a sprite
      if (w > SPRITE_SIZE * SCALE) {
        if (tileAtPosition(x + w / 2, y))
          return true;
        if (tileAtPosition(x + w / 2, y + h))
          return true;
      }
      if (h > SPRITE_SIZE * SCALE) {
        if (tileAtPosition(x, y + h / 2))
          return true;
        if (tileAtPosition(x + w, y + h / 2))
          return true;
      }
      //check all the points
      /*if((w > SPRITE_SIZE*SCALE) || (h > SPRITE_SIZE*SCALE)){
       for (int i = x; i <= (x + w); i += SPRITE_SIZE*SCALE){
       for (int j = y; j <= (y + h); j += SPRITE_SIZE*SCALE){
       if (tileAtPosition(i, j))
       return true;
       }
       }
       }*/
      return false;
    }

    void draw() {
      int xMin = cameraX / SPRITE_SIZE;
      int xMax = LCDWIDTH / SPRITE_SIZE + cameraX / SPRITE_SIZE + 1;
      int yMin = cameraY / SPRITE_SIZE;
      int yMax = LCDHEIGHT / SPRITE_SIZE + cameraY / SPRITE_SIZE + 1;

      int w = pgm_read_byte(tiles);
      int h = pgm_read_byte(tiles + 1);

      for (int y = yMin; y < yMax; y++) {
        for (int x = xMin; x < xMax; x++ ) {
          if (x < 0 || x >= w || y < 0 || y >= h)
            continue;
          if (gb.getBitmapPixel(tiles, x, y)) {
            byte tileNumber = 1; //platform by default
            byte flip = NOFLIP;
            byte offset = 0;
            const byte* bitmap = platform;
            if (y >= getHeight() / SPRITE_SIZE / SCALE - 1 || y <= 0 || gb.getBitmapPixel(tiles, x, y - 1)) {
              bitmap = wall; //bricks
            }
            else {
              if (edge) {
                if (y > 0 && !gb.getBitmapPixel(tiles, x + 1, y)) {
                  bitmap = edge; //platform corner
                  flip = FLIPH;
                  offset = 2;
                }
                if (y > 0 && !gb.getBitmapPixel(tiles, x - 1, y)) {
                  bitmap = edge; //platform corner
                }
              }
            }
            gb.drawBitmapF(x * SPRITE_SIZE - cameraX - offset, y * SPRITE_SIZE - cameraY, bitmap, NOROT, flip);
          }
        }
      }
    }

    void chooseMap() {
      int thisMap = mapNumber;
      uint8_t toggleBlink = 1;
      while (1) {
        if (gb.nextFrame()) {
          gb.display(CLEAR_BUFFER);
          gb.timer1();
          gb.timer2();
          gb.pollButtons();

          //assign the selected map
          tiles = maps[thisMap];
          mapNumber = thisMap;

          switch (thisMap) {
            case 1:
              wall = bricks;
              platform = beam;
              edge = 0;
              break;
            case 0:
            case 2:
            case 4:
              wall = bricks;
              platform = grass;
              edge = grass_edge;
              break;
            case 3:
              wall = blackWall;
              platform = roundPlatform;
              edge = roundPlatform_edge;
              break;

          }

          gb.setTextColor(WHITE);
          gb.setCursor(22, HEIGHT - 26);
          gb.print(F("\21 Select map \20"));
          gb.setCursor(34, HEIGHT - 16);
          gb.print(F("Score: "));
          gb.print(score[thisMap]);
          gb.setTextColor(BLACK);

          gb.drawBitmap(0, 5, logo);

          //draw the map centered on the screen
          gb.drawBitmap(96 - getWidth() / 2 / SCALE / SPRITE_SIZE, 20 - getHeight() / 2 / SCALE / SPRITE_SIZE, maps[thisMap]);

/*        for (byte x = SPRITE_SIZE; x < LCDWIDTH - SPRITE_SIZE; x += SPRITE_SIZE) {
            gb.drawBitmap(x, 0, platform);
          }
          for (byte y = SPRITE_SIZE; y < LCDHEIGHT; y += SPRITE_SIZE) {
            gb.drawBitmap(0, y, wall);
            gb.drawBitmap(LCDWIDTH - SPRITE_SIZE, y, wall);
          }
          if (edge) { //draw the ends with the according bitmap
            gb.drawBitmap(0, 0, edge);
            gb.drawBitmap(LCDWIDTH - SPRITE_SIZE - 2, 0, edge, NOROT, FLIPH);
          }
          else { //draw with end with a regular platform if there is no edge bitmap
            gb.drawBitmap(0, 0, platform);
            gb.drawBitmap(LCDWIDTH - SPRITE_SIZE, 0, platform);
          } */
          if (gb.everyXFrames(15)) toggleBlink ^= 1;
          if (thisMap == unlockedMaps) {
            gb.setTextColor(WHITE);
            for (byte i = 0; i < sizeof(scoreThresholds); i++) {
              if (score[thisMap] < scoreThresholds[i]) {
                gb.setCursor(22, HEIGHT - 7);
                if (toggleBlink) { //make it blink !
                  gb.print(F("Next unlock: "));
                  gb.print(scoreThresholds[i]);
                }
                break;
              }
            }
            gb.setTextColor(BLACK);
          }
          if (thisMap > unlockedMaps) {
            if (toggleBlink) { //make it blink !
              gb.drawRect(74, 15, 43, 9, WHITE);
              gb.setTextBackground(WHITE);
              gb.setTextColor(BLACK);
              gb.setCursor(75, 16);
              gb.print(F("LOCKED!"));
              gb.setTextBackground(BLACK);
            }
          }
          if (gb.pressed(A_BUTTON) && (thisMap <= unlockedMaps)) {
            initGame();
            return;
          }
          if (gb.justPressed(RIGHT_BUTTON))
            thisMap = (thisMap + 1) % NUMMAPS;
          if (gb.justPressed(LEFT_BUTTON))
            thisMap = (thisMap - 1 + NUMMAPS) % NUMMAPS;
        }
      }
    }

    boolean addScore(byte newScore) {
      if (newScore > score[mapNumber]) {
        score[mapNumber] = newScore;
        return true;
      }
      return false;
    }
};

World world;

/////////////////////////////////////////////
///////////////////////////////////////////// BOX
/////////////////////////////////////////////

class Box {
  public:
    int16_t x, y, vx, vy;
    int8_t dir;
    virtual byte getType() {
      return 0;
    };
    virtual int getWidth() {
      return 128;
    };
    virtual int getHeight() {
      return 128;
    };
    virtual int getGravity() {
      return 8;
    };
    virtual int getMaxSpeed() {
      return 128;
    }
    virtual int getXFriction() {
      return 5;
    }
    virtual int getYFriction() {
      return 5;
    }
    virtual int getXBounce() {
      return 100;
    }
    virtual int getYBounce() {
      return 100;
    }

    byte update() {
      vy += getGravity();
      vx = (vx * (100 - getXFriction())) / 100;
      vy = (vy * (100 - getYFriction())) / 100;
      vx = constrain(vx, - getMaxSpeed(), getMaxSpeed());
      vy = constrain(vy, - getMaxSpeed(), getMaxSpeed());
      boolean collided = 0;

      x += vx;
      if (getXBounce() >= 0) {
        int8_t vxdir = vx > 0 ? 1 : -1;
        if (world.solidCollisionAtPosition(x, y, getWidth(), getHeight())) {
          collided = 1;
          do {
            x -= vxdir; //move back step by step until we get out of the colision
          }
          while (world.solidCollisionAtPosition(x, y, getWidth(), getHeight()));
          vx = -(vx * getXBounce()) / 100;
        }
      }

      y += vy;
      if (getXBounce() >= 0) {
        int8_t vydir = vy > 0 ? 1 : -1;
        if (world.solidCollisionAtPosition(x, y, getWidth(), getHeight())) {
          collided = 1;
          do {
            y -= vydir;
          }
          while (world.solidCollisionAtPosition(x, y, getWidth(), getHeight()));
          vy = -(vy * getYBounce()) / 100;
        }
      }

      if (vx > 0) {
        dir = 1;
      }
      if (vx < 0) {
        dir = -1;
      }

      return collided;
    }

    boolean isOffScreen() {
      return (toScreenX(x) + toScreenX(x + getWidth()) < 0) || (toScreenX(x) > LCDWIDTH) || (toScreenY(y) + toScreenY(y + getHeight()) < 0) || (toScreenY(y) > LCDHEIGHT);
    }

    void draw() {
      if (isOffScreen())
        return; //skip boxes which are out of the screen
      if (gb.getTextColor() == INVERT)
        gb.fillRect(toScreenX(x), toScreenY(y), getWidth() / SCALE, getHeight() / SCALE, INVERT);
      else
        gb.fillRect(toScreenX(x), toScreenY(y), getWidth() / SCALE, getHeight() / SCALE);
    }
};

/////////////////////////////////////////////
///////////////////////////////////////////// BULLETS
/////////////////////////////////////////////

class Bullet :
  public Box {
  public:
    byte subtype;
    char timeLeft;

    int getWidth() {
      switch (subtype) {
        case W_CLUB :
          return 96;
        case W_PISTOL :
        case W_AKIMBO :
        case W_RIFLE :
        case W_SHOTGUN :
          return constrain(abs(vx), 8, 16);
        case W_MACHINEGUN :
          return constrain(abs(vx), 8, 24);
        case W_REVOLVER :
        case W_SNIPER :
          return constrain(abs(vx), 8, 32);
        case W_DISK :
          return min(timeLeft * 8, 48);
        case W_LASER :
          return 96;
        case W_GRENADE :
          return 24;
        case W_ROCKET :
          return 48;
        case W_MINE :
          return 32;
        case W_EXPLOSION :
          return 256;
        case W_SHELL :
          return 16;
        default :
          return 32;
      }
    };
    int getHeight() {
      switch (subtype) {
        case W_CLUB :
          return 16;
        case W_REVOLVER :
          return max(getWidth() / 2, 8);
        case W_SNIPER :
        case W_SHELL :
        case W_LASER :
          return 8;
        case W_DISK :
        case W_MINE :
          return 16;
        case W_ROCKET :
          return 24;
        default :
          return getWidth();
          break;
      }
    };
    int getGravity() {
      switch (subtype) {
        case W_GRENADE :
        case W_MINE :
        case W_SHELL :
          return 5;
        case W_CLUB :
        case W_DISK :
        case W_LASER :
        case W_ROCKET :
        case W_EXPLOSION :
          return 0;
        default:
          return abs(vx) > 16 ? 0 : 2;
      }
    };
    int getMaxSpeed() {
      return 128;
    }
    virtual int getXFriction() {
      switch (subtype) {
        case W_SHOTGUN :
          return 10;
        case W_CLUB :
        case W_DISK :
        case W_LASER :
          return 0;
        case W_ROCKET :
          return -20; //negative value so it accelerates
        case W_EXPLOSION :
          return 100;
        default :
          return Box::getXFriction();
      }
    }
    int getXBounce() {
      switch (subtype) {
        case W_DISK :
          return 100;
        case W_CLUB :
        case W_EXPLOSION :
        case W_LASER :
          return -1; //don't collide the world
        case W_GRENADE :
        case W_SHELL :
          return 80;
        case W_ROCKET :
          return 0;
        default :
          return 30;
      }
    };
    int getYBounce() {
      if ((subtype == W_GRENADE) || (subtype == W_SHELL))
        return getXBounce();
      else
        return 0;
    };

    int getDamage() {
      switch (subtype) {
          return 1;
        case W_REVOLVER :
        case W_CLUB :
          return 2;
        case W_SNIPER :
        case W_DISK :
        case W_EXPLOSION :
        case W_LASER :
          return 10;
        case W_GRENADE :
        case W_ROCKET :
        case W_MINE :
        case W_SHELL :
          return 0;
        default :
          return 1;
      }
    }

    int getMaxTimeLeft() {
      switch (subtype) {
        case W_CLUB :
          return 2;
        case W_SHELL :
          return 20;
        case W_MINE :
        case W_DISK :
          return 100;
        case W_EXPLOSION :
          return 5;
        case W_GRENADE :
        case W_ROCKET :
          return 40;
        default :
          return 25;
      }

    }

    boolean explodes() {
      switch (subtype) {
        case W_GRENADE :
        case W_ROCKET :
        case W_MINE :
          return true;
        default:
          return false;
      }
    }

    boolean destroyOnWorldContact() {
      return (subtype == W_ROCKET) ? true : false;
    }

    boolean destroyOnEnemyContact() {
      switch (subtype) {
        case W_CLUB :
        case W_DISK :
        case W_LASER :
        case W_EXPLOSION :
        case W_SHELL :
          return false;
        default:
          return true;
      }
    }

    boolean damagePlayer() {
      return (subtype == W_DISK) ? true : false;
    }

    void update() {
      if (timeLeft) {
        byte collided = Box::update();
        timeLeft--;
        if (destroyOnWorldContact() && collided == 1) {
          timeLeft = 0;
        }
        if ((timeLeft == 0) && explodes()) {
          subtype = W_EXPLOSION;
          x -= getWidth() / 2; //offset the explosion for it to be centered
          y -= getHeight() / 2;
          timeLeft = 8;
          //set the camera shake
          shakeTimeLeft = 10;
          shakeAmplitude = 2;
          gb.tone1(blast_sound);
        }
      }
    }

    void draw() {
      if (timeLeft) {
        Box::draw();
      }
    }
};

#define NUMBULLETS 20
static Bullet bullets[NUMBULLETS];

/////////////////////////////////////////////
///////////////////////////////////////////// WEAPON
/////////////////////////////////////////////

const byte club[] PROGMEM = {
  24, 6, 0xC0, 0x0, 0x0, 0xF0, 0x0, 0x0, 0x7C, 0x0, 0x0, 0x1F, 0x0, 0x0, 0x7, 0x80, 0x0, 0x1, 0x80, 0x0,
};
const byte pistol[] PROGMEM = {
  24, 3, 0x0, 0x0, 0xF0, 0x0, 0x1, 0xE0, 0x0, 0x1, 0x0
};
const byte laser[] PROGMEM = {
  24, 5, 0x0, 0x0, 0x80, 0x0, 0xFF, 0xA8, 0x0, 0xFF, 0xFC, 0x0, 0xFF, 0xA8, 0x0, 0x0, 0x80,
};
const byte revolver[] PROGMEM = {
  24, 4, 0x0, 0x0, 0x80, 0x0, 0x0, 0xF8, 0x0, 0x1, 0xF8, 0x0, 0x1, 0xC0
};
const byte rifle[] PROGMEM = {
  24, 4, 0x0, 0x0, 0x4, 0x0, 0x21, 0xFC, 0x0, 0x37, 0xF0, 0x0, 0x31, 0x0
};
const byte rifle_white[] PROGMEM = {
  24, 3, 0x0, 0x0, 0x0, 0x0, 0x1E, 0x0, 0x0, 0x8, 0x0
};
const byte sniper[] PROGMEM = {
  24, 4, 0x0, 0x0, 0xC0, 0x0, 0xFF, 0xFE, 0x0, 0xFF, 0xE0, 0x0, 0xE3, 0x0,
};
const byte shotgun[] PROGMEM = {
  24, 4, 0x0, 0x0, 0x10, 0x0, 0x63, 0xF0, 0x0, 0x7F, 0xF0, 0x0, 0x7C, 0x0
};
const byte shotgun_white[] PROGMEM = {
  24, 2, 0x0, 0x0, 0x0, 0x0, 0x1C, 0x0
};
const byte machinegun[] PROGMEM = {
  24, 5, 0x0, 0x0, 0x10, 0x0, 0xC7, 0xF0, 0x0, 0xC7, 0xF0, 0x0, 0xC7, 0xC0, 0x0, 0xFC, 0x0,
};
const byte machinegun_white[] PROGMEM = {
  24, 4, 0x0, 0x0, 0x0, 0x0, 0x38, 0x0, 0x0, 0x38, 0x0, 0x0, 0x38, 0x0
};
const byte disk[] PROGMEM = {
  24, 5, 0x0, 0x0, 0x20, 0x0, 0xFF, 0xF0, 0x0, 0xFC, 0x0, 0x0, 0xFC, 0x0, 0x0, 0xFF, 0xF0,
};
const byte rocket[] PROGMEM = {
  24, 5, 0x1, 0x0, 0x20, 0x1, 0xFF, 0xE0, 0x1, 0xD7, 0xE0, 0x1, 0xFF, 0xE0, 0x1, 0x0, 0x20
};
const byte grenade[] PROGMEM = {
  24, 4, 0x0, 0xC7, 0xE0, 0x0, 0xFF, 0xE0, 0x0, 0xC7, 0xE0, 0x0, 0x7C, 0x0
};
const byte grenade_white[] PROGMEM = {
  24, 3, 0x0, 0x38, 0x0, 0x0, 0x0, 0x0, 0x0, 0x38, 0x0
};

class Weapon {
  public:
    byte subtype;
    byte cooldown;
    Box* shooter;

    void init() {
      cooldown = 0;
      for (byte i = 0; i < NUMBULLETS; i++) {
        bullets[i].timeLeft = 0;
      }
    }

    byte getMaxCooldown() {
      switch (subtype) {
        case W_CLUB :
          return 10;
        case W_PISTOL :
        case W_AKIMBO :
        case W_REVOLVER :
          return 0;
        case W_SNIPER :
          return 7;
        case W_SHOTGUN :
          return 11;
        case W_RIFLE :
          return 2;
        case W_MACHINEGUN :
          return 1;
        case W_DISK :
          return 19;
        case W_LASER :
          return 30;
        case W_GRENADE :
        case W_ROCKET :
        case W_MINE :
          return 19;
        case W_EXPLOSION :
        default :
          return 5;
      }
    }

    void addBullet(int x, int y, int dir, byte subtype) {
      for (byte i = 0; i < NUMBULLETS; i++) {
        if (bullets[i].timeLeft == 0) {
          bullets[i].subtype = subtype;
          bullets[i].timeLeft = bullets[i].getMaxTimeLeft();

          //screen shaking
          switch (subtype) {
            case W_SNIPER :
            case W_REVOLVER :
              shakeTimeLeft = 4;
              shakeAmplitude = 4;
            case W_MACHINEGUN :
              shakeTimeLeft = 2;
              shakeAmplitude = 1;
          }

          //initial bullet speeds
          switch (subtype) {
            case W_CLUB :
              bullets[i].vx = dir * 32;
              bullets[i].vy = shooter->vy;
              break;
            case W_MACHINEGUN :
              bullets[i].vx = (dir * 48) + random(-8, 9);
              bullets[i].vy = random(-16, 17);
              shooter->vx -= shooter->dir * 32; //player recoil
            case W_SHOTGUN :
              bullets[i].vx = (dir * 48) + random(-8, 9);
              bullets[i].vy = random(-10, 11);
              break;
            case W_DISK :
              bullets[i].vx = dir * 26;
              bullets[i].vy = 0;
              break;
            case W_LASER :
              bullets[i].vx = dir * 50;
              bullets[i].vy = 0;
              break;
            case W_GRENADE : //grenade speed depends on player's speed
              bullets[i].vx = (dir * 32) + shooter->vx / 2;
              bullets[i].vy = -32 + shooter->vy / 2;
              break;
            case W_ROCKET :
              bullets[i].vx = dir * 16;
              bullets[i].vy = 0;
              break;
            case W_MINE :
              bullets[i].vx = 0;
              bullets[i].vy = 0;
              break;
            case W_SHELL :
              bullets[i].vx = - dir * random(16, 24);
              bullets[i].vy = shooter->vy - random(16, 24);
              break;
            default :
              bullets[i].vx = (dir * 64) + random(-8, 9);
              bullets[i].vy = random(0, 11) - 5;
          }

          //bullet vertical offset
          switch (subtype) {
            case W_SHOTGUN :
            case W_LASER :
            case W_DISK :
              y += 32;
              break;
            case W_ROCKET :
              y += 16;
              break;
            default :
              y += 24;
              break;
          }

          //horizontal bullet offset
          switch (subtype) {
            case W_SHELL :
              x -= 16;
            case W_ROCKET:
            case W_CLUB :
              x -= dir * 32;
            case W_MINE :
              break;
            default :
              x += dir * 46;
          }

          if (dir > 0) {
            x += shooter->getWidth();
          }
          else {
            x -= bullets[i].getWidth();
          }

          x += shooter->vx / 2;
          bullets[i].x = x;
          bullets[i].y = y;

          break;
        }
      }
    };

    void shoot() {
      cooldown = getMaxCooldown();
      addBullet(shooter->x, shooter->y, shooter->dir, subtype);
      if (subtype == W_SHOTGUN) {
        addBullet(shooter->x, shooter->y, shooter->dir, subtype);
        addBullet(shooter->x, shooter->y, shooter->dir, subtype);
        addBullet(shooter->x, shooter->y, shooter->dir, subtype);
        addBullet(shooter->x, shooter->y, shooter->dir, subtype);
      }
      if (subtype == W_AKIMBO) {
        addBullet(shooter->x, shooter->y, -shooter->dir, subtype);
      }
      switch (subtype) { //eject a shell
        case W_RIFLE:
        case W_SNIPER:
        case W_SHOTGUN:
          addBullet(shooter->x, shooter->y, shooter->dir, W_SHELL);
      }

      switch (subtype) {
        case W_ROCKET :
          gb.tone1(rocket_sound);
          break;
        case W_REVOLVER :
        case W_MACHINEGUN :
        case W_SNIPER :
          gb.tone1(machinegun_sound);
          break;
        case W_GRENADE :
        case W_DISK :
          gb.tone1(grenade_sound);
          break;
        case W_SHOTGUN :
          gb.tone1(shotgun_sound);
          break;
        case W_MINE :
          break;
        case W_PISTOL :
        case W_AKIMBO :
        case W_RIFLE :
          gb.tone1(playTick);
          break;
        case W_LASER :
          gb.tone1(laser_sound);
          break;
        case W_CLUB :
          gb.tone1(club_sound);
          break;
      }
    }

    boolean isAutomatic() {
      switch (subtype) {
        case W_RIFLE :
        case W_MACHINEGUN :
          return true;
        default :
          return false;
      }
    };

    void update() {
      for (byte i = 0; i < NUMBULLETS; i++) {
        bullets[i].update();
      }
      if (cooldown > 0) {
        cooldown--;
      }
      else {
        if (isAutomatic()) {
          if (repeat(GB_BTN_A, 1)) {
            shoot();
          }
        }
        else {
          if (gb.pressed(A_BUTTON)) {
            shoot();
          }
        }
      }
    }

    void draw() {
      int bx = toScreenX(shooter->x) - 9;
      int by = toScreenY(shooter->y);
      byte flip = (shooter->dir > 0) ? NOFLIP : FLIPH;
      const byte* bitmap;
      const byte* bitmapWhite;
      switch (subtype) {
        case W_CLUB :
          if (cooldown > 8) {
            bitmap = 0; //don't draw the club when already in use
          }
          else {
            bitmap = club;
            bx += shooter->dir * cooldown / 2; //sliding back the club
          }
          bitmapWhite = 0;
          by -= 2;
          break;
        case W_PISTOL :
        case W_AKIMBO :
          bitmap = pistol;
          bitmapWhite = 0;
          by += 3;
          break;
        case W_REVOLVER :
          bitmap = revolver;
          bitmapWhite = 0;
          by += 2;
          break;
        case W_SNIPER :
          bitmap = sniper;
          bitmapWhite = shotgun_white;
          by += 2;
          bx -= (cooldown > 4) ? shooter->dir : 0;
          break;
        case W_RIFLE :
          bitmap = rifle;
          bitmapWhite = rifle_white;
          by += 2;
          bx -= shooter->dir * cooldown;
          break;
        case W_SHOTGUN :
          bitmap = shotgun;
          bitmapWhite = shotgun_white;
          by += 3;
          bx -= shooter->dir * cooldown / 4;
          break;
        case W_MACHINEGUN :
          bitmap = machinegun;
          bitmapWhite = machinegun_white;
          by += 3;
          break;
        case W_DISK :
          bitmap = disk;
          bitmapWhite = 0;
          by += 2;
          break;
        case W_LASER :
          bitmap = laser;
          bitmapWhite = 0;
          by += 2;
          break;
        case W_GRENADE :
          bitmap = grenade;
          bitmapWhite = grenade_white;
          by += 4;
          break;
        case W_ROCKET :
          bitmap = rocket;
          bitmapWhite = 0;
          by += 1;
          break;
        default :
          bitmap = 0;
          bitmapWhite = 0;
      }

      if (bitmap) { //draw the weappon
        gb.drawBitmapF(bx, by, bitmap, NOROT, flip);
      }
      if (bitmapWhite) {
        gb.setTextColor(WHITE);
        gb.drawBitmapF(bx, by, bitmapWhite, NOROT, flip);
        gb.setTextColor(BLACK);
      }
      if (subtype == W_AKIMBO) { //draw the symetric of the pistol in the akimbo case
        if (bitmap) {
          gb.drawBitmapF(bx, by, bitmap, NOROT, (flip + 1) % 2);
        }
        if (bitmapWhite) {
          gb.setTextColor(WHITE);
          gb.drawBitmapF(bx, by, bitmapWhite, NOROT, (flip + 1) % 2);
          gb.setTextColor(BLACK);
        }
      }
      if (subtype == W_LASER) { //reloading line on the laser
        gb.drawFastHLine(toScreenX(shooter->x), toScreenY(shooter->y) + 4, 6 - cooldown / 5, BLACK);
      }
      if ((subtype == W_DISK) || (subtype == W_MINE)) { //refill animation
        if (shooter->dir > 0) {
          gb.fillRect(toScreenX(shooter->x) + 6, toScreenY(shooter->y) + 4, 4 - cooldown / 4, 2);
        }
        else {
          gb.fillRect(toScreenX(shooter->x) + cooldown / 4 - 4, toScreenY(shooter->y) + 4, 4, 2);
        }
      }
    }
};

/////////////////////////////////////////////
///////////////////////////////////////////// PLAYER
/////////////////////////////////////////////

const byte playerBitmap[][11] PROGMEM = {
  {8, 9, 0x0, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x66,},
  {8, 9, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x3C, 0x3C, 0xC,},
  {8, 9, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x38, 0x38, 0x18,},
  {8, 9, 0x0, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x1C, 0x1C, 0x18,},
  {8, 9, 0x0, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x7E, 0x3C, 0x30,}
};

class Player :
  public Box {
  public:
    Weapon weapon;
    boolean jumping;
    boolean doubleJumped;
    boolean dead;
    uint8_t score;

    void init() {
      x = 128;
      y = 150;
      dir = 1;
      score = 0;
      dead = false;
      weapon.init();
      weapon.shooter = this;
    }

    int getWidth() {
      return 48;
    }
    int getHeight() {
      return 72;
    }
    int getGravity() {
      return 8;
    }
    int getXFriction() {
      if (dead) {
        return 10;
      }
      else {
        return 40;
      };
    }
    int getXBounce() {
      if (dead) {
        return -1;
      }
      else {
        return 0;
      };
    }
    int getYBounce() {
      return 0;
    }

    void kill(int dir) {
      dead = true;
      vx = dir * 32;
      vy = -32;
      popupTimeLeft = 0;
      if (world.addScore(score)) {
        popup(F("NEW HIGHSCORE!"), 40);
      }
      saveEEPROM();
    }

    void update() {
      if (!dead) {
        //player input
        if (repeat(GB_BTN_LEFT, 1)) {
          dir = -1;
          vx += 16 * dir;
        }
        if (repeat(GB_BTN_RIGHT, 1)) {
          dir = 1;
          vx += 16;
        }
        if (gb.pressed(UP_BUTTON)) {
          gb.pollButtons();
          if (gb.justPressed(DOWN_BUTTON)) {
            weapon.subtype ++;
            weapon.subtype %= NUMWEAPONS;
            score = 0;
            popup(F("WEAPON CHEAT"));
          }
        }

        if (y > world.getHeight()) {
          kill(dir);
        }

        //jumping
        if (world.solidCollisionAtPosition(x, y + 1, getWidth(), getHeight())) {
          doubleJumped = false;
        }
        if (gb.pressed(B_BUTTON)) {
          if (world.solidCollisionAtPosition(x, y + 1, getWidth(), getHeight())) {
            vy = -32;
            jumping = true;
            gb.tone2(jump_sound);
          }
          else {
            if (doubleJumped == false) {
              vy = -32;
              doubleJumped = true;
              jumping = true;
              gb.tone2(jump_sound);
            }
          }
        }
        if ((timeHeld(GB_BTN_B) > 0) && (vy < 0) && jumping) {
          if (doubleJumped) {
            vy -= 6;
          }
          else {
            vy -= 12;
          }
        }
        if (vy > 0) {
          jumping = false;
        }
      }

      weapon.update();

      int8_t d = dir;
      Box::update(); //update physics
      dir = d; //override the direction calculated by Box::update(), which depend on velocity.
      //here the direction of the player should only depend on user input
      //I know, it's not very clean to do that this way
    }

    void draw() {
      if (isOffScreen()) {
        return;
      }
      byte flip = (dir > 0) ? NOFLIP : FLIPH;
      byte frame = (dir * x / 32 + 255) % 5; //get the current frame from the x position. Add 255 to avoid being under 0.
      if (vx == 0) {
        frame = 0;
      }
      if (!world.solidCollisionAtPosition(x, y + 1, getWidth(), getHeight())) { //in the air
        if (vy < 0) {
          frame = 4;
        }
        else {
          frame = 1;
        }
      }
      gb.drawBitmapF(toScreenX(x) - 1, toScreenY(y), playerBitmap[frame], NOROT, flip);
      weapon.draw();
    }
};

Player player;

/////////////////////////////////////////////
///////////////////////////////////////////// ENEMY
/////////////////////////////////////////////

const byte smallEnemyBitmap[][10] PROGMEM = {
  { 8, 8, 0x0, 0x7E, 0x6C, 0x6C, 0x7E, 0x7E, 0x7E, 0x66,},
  { 8, 8, 0x7E, 0x6C, 0x6C, 0x7E, 0x7E, 0x3C, 0x3C, 0xC,},
  { 8, 8, 0x7E, 0x6C, 0x6C, 0x7E, 0x7E, 0x38, 0x38, 0x18,},
  { 8, 8, 0x7E, 0x6C, 0x6C, 0x7E, 0x7E, 0x1C, 0x1C, 0x18,},
  { 8, 8, 0x0, 0x7E, 0x6C, 0x6C, 0x7E, 0x7E, 0x3C, 0x30,}
};

const byte bigEnemyBitmap[][22] PROGMEM = {
  { 16, 10, 0x0, 0x0, 0x3F, 0xE0, 0x3F, 0xE0, 0x3D, 0xA0, 0x3D, 0xA0, 0x3F, 0xE0, 0x3F, 0xE0, 0x3F, 0xE0, 0x38, 0xE0, 0x38, 0xE0,},
  { 16, 10, 0x0, 0x0, 0x3F, 0xE0, 0x3F, 0xE0, 0x3D, 0xA0, 0x3D, 0xA0, 0x3F, 0xE0, 0x3F, 0xE0, 0x3F, 0xE0, 0x3F, 0xE0, 0x1, 0xC0,},
  { 16, 10, 0x3F, 0xE0, 0x3F, 0xE0, 0x3D, 0xA0, 0x3D, 0xA0, 0x3F, 0xE0, 0x3F, 0xE0, 0x3F, 0xE0, 0x3F, 0xE0, 0xF, 0x0, 0x7, 0x0,},
  { 16, 10, 0x3F, 0xE0, 0x3F, 0xE0, 0x3D, 0xA0, 0x3D, 0xA0, 0x3F, 0xE0, 0x3F, 0xE0, 0x3F, 0xE0, 0x3F, 0xE0, 0x7, 0x80, 0x7, 0x0,},
  { 16, 10, 0x3F, 0xE0, 0x3F, 0xE0, 0x3D, 0xA0, 0x3D, 0xA0, 0x3F, 0xE0, 0x3F, 0xE0, 0x3F, 0xE0, 0x3F, 0xE0, 0xF, 0x80, 0xE, 0x0,},
  { 16, 10, 0x3F, 0xE0, 0x3F, 0xE0, 0x3D, 0xA0, 0x3D, 0xA0, 0x3F, 0xE0, 0x3F, 0xE0, 0x3F, 0xE0, 0x3F, 0xE0, 0x1D, 0xC0, 0x1C, 0x0,}
};

class Enemy :
  public Box {
  public:
    byte subtype;
    boolean active;
    char health;
    int getWidth() {
      return (subtype == E_SMALL) ? 48 : 72;
    }
    int getHeight() {
      return (subtype == E_SMALL) ? 64 : 80;
    }
    int getGravity() {
      if (health > 0)
        return 4;
      else
        return 10;
    }
    int getXFriction() {
      return 0;
    }
    int getXBounce() {
      if (health > 0) {
        return 100;
      }
      else {
        return -1;
      }
    }
    int getYBounce() {
      return 0;
    }
    int getMaxHealth() {
      return (subtype == E_SMALL) ? 2 : 10;
    }
    int getMaxSpeed() {
      return 32;
    }

    int update() {
      if (active) {
        Box::update();
        //respawn in "angry" mod whel enemies falls at the bottom of the map
        if (y > world.getHeight()) {
          if (health > 0) {
            x = world.getWidth() / 2 - getWidth() / 2;
            y = 0;
            vx = dir * 20;
            gb.tone2(enemy_felt_sound);
          }
          else {
            active = false;
          }
        }
      }
    }
    void draw() {
      if (isOffScreen()) {
        return;
      }
      int flip = (dir > 0) ? NOFLIP : FLIPH;
      if (subtype == E_SMALL) {
        byte frame = (dir * x / 16 + 255) % 5; //get the current frame from the x position. Add 255 to avoid being under 0.
        gb.drawBitmapF(toScreenX(x) - 1, toScreenY(y), smallEnemyBitmap[frame], NOROT, flip);
      }
      else {
        byte frame = (dir * x / 16 + 255) % 6; //get the current frame from the x position. Add 255 to avoid being under 0.
        gb.drawBitmapF(toScreenX(x) - 4, toScreenY(y), bigEnemyBitmap[frame], NOROT, flip);
      }
    }
};

/////////////////////////////////////////////
///////////////////////////////////////////// ENEMY ENGINE
/////////////////////////////////////////////

#define NUMENEMIES 10
Enemy enemies[NUMENEMIES];

class EnemiesEngine {
  public:
    char nextSpawnCount;
    void init() {
      nextSpawnCount = 10;
      for (byte i = 0; i < NUMENEMIES; i++) {
        enemies[i].active = false;
        enemies[i].health = 0;
      }
    }

    void addEnemy() {
      for (byte i = 0; i < NUMENEMIES; i++) {
        if (!enemies[i].active) {
          enemies[i].active = true;
          if ((random(0, 6) == 0) && (world.mapNumber != 0)) { //randomly spawn a few big monsters
            enemies[i].subtype = E_BIG;
          }
          else {
            enemies[i].subtype = E_SMALL;
          }
          enemies[i].health = enemies[i].getMaxHealth();
          enemies[i].x = world.getWidth() / 2 - enemies[i].getWidth() / 2;
          enemies[i].y = 0;
          enemies[i].vx = random(0, 2) * 20 - 10;
          enemies[i].vy = 0;
          return;
        }
      }
    }

    void update() {
      for (byte j = 0; j < NUMBULLETS; j++) {
        //skip inactive bullets
        if (bullets[j].timeLeft <= 0) {
          continue;
        }
        for (byte i = 0; i < NUMENEMIES; i++) {
          //skip dead enemies
          if (enemies[i].health <= 0) {
            continue;
          }
          //skip bullets with a low speed (falling particles) except explosions, mines and grenades
          if ((abs(bullets[j].vx) < 20) && !((bullets[j].subtype == W_EXPLOSION) || (bullets[j].subtype == W_MINE) || (bullets[j].subtype == W_GRENADE))) {
            break;
          }
          if (gb.collideRectRect(enemies[i].x, enemies[i].y, enemies[i].getWidth(), enemies[i].getHeight(),
                                 bullets[j].x, bullets[j].y, bullets[j].getWidth(), bullets[j].getHeight())) {
            if (bullets[j].explodes()) {
              bullets[j].timeLeft = 1;
            }
            if (bullets[j].destroyOnEnemyContact()) {
              bullets[j].vx = (bullets[j].vx * bullets[j].getXBounce()) / 100;
            }
            if (bullets[j].getDamage() > enemies[i].health) enemies[i].health = 0;
            else enemies[i].health -= bullets[j].getDamage();
            //make the ennemy jump when dead
            if (enemies[i].health <= 0) {
              int dir;
              if (bullets[j].subtype == W_EXPLOSION) { //fly away from the explosive
                dir = (((enemies[i].x + enemies[i].getWidth() / 2) - (bullets[j].x + bullets[j].getWidth() / 2)) > 0) ? 1 : -1;
              }
              else { //fly in the same direction that the incoming bullet
                dir = bullets[j].vx > 0 ? 1 : -1;
              }
              //throw the enemy in the air
              enemies[i].vx = dir * random(24, 32);
              enemies[i].vy = random(-48, -64);
              gb.tone2(enemy_death_sound);
            }
            else {
              if (bullets[j].subtype == W_CLUB) { // if not dead, go away from the player when hit by a club
                int dir = (enemies[i].x + enemies[i].getWidth() / 2) - (player.x + player.getWidth() / 2) > 0 ? 1 : -1;
                enemies[i].vx = dir * abs(enemies[i].vx);
              }
            }
          }
        }
      }
      nextSpawnCount--;
      if (!nextSpawnCount) { //spawn enemies
        //spawn rate increase slowly depending on score
        //when score is 0, monsters spawn every 60 frames (3 s)
        //when score is 50 monsters swpan every 30 frames (1.5 s)
        //they won't spaw faster than every 10 frames (0.5 s)
        nextSpawnCount = map(player.score, 0, 50, 60, 30);
        nextSpawnCount = maxVal(nextSpawnCount, 10);
        addEnemy();
      }
      for (byte i = 0; i < NUMENEMIES; i++) {
        enemies[i].update();
      }

    }

    void draw() {
      for (byte i = 0; i < NUMENEMIES; i++) {
        if (enemies[i].active) {
          enemies[i].draw();
        }
      }
    }

};

EnemiesEngine enemiesEngine;

///////////////////////////////////////////// CRATE
const byte crateBitmap[] PROGMEM = {
  8, 6, 0xFC, 0x84, 0xEC, 0xDC, 0x84, 0xFC,
};

class Crate :
  public Box {
  public:
    int getWidth() {
      return 48;
    }
    int getHeight() {
      return 48;
    }

    void init() {
      vy = 0;
      boolean goodSpot;
      do { //pick a random location somewhere in the world (distance from the border = 1 sprite max)
        x = random(SPRITE_SIZE * SCALE , world.getWidth() - SPRITE_SIZE * SCALE - getWidth());
        y = random(SPRITE_SIZE * SCALE, world.getHeight() - SPRITE_SIZE * SCALE - getHeight());
        goodSpot = true;
        if ((abs(player.x - x) < 128) || (abs(player.y - y) < 128))
          goodSpot = false; //too close to the player
        if ((x > (world.getWidth() / 2 - 128)) && (x < (world.getWidth() / 2 + 128)) && (y < 336))
          goodSpot = false; //avoid the top central zone where mobs spawn
      }
      while (!goodSpot);
    }

    void update() {
      Box::update();
      //reinit the crate if it felt out of the world
      if (y > world.getHeight()) {
        init();
      }
      //pick random weapon when taken by the player
      if (gb.collideRectRect(x, y, getWidth(), getHeight(),
                             player.x, player.y, player.getWidth(), player.getHeight())) {
        player.score++;
        gb.tone1(playOK);
        //add a random value to the weapon type inferior to the number of weapons
        //to avoid picking the same weapon
        player.weapon.subtype = (player.weapon.subtype + random(1, unlockedWeapons + 1)) % (unlockedWeapons + 1);
        switch (player.weapon.subtype) {
          case W_CLUB :
            popup(F("CLUB"));
            break;
          case W_PISTOL :
            popup(F("PISTOL"));
            break;
          case W_AKIMBO :
            popup(F("AKIMBO"));
            break;
          case W_REVOLVER :
            popup(F("REVOLVER"));
            break;
          case W_SNIPER :
            popup(F("SNIPER"));
            break;
          case W_SHOTGUN :
            popup(F("SHOTGUN"));
            break;
          case W_RIFLE :
            popup(F("RIFLE"));
            break;
          case W_MACHINEGUN :
            popup(F("MACHINEGUN"));
            break;
          case W_DISK :
            popup(F("DISK"));
            break;
          case W_LASER :
            popup(F("LASER"));
            break;
          case W_GRENADE :
            popup(F("GRENADE"));
            break;
          case W_ROCKET :
            popup(F("ROCKET"));
            break;
          case W_MINE :
            popup(F("MINE"));
            break;
        }
        if (world.mapNumber == 0) {
          switch (player.score) {
            case (SCORETHRESHOLD_1):
              if (unlockedWeapons < W_RIFLE) {
                unlockedWeapons = W_RIFLE;
                player.weapon.subtype = W_RIFLE;
                popup(F("RIFLE UNLOCKED!"), 40);
                gb.tone2(power_up_sound);
              }
              break;
            case (SCORETHRESHOLD_2):
              if (unlockedWeapons < W_SHOTGUN) {
                unlockedWeapons = W_SHOTGUN;
                player.weapon.subtype = W_SHOTGUN;
                popup(F("SHOTGUN UNLOCKED!"), 40);
                gb.tone2(power_up_sound);
              }
              break;
            case (SCORETHRESHOLD_3):
              if (unlockedMaps < 1) {
                unlockedMaps = 1;
                popup(F("NEW MAP UNLOCKED!"), 40);
                gb.tone2(power_up_sound);
              }
              break;
          }
        }
        if (world.mapNumber == 1) {
          switch (player.score) {
            case (SCORETHRESHOLD_1):
              if (unlockedWeapons < W_ROCKET) {
                unlockedWeapons = W_ROCKET;
                player.weapon.subtype = W_ROCKET;
                popup(F("ROCKETS UNLOCKED!"), 40);
                gb.tone2(power_up_sound);
              }
              break;
            case (SCORETHRESHOLD_2):
              if (unlockedWeapons < W_CLUB) {
                unlockedWeapons = W_CLUB;
                player.weapon.subtype = W_CLUB;
                popup(F("CLUB UNLOCKED!"), 40);
                gb.tone2(power_up_sound);
              }
              break;
            case (SCORETHRESHOLD_3):
              if (unlockedWeapons < W_REVOLVER) {
                unlockedWeapons = W_REVOLVER;
                player.weapon.subtype = W_REVOLVER;
                popup(F("REVOLVER UNLOCKED!"), 40);
                gb.tone2(power_up_sound);
              }
              break;
            case (SCORETHRESHOLD_4):
              if (unlockedWeapons < W_MINE) {
                unlockedWeapons = W_MINE;
                player.weapon.subtype = W_MINE;
                popup(F("MINES UNLOCKED!"), 40);
                gb.tone2(power_up_sound);
              }
              break;
            case (SCORETHRESHOLD_5):
              if (unlockedMaps < 2) {
                unlockedMaps = 2;
                popup(F("NEW MAP UNLOCKED!"), 40);
                gb.tone2(power_up_sound);
              }
              break;
          }
        }
        if (world.mapNumber == 2) {
          switch (player.score) {
            case (SCORETHRESHOLD_1):
              if (unlockedWeapons < W_SNIPER) {
                unlockedWeapons = W_SNIPER;
                player.weapon.subtype = W_SNIPER;
                popup(F("SNIPER UNLOCKED!"), 40);
                gb.tone2(power_up_sound);
              }
              break;
            case (SCORETHRESHOLD_2):
              if (unlockedWeapons < W_MACHINEGUN) {
                unlockedWeapons = W_MACHINEGUN;
                player.weapon.subtype = W_MACHINEGUN;
                popup(F("MACHINEGUN UNLOCKED!"), 40);
                gb.tone2(power_up_sound);
              }
              break;
            case (SCORETHRESHOLD_3):
              if (unlockedWeapons < W_GRENADE) {
                unlockedWeapons = W_GRENADE;
                player.weapon.subtype = W_GRENADE;
                popup(F("GRENADES UNLOCKED!"), 40);
                gb.tone2(power_up_sound);
              }
              break;
            case (SCORETHRESHOLD_4):
              if (unlockedWeapons < W_AKIMBO) {
                unlockedWeapons = W_AKIMBO;
                player.weapon.subtype = W_AKIMBO;
                popup(F("AKIMBO UNLOCKED!"), 40);
                gb.tone2(power_up_sound);
              }
              break;
            case (SCORETHRESHOLD_5):
              if (unlockedMaps < 3) {
                unlockedMaps = 3;
                popup(F("NEW MAP UNLOCKED!"), 40);
                gb.tone2(power_up_sound);
              }
              break;
          }
        }
        if (world.mapNumber == 3) {
          switch (player.score) {
            case (SCORETHRESHOLD_3):
              if (unlockedWeapons < W_DISK) {
                unlockedWeapons = W_DISK;
                player.weapon.subtype = W_DISK;
                popup(F("DISK UNLOCKED!"), 40);
              }
              break;
            case (SCORETHRESHOLD_4):
              if (unlockedWeapons < W_LASER) {
                unlockedWeapons = W_LASER;
                player.weapon.subtype = W_LASER;
                popup(F("LASER UNLOCKED!"), 40);
              }
              break;
            case (SCORETHRESHOLD_5):
              if (unlockedMaps < 4) {
                unlockedMaps = 4;
                popup(F("LAST MAP UNLOCKED!"), 40);
              }
              break;
          }
        }
        init(); //move the crate
      }
    }

    void draw() {
      if (isOffScreen())
        return;
      gb.drawBitmap(toScreenX(x), toScreenY(y), crateBitmap);
    }
};

Crate crate;

///////////////////////////////////////////// SETUP
void setup() {
  EEPROM.begin(500);
  gb.setFrameRate(20);
  gb.begin();
  beep1.begin();
  beep2.begin();
  loadEEPROM();
  world.chooseMap();
}

///////////////////////////////////////////// LOOP
void loop() {
  if (gb.nextFrame()) {
    //if () {
    //  gamePaused();
    //}
    gb.display(CLEAR_BUFFER);
    gb.timer1();
    gb.timer2();
    updateButtons();

    crate.update();
    player.update();
    enemiesEngine.update();
    //saveEEPROM(); //it checks if the values have changed before writting so it won't wear out the EEPROM

    //camera smoothing
    //int x = (player.x + player.getWidth()/2)/SCALE - LCDWIDTH/2;
    //int y = (player.y + player.getHeight()/2)/SCALE - LCDHEIGHT/2;
    //cameraX = (3*cameraX + x)/4;
    //cameraY = (3*cameraY + y)/4;

    //update camera
    if (world.getWidth()*SCALE <= LCDWIDTH) {
      cameraX = 0;
    } else {
      cameraX = (player.x + player.getWidth() / 2) / SCALE - LCDWIDTH / 2;
      cameraX = constrain(cameraX, 0, world.getWidth() / SCALE - LCDWIDTH);
    }
    if (world.getHeight()*SCALE <= LCDHEIGHT) {
      cameraY = 0;
    } else {
      cameraY = (player.y + player.getHeight() / 2) / SCALE - LCDHEIGHT / 2;
      cameraY = constrain(cameraY, 0, world.getHeight() / SCALE - LCDHEIGHT - SPRITE_SIZE / 2);
    }

    if (shakeTimeLeft > 0) {
      shakeTimeLeft --;
      cameraX += random(- 1, 2) * shakeAmplitude;
      cameraY += random(- 1, 2) * shakeAmplitude;
    }

    drawAll();

    //check for player - monsters collisions
    for (byte i = 0; i < NUMENEMIES; i++) {
      if (gb.collideRectRect(enemies[i].x, enemies[i].y, enemies[i].getWidth(), enemies[i].getHeight(),
                             player.x, player.y, player.getWidth(), player.getHeight())) {
        if (enemies[i].health > 0) {
          player.dead = true;
          int dir = (((enemies[i].x + enemies[i].getWidth() / 2) - (player.x + player.getWidth() / 2)) > 0) ? -1 : 1;
          player.kill(dir);
          break;
        }
      }
    }
    //check for player - bullet collisons
    for (byte i = 0; i < NUMBULLETS; i++) {
      //skip inactive bullets
      if (bullets[i].timeLeft <= 0) {
        continue;
      }
      if (bullets[i].damagePlayer() && gb.collideRectRect(bullets[i].x, bullets[i].y, bullets[i].getWidth(), bullets[i].getHeight(),
          player.x, player.y, player.getWidth(), player.getHeight())) {
        player.kill(bullets[i].dir);
      }
    }

    if (player.dead) {
      byte count = 20;
      if (!popupTimeLeft) { //if the "new highscore" popup is no here
        popup(F("GAME OVER!"), 20);
      }
      while (1) {
        if (gb.nextFrame()) {
          gb.display(CLEAR_BUFFER);
          gb.timer1();
          gb.timer2();
          player.update();
          enemiesEngine.update();
          drawAll();
          count --;
          if (!count) {
            break;
          }
          if (gb.pressed(A_BUTTON) || gb.pressed(B_BUTTON)) {
            break;
          }
        }
      }
      initGame();
    }
  }
}

void gamePaused() {
  while (1) {
    if (gb.nextFrame()) {
      gb.display(CLEAR_BUFFER);
      gb.timer1();
      gb.timer2();
      drawAll();
      gb.cursor_x = 0;
      gb.cursor_y = 0;
      gb.setTextColor(WHITE);
      gb.println(F("GAME PAUSED"));
      gb.println(F("A: SAVE & QUIT"));
      gb.println(F("B: RESUME"));
      gb.setTextColor(BLACK);

      if (gb.pressed(B_BUTTON)) {
        return;
      }
      if (gb.pressed(A_BUTTON)) {
        world.addScore(player.score);
        saveEEPROM();
        world.chooseMap();
        return;
      }
    }
  }
}

void initGame() {
  player.init();
  enemiesEngine.init();
  crate.init();
  player.weapon.subtype = 0;
  shakeTimeLeft = 0;
}

void drawAll() {
  gb.setTextColor(WHITE);
  gb.setCursor(103,8);
  gb.print(player.score);
  gb.setTextColor(BLACK);
  world.draw();
  crate.draw();
  enemiesEngine.draw();
  player.draw();
  if (!player.dead && world.mapNumber) {
    drawCompass();
  }
  //draw explosions
  for (byte i = 0; i < NUMBULLETS; i++) {
    if (bullets[i].subtype == W_EXPLOSION) {
      gb.setTextColor(INVERT);
    }
    bullets[i].draw();
    gb.setTextColor(BLACK);
  }
  updatePopup();
}

void loadEEPROM() {
  if (EEPROM.read(EEPROM_TOKEN_OFFSET) != EEPROM_TOKEN) {
    uint8_t data[3 + NUMMAPS] = {EEPROM_TOKEN,0,0,0,0,0,0,0};
    EEPROM.put(EEPROM_TOKEN_OFFSET, data);
  }

  unlockedWeapons = EEPROM.read(EEPROM_WEAPONS_OFFSET);
  unlockedMaps = EEPROM.read(EEPROM_MAPS_OFFSET);
  for (uint16_t i = 0; i < NUMMAPS; i++) {
    score[i] = EEPROM.read(EEPROM_SCORE_OFFSET + i);
  }
  world.mapNumber = unlockedMaps; //select the last unlocked map by default
}

void saveEEPROM() {
  uint8_t data[2 + NUMMAPS] = {unlockedWeapons, unlockedMaps,
                               score[0], score[1], score[2], score[3], score[4]};

  EEPROM.put(EEPROM_WEAPONS_OFFSET, data);
  EEPROM.commit();
}

void printCentered(const __FlashStringHelper* text) {
  String bufStr;
  bufStr = text;
  gb.cursor_x = (bufStr.length() < 14) ? (LCDWIDTH / 2) - (bufStr.length() * 6 / 2) : 2;
  gb.print(text);
}

void popup(const __FlashStringHelper* text) {
  popup(text, 20);
}

void popup(const __FlashStringHelper* text, uint8_t duration) {
  popupText = text;
  popupTimeLeft = duration + 12;
}

void updatePopup() {
  static uint8_t yOffset;

  if (popupTimeLeft) {
    if (popupTimeLeft < 12) {
      yOffset++;
    }
    else yOffset = 55;
    String bufStr = popupText;
    byte width = bufStr.length() * 6;
    byte rectX = (bufStr.length() < 14) ? (LCDWIDTH / 2) - (width / 2) - 2 : 0;
    gb.setTextSize(1);
    gb.drawRect(rectX, yOffset - 2, width + 3, 11, WHITE);
    gb.setTextColor(WHITE);
    gb.cursor_y = yOffset;
    printCentered(popupText);
    popupTimeLeft--;
  }
  gb.setTextColor(BLACK);
}

void drawCompass() {
  //if (crate.isOffScreen()) {
  int x = (crate.x + crate.getWidth() / 2 - player.x - player.getWidth() / 2) / SCALE;
  int y = (crate.y + crate.getWidth() / 2 - player.y - player.getHeight() / 2) / SCALE;
  int dist = sqrt(x * x + y * y);
  if (dist > 20  ) {
    int dx = toScreenX(player.x + player.getWidth() / 2)   + (16 * x / dist);
    int dy = toScreenY(player.y + player.getHeight() / 2)  + (16 * y / dist);
    //gb.setTextColor(GRAY);
    gb.drawLine(dx, dy, dx + x / 8, dy + y / 8);
    //gb.setTextColor(BLACK);
  }
}

uint8_t states[6];
uint8_t bits[6] = {RIGHT_BUTTON, LEFT_BUTTON, DOWN_BUTTON, UP_BUTTON, A_BUTTON, B_BUTTON};
//---------------------------------------------------------------------------
void updateButtons()
{
    for(uint8_t thisButton=0; thisButton<6; thisButton++)
    {
        // if button pressed
        if(gb.pressed(bits[thisButton]))
        {
            // increase button hold time
            states[thisButton]++;
        }
        else
        {
            // button idle
            if(states[thisButton] == 0)
            {
                continue;
            }

            // if previously released
            if(states[thisButton] == 0xFF)
            {
                // set to idle
                states[thisButton] = 0;
            }
            else
            {
                // button just released
                states[thisButton] = 0xFF;
            }
        }
    }
}
//---------------------------------------------------------------------------
boolean repeat(uint8_t button, uint8_t period)
{
    if(period <= 1)
    {
        if((states[button] != 0xFF) && (states[button]))
        {
            return true;
        }
    }
    else
    {
        if((states[button] != 0xFF) && ((states[button] % period) == 1))
        {
            return true;
        }
    }

    return false;
}
//---------------------------------------------------------------------------
uint8_t timeHeld(uint8_t button)
{
    return (states[button] != 0xFF) ? states[button] : 0;
}
