#include "src/Utils/Arduboy2Ext.h"

uint8_t clearScores = 0;


// --------------------------------------------------------------------------------------
//  Render High Score screen ..
//

void renderHighScore(HighScore &highScore) {

  uint8_t xOffset = 35;
  uint8_t yOffset = 21;

  scrollingBackground(false);

  arduboy.fillRect(23, 0, 82, 64, BLACK);
  arduboy.drawFastVLine(24, 0, 64, WHITE);
  arduboy.drawFastVLine(103, 0, 64, WHITE);

  Sprites::drawOverwrite(28, 6, highscoreText, 0);
  
  for (uint8_t x =0; x < MAX_NUMBER_OF_SCORES; x++) {

    Slot slot = EEPROM_Utils::getSlot(x);

    Sprites::drawOverwrite(xOffset, yOffset, font4x6_Full, slot.getChar0());
    Sprites::drawOverwrite(xOffset + 6, yOffset, font4x6_Full, slot.getChar1());
    Sprites::drawOverwrite(xOffset + 12, yOffset, font4x6_Full, slot.getChar2());


    // Score ..
    {
      uint8_t digits[6] = {};
      extractDigits(digits, slot.getScore());
    
      for (uint8_t i = 0, x2 = xOffset + 54; i < 6; ++i, x2 -= 5) {
        Sprites::drawOverwrite(x2, yOffset, font4x6_Full, (digits[i] + 52));
      }
      
    }

    yOffset = yOffset + 8;

  }


  if (highScore.getSlotNumber() != DO_NOT_EDIT_SLOT) {

    yOffset = 21;
    alternate++;
  
    if (alternate < 15) {

      Sprites::drawOverwrite(xOffset - 5, yOffset + (highScore.getSlotNumber() * 8) - 1, arrow, 0);
      Sprites::drawOverwrite(xOffset + 62, yOffset + (highScore.getSlotNumber() * 8) - 1, arrow, 0);
    
    }
    else if (alternate > 30) {

      alternate = 0;

    }

    if (highScore.getCharIndex() == 0) {
      arduboy.fillRect(xOffset - 1, yOffset + (highScore.getSlotNumber() * 8) - 1, 6, 8, WHITE);
      Sprites::drawErase(xOffset, yOffset + (highScore.getSlotNumber() * 8), font4x6_Full, highScore.getChar(0));
    }
    else {
      Sprites::drawOverwrite(xOffset, yOffset + (highScore.getSlotNumber() * 8), font4x6_Full, highScore.getChar(0));
    }

    if (highScore.getCharIndex() == 1) {
      arduboy.fillRect(xOffset + 5, yOffset + (highScore.getSlotNumber() * 8) - 1, 6, 8, WHITE);
      Sprites::drawErase(xOffset + 6, yOffset + (highScore.getSlotNumber() * 8), font4x6_Full, highScore.getChar(1));
    }
    else {
      Sprites::drawOverwrite(xOffset + 6, yOffset + (highScore.getSlotNumber() * 8), font4x6_Full, highScore.getChar(1));
    }

    if (highScore.getCharIndex() == 2) {
      arduboy.fillRect(xOffset + 11, yOffset + (highScore.getSlotNumber() * 8) - 1, 6, 8, WHITE);
      Sprites::drawErase(xOffset + 12, yOffset + (highScore.getSlotNumber() * 8), font4x6_Full, highScore.getChar(2));
    }
    else {
      Sprites::drawOverwrite(xOffset + 12, yOffset + (highScore.getSlotNumber() * 8), font4x6_Full, highScore.getChar(2));
    }


    // Handle buttons ..

    uint8_t charIndex = highScore.getCharIndex();

    if (arduboy.justPressed(UP_BUTTON))       { highScore.incChar(charIndex); }
    if (arduboy.justPressed(DOWN_BUTTON))     { highScore.decChar(charIndex); }
    if (arduboy.justPressed(LEFT_BUTTON))     { highScore.decCharIndex(); } 
    if (arduboy.justPressed(RIGHT_BUTTON))    { highScore.incCharIndex(); } 

    if (arduboy.justPressed(A_BUTTON)) { 
      
      EEPROM_Utils::writeChars(highScore.getSlotNumber(), highScore);
      highScore.disableEditting(); 
      
    }

  }
  else {


    // Clear scores ..

    if (arduboy.pressed(UP_BUTTON) && arduboy.pressed(DOWN_BUTTON)) {

      clearScores++;

      switch (clearScores) {

        case 21 ... 60:
          arduboy.setRGBled(128 - (clearScores * 2), 0, 0);
          break;

        case 61:
          clearScores = 0;
          arduboy.setRGBled(0, 0, 0);
          EEPROM_Utils::initEEPROM(true);
          sound.tone(NOTE_C6, 100);
          return;

      }

    }
    else {

      if (clearScores > 0) {
      
        arduboy.setRGBled(0, 0, 0);
        clearScores = 0;

      }
      
    }

    if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) { 
    
      arduboy.setRGBled(0, 0, 0);
      state = GameState::SplashScreen_Init; 
      
    }
    
  }

}
