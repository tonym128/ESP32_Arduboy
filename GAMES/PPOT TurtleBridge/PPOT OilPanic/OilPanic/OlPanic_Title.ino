#include <Arduboy2.h>

void title() {

    counter++;
    #ifdef SOUNDS
        if (soundCounter > 0) soundCounter--;
    #endif

    Sprites::drawOverwrite(0, 0, Images::Title, 0);

    switch (counter) {

        case 60 ... 63:
            Sprites::drawOverwrite(80, 18, Images::Title_Eyes, 0);
            break;

        case 64 ... 67:
            Sprites::drawOverwrite(80, 18, Images::Title_Eyes, 1);
            break;

        case 68 ... 71:
            Sprites::drawOverwrite(80, 18, Images::Title_Eyes, 2);
            break;


        case 120:
            counter = 0;
            break;
    }

    if (arduboy.justPressed(A_BUTTON)) { 

        gameState = GameState::Instructions_Init;

    }

    if (arduboy.justPressed(B_BUTTON)) { 

        score = 0;
        gameState = GameState::HighScore_Init;

    }

    #ifdef SOUNDS

        if (arduboy.justPressed(UP_BUTTON) || arduboy.justPressed(DOWN_BUTTON)) {
            soundsOnOff = toggleSoundSettings(arduboy);
            soundCounter = 32;
        }

        if (soundCounter > 0) {
            Sprites::drawOverwrite(116, 54, Images::Sound, !soundsOnOff);
        }

    #endif

}
