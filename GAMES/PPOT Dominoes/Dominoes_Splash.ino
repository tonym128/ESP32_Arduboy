#include "src/utils/Arduboy2Ext.h"
#include "src/images/Images.h"
#include "src/utils/Enums.h"


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void drawSplash_Loop() {

  uint8_t counter = 0;

  while (gameState == GameState::SplashScreen) {

    if (!(arduboy.nextFrame())) { delay(5); };
    arduboy.pollButtons();
      
    auto justPressed = arduboy.justPressedButtons();
      

    if (justPressed > 0 && counter > 0) {

      counter = 124;

    }

    if (justPressed > 0 && counter == 0) {

  #ifdef SOUNDS    
      sound.tone(NOTE_A2, 30);
      sound.tone(NOTE_F1, 30);
  #endif

      counter = 1;

    }

    if (counter > 0) {

      if (arduboy.everyXFrames(2)) {
        counter++;
        if (counter == 125) gameState = GameState::TitleScreen; 
      }

    }


    // -------------------------------------------------------------------------------------------------------------


    Sprites::drawOverwrite(47, 17, Images::Ppot_Buttons, 0);
    Sprites::drawOverwrite(43, 26, Images::Ppot_ButtonUp, 0);
    Sprites::drawOverwrite(73, 26, Images::Ppot_ButtonUp, 0);

    if (counter == 0) {

      Sprites::drawOverwrite(58, 26, Images::Ppot_ButtonUp, 0);
      Sprites::drawOverwrite(26, 46, Images::Ppot_Caption, 0);

    }
    else {

      Sprites::drawOverwrite(58, 26, Images::Ppot_ButtonDown, 0);
      Sprites::drawOverwrite(44, 46, Images::Ppot_Loading, 0);

      uint8_t i = (counter / 15) % 4;

      for (uint8_t j = 0; j < i; j++) {

          arduboy.drawPixel(79 + (j * 2), 49, WHITE);

      }

    }

    arduboy.display(true);

  }

}
