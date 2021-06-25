#pragma once

#include "../Utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"

class Fish {

  public:

    Fish();


    // Properties ..

    bool getEnabled();
    uint8_t getFishIndex();
    uint8_t getImageIndex();
    uint8_t getDelay();
    bool isEdible();

    void setEnabled(bool enabled);
    void setFishIndex(uint8_t value);
    void setImageIndex(uint8_t value);
    void setDelay(uint8_t value);


    // Methods ..

    uint8_t getDisplayX();
    uint8_t getDisplayY();
    void updateImageIndex();
    void updatePositionIndex();

  protected:

    bool enabled;
    bool edible;
    uint8_t fishIndex;
    uint8_t imageIndex;
    uint8_t positionIndex;
    uint8_t counter;
    uint8_t delayValue;
    uint8_t delayValueInit;

};

