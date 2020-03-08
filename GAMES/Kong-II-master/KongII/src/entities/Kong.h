#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"
#include "../map/Coordinates.h"

class Kong {

    public:

        Kong();

        KongImage getImage();
        int8_t getXPosition();
        int8_t getYPosition(uint8_t yOffset);
        bool getChain(uint8_t index);
        bool getFlashChain(uint8_t index);
        bool getExit();
        bool getEnabled();

        bool isEnabled();
        void setChain(uint8_t index, bool value);
        void setFlashChain(uint8_t index, bool value);
        void setChains(bool value);
        void setExit(bool value);
        void setEnabled(bool value);

        bool updateChains();                    // Returns true if all 4 chains gone.
        bool updatePosition();                  // Returns tru if exit sequence is complete.
        bool getDisplayChain(uint8_t index);
        void reset();

    protected:

        bool chains[4] = { true, true, true, true };  
        bool flash[4] = { false, false, false, false };

        bool exit = false;
        bool enabled = false;
        uint8_t counter = 0;
        uint8_t position = 0;

};

