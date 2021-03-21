#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"
#include "Base.h"

class Plate : public Base {

  public:

    Plate();
 
    // Properties ..

    uint8_t getPlateNumber();
    int8_t getXOffset();
    uint8_t getYOffset();
    uint8_t getImage();

    void setPlateNumber(uint8_t plateNumber);

    // Methods ..

    void incCounter();

  protected:

    uint8_t plateNumber = 0;

};

