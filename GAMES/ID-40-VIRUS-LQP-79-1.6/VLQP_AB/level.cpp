#include "level.h"

// method implementations ///////////////////////////////////////////////////

int mapPositionX;
int mapPositionY;
int level;
int displayLevel;


// getTileType
// takes x and y in tile coordinates
// returns the tile type
uint8_t getTileType(unsigned int posX, unsigned int posY) {
  return
    // Block:read type (int16_t)
    pgm_read_byte(
      // Block:first index is block number
      &blocks[
        // Levels:read index (int16_t) poso block[] array
        (pgm_read_byte(
           // Levels:first index is level
           &maps[level - 1]
           // Levels:second index is map section (upper x/y bits)
           [((posX >> 3) + (posY & 0xF8)) >> 1]
         ) & ((posX & 8) ? 0x0F : 0xFF)) >> ((posX & 8) ? 0 : 4)
        // Block:second index is map tiles (lower x/y bits)
      ][
        (posX & 0x07) + ((posY & 0x07) << 3)
      ]
    );
}

void newDraw(unsigned posX, unsigned posY) {
  unsigned int intX = posX >> 3, subX = posX & 0x07;
  unsigned int intY = posY >> 3, subY = posY & 0x07;
  for (int16_t x = 0; x < (subX ? 17 : 16); x++) {
    for (int16_t y = 0; y < (subY ?  9 :  8); y++) {
      sprites.drawSelfMasked(
        ((int)x << 3) - subX, ((int)y << 3) - subY, tileset,
        getTileType(intX + x, intY + y)
        + (40 * ((level-1)%3))
      );
    }
  }
}


void drawLevel()
{
  newDraw (mapPositionX, mapPositionY);
}


void drawNumbers(int16_t NumbersX, int16_t NumbersY, int16_t fontType, int timerOrScoreOrLevel)
{
  char buf[10];
  int8_t intLen;
  int8_t pad;
  //scorePlayer = arduboy.cpuLoad();
  switch (timerOrScoreOrLevel)
  {
    case DATA_TIMER:
      itoa(exitDoor.counter, buf, 10);
      intLen = strlen(buf);
      pad = 3 - intLen;
      break;
    case DATA_SCORE:
      ltoa(scorePlayer, buf, 10);
      intLen = strlen(buf);
      pad = 6 - intLen;
      break;
    case DATA_LEVEL:
      itoa(displayLevel, buf, 10);
      intLen = strlen(buf);
      pad = 3 - intLen;
      break;
  }


  //draw 0 padding
  for (int16_t i = 0; i < pad; i++)
  {
    switch (fontType)
    {
      case FONT_TINY:
        sprites.drawSelfMasked(NumbersX + (4 * i), NumbersY, numbersTiny, 0);
        break;
      case FONT_SMALL:
        sprites.drawPlusMask(NumbersX + (7 * i), NumbersY, numbersSmall_plus_mask, 0);
        break;
      case FONT_BIG:
        sprites.drawSelfMasked(NumbersX + (10 * i), NumbersY, numbersBig, 0);
        break;
    }
  }

  for (int16_t i = 0; i < intLen; i++)
  {
    int8_t digit = buf[i];
    int16_t j;
    if (digit <= 48)
    {
      digit = 0;
    }
    else {
      digit -= 48;
      if (digit > 9) digit = 0;
    }

    for (int16_t z = 0; z < 10; z++)
    {
      if (digit == z) j = z;
    }
    switch (fontType)
    {
      case FONT_TINY:
        sprites.drawSelfMasked(NumbersX + (pad * 4) + (4 * i), NumbersY, numbersTiny, digit);
        break;
      case FONT_SMALL:
        sprites.drawPlusMask(NumbersX + (pad * 7) + (7 * i), NumbersY, numbersSmall_plus_mask, digit);
        break;
      case FONT_BIG:
        sprites.drawSelfMasked(NumbersX + (pad * 10) + (10 * i), NumbersY, numbersBig, digit);
        break;
    }
  }
}

void mapCollide(int& x, int& y, bool horizontal, int16_t& vel, int8_t w, int8_t h)
{
  short tilex;
  short tiley;
  int16_t tileXMax = x % TILE_WIDTH != 0;
  int16_t tileYMax = y % TILE_HEIGHT != 0;
  for (tilex = x / TILE_WIDTH; tilex < x / TILE_WIDTH + 2 + tileXMax; tilex++)
  {
    for (tiley = y / TILE_HEIGHT; tiley < y / TILE_HEIGHT + 2 + tileYMax; tiley++)
    {
      if (getTileType(tilex, tiley) > 10)
      {
        if (horizontal)
        {
          if (vel < 0)
            x = tilex * TILE_WIDTH + TILE_WIDTH;
          else if (vel > 0)
            x = tilex * TILE_WIDTH - w;
        }
        else
        {
          if (vel < 0)
            y = tiley * TILE_HEIGHT + TILE_HEIGHT;
          else if (vel > 0)
            y = tiley * TILE_HEIGHT - h;
        }
        vel = 0;
      }
    }
  }
}
