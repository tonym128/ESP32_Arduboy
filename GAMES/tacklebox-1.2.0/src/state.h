#pragma once

#include <Arduino.h>
//#include <EEPROM.h>
#include <ESP_EEPROM.h>
#include "fishType.h"
#include "fish.h"

const uint8_t GAME_ID = 90;
const uint8_t EEPROM_START = 64;

struct GameState {
    bool canChooseProMode;
    bool useProMode;
    uint16_t minute;
    uint8_t baitCounts[4];
    uint16_t money;
    bool acquiredFish[static_cast<int8_t>(FishType::COUNT)];
    int16_t currentFishCount[static_cast<int8_t>(FishType::COUNT)];
    int16_t bestLength[static_cast<int8_t>(FishType::COUNT)];
    bool hasProPole;
    bool hasOars;
    bool canBuyMeat;
    int8_t adviceLevel;
};

class State {
    public:
        static GameState gameState;

        static void setFishAcquired(FishType fishType);
        static void incrementCurrentCount(FishType fishType);
        static void decreaseCurrentCount(FishType fishType, uint8_t count);
        static void setFishLength(Fish& fish);
        static bool sellAllFish();

        static bool hasUserSaved();
        static void saveToEEPROM();
        static void clearEEPROM();
        static void load();

        static bool isDay();
        static uint8_t getCurrentHour();
};

