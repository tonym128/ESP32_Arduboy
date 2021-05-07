#include <Arduboy2.h>

void gameOver_Init() {

    gameState = GameState::GameOver;
    gameOverScreenVars.reset();

    switch (gameRotation) {

        case GameRotation::Portrait:
            gameOverScreenVars.counter = 130;
            break;

        case GameRotation::Landscape:
            gameOverScreenVars.counter = -20;
            break;

    }

    uint16_t oldScore = EEPROM_Utils::getScore(gameMode);
    uint16_t score = (gameMode == GameMode::Single ? player1.getScore() : player1.getScore() > player2.getScore() ? player1.getScore() : player2.getScore());

    if (score > 80) {

        if (EEPROM_Utils::getLevel(0) == 0) {

            EEPROM_Utils::saveLevel(0, 1);

        }
        else if (score > 160) {

            if (EEPROM_Utils::getLevel(1) == 0) {

                EEPROM_Utils::saveLevel(1, 1);
                
            }

        }

    }


    if (gameMode == GameMode::Double) {

        gameOverScreenVars.flashPlayer1 = player1.getScore() > player2.getScore();
        gameOverScreenVars.flashPlayer2 = player2.getScore() > player1.getScore();
        
    }
    
    if (oldScore < score) {

        if (gameMode == GameMode::Single) {
            gameOverScreenVars.flashPlayer1 = true;
        }

        gameOverScreenVars.newHighScore = true;
        EEPROM_Utils::saveScore(gameMode, score);

    }

    #ifdef SOUNDS
        sound.tones(Sounds::Game_Over);
    #endif

}   


void gameOver() {


    // Move mothership ..

    switch (gameRotation) {

        case GameRotation::Portrait:

            if (gameOverScreenVars.counter > 75) {

                gameOverScreenVars.counter--;

            }

            break;

        case GameRotation::Landscape:

            if (gameOverScreenVars.counter < 4) {

                gameOverScreenVars.counter++;

            }

            break;

    }


    // Handle User Input -----------------------------------------------------

    if (arduboy.justPressed(A_BUTTON)) { 

        gameState = GameState::Title_Init;

    }


    // Render screen ---------------------------------------------------------------------

    renderScores(gameOverScreenVars.flashPlayer1, gameOverScreenVars.flashPlayer2);
    renderScenery(gameMode);

    switch (gameRotation) {

        case GameRotation::Portrait:

            Sprites::drawExternalMask(gameOverScreenVars.counter, 11, Images::Portrait::MOTHERSHIP_SIZE_PORTRAIT::Mothership_Title, Images::Portrait::MOTHERSHIP_SIZE_PORTRAIT::Mothership_Title_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);

            if (gameOverScreenVars.counter == 75) {

                Sprites::drawSelfMasked(52, 17, Images::Portrait::GameOver, 0);
                
                if (!gameOverScreenVars.newHighScore || arduboy.getFrameCountHalf(Constants::FlashSpeed)) {
                    renderHighScore(gameMode, false);
                }

            }
            break;

        case GameRotation::Landscape:

            Sprites::drawExternalMask(43, gameOverScreenVars.counter, Images::Landscape::MOTHERSHIP_SIZE_LANDSCAPE::Mothership_Title, Images::Landscape::MOTHERSHIP_SIZE_LANDSCAPE::Mothership_Title_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);

            if (gameOverScreenVars.counter == 4) {

                Sprites::drawSelfMasked(32, 32, Images::Landscape::GameOver, 0);
                
                if (!gameOverScreenVars.newHighScore || arduboy.getFrameCountHalf(Constants::FlashSpeed)) {
                    renderHighScore(gameMode, true);
                }

            }
            break;

    }

}
