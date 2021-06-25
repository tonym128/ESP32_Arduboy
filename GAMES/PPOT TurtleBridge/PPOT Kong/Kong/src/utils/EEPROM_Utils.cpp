#include "EEPROM_Utils.h"
#include "Arduboy2Ext.h"


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters 'F' and 'P' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
 *   it resets the settings ..
 */

uint8_t letter1 = 'D';
uint8_t letter2 = 'K';



namespace EEPROM_Utils {


  void initialiseEEPROM() {
    uint8_t c1, c2; 
    EEPROM.get(eepromStart_C1, c1);
    EEPROM.get(eepromStart_C2, c2);
    if ((c1 != letter1) || (c2 != letter2))
      clearEEPROM();
}


  void clearEEPROM() {
    EEPROM.put(eepromStart_C1, letter1);
    EEPROM.put(eepromStart_C2, letter2);    
    for(uint8_t i = 0; i < saveEntriesCount; i++) {
      SaveEntry entry { 0, "AAA" };
      EEPROM.put(sizeof(SaveEntry)*i+saveEntriesEasyStart, entry);
    }
    
    EEPROM.put(eepromStart_C1, letter1);
    EEPROM.put(eepromStart_C2, letter2);    
    for(uint8_t i = 0; i < saveEntriesCount; i++) {
      SaveEntry entry { 0, "BBB" };
      EEPROM.put(sizeof(SaveEntry)*i+saveEntriesHardStart, entry);
    }
    
    EEPROM.commit();
  }



  uint8_t findScore(uint16_t newScore) {
    for (uint8_t i = 0; i < saveEntriesCount; i++) {
      uint16_t oldScore;
      EEPROM.get(saveEntriesStart+i*sizeof(SaveEntry),oldScore);
      if (newScore >= oldScore) {
        return i;
      }
    }
    return NO_WINNER;
  }




  /* -----------------------------------------------------------------------------
   *   Save score if it is in the top 3, return slot number (or Constants::no_Winner) ..
   */
  uint8_t saveScore(GameMode gameMode, uint16_t newScore) {
  
    uint16_t saveEntriesStartSelect = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    
    uint8_t targetIndex = findScore(newScore);
    if(targetIndex == NO_WINNER) {
      return NO_WINNER;}

    for (uint8_t i = (saveEntriesCount - 1); i > targetIndex; i--) {
      SaveEntry entry;
      EEPROM.get(saveEntriesStartSelect+(i-1)*sizeof(SaveEntry), entry);
      EEPROM.put(saveEntriesStartSelect+i*sizeof(SaveEntry), entry);
    }

    // Save new name and score ..
    SaveEntry entry { newScore, { '?', '?', '?', '\0' } };
    EEPROM.put(targetIndex*sizeof(SaveEntry)+saveEntriesStartSelect,entry);
    
    EEPROM.commit();
    
    return targetIndex;
  }



  void saveChar(GameMode gameMode, uint8_t saveIndex, uint8_t charIndex, char newChar) {
    uint16_t saveEntriesStartSelect = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    EEPROM.write(sizeof(SaveEntry)*saveIndex+saveEntriesStartSelect+sizeof (uint16_t)+charIndex, newChar);
    EEPROM.commit();
}



  void readSaveEntry(GameMode gameMode, SaveEntry &entry, uint8_t saveIndex) {
    uint16_t saveEntriesStartSelect = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    EEPROM.get(sizeof(SaveEntry)*saveIndex+saveEntriesStartSelect,entry);
}



  void readSaveEntryName(GameMode gameMode, char (&name), uint8_t saveIndex) {
    uint16_t saveEntriesStartSelect = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    char nme[sizeof(SaveEntry::name)];
    for (uint8_t i=0; i< sizeof(SaveEntry::name); i++)
     EEPROM.get(sizeof(SaveEntry)*saveIndex+saveEntriesStartSelect+i+sizeof(uint16_t), nme[i]);
    memcpy(&name, &nme[0], sizeof(SaveEntry::name));
  }

  
  
  uint16_t readSaveEntryScore(GameMode gameMode, uint8_t saveIndex) {
    uint16_t saveEntriesStartSelect = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    uint16_t scr;
    EEPROM.get(sizeof(SaveEntry)*saveIndex+saveEntriesStartSelect,scr);
    return (scr);
  }



  void writeSaveEntry(GameMode gameMode, SaveEntry &entry, uint8_t saveIndex) {
    uint16_t saveEntriesStartSelect = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    EEPROM.put(sizeof(SaveEntry)*saveIndex+saveEntriesStartSelect, entry);
    EEPROM.commit();
  }



  void writeSaveEntryName(GameMode gameMode, char (&name), uint8_t saveIndex) {
    uint16_t saveEntriesStartSelect = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    char nme[sizeof(SaveEntry::name)];
    memcpy(&nme[0], &name, sizeof(SaveEntry::name));
    for (uint8_t i=0; i< sizeof(SaveEntry::name); i++)
      EEPROM.write(sizeof(SaveEntry)*saveIndex+saveEntriesStartSelect+i+sizeof(uint16_t),(uint8_t)nme[i]);
    EEPROM.commit();
  }



  void writeSaveEntryScore(GameMode gameMode, uint16_t score, uint8_t saveIndex) {
    uint16_t saveEntriesStartSelect = (gameMode == GameMode::Easy) ? saveEntriesEasyStart : saveEntriesHardStart;
    EEPROM.put(sizeof(SaveEntry)*saveIndex+saveEntriesStartSelect, score);
    EEPROM.commit();
  }


}