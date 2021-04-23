#include "src/utils/Arduboy2Ext.h"

// ----------------------------------------------------------------------------
//  Initialise state ..
//
void drawHighScore_Loop(Name *name) {

	arduboy.setRGBled(0, 0, 0);
    
  uint8_t winnerIdx = NO_WINNER;
  uint8_t clearScores = 0;
  uint8_t flashCount = 255;

  char player1[NAME_LENGTH + 1];
  char player2[NAME_LENGTH + 1];
  char player3[NAME_LENGTH + 1];
  char player4[NAME_LENGTH + 1];
  char player5[NAME_LENGTH + 1];

  uint16_t score1 = 0;
  uint16_t score2 = 0;
  uint16_t score3 = 0;
  uint16_t score4 = 0;
  uint16_t score5 = 0;

  if (name != nullptr) winnerIdx = EEPROM_Utils::saveScore(name->getString());

  EEPROM_Utils::getName(player1, EEPROM_HS_NAME_1);
  EEPROM_Utils::getName(player2, EEPROM_HS_NAME_2);
  EEPROM_Utils::getName(player3, EEPROM_HS_NAME_3);
  EEPROM_Utils::getName(player4, EEPROM_HS_NAME_4);
  EEPROM_Utils::getName(player5, EEPROM_HS_NAME_5);

  score1 = EEPROM_Utils::getHighScore(EEPROM_HS_SCORE_1);
  score2 = EEPROM_Utils::getHighScore(EEPROM_HS_SCORE_2);
  score3 = EEPROM_Utils::getHighScore(EEPROM_HS_SCORE_3);
  score4 = EEPROM_Utils::getHighScore(EEPROM_HS_SCORE_4);
  score5 = EEPROM_Utils::getHighScore(EEPROM_HS_SCORE_5);

  while (gameState == GameState::HighScore){

    if (!(arduboy.nextFrame())) { delay(20); };
    arduboy.pollButtons();

    auto justPressed = arduboy.justPressedButtons();
    auto pressed = arduboy.pressedButtons();



    // Clear scores ..

    if ((pressed & LEFT_BUTTON) && (pressed & RIGHT_BUTTON)) {

      clearScores++;

      switch (clearScores) {

        case 21 ... 60:

          arduboy.setRGBled(128 - (clearScores * 2), 0, 0);
          break;

        case 61:

          clearScores = 0;
          arduboy.setRGBled(0, 0, 0);
          EEPROM_Utils::initEEPROM(true);

          EEPROM_Utils::getName(player1, EEPROM_HS_NAME_1);
          EEPROM_Utils::getName(player2, EEPROM_HS_NAME_2);
          EEPROM_Utils::getName(player3, EEPROM_HS_NAME_3);
          EEPROM_Utils::getName(player4, EEPROM_HS_NAME_4);
          EEPROM_Utils::getName(player5, EEPROM_HS_NAME_5);

          score1 = EEPROM_Utils::getHighScore(EEPROM_HS_SCORE_1);
          score2 = EEPROM_Utils::getHighScore(EEPROM_HS_SCORE_2);
          score3 = EEPROM_Utils::getHighScore(EEPROM_HS_SCORE_3);
          score4 = EEPROM_Utils::getHighScore(EEPROM_HS_SCORE_4);
          score5 = EEPROM_Utils::getHighScore(EEPROM_HS_SCORE_5);

          winnerIdx = NO_WINNER;

          break;

      }

    }
    else {

      if (clearScores > 0) {
      
        arduboy.setRGBled(0, 0, 0);
        clearScores = 0;

      }
      
    }


    // Handle other input ..

    if (justPressed & A_BUTTON) {
      gameState = GameState::TitleScreen; 
    }



    const bool flash = arduboy.getFrameCountHalf(FLASH_FRAME_COUNT_2);
    if (flashCount > 0 && arduboy.getFrameCount(FLASH_FRAME_COUNT_2)) flashCount--;
    if (flashCount == 0) winnerIdx = NO_WINNER;

    renderSideBones();
    // Sprites::drawOverwrite(1, 0, Images::HighScore_LHS, 0);
    // Sprites::drawOverwrite(101, 0, Images::HighScore_RHS, 0);
    Sprites::drawOverwrite(31, 3, Images::HighScore_Top, 0);

    if ((flash && winnerIdx == 0) || winnerIdx != 0) {

      font4x6.setCursor(32, 20);
      font4x6.print(player1);
      renderScore(score1, 82, 20);

    }

    if ((flash && winnerIdx == 1) || winnerIdx != 1) {

      font4x6.setCursor(32, 29);
      font4x6.print(player2);
      renderScore(score2, 82, 29);

    }

    if ((flash && winnerIdx == 2) || winnerIdx != 2) {

      font4x6.setCursor(32, 38);
      font4x6.print(player3);
      renderScore(score3, 82, 38);

    }

    if ((flash && winnerIdx == 3) || winnerIdx != 3) {

      font4x6.setCursor(32, 47);
      font4x6.print(player4);
      renderScore(score4, 82, 47);

    }

    if ((flash && winnerIdx == 4) || winnerIdx != 4) {

      font4x6.setCursor(32, 56);
      font4x6.print(player5);
      renderScore(score5, 82, 56);

    }

    arduboy.display(true);

  }

}


void renderScore(int16_t score, uint8_t x, uint8_t y) {
    
  uint8_t digits[3] = {};
  extractDigits(digits, static_cast<uint16_t>(absT(score)));

  for (uint8_t j = 3, x2 = x; j > 0; --j, x2 += 5) {

    font4x6.setCursor(x2, y);
    font4x6.print(digits[j - 1]);
  }

}