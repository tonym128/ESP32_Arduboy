#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"
#include "Bird.h"

class Birds {

    public:

        Birds();

        Bird & getBird(uint8_t index);

        void updatePositions();
        void setDelayMax(uint16_t delayMax, bool updateDelay);
        void launchBird();
        void reset();

    protected:

        Bird birds[4];
        uint16_t delay;
        uint16_t delayMax;

};

