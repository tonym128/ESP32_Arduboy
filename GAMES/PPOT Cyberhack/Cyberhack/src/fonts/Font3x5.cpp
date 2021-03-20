#include <Arduino.h>
#include <Sprites.h>
#include <Print.h>
#include "Font3x5.h"

#define FONT3X5_WIDTH 3
#define FONT3X5_HEIGHT 6

#define CHAR_EXCLAMATION 33
#define CHAR_PER_CENT 37
#define CHAR_LEFT_BRACKET 40
#define CHAR_RIGHT_BRACKET 41
#define CHAR_COMMA 44
#define CHAR_DASH 45
#define CHAR_PERIOD 46
#define CHAR_NUMBER_0 48
#define CHAR_NUMBER_9 57
#define CHAR_COLON 58
#define CHAR_LETTER_A 65
#define CHAR_LETTER_Z 90
#define CHAR_LETTER_A_LOWER 97
#define CHAR_LETTER_Z_LOWER 122
#define CHAR_APOSTROPHE 39
#define CHAR_SLASH 47

#define FONT_EXCLAMATION_INDEX 36
#define FONT_PER_CENT_INDEX 39
#define FONT_APOSTROPHE_INDEX 41
#define FONT_LEFT_BRACKET 42
#define FONT_RIGHT_BRACKET 43
#define FONT_COMMA_INDEX 40
#define FONT_DASH_INDEX 44
#define FONT_PERIOD_INDEX 37
#define FONT_NUMBER_INDEX 26
#define FONT_COLON_INDEX 38
#define FONT_SLASH_INDEX 45

const uint8_t PROGMEM font_images[] = {
3, 8,
0x1F, 0x05, 0x1F, // A
0x1F, 0x15, 0x1B, 
0x1F, 0x11, 0x11, 
0x1F, 0x11, 0x0E, 
0x1F, 0x15, 0x11, 
0x1F, 0x05, 0x01, 
0x1F, 0x11, 0x1D, 
0x1F, 0x04, 0x1F, 
0x00, 0x1F, 0x00, 
0x10, 0x10, 0x1F, 
0x1F, 0x04, 0x1B, 
0x1F, 0x10, 0x10, 
0x1F, 0x06, 0x1F, 
0x1F, 0x01, 0x1F, 
0x1F, 0x11, 0x1F, 
0x1F, 0x05, 0x07, 
0x1F, 0x31, 0x1F, 
0x1F, 0x05, 0x1B,  
0x17, 0x15, 0x1D,  
0x01, 0x1F, 0x01,  
0x1F, 0x10, 0x1F,  
0x0F, 0x10, 0x0F,  
0x1F, 0x0C, 0x1F,  
0x1B, 0x04, 0x1B,  
0x07, 0x1C, 0x07,  
0x19, 0x15, 0x13, // Z
0x1F, 0x11, 0x1F, // 0  
0x00, 0x1F, 0x00,
0x1D, 0x15, 0x17,  
0x11, 0x15, 0x1F,  
0x07, 0x04, 0x1F,  
0x17, 0x15, 0x1D,  
0x1F, 0x15, 0x1D,  
0x01, 0x01, 0x1F,  
0x1F, 0x15, 0x1F,  
0x17, 0x15, 0x1F, // 9
0x00, 0x17, 0x00, // !  36
0X00, 0x10, 0x00, // .  37
0X00, 0x0A, 0x00, // :  38
0x19, 0x04, 0x13, // %  39
0X20, 0x10, 0x00, // ,  40
0X00, 0x03, 0x00, // '  41
0X00, 0x0E, 0x11, // (  42
0X11, 0x0E, 0x00, // )  43
0X04, 0x04, 0x04, // -  44
0X18, 0x04, 0x03, // /  45
};


Font3x5::Font3x5(uint8_t lineSpacing) {

    this->lineHeight = lineSpacing;
    this->letterSpacing = 1;

    this->cursorX = this->cursorY = this->baseX = 0;
    this->textColor = 1;

}

size_t Font3x5::write(uint8_t c) {

  if (c == '\n')      { this->cursorX = this->baseX; this->cursorY += this->lineHeight; }
  else {

    
    if (c == '~') {
       this->cursorX += 2;

    }
    else {

      printChar(c, this->cursorX, this->cursorY);
      this->cursorX += FONT3X5_WIDTH + this->letterSpacing;

    }

  }

  return 1;

}

void Font3x5::printChar(const char c, const int8_t x, int8_t y) {

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
            
        case CHAR_COMMA:
            idx = FONT_COMMA_INDEX;
            break;
            
        case CHAR_COLON:
            idx = FONT_COLON_INDEX;
            break;
            
        case CHAR_PER_CENT:
            idx = FONT_PER_CENT_INDEX;
            break;

        case CHAR_APOSTROPHE:
            idx = FONT_APOSTROPHE_INDEX;
            break;

        case CHAR_LEFT_BRACKET:
            idx = FONT_LEFT_BRACKET;
            break;

        case CHAR_RIGHT_BRACKET:
            idx = FONT_RIGHT_BRACKET;
            break;

        case CHAR_DASH:
            idx = FONT_DASH_INDEX;
            break;

        case CHAR_SLASH:
            idx = FONT_SLASH_INDEX;
            break;

    }

    if (idx > -1) {

        if (this->textColor == WHITE) {
            Sprites::drawSelfMasked(x, y, font_images, idx);
        }
        else {
            Sprites::drawErase(x, y, font_images, idx);
        }

    }

}

void Font3x5::setCursor(const int8_t x, const int8_t y) {
    this->cursorX = this->baseX = x;
    this->cursorY = y;
}

void Font3x5::setTextColor(const uint8_t color){
    this->textColor = color;
}

void Font3x5::setHeight(const uint8_t color){
    this->lineHeight = color;
}
