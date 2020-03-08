#pragma once

#include <Arduino.h>
#include "../utils/Constants.h"

class Font3x6 {
    
  public:

    Font3x6(uint8_t lineHeight = 9);   

    void print(char c);
    void print(char *str);
    void print(const __FlashStringHelper *); // For using F() Macro
    void PrintMessage(uint8_t idx);
    void printMessage(uint8_t idx, const uint8_t *messagesArray, bool eighty);
    void printNumber(uint8_t n);
    void printNumberln(uint8_t n);
    void printMessage(uint8_t n);
    
    void write(uint8_t c);
    
    void printChar(const char c, const int8_t x, int8_t y);
    void setCursor(const int8_t x, const int8_t y);

    void setTextColor(const uint8_t color);
    void setHeight(const uint8_t lineHeight);
    

  private:

    int8_t _cursorX;    // Default is 0.
    int8_t _baseX;      // needed for linebreak.
    int8_t _cursorY;    // Default is 0.

    int8_t _textColor;  // BLACK == 0, everything else is WHITE. Default is WHITE.

    uint8_t _letterSpacing;  // letterSpacing controls the distance between letters. Default is 1.
    uint8_t _lineHeight;     // lineHeight controls the height between lines breakend by \n. Default is 8.

};
