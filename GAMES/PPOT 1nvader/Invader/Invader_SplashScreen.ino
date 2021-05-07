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

        gameState = GameState::Title_Init; 

    }

    switch (gameRotation) {

        case GameRotation::Portrait:

            Sprites::drawOverwrite(51, 0, Images::Portrait::PPOT, 0);

            switch (arduboy.getFrameCount(48)) {

                case 0 ... 11:
                    Sprites::drawOverwrite(67, 58, Images::Portrait::PPOT_Arrow, 0);
                    break;

                case 12 ... 23:
                    Sprites::drawOverwrite(67, 9, Images::Portrait::PPOT_Spindle, 0);
                    Sprites::drawOverwrite(67, 58, Images::Portrait::PPOT_Arrow, 0);
                    arduboy.drawPixel(69, 20, WHITE);
                    break;

                case 24 ... 35:
                    arduboy.drawPixel(68, 20, WHITE);
                    break;

                case 36 ... 47:
                    Sprites::drawOverwrite(67, 9, Images::Portrait::PPOT_Spindle, 0);
                    arduboy.drawPixel(67, 20, WHITE);
                    break;

            }

            break;

        case GameRotation::Landscape:

            Sprites::drawOverwrite(32, 17, Images::Landscape::PPOT, 0);

            switch (arduboy.getFrameCount(96)) {

                case 0 ... 23:
                    Sprites::drawOverwrite(91, 25, Images::Landscape::PPOT_Arrow, 0);
                    break;

                case 24 ... 47:
                    Sprites::drawOverwrite(45, 28, Images::Landscape::PPOT_Spindle, 0);
                    Sprites::drawOverwrite(91, 25, Images::Landscape::PPOT_Arrow, 0);
                    arduboy.drawPixel(52, 30, WHITE);
                    break;

                case 48 ... 71:
                    arduboy.drawPixel(52, 31, WHITE);
                    break;

                case 72 ... 95:
                    Sprites::drawOverwrite(45, 28, Images::Landscape::PPOT_Spindle, 0);
                    arduboy.drawPixel(52, 32, WHITE);
                    break;

            }

            break;

    }

}
