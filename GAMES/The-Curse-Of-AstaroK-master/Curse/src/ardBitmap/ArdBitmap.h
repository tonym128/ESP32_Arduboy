#pragma once
/*
  Copyright (C) 2016 Ignacio Vina (@igvina)

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  Modified by Filmote and Vampirics


  ArdBitmap: version 2.0.3

*/

//Uncomment NO_SPEED_HACK if speed is not important (reduce ~100 bytes)
#define NO_SPEED_HACK

//Uncomment RESIZE_HACK for fast drawResized with resize >= 1.0
//#define RESIZE_HACK

#include <Arduino.h>

#define MIRROR_NONE       0b00000000
#define MIRROR_HORIZONTAL 0b00000001
#define MIRROR_VERTICAL   0b00000010
#define MIRROR_HOR_VER    0b00000011

#define ARDBITMAP_SBUF Arduboy2Base::sBuffer

	
static const uint8_t BIT_SHIFT[8] = {
  0b00000001,
  0b00000010,
  0b00000100,
  0b00001000,
  0b00010000,
  0b00100000,
  0b01000000,
  0b10000000,
};

template<int16_t SB_WIDTH, int16_t SB_HEIGHT> class ArdBitmap {

  public:

    void drawCompressed(int16_t sx, int16_t sy, const uint8_t *compBitmap, uint8_t color, uint8_t mirror);
    void drawCompressed_WithMask(int16_t sx, int16_t sy, const uint8_t *compBitmap, const uint8_t *compBitmapMask, uint8_t mirror);
};

template<int16_t SB_WIDTH, int16_t SB_HEIGHT>
void ArdBitmap<SB_WIDTH, SB_HEIGHT>::drawCompressed_WithMask(int16_t sx, int16_t sy, const uint8_t *compBitmap, const uint8_t *compBitmapMask, uint8_t mirror) {

  drawCompressed(sx, sy, compBitmapMask, BLACK, mirror);
  drawCompressed(sx, sy, compBitmap, WHITE, mirror);

}

template<int16_t SB_WIDTH, int16_t SB_HEIGHT>
void ArdBitmap<SB_WIDTH, SB_HEIGHT>::drawCompressed(int16_t sx, int16_t sy, const uint8_t *compBitmap, uint8_t color, uint8_t mirror) {

  int16_t sizeCounter;
  uint16_t len;
  int a, iCol;
  uint8_t decByte;
  uint8_t w, h;
  uint8_t col;
  boolean scanMode, scanZigZag;
  uint16_t encoderPos;
  uint8_t characterPos;


  // Read size from header (Max image size = 128 x 64)

  uint8_t byte0 = pgm_read_byte(&compBitmap[0]);
  uint8_t byte1 = pgm_read_byte(&compBitmap[1]);

  w = (byte0 & 0b01111111) + 1;
  h = (byte1 & 0b00111111) + 1;


  // No need to draw at all if we're offscreen
  if (sx + w < 0 || sx > SB_WIDTH - 1 || sy + h < 0 || sy > SB_HEIGHT - 1)
    return;

  col = (byte0 >> 7) & 0x01;
  scanMode = ((byte1 >> 6) & 0x01) > 0;
  scanZigZag = ((byte1 >> 7) & 0x01) > 0;

  int yOffset = abs(sy) % 8;
  int sRow = sy / 8;
  if (sy < 0 && yOffset > 0) {
    sRow--;
    yOffset = 8 - yOffset;
  }

  uint8_t data;
  uint16_t bitmap_data;
  uint8_t mul_amt = 1 << yOffset;

  //uint16_t boffs;

  int8_t rows = h / 8;
  if (h % 8 != 0) rows++;

  // Init values
  iCol = 0;
  decByte = 0;
  encoderPos = 16;
  characterPos = 7;
  a = 0;

  int iColMod = (mirror & MIRROR_HORIZONTAL) ? w - 1  : 0;
  while (a < rows && a > -1) {

    sizeCounter = 1;
    while (((pgm_read_byte(&compBitmap[encoderPos / 8]) >> (encoderPos % 8)) & 0x01)  == 1) {
      sizeCounter ++;
      encoderPos++;
    }
    encoderPos ++;

    if (sizeCounter == 1) {
      len = 1 + ((pgm_read_byte(&compBitmap[encoderPos / 8]) >> (encoderPos % 8)) & 0x01);
      encoderPos++;
    } 
    else {
    
      len = (1 << (sizeCounter - 1)) + 1 ;

      for (int j = 0; j < sizeCounter - 1; j++) {
        if (((pgm_read_byte(&compBitmap[encoderPos / 8]) >> (encoderPos % 8)) & 0x01) == 1) {
          len += (1 << j);
        }
        encoderPos++;
      }
    }

    for (uint16_t i = 0; i < len; i++)
    {

      #ifndef NO_SPEED_HACK
      if (col == 0) {
        if (len - i > characterPos) {
          i += characterPos;
          characterPos = 0;
        } else {
          characterPos -= (len - i - 1);
          i = len;
        }
      } else if (len - i > characterPos) {
        if (characterPos == 7) {
          decByte = 0xFF;
        } else {
          decByte |= scanMode ? 0xFF >> (7 - characterPos) : (0xFF80 >> characterPos);
        }
        i += characterPos;
        characterPos = 0;
      } else {
        decByte |= scanMode ? BIT_SHIFT[characterPos] : BIT_SHIFT[7 - characterPos];
      }
      #else
      if (col) {
        decByte |= scanMode ? BIT_SHIFT[characterPos] : BIT_SHIFT[7 - characterPos];
      }
      #endif

      characterPos--;

      if (characterPos == 0xFF){

        //Paint decoded byte
        int8_t bRow = sRow + a;

        if (decByte && bRow < (SB_HEIGHT / 8) && iColMod + sx < SB_WIDTH && iColMod + sx >= 0){

          bitmap_data = decByte * mul_amt;

          if (bRow >= 0) {

            data = ARDBITMAP_SBUF[(bRow * SB_WIDTH) + sx + iColMod];
            if (color) {
              data |= bitmap_data & 0xFF;
            }else {
              data &= ~(bitmap_data & 0xFF);
            }
            ARDBITMAP_SBUF[(bRow * SB_WIDTH) + sx + iColMod] = data;
          }

          if (yOffset && bRow < (SB_HEIGHT / 8) - 1 && bRow > -2) {

            data = ARDBITMAP_SBUF[((bRow + 1) * SB_WIDTH) + sx + iColMod];
            if (color) {
              data |= ((bitmap_data >> 8) & 0xFF);
            } else {
              data &= ~(((bitmap_data >> 8) & 0xFF));
            }
            ARDBITMAP_SBUF[((bRow + 1)*SB_WIDTH) + sx + iColMod] = data;
          }
        }

        // Iterate next column-byte

        if (scanZigZag) {
          scanMode = !scanMode;
        }

        iCol++;

        if(mirror & MIRROR_HORIZONTAL){
          iColMod--;
        }else{
          iColMod++;
        }
        if (iCol >= w){

          iCol = 0;
          a++;
          iColMod = (mirror & MIRROR_HORIZONTAL) ? w - 1  : 0;
        }

        // Reset decoded byte
        decByte = 0;
        characterPos = 7;
      }
    }

    // Toggle color for next span
    col = 1 - col;
  }
}

