#pragma once

#include "Arduboy2Ext.h"
#include "Enums.h"
#include "SaveEntry.h"

namespace EEPROM_Utils {

  void initialiseEEPROM();
  void clearEEPROM();

  uint8_t saveScore(uint16_t score);
  void saveChar(uint8_t saveIndex, uint8_t charIdx, char newChar);
  
  void readSaveEntry(SaveEntry &entry, uint8_t saveIndex);
  void readSaveEntryName(char (&name), uint8_t saveIndex);
  uint16_t readSaveEntryScore(uint8_t saveIndex);
  
  void writeSaveEntry(const SaveEntry &entry, uint8_t saveIndex);
  void writeSaveEntryName(const char (&name), uint8_t saveIndex);
  void writeSaveEntryScore(uint16_t score, uint8_t saveIndex);
  
  template< size_t size >
  void readSaveEntries(SaveEntry (&entries)[size])
  {
    constexpr size_t count = (size <= eepromSaveEntriesCount) ? size : eepromSaveEntriesCount;
    for(size_t index = 0; index < count; ++index)
      readSaveEntry(entries[index], index);
  }
  
};

