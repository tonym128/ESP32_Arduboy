#include "EEPROM_Utils.h"
#include "Arduboy2Ext.h"


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters 'F' and 'P' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
 *   it resets the settings ..
 */

uint8_t letter1 = 'F';
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
    
    for(uint8_t i = 0; i < eepromSaveEntriesCount; i++) {
      SaveEntry entry { 0, "AAA" };
      EEPROM.put(sizeof(SaveEntry)*i+eepromSaveEntriesStart, entry);
    }
    EEPROM.commit();
  }


  uint8_t findScore(uint16_t newScore) {
    for (uint8_t i = 0; i < eepromSaveEntriesCount; i++) {
      uint16_t oldScore;
      EEPROM.get(eepromSaveEntriesStart+i*sizeof(SaveEntry),oldScore);
      if (newScore >= oldScore) {
        return i;
      }
    }
    return NO_WINNER;
  }


  /* -----------------------------------------------------------------------------
   *   Save score if it is in the top 3, return slot number (or NO_WINNER) ..
   */
  uint8_t saveScore(uint16_t newScore) {
    uint8_t targetIndex = findScore(newScore);
    if(targetIndex == NO_WINNER) {
      return NO_WINNER;}

    for (uint8_t i = (eepromSaveEntriesCount - 1); i > targetIndex; i--) {
      SaveEntry entry;
      EEPROM.get(eepromSaveEntriesStart+(i-1)*sizeof(SaveEntry), entry);
      EEPROM.put(eepromSaveEntriesStart+i*sizeof(SaveEntry), entry);
    }

    // Save new name and score ..
    SaveEntry entry { newScore, { '?', '?', '?', '\0' } };
    EEPROM.put(targetIndex*sizeof(SaveEntry)+eepromSaveEntriesStart,entry);
    
    EEPROM.commit();
    
    return targetIndex;
  }



  void saveChar(uint8_t saveIndex, uint8_t charIndex, char newChar) {
    EEPROM.write(sizeof(SaveEntry)*saveIndex+eepromSaveEntriesStart+sizeof (uint16_t)+charIndex, newChar);
    EEPROM.commit();
}



  void readSaveEntry(SaveEntry &entry, uint8_t saveIndex) {
    EEPROM.get(sizeof(SaveEntry)*saveIndex+eepromSaveEntriesStart,entry);
}



  void readSaveEntryName(char (&name), uint8_t saveIndex) {
    char nme[sizeof(SaveEntry::name)];
    for (uint8_t i=0; i< sizeof(SaveEntry::name); i++)
     EEPROM.get(sizeof(SaveEntry)*saveIndex+eepromSaveEntriesStart+i+sizeof(uint16_t), nme[i]);
    memcpy(&name, &nme[0], sizeof(SaveEntry::name));
  }

  
  
  uint16_t readSaveEntryScore(uint8_t saveIndex) {
    uint16_t scr;
    EEPROM.get(sizeof(SaveEntry)*saveIndex+eepromSaveEntriesStart,scr);
    return (scr);
  }



  void writeSaveEntry(SaveEntry &entry, uint8_t saveIndex) {
    EEPROM.put(sizeof(SaveEntry)*saveIndex+eepromSaveEntriesStart, entry);
    EEPROM.commit();
  }



  void writeSaveEntryName(char (&name), uint8_t saveIndex) {
    char nme[sizeof(SaveEntry::name)];
    memcpy(&nme[0], &name, sizeof(SaveEntry::name));
    for (uint8_t i=0; i< sizeof(SaveEntry::name); i++)
      EEPROM.write(sizeof(SaveEntry)*saveIndex+eepromSaveEntriesStart+i+sizeof(uint16_t),(uint8_t)nme[i]);
    EEPROM.commit();
  }



  void writeSaveEntryScore(uint16_t score, uint8_t saveIndex) {
    EEPROM.put(sizeof(SaveEntry)*saveIndex+eepromSaveEntriesStart, score);
    EEPROM.commit();
  }

}