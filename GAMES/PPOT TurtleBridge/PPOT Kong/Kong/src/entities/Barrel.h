#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"
#include "../map/Coordinates.h"

class Barrel {

  public:

    Barrel();

 
    // Properties ..
    uint8_t getPosition();
    uint8_t getRotation();
    uint8_t getAisle();
    uint8_t getEnabledCountdown();
    bool isEnabled();

    void setPosition(uint8_t position);
    void setAisle(uint8_t aisle);
    void setRotation(uint8_t rotation);
    void setEnabledCountdown(uint8_t enabledCountdown);
    void setEnabled(bool enabled);


    // Methods ..

    int8_t getXPosition();
    int8_t getYPosition(uint8_t yOffset);
    void updatePosition();
    void rotate();
    void launch(uint8_t startingPosition);
    Rect getRect(uint8_t yOffset, GameMode mode);
    void decEnabledCountdown();
    bool isEnabledOrPending();

  protected:

    uint8_t position;
    uint8_t aisle;
    uint8_t rotation;
    bool enabled;
    uint8_t enabledCountdown;

    Coordinates::BarrelData barrelData;

};

