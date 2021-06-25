#pragma once

#include "../Utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"

class OtherCar {

  public:

    OtherCar();
     
    int16_t getX();
    uint8_t getLane();
    uint8_t getSpeed();
    uint8_t getType();
    bool getEnabled();
    bool getDoNotMove();

    void setX(int16_t value);
    void setLane(uint8_t value);
    void setSpeed(uint8_t value);
    void setType(uint8_t value);
    void setEnabled(bool value);
    void setDoNotMove(bool value);
    
  protected:

    int16_t x;
    uint8_t lane;
    uint8_t type;
    uint8_t speed;

    bool enabled;
    bool doNotMove = false;  

};



