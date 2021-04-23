#include "EEPROM_Utils.h"
#include "Arduboy2Ext.h"
#include "Enums.h"

/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters 'F' and 'A' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
 *   it resets the settings ..
 */
const char chars0[8] = { 'C', 'o', 'm', 'p', 'u', 't', 'e', 'r'   };
const char chars1[8] = { 'F', 'i', 'l', 'm', 'o', 't', 'e', '\0'  };
const char chars2[8] = { 'V', 'a', 'm', 'p', 'i', 'r', 'i', 'c'   };
const char chars3[8] = { 'H', 'e', 'c', 't', 'o', 'r', '\0', '\0' };
const char chars4[8] = { 'C', 'o', 'm', 'p', 'u', 't', 'e', 'r'   };
const char chars5[8] = { 'M', 'a', 't', 'i', 'l', 'd', 'a', '\0'  };

const uint8_t letter1 = 'D'; 
const uint8_t letter2 = 'A'; 

void EEPROM_Utils::initEEPROM(bool forceClear) {

  byte c1 = EEPROM.read(EEPROM_START_C1);
  byte c2 = EEPROM.read(EEPROM_START_C2);

  if (forceClear || c1 != letter1 || c2 != letter2) { 

    EEPROM.write(EEPROM_START_C1, letter1);
    EEPROM.write(EEPROM_START_C2, letter2);

    for (uint16_t x = EEPROM_PLAYER1; x <= EEPROM_END; x++) {

      EEPROM.write(x, 0);

    }

    for (uint16_t x = EEPROM_PLAYER1 + 1, y = 0; x < EEPROM_PLAYER2 - 1; x++, y++) {

      EEPROM.write(x + 10, 32);
      EEPROM.write(x + 20, 32);

      EEPROM.write(x - 1, chars0[y]);
      EEPROM.write(x + 39, chars1[y]);
      EEPROM.write(x + 49, chars2[y]);
      EEPROM.write(x + 59, chars3[y]);
      EEPROM.write(x + 69, chars4[y]);
      EEPROM.write(x + 79, chars5[y]);

    }
    
    EEPROM.commit();

  }

  uint8_t const scores[] = {3, 2, 1, 0, 0};
  
  for (uint8_t x= 0; x < 5; x++) {
    EEPROM.put(EEPROM_HS_SCORE_1 + (x * 2), static_cast<uint16_t>(scores[x]));
  }

}


/* -----------------------------------------------------------------------------
 *   Get name ..
 */
void EEPROM_Utils::getName(char *name, uint16_t startLoc) {

  uint8_t chars[NAME_LENGTH + 1];

  for (uint16_t x = 0; x < NAME_LENGTH; x++) {
    
    chars[x] = EEPROM.read(startLoc + x);

  }

  chars[NAME_LENGTH] = 0;

  memcpy(name, &chars, NAME_LENGTH + 1);

}


/* -----------------------------------------------------------------------------
 *   Get high score ..
 */
uint16_t EEPROM_Utils::getHighScore(uint16_t startLoc) {

  uint16_t score = 0;
  EEPROM.get(startLoc, score);

  return score;

}


uint8_t EEPROM_Utils::saveScore(char *name) {

  char chars[NAME_LENGTH + 1];
  uint16_t score = 0;
  uint8_t idx = NO_WINNER;

  for (uint8_t i = 0; i < 5; i++) {

    getName(chars, EEPROM_HS_NAME_1 + (i*10));
    score = getHighScore(EEPROM_HS_SCORE_1 + (i*2));

    bool match = true;

    for (uint8_t x = 0; x < 10; x++) {

      if ((char)chars[x] != name[x]) {
        match = false;
      }

    }
    
    if (match) {

      score++;
      EEPROM.put(EEPROM_HS_SCORE_1 + (i * 2), score);

      for (uint8_t j = i; j > 0; j--) {

        if (getHighScore(EEPROM_HS_SCORE_1 + (j * 2)) >= getHighScore(EEPROM_HS_SCORE_1 + ((j -1 ) * 2))) {

          for (uint8_t k = 0; k < NAME_LENGTH + 1; k++) {

            uint8_t a = EEPROM.read(EEPROM_HS_NAME_1 + (j * 10) + k);
            uint8_t b = EEPROM.read(EEPROM_HS_NAME_1 + ((j - 1) * 10) + k);
            EEPROM.write(EEPROM_HS_NAME_1 + (j * 10) + k, b);
            EEPROM.write(EEPROM_HS_NAME_1 + ((j - 1) * 10) + k, a);

          }

          uint16_t a = 0;
          uint16_t b = 0;
          EEPROM.get(EEPROM_HS_SCORE_1 + (j * 2), a);
          EEPROM.get(EEPROM_HS_SCORE_1 + ((j -1) * 2), b);
          EEPROM.put(EEPROM_HS_SCORE_1 + (j * 2), b);
          EEPROM.put(EEPROM_HS_SCORE_1 + ((j - 1) * 2), a);

          if (j == i) { i--; }

        }

      }

      return i;

    }

    if (score <= 1 && idx == NO_WINNER) {

      idx = i;

    }    

  }


  // New High Score ..

  if (idx < NO_WINNER) {

    for (uint8_t i = 4; i > idx; i--) {

      for (uint8_t j = 0; j < NAME_LENGTH + 1; j++) {

        uint8_t x = EEPROM.read(EEPROM_HS_NAME_1 + ((i - 1) * 10) + j);
        EEPROM.write(EEPROM_HS_NAME_1 + (i * 10) + j, x);

      }

      uint16_t score = 0;
      EEPROM.get(EEPROM_HS_SCORE_1 + ((i -1) * 2), score);
      EEPROM.put(EEPROM_HS_SCORE_1 + (i * 2), score);

    }


    // Write out new name and score ..

    for (uint8_t j = 0; j < NAME_LENGTH; j++) {

      EEPROM.write(EEPROM_HS_NAME_1 + (idx * 10) + j, name[j]);

    }

    score = 1;
    EEPROM.put(EEPROM_HS_SCORE_1 + (idx * 2), score);

  }

  EEPROM.commit();
  return idx;

}