#include "state.h"
#include "fish.h"

GameState State::gameState = {};

bool State::hasUserSaved() {
    uint8_t id;
    EEPROM.get(EEPROM_START, id);

    return id == GAME_ID;
}

void State::saveToEEPROM() {
    EEPROM.write(EEPROM_START, GAME_ID);
    EEPROM.put(EEPROM_START + 1, gameState);
    EEPROM.commit();
}

void State::load() {
    if (hasUserSaved()) {
        EEPROM.get(EEPROM_START + 1, gameState);
    } else {
        gameState.canChooseProMode = false;
        gameState.useProMode = false;

        // start the game at 4pm
        gameState.minute = 16 * 60;
        gameState.baitCounts[0] = 0;
        gameState.baitCounts[1] = 0;
        gameState.baitCounts[2] = 0;
        gameState.baitCounts[3] = 0;
        gameState.money = 10;
        gameState.hasProPole = false;
        gameState.hasOars = false;
        gameState.canBuyMeat = false;
        gameState.adviceLevel = 0;

        for (int8_t i = 0; i < static_cast<int8_t>(FishType::COUNT); ++i) {
            gameState.acquiredFish[i] = false;
            gameState.currentFishCount[i] = 0;
            gameState.bestLength[i] = 0;
        }
    }
}

void State::setFishAcquired(FishType fishType) {
    gameState.acquiredFish[static_cast<int8_t>(fishType)] = true;
}

void State::incrementCurrentCount(FishType fishType) {
    gameState.currentFishCount[static_cast<int8_t>(fishType)] += 1;
}

void State::decreaseCurrentCount(FishType fishType, uint8_t count) {
    gameState.currentFishCount[static_cast<int8_t>(fishType)] -= count;
}

void State::setFishLength(Fish& fish) {
    int8_t index = static_cast<int8_t>(fish.type);
    int16_t currentLength = gameState.bestLength[index];
    gameState.bestLength[index] = max(currentLength, fish.length);
}

bool State::sellAllFish() {
    Fish fish;
    bool soldSomething = false;

    for (uint8_t f = 0; f < static_cast<uint8_t>(FishType::OLD_BOOT); ++f) {
        Fish::loadFish(static_cast<FishType>(f), fish);

        soldSomething = soldSomething || gameState.currentFishCount[f] > 0;

        gameState.money += gameState.currentFishCount[f] * fish.value;
        gameState.currentFishCount[f] = 0;
    }

    return soldSomething;
}

void State::clearEEPROM() {
    EEPROM.put(EEPROM_START, 0);
    EEPROM.commit();
    load();
}

bool State::isDay() {
    uint8_t hour = getCurrentHour();

    return hour >= 5 && hour < 21;
}

uint8_t State::getCurrentHour() {
    return gameState.minute / 60;
}

