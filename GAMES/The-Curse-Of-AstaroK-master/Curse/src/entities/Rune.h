#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../ardBitmap/ArdBitmap.h"


class Rune { 
    
  public:

    Rune();

    bool getReroll();
    void setReroll(const bool value);
    void alternateReroll();

    void startSpinning();
    void update(Arduboy2Ext &arduboy);
    bool isRotating();
    bool isRotatingOrRerolling();

    uint8_t getValue();
    uint8_t getFrame();
    void setValue(uint8_t value);
    
  private:

  private:

    uint8_t index;
    uint8_t frame;
    uint8_t value;
    bool reroll;

};
