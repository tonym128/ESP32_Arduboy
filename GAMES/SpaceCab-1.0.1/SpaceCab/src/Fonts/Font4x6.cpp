#include <Arduino.h>
#include <Sprites.h>
#include <Print.h>
#include "Font4x6.h"

#define USE_LOWER_CASE

#define FONT4x6_WIDTH 4
#define FONT4x6_HEIGHT 7

#define CHAR_EXCLAMATION 33
#define CHAR_PERIOD 46
#define CHAR_QUESTION 63
#define CHAR_LETTER_A 65
#define CHAR_LETTER_Z 90
#define CHAR_LETTER_A_LOWER 97
#define CHAR_LETTER_Z_LOWER 122
#define CHAR_NUMBER_0 48
#define CHAR_NUMBER_9 57

#ifdef USE_LOWER_CASE
  #define FONT_EXCLAMATION_INDEX 62
  #define FONT_PERIOD_INDEX 63
  #define FONT_QUESTION_INDEX 64
  #define FONT_NUMBER_INDEX 52
#else
  #define FONT_EXCLAMATION_INDEX 36
  #define FONT_PERIOD_INDEX 37
  #define FONT_QUESTION_INDEX 38
  #define FONT_NUMBER_INDEX 26
#endif


Font4x6::Font4x6(uint8_t lineSpacing) {

  _lineHeight = lineSpacing;
  _letterSpacing = 1;

  _cursorX = _cursorY = _baseX = 0;
  _textColor = 1;

}

size_t Font4x6::write(uint8_t c) {

  if (c == '\n')      { _cursorX = _baseX; _cursorY += _lineHeight; }
  else {

    printChar(c, _cursorX, _cursorY);
    _cursorX += FONT4x6_WIDTH + _letterSpacing;

  }

  return 1;

}

void Font4x6::printChar(const char c, const int8_t x, int8_t y) {

  int8_t idx = -1;

  ++y;

  switch (c) {
    
    case CHAR_LETTER_A ... CHAR_LETTER_Z:
      idx = c - CHAR_LETTER_A;
      break;

#ifdef USE_LOWER_CASE    
    case CHAR_LETTER_A_LOWER ... CHAR_LETTER_Z_LOWER:
      idx = c - CHAR_LETTER_A_LOWER + 26;
      break;
#endif

    case CHAR_NUMBER_0 ... CHAR_NUMBER_9:
      idx = c - CHAR_NUMBER_0 + FONT_NUMBER_INDEX;
      break;
      
    case CHAR_EXCLAMATION:
      idx = FONT_EXCLAMATION_INDEX;
      break;
      
    case CHAR_PERIOD:
      idx = FONT_PERIOD_INDEX;
      break;
      
    case CHAR_QUESTION:
      idx = FONT_QUESTION_INDEX;
      break;

  }

  if (idx > -1) {
    
    if (_textColor == WHITE) {
      Sprites::drawSelfMasked(x, y, font4x6_Full, idx);
    }
    else {
      Sprites::drawErase(x, y, font4x6_Full, idx);
    }

  }

}

void Font4x6::setCursor(const int8_t x, const int8_t y) {
  _cursorX = _baseX = x;
  _cursorY = y;
}

void Font4x6::setTextColor(const uint8_t color){
  _textColor = color;
}

void Font4x6::setHeight(const uint8_t color){
  _lineHeight = color;
}
