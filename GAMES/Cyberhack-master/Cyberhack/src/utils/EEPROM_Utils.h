#pragma once

#include "Arduboy2Ext.h"
#include "../entities/Player.h"

#define EEPROM_START                  EEPROM_STORAGE_SPACE_START + 657
#define EEPROM_START_C1               EEPROM_START
#define EEPROM_START_C2               EEPROM_START + 1
#define EEPROM_START_GAME_SAVED       EEPROM_START + 2
#define EEPROM_START_DATA             EEPROM_START + 3

class EEPROM_Utils {

  public: 

    EEPROM_Utils(){};
        
    static void initEEPROM(bool force);
    static bool isGameSaved();
    static void loadGame(Player &player);
    static void saveGame(Player &player);

};


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters 'C' and 'H' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_START.  
 */

const uint8_t letter1 = 'C'; 
const uint8_t letter2 = 'H'; 

void EEPROM_Utils::initEEPROM(bool force) {

    byte c1 = EEPROM.read(EEPROM_START_C1);
    byte c2 = EEPROM.read(EEPROM_START_C2);

    if (c1 != letter1 || c2 != letter2 || force) { 

        uint8_t gameSaved = 0;

        EEPROM.put(EEPROM_START_C1, letter1);
        EEPROM.put(EEPROM_START_C2, letter2);
        EEPROM.put(EEPROM_START_GAME_SAVED, gameSaved);

    }

}


/* -----------------------------------------------------------------------------
 *   Has the game been saved? 
 */
bool EEPROM_Utils::isGameSaved() {

    uint8_t gameSaved = 0;
    EEPROM.get(EEPROM_START_GAME_SAVED, gameSaved);
    return gameSaved == 1;

}


/* -----------------------------------------------------------------------------
 *   Save game ..
 */
void EEPROM_Utils::saveGame(Player &player) {

    EEPROM.put(EEPROM_START_GAME_SAVED, 1);
    EEPROM.put(EEPROM_START_DATA, player);

}


/* -----------------------------------------------------------------------------
 *   Save game ..
 */
void EEPROM_Utils::loadGame(Player &player) {

    EEPROM.get(EEPROM_START_DATA, player);

}