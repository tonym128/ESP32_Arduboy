#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"
#include "../map/Coordinates.h"

class Bird {

    public:

        Bird();

        // Properties ..

        uint16_t getPosition();
        int8_t getXPosition();
        int8_t getYPosition(uint8_t yOffset);
        uint8_t getImage();
        bool isEnabled();

        void setPosition(uint16_t position);
        void setEnabled(bool enabled);


        // Methods

        void updatePosition();
        Rect getRect(uint8_t yOffset, GameMode mode);

    protected:

        bool enabled = false;
        uint16_t position = 0;
        uint8_t counter = 0;

};

