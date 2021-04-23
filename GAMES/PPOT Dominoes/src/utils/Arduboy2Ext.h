#ifndef ARDUBOY2EXT_H
#define ARDUBOY2EXT_H

#include <Arduboy2.h>

#define DEFAULT_VERT_SPACING 12 

class Arduboy2Ext : public Arduboy2Base {
  
  public:
  
    Arduboy2Ext();

    uint8_t justPressedButtons() const;
    uint8_t pressedButtons() const;
    uint16_t getFrameCount() const;
    uint8_t getFrameCount(uint8_t mod) const;
    bool getFrameCountHalf(uint8_t mod) const;
    bool isFrameCount(uint8_t mod) const;
    bool isFrameCount(uint8_t mod, uint8_t val) const;
    
    void clearButtonState();
    void resetFrameCount();
    
    void drawHorizontalDottedLine(uint8_t x1, uint8_t x2, uint8_t y, uint8_t colour = WHITE);
    void drawVerticalDottedLine(uint8_t y1, uint8_t y2, uint8_t x, uint8_t colour = WHITE);

};
#endif
