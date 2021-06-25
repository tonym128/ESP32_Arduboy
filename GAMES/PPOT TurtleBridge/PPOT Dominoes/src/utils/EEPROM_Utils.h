#pragma once

#include "Arduboy2Ext.h"
#include "Enums.h"

class EEPROM_Utils {

  public: 

    EEPROM_Utils(){};
        
    static void initEEPROM(bool forceClear);
    static void getName(char *name, uint16_t startLoc);
    static uint16_t getHighScore(uint16_t startLoc);
    static uint8_t saveScore(char *name);

};

