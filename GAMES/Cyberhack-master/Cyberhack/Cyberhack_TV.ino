#include <Arduboy2.h>

#ifdef TV_TRANSITION

void tv_Init() {

    gameState = GameState::TV;
    tvSequenceVars.counter = tvSequenceVars.mode == TVMode::TurnOff ? 0 : 34;

}   

void tv() {

    const uint8_t offsets[] = { 0, 10, 25, 54, 62 };

    switch (tvSequenceVars.counter) {

        case 0:
        case 23:
            arduboy.fillScreen(WHITE);
            break;

        case 1 ... 2:
        case 28 ... 34:
            arduboy.fillScreen(BLACK);
            break;

        case 3 ... 18:
            {
                const uint8_t offset = offsets[(tvSequenceVars.counter - 3) / 4];
                arduboy.drawFastHLine(offset, 32, 128 - offset - offset + 1);
            }
            break;

        case 19 ... 22:
            {
                const uint8_t offset = offsets[4];
                arduboy.drawFastHLine(offset, 32, 128 - offset - offset + 1);
                arduboy.drawFastVLine( 64, 31, 3);
            }
            break;

        case 24 ... 27:
            Sprites::drawSelfMasked(63, 31, Images::Cathode, 0);
            break;

        case 35:
        case 255:
            gameState = nextState;
            break;

    }

    tvSequenceVars.counter = tvSequenceVars.mode == TVMode::TurnOff ? tvSequenceVars.counter + 1 : tvSequenceVars.counter - 1;

}
#endif