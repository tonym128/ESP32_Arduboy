#pragma once

#include "Arduboy2Ext.h"
#include "Enums.h"

#define EEPROM_START                  EEPROM_STORAGE_SPACE_START + 100
#define EEPROM_START_C1               EEPROM_START
#define EEPROM_START_C2               EEPROM_START + 1
#define EEPROM_GAME_NO                EEPROM_START + 2
#define EEPROM_LEVEL_NO               EEPROM_START + 3
#define EEPROM_MEN_LEFT               EEPROM_START + 4
#define EEPROM_SCORE                  EEPROM_START + 5
#define EEPROM_GAME_NO_ORIG           EEPROM_START + 7
#define EEPROM_LEVEL_NO_ORIG          EEPROM_START + 8
#define EEPROM_MEN_LEFT_ORIG          EEPROM_START + 9
#define EEPROM_SCORE_ORIG             EEPROM_START + 10


class EEPROM_Utils {

  public: 

    EEPROM_Utils(){};
        
    static void initEEPROM(bool forceClear);
    static uint8_t getGameNumber();
    static uint8_t getLevelNumber();
    static uint8_t getMen();
    static void saveLevelNumber(uint8_t levelNumber);

    static void getSavedGameData(Level *level, Player *player);
    static void saveGameData(Level *level, Player *player);
    static void setGameNumber(uint8_t val);

};


/* ----------------------------------------------------------------------------
 *   Is the EEPROM initialised?
 *
 *   Looks for the characters 'L' and 'R' in the first two bytes of the EEPROM
 *   memory range starting from byte EEPROM_STORAGE_SPACE_START.  If not found,
 *   it resets the settings ..
 */
void EEPROM_Utils::initEEPROM(bool forceClear) {

  EEPROM.begin(300);
  byte c1 = EEPROM.read(EEPROM_START_C1);
  byte c2 = EEPROM.read(EEPROM_START_C2);

  if (forceClear || c1 != 76 || c2 != 82) { // LR 76 82

    const uint16_t score = 0;
    EEPROM.write(EEPROM_START_C1, 76);
    EEPROM.write(EEPROM_START_C2, 82);
    EEPROM.get(EEPROM_SCORE, score);

    //if (GAME_NUMBER == 1) {

      EEPROM.write(EEPROM_GAME_NO, 1);
      EEPROM.write(EEPROM_LEVEL_NO, 1);
      EEPROM.write(EEPROM_MEN_LEFT, 99);

      EEPROM.write(EEPROM_GAME_NO_ORIG, 1);
      EEPROM.write(EEPROM_LEVEL_NO_ORIG, 1);
      EEPROM.write(EEPROM_MEN_LEFT_ORIG, 99);
      EEPROM.write(EEPROM_SCORE_ORIG, score);

    //}
  /*  else {

      EEPROM.write(EEPROM_GAME_NO, EEPROM.read(EEPROM_GAME_NO_ORIG));
      EEPROM.write(EEPROM_LEVEL_NO, EEPROM.read(EEPROM_LEVEL_NO_ORIG));
      EEPROM.write(EEPROM_MEN_LEFT, EEPROM.read(EEPROM_MEN_LEFT_ORIG));

    }*/

  }
  EEPROM.commit();

}


/* -----------------------------------------------------------------------------
 *   Get the current level number. 
 */
uint8_t EEPROM_Utils::getGameNumber() {

  return EEPROM.read(EEPROM_GAME_NO);

}


/* -----------------------------------------------------------------------------
 *   Get the current level number. 
 */
uint8_t EEPROM_Utils::getLevelNumber() {

  return EEPROM.read(EEPROM_LEVEL_NO);

}


/* -----------------------------------------------------------------------------
 *   Get the number of men remaining. 
 */
uint8_t EEPROM_Utils::getMen() {

  return EEPROM.read(EEPROM_MEN_LEFT);

}

/* -----------------------------------------------------------------------------
 *   Save the current level number. 
 */
void EEPROM_Utils::saveLevelNumber(uint8_t levelNumber) {

  EEPROM.put(EEPROM_LEVEL_NO, levelNumber);
  EEPROM.commit();

}


/* -----------------------------------------------------------------------------
 *   Get saved game data.
 */
void EEPROM_Utils::getSavedGameData(Level *level, Player *player) {

  int16_t score = 0;

  level->setLevelNumber(EEPROM.read(EEPROM_LEVEL_NO));
  player->setMen(EEPROM.read(EEPROM_MEN_LEFT));
  EEPROM.get(EEPROM_SCORE, score);
  player->setScore(score);

}


/* -----------------------------------------------------------------------------
 *   Save game data.
 */
void EEPROM_Utils::saveGameData(Level *level, Player *player) {
 
  uint8_t levelNumber = level->getLevelNumber();
  EEPROM.write(EEPROM_LEVEL_NO, levelNumber);
  EEPROM.write(EEPROM_LEVEL_NO_ORIG, levelNumber);

  uint8_t menLeft = player->getMen();
  EEPROM.write(EEPROM_MEN_LEFT, menLeft);
  EEPROM.write(EEPROM_MEN_LEFT_ORIG, menLeft);

  uint16_t score = player->getScore();
  EEPROM.put(EEPROM_SCORE, score);
  EEPROM.put(EEPROM_SCORE_ORIG, score);

  EEPROM.commit();
}


/* -----------------------------------------------------------------------------
 *   Save game number.
 */
void EEPROM_Utils::setGameNumber(uint8_t val) {

  EEPROM.write(EEPROM_GAME_NO, val);
  EEPROM.write(EEPROM_GAME_NO_ORIG, val);
  EEPROM.commit();
}
