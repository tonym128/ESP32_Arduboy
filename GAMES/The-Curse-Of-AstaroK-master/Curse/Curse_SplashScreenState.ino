#include "src/utils/Arduboy2Ext.h"

// ----------------------------------------------------------------------------
//  Initialise state ..
//
void splashScreen_Activate() {

  splashScreenVars.counter = 0;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void splashScreen_Update() { 

	auto justPressed = arduboy.justPressedButtons();

  if (justPressed > 0 && splashScreenVars.counter > 0) {

    splashScreenVars.counter = 124;

  }

  if (justPressed > 0 && splashScreenVars.counter == 0) {

    splashScreenVars.counter = 1;

  }

  if (splashScreenVars.counter > 0) {

    splashScreenVars.counter++;

    switch (splashScreenVars.counter) {
        
      case 125:   
        gameState = GameStateType::TitleScreen_Activate;    
        break;

      default:
        break;

    }

  }

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void splashScreen_Render() {

  ardBitmap.drawCompressed(47, 17, Images::Ppot_Buttons, WHITE, MIRROR_NONE);
  ardBitmap.drawCompressed(43, 26, Images::Ppot_ButtonUp, WHITE, MIRROR_NONE);
  ardBitmap.drawCompressed(73, 26, Images::Ppot_ButtonUp, WHITE, MIRROR_NONE);

  if (splashScreenVars.counter == 0) {

    ardBitmap.drawCompressed(58, 26, Images::Ppot_ButtonUp, WHITE, MIRROR_NONE);
    ardBitmap.drawCompressed(26, 46, Images::Ppot_Caption, WHITE, MIRROR_NONE);

  }
  else {

    ardBitmap.drawCompressed(58, 26, Images::Ppot_ButtonDown, WHITE, MIRROR_NONE);
    ardBitmap.drawCompressed(44, 46, Images::Ppot_Loading, WHITE, MIRROR_NONE);

    uint8_t i = (splashScreenVars.counter / 15) % 4;

    for (uint8_t j = 0; j < i; j++) {
      
      arduboy.drawPixel(79 + (j * 2), 49);

    }

  }

  arduboy.display(true);

}