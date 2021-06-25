#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"
#include "../map/Coordinates.h"

class Girder {

  public:

    Girder();
 
    // Properties ..

    uint8_t getPosition();
    int8_t getXPosition();
    int8_t getYPosition(uint8_t yOffset);
    uint8_t getImage();
    bool isEnabled();

    void setPosition(uint8_t position);
    void setEnabled(bool enabled);


    // Methods

    void updatePosition();
    Rect getRect(uint8_t yOffset);
    
  protected:

    bool enabled = false;
    uint8_t position = 0;
    uint8_t counter = 0;
    Coordinates::GirderData girderData;

};

