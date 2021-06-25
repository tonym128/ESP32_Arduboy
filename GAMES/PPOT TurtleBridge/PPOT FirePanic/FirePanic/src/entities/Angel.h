#pragma once

#include "../Utils/Arduboy2Ext.h"

class Angel {

  public:

    Angel();
     
    uint8_t getX();
    uint8_t getY();
    uint8_t getEnabled();
    uint8_t getImageIndex();

    void setEnabled(bool value);
    void setSequence(uint8_t sequence);
    void init(uint8_t sequence, uint8_t missNumber);
    bool move();
    bool renderImage();
    
  protected:

    uint8_t x;
    uint8_t y;
    bool enabled;
    uint8_t sequence;
    uint8_t sequenceLen;
    int8_t posIndex;

};

