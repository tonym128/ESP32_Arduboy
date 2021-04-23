#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Enums.h"
#include "src/utils/Name.h"

   
Name *name;

void getNames_Loop() {

  players[1]->setStartLoc(EEPROM_PLAYER2);
  players[2]->setStartLoc(EEPROM_PLAYER3);

  name = players[1];

  while (gameState == GameState::GetNames) {

    if (!(arduboy.nextFrame())) {
      delay(5);
    };
  
    arduboy.pollButtons();

    auto justPressed = arduboy.justPressedButtons();

    uint8_t charIndex = name->getCharIndex();

    if (justPressed & LEFT_BUTTON) 						 	{ name->decCharIndex(); } 
    if (justPressed & RIGHT_BUTTON) 					 	{ name->incCharIndex(); } 

    if (justPressed & UP_BUTTON) { 

      name->incChar(charIndex); 

    }
    else if (justPressed & DOWN_BUTTON) { 

      name->decChar(charIndex); 

    }
    


    // Go back a player or to number of players screen ..

    if (justPressed & B_BUTTON) { 

      if (playerBeingEdited > 1) {
      
        playerBeingEdited--;
        name = players[playerBeingEdited];

      }
      else {

        gameState = GameState::TitleScreen;

      }

    }


    // Go forward to next player or start game ..

    if (justPressed & A_BUTTON) { 

      bool allSpaces = true;
      
      for (uint8_t x = 0; x < NAME_LENGTH; x++) {

        if (static_cast<uint8_t>(name->getChar(x)) != 32 && static_cast<uint8_t>(name->getChar(x)) != 0) allSpaces = false;

      }			

      if (!allSpaces) {

        if (playerBeingEdited == 1 && gameMode == GameMode::TwoPlayer)  {
          
            playerBeingEdited++;
            name = players[playerBeingEdited];
        
        }
        else {

          gameState = GameState::PlayGame;

        }

      }

    }


    // -----------------------------------------------------------------------------------------
    //  Render details ..
    // -----------------------------------------------------------------------------------------
    
    const bool flash = arduboy.getFrameCountHalf(FLASH_FRAME_COUNT_2);

    renderSideBones();
    // Sprites::drawOverwrite(1, 0, Images::HighScore_LHS, 0);
    // Sprites::drawOverwrite(101, 0, Images::HighScore_RHS, 0);
    Sprites::drawOverwrite(33, 5, Images::Players, 0);

    font4x6.setCursor(45, 30);
    font4x6.print(F("Player "));
    font4x6.print(playerBeingEdited);


    arduboy.drawHorizontalDottedLine(42, 84, 54);

    for (uint8_t x = 0; x < NAME_LENGTH; x++) {

      font4x6.setCursor(42 + (x * NAME_CHARACTER_SPACING), 46);

      if (flash && name->getCharIndex() == x) {
        arduboy.drawFastHLine(42 + (x * NAME_CHARACTER_SPACING), 54, 4, WHITE);
      }

      font4x6.print(name->getChar(x));

    }

    arduboy.display(true);

  }

}

void renderSideBones() {
  Sprites::drawOverwrite(1, 0, Images::HighScore_LHS, 0);
  Sprites::drawOverwrite(101, 0, Images::HighScore_RHS, 0);
}