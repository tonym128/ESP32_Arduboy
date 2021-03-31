#pragma once

#include "Arduboy2Ext.h"
#include "consts.h"

#define EEPROM_START                  EEPROM_STORAGE_SPACE_START + 190
#define EEPROM_START_C1               EEPROM_START
#define EEPROM_START_C2               EEPROM_START + 1
#define EEPROM_EASY                   EEPROM_START_C2 + 1
#define EEPROM_HARD                   EEPROM_EASY + 2

class EEPROM_Utils {

  public: 

    EEPROM_Utils(){};
        
    static void initEEPROM(bool force);
    static uint16_t getScore(GameMode mode);
    static void saveScore(GameMode mode, uint16_t score);
    static bool toggleSoundSettings(Arduboy2Ext &arduboy);

};


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters 'L' and 'T' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
 *   it resets the settings ..
 */

const uint8_t letter1 = 'L'; 
const uint8_t letter2 = 'T'; 

void EEPROM_Utils::initEEPROM(bool force) {

    byte c1 = EEPROM.read(EEPROM_START_C1);
    byte c2 = EEPROM.read(EEPROM_START_C2);

    if (c1 != letter1 || c2 != letter2 || force) { 

        uint16_t score = 0;

        EEPROM.put(EEPROM_START_C1, letter1);
        EEPROM.put(EEPROM_START_C2, letter2);
        EEPROM.put(EEPROM_EASY, score);
        EEPROM.put(EEPROM_HARD, score);
        EEPROM.commit();

    }

}


/* -----------------------------------------------------------------------------
 *   Get slot details. 
 */
uint16_t EEPROM_Utils::getScore(GameMode mode) {

    uint16_t score = 0;
    EEPROM.get(mode == GameMode::Easy ? EEPROM_EASY : EEPROM_HARD, score);
    return score;

}


/* -----------------------------------------------------------------------------
 *   Save score ..
 */
void EEPROM_Utils::saveScore(GameMode mode, uint16_t score) {

    uint16_t oldScore = 0;
    EEPROM.get(mode == GameMode::Easy ? EEPROM_EASY : EEPROM_HARD, oldScore);

    if (oldScore < score) {

        EEPROM.put(mode == GameMode::Easy ? EEPROM_EASY : EEPROM_HARD, score);
        EEPROM.commit();
    }

}


/* ----------------------------------------------------------------------------
 *  Toggle the sound setting and commit to the EEPROM.
 */
bool EEPROM_Utils::toggleSoundSettings(Arduboy2Ext &arduboy) {

    if (arduboy.audio.enabled()) {

        arduboy.audio.off(); 
        arduboy.audio.saveOnOff();
        return false;

    }
    else {

        arduboy.audio.on(); 
        arduboy.audio.saveOnOff();
        return true;

    }
    
}