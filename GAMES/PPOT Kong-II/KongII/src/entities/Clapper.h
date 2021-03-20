#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"

class Clapper {

    public:

        Clapper();

        // Properties ..

        int16_t getXPosition();
        int8_t getYPosition(uint8_t yOffset);
        uint8_t getImage();
        bool isEnabled();

        void setEnabled(bool enabled);
        void setY(uint8_t y);


        // Methods

        void updatePosition();
        Rect getRect(uint8_t yOffset, GameMode mode);

    protected:

        bool enabled = false;
        int16_t x;
        uint8_t y;

};

