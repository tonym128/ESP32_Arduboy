#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"

class Base {

  public:

    Base();
 

    // Properties ..

    uint8_t getCounter();
    void setCounter(uint8_t counter);


  protected:

    uint8_t counter = 0;

};

