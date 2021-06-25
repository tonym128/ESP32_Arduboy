#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"
#include "../map/Coordinates.h"

class Key {

    public:

        Key();

        // Properties ..

        uint8_t getPosition();
        int8_t getXPosition();
        int8_t getYPosition(uint8_t yOffset);
        uint8_t getImage();
        KeyLocation getKeyLocation();
        bool isEnabled();

        void setEnabled(bool enabled);
        void setPath(uint8_t const *path);
        void setKeyLocation(KeyLocation keyLocation);
        void setFlash(bool value);


        // Methods

        bool getDisplay();
        void updatePosition();
        Rect getRect(uint8_t yOffset);

    protected:

        bool enabled = false;
        bool flash = false;
        uint8_t position = 0;
        uint8_t counter = 0;
        uint8_t const *path = nullptr; 
        KeyLocation keyLocation = KeyLocation::LowerPosition;

};

