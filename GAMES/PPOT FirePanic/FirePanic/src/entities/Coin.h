#pragma once

#include "../Utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"

class Coin {

  public:

    Coin();
     
    int16_t getX();
    uint8_t getCountdown();
    uint8_t getLane();
    bool getEnabled();

    void setX(int16_t value);
    void setLane(uint8_t value);
    void setEnabled(bool vlaue);
    void decCountdown();

    void move();
    
  protected:

    int16_t x;
    uint8_t lane;
    uint8_t countdown;
    bool enabled;

};

