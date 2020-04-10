#pragma once

#include "Arduboy2Ext.h"
#include "Enums.h"

namespace EEPROM_Utils {

//  constexpr size_t eepromStart = 713;
  constexpr size_t eepromStart = 20;
  constexpr size_t eepromStart_C1 = (eepromStart + 0);
  constexpr size_t eepromStart_C2 = (eepromStart + 1);
  constexpr size_t eepromEasy = (eepromStart_C2 + 1);
  constexpr size_t eepromHard = (eepromEasy + 2);

  void initialiseEEPROM();
  void clearEEPROM();

  void saveScore(GameMode gameMode, uint16_t score);
  uint16_t readSaveScore(GameMode gameMode);

};

