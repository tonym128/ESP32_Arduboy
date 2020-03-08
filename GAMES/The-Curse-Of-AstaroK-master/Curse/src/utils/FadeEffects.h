#pragma once

#include "Arduboy2Ext.h"

class FadeInEffect {

  private:
    uint8_t fadeWidth = WIDTH;
    uint8_t fadeSpeed = 1;
    
  public:	

    void reset(uint8_t speed) {
      this->fadeWidth = WIDTH;
      this->fadeSpeed = speed;
    }
    
    bool isComplete(void) const {
      return (this->fadeWidth == 0);
    }

    void update(void) {
      if(this->fadeWidth > 0)
        this->fadeWidth = this->fadeWidth - this->fadeSpeed;
    }

    void draw(Arduboy2Ext & arduboy) const {

      for(uint8_t i = 0; i < (HEIGHT / 2); ++i) {

        arduboy.drawFastHLine(0, (i * 2), this->fadeWidth, BLACK);
        arduboy.drawFastHLine((WIDTH - this->fadeWidth), (i * 2) + 1, this->fadeWidth, BLACK);

      }

    }

};
