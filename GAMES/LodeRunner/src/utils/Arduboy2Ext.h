#pragma once

#include <Arduboy2.h>

class Arduboy2Ext : public Arduboy2Base {

  public:

    Arduboy2Ext();

    uint8_t justPressedButtons() const;
    uint8_t pressedButtons() const;

    void clearButtonState();
    void drawCompressedMirror(int16_t sx, int16_t sy, const uint8_t *bitmap, uint8_t color, bool mirror);
    void drawHorizontalDottedLine(uint8_t x1, uint8_t x2, uint8_t y);
    void drawVerticalDottedLine(uint8_t y1, uint8_t y2, uint8_t x);
    
};
