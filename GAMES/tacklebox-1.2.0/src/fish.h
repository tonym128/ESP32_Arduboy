#pragma once

#include <Arduino.h>
#include "fishType.h"
#include "baitType.h"

class Fish {
    public:
        FishType type;
        uint8_t minHour;
        uint8_t maxHour;
        uint8_t baitPreferences[static_cast<uint8_t>(BaitType::COUNT)];
        uint8_t ratio;
        uint8_t value;
        uint8_t pull;
        bool deepWater;
        uint8_t proWindow;
        
        int16_t minX;
        int16_t maxX;
        int16_t minLength;
        int16_t maxLength;
        int16_t length;

        const uint8_t* nameString;
        const uint8_t* bmp;

        uint8_t nameLength;
        uint8_t bmpWidth;

        void rollForLength();

        static void loadFish(FishType fishType, Fish& fish);
};

