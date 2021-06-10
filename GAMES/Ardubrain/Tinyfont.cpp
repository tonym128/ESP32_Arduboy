/**
 * @file Tinyfont.h
 * \brief An Arduboy library to write a 4x4 font.
 */

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2017, Botond Kis
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of the copyright holder nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "Tinyfont.hpp"
//#include <avr/io.h>
#include <avr/pgmspace.h>

#define TINYFONT_WIDTH 4
#define TINYFONT_HEIGHT 4

const unsigned char PROGMEM TINYFONT_SPRITE[] = {
    0x00, 0xb0, 0x00, 0x00, 0xa1, 0x70, 0xe1, 0x50, /**/
    0x96, 0x4f, 0x26, 0x90, 0x0f, 0x1d, 0x07, 0x0c, /**/
    0x00, 0x96, 0x69, 0x00, 0x4a, 0xe4, 0x4a, 0x00, /**/
    0x48, 0x44, 0x40, 0x00, 0x80, 0x68, 0x10, 0x00, /**/
    0x0f, 0x99, 0xfb, 0x8f, 0x9d, 0xbd, 0xbb, 0xfb, /**/
    0x77, 0xd4, 0xd4, 0xdf, 0x1f, 0x1a, 0x1a, 0xfe, /**/
    0x7f, 0x5d, 0x5d, 0xff, 0x80, 0x5a, 0x00, 0x00, /**/
    0xa0, 0xa4, 0xaa, 0x00, 0x10, 0xba, 0x34, 0x00, /**/
    0xff, 0x59, 0x53, 0xf3, 0xff, 0x9b, 0x9b, 0x9e, /**/
    0xff, 0xb9, 0xb9, 0x96, 0xff, 0x95, 0x95, 0xd1, /**/
    0x9f, 0xf4, 0x94, 0x0f, 0xfc, 0x29, 0x5f, 0x91, /**/
    0xff, 0x18, 0x38, 0xf8, 0xff, 0x92, 0x94, 0xff, /**/
    0xff, 0x95, 0xd5, 0xf7, 0xbf, 0xb5, 0xdd, 0xd7, /**/
    0xf1, 0x8f, 0x81, 0xf1, 0xf7, 0x88, 0xc8, 0xf7, /**/
    0x79, 0xc6, 0x46, 0x79, 0x09, 0xfd, 0x9b, 0x09, /**/
    0x01, 0x96, 0xf8, 0x00, 0x82, 0x81, 0x82, 0x80, /**/
    0x50, 0x71, 0x62, 0x00, 0x77, 0x56, 0x56, 0x00, /**/
    0x76, 0x76, 0x37, 0x00, 0xa2, 0xb7, 0x73, 0x01, /**/
    0x07, 0x72, 0x06, 0x00, 0x78, 0x27, 0x50, 0x00, /**/
    0x73, 0x34, 0x74, 0x00, 0x77, 0x51, 0x76, 0x00, /**/
    0x7f, 0x55, 0xf7, 0x00, 0x47, 0x71, 0x10, 0x00, /**/
    0x32, 0x47, 0x72, 0x00, 0x73, 0x64, 0x73, 0x00, /**/
    0x15, 0xa2, 0x75, 0x00, 0x61, 0x67, 0x94, 0x00, /**/
    0x00, 0x9f, 0x60, 0x60, 0xf4, 0xf2, 0xf6, 0xf2, /**/
};

Tinyfont::Tinyfont(uint8_t* screenBuffer, int16_t width, int16_t height) {
  sBuffer = screenBuffer;
  sWidth = width;
  sHeight = height;

  // default values
  lineHeight = TINYFONT_HEIGHT + 1;
  letterSpacing = 1;

  cursorX = cursorY = baseX = 0;
  charLimit = -1;
  charPrinted = 0;
  textColor = 1;

  maskText = false;
}

size_t Tinyfont::write(uint8_t c) {
  if (charLimit >= 0 && charPrinted > charLimit)
    return 1;
  if (c == '\n') {
    cursorX = baseX;        // cr
    cursorY += lineHeight;  // lf
  }
  // check for space
  else if (c == ' ') {
    cursorX += TINYFONT_WIDTH + 1;
  }
  // check for tab
  else if (c == '\t') {
    cursorX += TINYFONT_WIDTH + 5;
  }
  // check \n
  else if (c == '\n') {
    cursorX = baseX;
    cursorY += lineHeight;
  } else {
    // draw char
    printChar(c, cursorX, cursorY);
    cursorX += TINYFONT_WIDTH + letterSpacing;
  }
  charPrinted++;
  return 1;
}

void Tinyfont::printChar(char c, int16_t x, int16_t y) {
  // no need to draw at all if we're offscreen
  if (x + TINYFONT_WIDTH <= 0 || x > sWidth - 1 || y + TINYFONT_HEIGHT <= 0 || y > sHeight - 1)
    return;

  // check if char is available
  if ((static_cast<uint8_t>(c) < 32) || (static_cast<uint8_t>(c) > 255))
    c = static_cast<char>(127);

  uint8_t cval = static_cast<uint8_t>(c) - 32;

  // layout lowercase letters
  if (cval >= 65 && cval <= 90)
    y++;

  // layout comma letters
  if (cval == 12 || cval == 27)
    y++;

  // get sprite frames
  const uint8_t* letter = TINYFONT_SPRITE + ((cval / 2) * 4);

  for (uint8_t i = 0; i < 4; i++) {
    uint8_t colData = pgm_read_byte(letter++);
    if (c % 2 == 0) {
      // mask upper sprite
      colData &= 0x0f;
    } else {
      // for every odd character, shift pixels to get the correct character
      colData >>= 4;
    }

    if (maskText) {
      drawByte(x + i, y - 1, 0x3f, (textColor == 0) ? 1 : 0);
    }

    drawByte(x + i, y, colData, textColor);
  }

  if (maskText) {
    drawByte(x + 4, y - 1, 0x3f, (textColor == 0) ? 1 : 0);
  }
}

void Tinyfont::drawByte(int16_t x, int16_t y, uint8_t pixels, uint8_t color) {
  uint8_t row = static_cast<uint8_t>(y) / 8;

  // check if byte needs to be seperated
  if ((static_cast<uint8_t>(y) % 8) == 0) {
    uint8_t col = static_cast<uint8_t>(x) % sWidth;
    if (color == 0)
      sBuffer[col + row * sWidth] &= ~pixels;
    else
      sBuffer[col + row * sWidth] |= pixels;
  } else {
    uint8_t d = static_cast<uint8_t>(y) % 8;

    drawByte(x, row * 8, pixels << d, color);
    drawByte(x, (row + 1) * 8, pixels >> (8 - d), color);
  }
}

void Tinyfont::setCursor(int16_t x, int16_t y, int16_t cl) {
  cursorX = baseX = x;
  cursorY = y;
  charLimit = cl;
  charPrinted = 0;
}

int16_t Tinyfont::getCursorX() const {
  return cursorX;
}

int16_t Tinyfont::getCursorY() const {
  return cursorY;
}

void Tinyfont::setTextColor(uint8_t color) {
  textColor = color;
}

uint8_t Tinyfont::getTextColor() const {
  return textColor;
}
