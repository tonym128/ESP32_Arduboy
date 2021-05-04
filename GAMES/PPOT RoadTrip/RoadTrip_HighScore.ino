#include "src/Utils/Arduboy2Ext.h"

uint8_t clearScores = 0;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void HighScore_Init() {

    cameraPos.setX(0);
    cameraPos.setY(UPM);
    cameraPos.setZ(-40);

    world.reset();
    gamePlayVars.reset();
    gameState = GameState::HighScore;

    #ifdef CACTII
        placeCactii();
    #endif
    
}   


// --------------------------------------------------------------------------------------
//  Render High Score screen ..
//
void renderHighScore(HighScore &highScore) {

    uint8_t xOffset = 35;
    uint8_t yOffset = 18;

    draw(false, true);

    arduboy.fillRect(23, 0, 82, 64, BLACK);
    arduboy.drawFastVLine(24, 16, 47, WHITE);
    arduboy.drawFastVLine(103, 16, 47, WHITE);
    arduboy.drawFastHLine(25, 63, 78, WHITE);

    Sprites::drawOverwrite(16, -2, Images::HighScore, 0);

    for (uint8_t x =0; x < Constants::MaxNumberOfScores; x++) {

        Slot slot = EEPROM_Utils::getSlot(x);

        Sprites::drawOverwrite(xOffset, yOffset, Images::Font4x6, slot.getChar0());
        Sprites::drawOverwrite(xOffset + 6, yOffset, Images::Font4x6, slot.getChar1());
        Sprites::drawOverwrite(xOffset + 12, yOffset, Images::Font4x6, slot.getChar2());


        // Score ..
        {
            uint8_t digits[6] = {};
            extractDigits(digits, slot.getScore());

            for (uint8_t i = 0, x2 = xOffset + 54; i < 6; ++i, x2 -= 5) {
                Sprites::drawOverwrite(x2, yOffset, Images::Font4x6, (digits[i] + 26));
            }

        }

        yOffset = yOffset + 9;

    }


    if (highScore.getSlotNumber() != Constants::DoNotEditSlot) {

        yOffset = 18;
        highScoreVars.alternate++;

        if (highScoreVars.alternate < 12) {

            Sprites::drawOverwrite(xOffset - 5, yOffset + (highScore.getSlotNumber() * 9) - 1, Images::Arrow, 0);
            Sprites::drawOverwrite(xOffset + 62, yOffset + (highScore.getSlotNumber() * 9) - 1, Images::Arrow, 0);

        }
        else if (highScoreVars.alternate > 24) {

            highScoreVars.alternate = 0;

        }

        if (highScore.getCharIndex() == 0) {
            renderChar(xOffset, yOffset, 0, highScore);
        }

        if (highScore.getCharIndex() == 1) {
            renderChar(xOffset, yOffset, 1, highScore);
        }

        if (highScore.getCharIndex() == 2) {
            renderChar(xOffset, yOffset, 2, highScore);
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

        if (arduboy.pressed(LEFT_BUTTON) && arduboy.pressed(RIGHT_BUTTON)) {

            clearScores++;

            switch (clearScores) {

                case 21 ... 60:
                    arduboy.setRGBled(128 - (clearScores * 2), 0, 0);
                    break;

                case 61:
                    clearScores = 0;
                    arduboy.setRGBled(0, 0, 0);
                    EEPROM_Utils::initEEPROM(true);
                    //(NOTE_C6, 100);
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
            gameState = GameState::Title_Init; 

        }

    }


    houseKeeping();

    #ifdef CACTII
        moveCactii();
    #endif
    
    cameraPos.setZ(cameraPos.getZ() + 6);
    cameraPos.setY(world.roadHeightAt(cameraPos.getZ() + 2 * UPM) + UPM);
    gamePlayVars.timeOfDay = 1700;

}

void renderChar(uint8_t xOffset, uint8_t yOffset, uint8_t charIndex, HighScore &highScore) {

    for (uint8_t x = 0; x < 2; x++) {
        Sprites::drawOverwrite(xOffset + (x * 6), yOffset + (highScore.getSlotNumber() * 9), Images::Font4x6, highScore.getChar(x));
    }

    arduboy.fillRect(xOffset + (charIndex * 6) - 1, yOffset + (highScore.getSlotNumber() * 9) - 1, 6, 8, WHITE);
    Sprites::drawErase(xOffset + (charIndex * 6), yOffset + (highScore.getSlotNumber() * 9), Images::Font4x6, highScore.getChar(charIndex));

}