#pragma once

#include "Arduboy2Ext.h"
#include "SaveEntry.h"
#include "Enums.h"

namespace EEPROM_Utils {

  constexpr size_t eepromStart = 171;
  constexpr size_t eepromStart_C1 = (eepromStart + 0);
  constexpr size_t eepromStart_C2 = (eepromStart + 1);
  constexpr size_t saveEntriesStart = (eepromStart + 2);
  constexpr size_t saveEntriesCount = 3;
  constexpr size_t saveEntriesEasyStart = (saveEntriesStart + (saveEntriesCount * sizeof(SaveEntry) * 0));
  constexpr size_t saveEntriesHardStart = (saveEntriesStart + (saveEntriesCount * sizeof(SaveEntry) * 1));
  constexpr size_t saveEntriesTotal = (saveEntriesCount * 2);


  void initialiseEEPROM();
  void clearEEPROM();

  uint8_t saveScore(GameMode gameMode, uint16_t score);
  void saveChar(GameMode gameMode, uint8_t saveIndex, uint8_t charIdx, char newChar);
  
  void readSaveEntry(GameMode gameMode, SaveEntry & entry, uint8_t saveIndex);
  void readSaveEntryName(GameMode gameMode, char (&name)[SaveEntry::nameSize], uint8_t saveIndex);
  uint16_t readSaveEntryScore(GameMode gameMode, uint8_t saveIndex);
  
  void writeSaveEntry(GameMode gameMode, const SaveEntry & entry, uint8_t saveIndex);
  void writeSaveEntryName(GameMode gameMode, const char (&name)[SaveEntry::nameSize], uint8_t saveIndex);
  void writeSaveEntryScore(GameMode gameMode, uint16_t score, uint8_t saveIndex);
  
  template< size_t size >
  void readSaveEntries(GameMode gameMode, SaveEntry (&entries)[size]) {
    constexpr size_t count = (size <= saveEntriesCount) ? size : saveEntriesCount;
    for(size_t index = 0; index < count; ++index)
       readSaveEntry(gameMode, entries[index], index);
  }
  
};

