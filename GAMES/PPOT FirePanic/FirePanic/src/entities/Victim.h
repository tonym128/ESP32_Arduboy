#pragma once

#include "../Utils/Arduboy2Ext.h"

class Victim {

  public:

    Victim();
     
    uint8_t getX();
    uint8_t getY();
    uint8_t getPosIndex();
    uint8_t getRotation();
    uint8_t getEnabled();
    uint8_t getAlive();
    uint8_t getHaloIndex();
    uint8_t getPuffIndex();
    bool getPrevBounce();

    void setStartingLevel(bool value);
    void setEnabled(bool value);
    void setAlive(uint8_t value);
    void setPrevBounce(bool value);

    void move();
    void rotate();
    void init(uint8_t level);
    void incPuffIndex();
    
  protected:

    uint8_t x;
    uint8_t y;
    uint8_t startingLevel;
    uint8_t posIndex;
    uint8_t rotIndex;
    uint8_t puffIndex;
    uint8_t alive;

    bool enabled;
    bool prevBounce;  // Bounced on tramp one position before lowest?

};

