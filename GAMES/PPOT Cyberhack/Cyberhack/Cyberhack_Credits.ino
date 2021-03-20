#include <Arduboy2.h>

#ifdef CREDITS

void credits_Init() {

    gameState = GameState::Credits;
    tvSequenceVars.counter = 0;

}   

void credits() {

    Sprites::drawOverwrite(5, 12, Images::Title_02, 0);
    flickering(12);
    
    if (arduboy.isFrameCount(4)) tvSequenceVars.counter++;
    if (tvSequenceVars.counter >= 200) tvSequenceVars.counter = 0;

    Sprites::drawOverwrite(13, 43, Images::LeftFade2, 0);
    Sprites::drawOverwrite(107, 43, Images::RightFade, 0);

    arduboy.drawFastHLine(18, 43, 89);
    arduboy.drawFastHLine(18, 51, 89);
    
    switch (tvSequenceVars.counter % 40) {

        case 0 ... 32:
            font3x5.setCursor(20, 44);
            print(font3x5, 137 + tvSequenceVars.counter / 40);
            break;

        default:
            break;
    }

    if (arduboy.justPressed(A_BUTTON)) { 

        gameState = GameState::Title_Init;

    }

}
#endif