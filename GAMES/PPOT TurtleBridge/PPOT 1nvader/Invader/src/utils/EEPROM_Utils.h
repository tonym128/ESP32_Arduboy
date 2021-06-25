#pragma once

#include "Arduboy2Ext.h"

#define EEPROM_START                EEPROM_STORAGE_SPACE_START + 143
#define EEPROM_START_C1             EEPROM_START
#define EEPROM_START_C2             EEPROM_START + 1
#define EEPROM_SINGLE               EEPROM_START + 2
#define EEPROM_DOUBLE               EEPROM_START + 4
#define EEPROM_TUG_OF_WAR           EEPROM_START + 6
#define EEPROM_MODE                 EEPROM_START + 8
#define EEPROM_ROTATION             EEPROM_START + 9
#define EEPROM_LOCK_LVL_0           EEPROM_START + 10
#define EEPROM_LOCK_LVL_1           EEPROM_START + 11

class EEPROM_Utils {

    public: 

        EEPROM_Utils(){};
            
        static void initEEPROM(bool force);
        static uint16_t getScore(GameMode mode);
        static void saveScore(GameMode mode, uint16_t score);
        static GameMode getMode();
        static void saveMode(GameMode mode);
        static GameRotation getRotation();
        static void saveRotation(GameRotation rotation);
        static uint8_t getLevel(uint8_t level);
        static void saveLevel(uint8_t level, uint8_t value);

};


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters 'I' and 'H' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_START.  
 */

const uint8_t letter1 = 'I'; 
const uint8_t letter2 = 'H'; 

void EEPROM_Utils::initEEPROM(bool force) {

    byte c1 = EEPROM.read(EEPROM_START_C1);
    byte c2 = EEPROM.read(EEPROM_START_C2);

    if (c1 != letter1 || c2 != letter2 || force) { 

        uint16_t hs = 0;
        uint8_t mode = static_cast<uint8_t>(GameMode::Single);
        uint8_t rotation = static_cast<uint8_t>(GameRotation::Portrait);

        EEPROM.put(EEPROM_START_C1, letter1);
        EEPROM.put(EEPROM_START_C2, letter2);
        EEPROM.put(EEPROM_SINGLE, hs);
        EEPROM.put(EEPROM_DOUBLE, hs);
        EEPROM.put(EEPROM_TUG_OF_WAR, hs);
        EEPROM.put(EEPROM_MODE, mode);
        EEPROM.put(EEPROM_ROTATION, rotation);
        EEPROM.put(EEPROM_LOCK_LVL_0, 0);
        EEPROM.put(EEPROM_LOCK_LVL_1, 0);
        EEPROM.commit();

    }

}

/* -----------------------------------------------------------------------------
 *   Get score. 
 */
uint16_t EEPROM_Utils::getScore(GameMode mode) {

    uint16_t score = 0;
    EEPROM.get(EEPROM_SINGLE + (static_cast<uint8_t>(mode) * 2), score);
    return score;

}


/* -----------------------------------------------------------------------------
 *   Save score ..
 */
void EEPROM_Utils::saveScore(GameMode mode, uint16_t score) {

    uint16_t oldScore = 0;
    EEPROM.get(EEPROM_SINGLE + (static_cast<uint8_t>(mode) * 2), oldScore);

    if (oldScore < score) {

        EEPROM.write(EEPROM_SINGLE + (static_cast<uint8_t>(mode) * 2), score);
        EEPROM.commit();

    }

}


/* -----------------------------------------------------------------------------
 *   Get mode. 
 */
GameMode EEPROM_Utils::getMode() {

    uint8_t mode = 0;
    EEPROM.get(EEPROM_MODE, mode);
    return static_cast<GameMode>(mode);

}


/* -----------------------------------------------------------------------------
 *   Save mode ..
 */
void EEPROM_Utils::saveMode(GameMode mode) {

    EEPROM.write(EEPROM_MODE, static_cast<uint8_t>(mode));
    EEPROM.commit();
}


/* -----------------------------------------------------------------------------
 *   Get rotation. 
 */
GameRotation EEPROM_Utils::getRotation() {

    uint8_t rotation = 0;
    EEPROM.get(EEPROM_ROTATION, rotation);
    return static_cast<GameRotation>(rotation);

}


/* -----------------------------------------------------------------------------
 *   Save rotation ..
 */
void EEPROM_Utils::saveRotation(GameRotation rotation) {

    EEPROM.write(EEPROM_ROTATION, static_cast<uint8_t>(rotation));
    EEPROM.commit();

}

/* -----------------------------------------------------------------------------
 *   Get level. 
 */
uint8_t EEPROM_Utils::getLevel(uint8_t level) {

    uint8_t rotation = 0;
    EEPROM.get(EEPROM_LOCK_LVL_0 + level, rotation);
    return rotation;

}


/* -----------------------------------------------------------------------------
 *   Save level ..
 */
void EEPROM_Utils::saveLevel(uint8_t level, uint8_t value) {

    EEPROM.write(EEPROM_LOCK_LVL_0 + level, value);
    EEPROM.commit();

}
