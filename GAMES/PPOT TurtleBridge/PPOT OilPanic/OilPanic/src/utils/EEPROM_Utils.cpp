#include "EEPROM_Utils.h"
#include "Arduboy2Ext.h"


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters 'F' and 'P' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
 *   it resets the settings ..
 */

uint8_t letter1 = 'O';
uint8_t letter2 = 'P';



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
      EEPROM.put(sizeof(SaveEntry)*i+saveEntriesStart, entry);
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
    return Constants::no_Winner;
  }


  /* -----------------------------------------------------------------------------
   *   Save score if it is in the top 3, return slot number (or Constants::no_Winner) ..
   */
  uint8_t saveScore(uint16_t newScore) {
    uint8_t targetIndex = findScore(newScore);
    if(targetIndex == Constants::no_Winner) {
      return Constants::no_Winner;}

    for (uint8_t i = (saveEntriesCount - 1); i > targetIndex; i--) {
      SaveEntry entry;
      EEPROM.get(saveEntriesStart+(i-1)*sizeof(SaveEntry), entry);
      EEPROM.put(saveEntriesStart+i*sizeof(SaveEntry), entry);
    }

    // Save new name and score ..
    SaveEntry entry { newScore, { '?', '?', '?', '\0' } };
    EEPROM.put(targetIndex*sizeof(SaveEntry)+saveEntriesStart,entry);
    
    EEPROM.commit();
    
    return targetIndex;
  }



  void saveChar(uint8_t saveIndex, uint8_t charIndex, char newChar) {
    EEPROM.write(sizeof(SaveEntry)*saveIndex+saveEntriesStart+sizeof (uint16_t)+charIndex, newChar);
    EEPROM.commit();
}



  void readSaveEntry(SaveEntry &entry, uint8_t saveIndex) {
    EEPROM.get(sizeof(SaveEntry)*saveIndex+saveEntriesStart,entry);
}



  void readSaveEntryName(char (&name), uint8_t saveIndex) {
    char nme[sizeof(SaveEntry::name)];
    for (uint8_t i=0; i< sizeof(SaveEntry::name); i++)
     EEPROM.get(sizeof(SaveEntry)*saveIndex+saveEntriesStart+i+sizeof(uint16_t), nme[i]);
    memcpy(&name, &nme[0], sizeof(SaveEntry::name));
  }

  
  
  uint16_t readSaveEntryScore(uint8_t saveIndex) {
    uint16_t scr;
    EEPROM.get(sizeof(SaveEntry)*saveIndex+saveEntriesStart,scr);
    return (scr);
  }



  void writeSaveEntry(SaveEntry &entry, uint8_t saveIndex) {
    EEPROM.put(sizeof(SaveEntry)*saveIndex+saveEntriesStart, entry);
    EEPROM.commit();
  }



  void writeSaveEntryName(char (&name), uint8_t saveIndex) {
    char nme[sizeof(SaveEntry::name)];
    memcpy(&nme[0], &name, sizeof(SaveEntry::name));
    for (uint8_t i=0; i< sizeof(SaveEntry::name); i++)
      EEPROM.write(sizeof(SaveEntry)*saveIndex+saveEntriesStart+i+sizeof(uint16_t),(uint8_t)nme[i]);
    EEPROM.commit();
  }



  void writeSaveEntryScore(uint16_t score, uint8_t saveIndex) {
    EEPROM.put(sizeof(SaveEntry)*saveIndex+saveEntriesStart, score);
    EEPROM.commit();
  }


}