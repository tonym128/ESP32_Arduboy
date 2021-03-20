#include "src/utils/Arduboy2Ext.h"

// ----------------------------------------------------------------------------
//  Initialise state ..
//
void splashScreen_Init() {

    gameState = GameState::Splash;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void splashScreen() { 

    auto justPressed = arduboy.justPressedButtons();

    if (justPressed > 0) {
        
        #ifdef TV_TRANSITION
            gameState = GameState::TV_Init; 
            nextState = GameState::Title_Init; 
        #else
            gameState = GameState::Title_Init; 
        #endif

    }


    Sprites::drawOverwrite(32, 17, Images::PPOT, 0);

    switch (arduboy.getFrameCount(48)) {

        case 0 ... 11:
            Sprites::drawOverwrite(91, 25, Images::PPOT_Arrow, 0);
            break;

        case 12 ... 23:
            Sprites::drawOverwrite(45, 28, Images::PPOT_Spindle, 0);
            Sprites::drawOverwrite(91, 25, Images::PPOT_Arrow, 0);
            arduboy.drawPixel(52, 30, WHITE);
            break;

        case 24 ... 35:
            arduboy.drawPixel(52, 31, WHITE);
            break;

        case 36 ... 47:
            Sprites::drawOverwrite(45, 28, Images::PPOT_Spindle, 0);
            arduboy.drawPixel(52, 32, WHITE);
            break;

    }

}
