#pragma once

#include "Arduboy2Ext.h"

class FadeInEffect {

  private:
    uint8_t fadeWidth = WIDTH;
    
  public:	

    void reset(void) {
      this->fadeWidth = WIDTH;
    }
    
    bool isComplete(void) const {
      return (this->fadeWidth == 0);
    }

    void update(void) {
      if(this->fadeWidth > 0)
        --this->fadeWidth;
    }

    void draw(Arduboy2Ext & arduboy) const {

      for(uint8_t i = 0; i < (HEIGHT / 2); ++i) {

        arduboy.drawFastHLine(0, (i * 2), this->fadeWidth, BLACK);
        arduboy.drawFastHLine((WIDTH - this->fadeWidth), (i * 2) + 1, this->fadeWidth, BLACK);

      }

    }

};

class FadeOutEffect {

  private:

    uint8_t fadeWidth = 0;
    
  public:

    void reset(void) {
      this->fadeWidth = 0;
    }
    
    bool isComplete(void) const {
      return (this->fadeWidth >= WIDTH);
    }

    void update(void) {
      if(this->fadeWidth >= WIDTH)
        ++this->fadeWidth;
    }

    void draw(Arduboy2Ext & arduboy) const {

      for(uint8_t i = 0; i < (HEIGHT / 2); ++i) {

        arduboy.drawFastHLine(0, (i * 2), this->fadeWidth, BLACK);
        arduboy.drawFastHLine((WIDTH - this->fadeWidth), (i * 2) + 1, this->fadeWidth, BLACK);

      }

    }
    
};

class FadeEffect {

  private:

    uint8_t fadeWidth = 0;
    
  public:

    void prepareFadeOut(void) {
      this->fadeWidth = 0;
    }
    
    void prepareFadeIn(void) {
      this->fadeWidth = WIDTH;
    }
    
    bool isFadeOutComplete(void) const {
      return (this->fadeWidth >= WIDTH);
    }
    
    bool isFadeInComplete(void) const {
      return (this->fadeWidth == 0);
    }

    void updateFadeOut(void) {
      if(this->fadeWidth >= WIDTH)
        ++this->fadeWidth;
    }

    void updateFadeIn(void)	{
      if(this->fadeWidth > 0)
        --this->fadeWidth;
    }

    void draw(Arduboy2Ext & arduboy) const {

      for (uint8_t i = 0; i < (HEIGHT / 2); ++i) {
        arduboy.drawFastHLine(0, (i * 2), this->fadeWidth, BLACK);
        arduboy.drawFastHLine((WIDTH - this->fadeWidth), (i * 2) + 1, this->fadeWidth, BLACK);
      }

    }

};