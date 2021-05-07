#include <Arduboy2.h>


void title_Init() {

    gameState = GameState::Title;
    titleScreenVars.reset(gameRotation);

    #ifdef SOUNDS
        sound.tones(Sounds::Theme);
    #endif

    titleScreenVars.levels = 0;

    for (uint8_t i = 0; i < 3; i++) {
        if (EEPROM_Utils::getLevel(i) == 1) titleScreenVars.levels++;
    }

}   



void title() {

    renderScenery(GameMode::Single);
    renderHighScore(gameMode, false);
    
    if (arduboy.justPressed(B_BUTTON)) {

        gameRotation = gameRotation == GameRotation::Landscape ? GameRotation::Portrait : GameRotation::Landscape;
        gameState = GameState::Title_Init;
        EEPROM_Utils::saveRotation(gameRotation);

    }

    switch (gameRotation) {

        case GameRotation::Portrait:

            Sprites::drawExternalMask(titleScreenVars.counter, 12, Images::Portrait::MOTHERSHIP_SIZE_PORTRAIT::Mothership_Title, Images::Portrait::MOTHERSHIP_SIZE_PORTRAIT::Mothership_Title_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);

            switch (titleScreenVars.counter) {

                case 89:
                    renderPlayerSelection(true);
                    renderInvaderSign();
                    break;

                case 90 ... 97:
                    renderPlayerSelection(true);
                    renderInvaderSign();
                    titleScreenVars.counter++;
                    break;

                case 98 ... 105:
                    renderPlayerSelection(false);
                    renderInvaderSign();
                    titleScreenVars.counter++;
                    break;

                case 106 ... 128:
                    renderPlayerSelection(false);
                    titleScreenVars.counter++;
                    break;

                case 129:

                    switch (gameMode) {

                        case GameMode::Single ... GameMode::Double:
                            gameState = GameState::Game_Init;
                            break;

                        case GameMode::TugOfWar:
                            gameState = GameState::TugOfWar_Init;
                            break;

                    }

                    break;
                    
            }

            if (arduboy.justPressed(RIGHT_BUTTON) && gameMode != GameMode::Single) { 

                gameMode--;
            
            }

            if (arduboy.justPressed(LEFT_BUTTON) && static_cast<uint8_t>(gameMode) < titleScreenVars.levels) { 

                gameMode++;

            }

            if (arduboy.justPressed(A_BUTTON) && titleScreenVars.counter == 89) { 

                titleScreenVars.counter = 90;

            }

            break;

        case GameRotation::Landscape:

            Sprites::drawExternalMask(14, titleScreenVars.counter, Images::Landscape::MOTHERSHIP_SIZE_LANDSCAPE::Mothership_Title, Images::Landscape::MOTHERSHIP_SIZE_LANDSCAPE::Mothership_Title_Mask, Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6], Constants::Mothership_Frames[arduboy.getFrameCount(36) / 6]);

            switch (titleScreenVars.counter) {

                case 0:
                    renderPlayerSelection(true);
                    renderInvaderSign();
                    break;

                case -8 ... -1:
                    renderPlayerSelection(true);
                    renderInvaderSign();
                    titleScreenVars.counter--;
                    break;

                case -18 ... -9:
                    renderPlayerSelection(false);
                    renderInvaderSign();
                    titleScreenVars.counter--;
                    break;

                case -38 ... -19:
                    renderPlayerSelection(false);
                    titleScreenVars.counter--;
                    break;

                case -39:

                    switch (gameMode) {

                        case GameMode::Single ... GameMode::Double:
                            gameState = GameState::Game_Init;
                            break;

                        case GameMode::TugOfWar:

                            if (gameRotation == GameRotation::Landscape) {
                                gameRotation = gameRotation == GameRotation::Landscape ? GameRotation::Portrait : GameRotation::Landscape;
                                gameState = GameState::Title_Init;
                                EEPROM_Utils::saveRotation(gameRotation);
                            }
                            
                            gameState = GameState::TugOfWar_Init;
                            break;

                    }

                    break;
                    
            }

            if (arduboy.justPressed(UP_BUTTON) && gameMode != GameMode::Single) { 

                gameMode--;

            }

            if (arduboy.justPressed(DOWN_BUTTON) && static_cast<uint8_t>(gameMode) < titleScreenVars.levels) { 

                gameMode++;

            }

            if (arduboy.justPressed(A_BUTTON) && titleScreenVars.counter == 0) { 

                titleScreenVars.counter = -1;

            }

            break;

    }

}

void renderPlayerSelection(bool renderPlayerSelection) {

    switch (gameRotation) {

        case GameRotation::Portrait:

            Sprites::drawOverwrite(31, 20, Images::Portrait::Rotate, 0);

            if (!renderPlayerSelection) return;

            Sprites::drawSelfMasked(62 - (static_cast<uint8_t>(gameMode) * 8), 5, Images::Portrait::DownArrow, Constants::Arrow_Frames[arduboy.getFrameCount(48) / 12]);
            Sprites::drawOverwrite(62, 13, Images::Portrait::SurvivalMode, 0);
            Sprites::drawOverwrite(54, 13, Images::Portrait::VSMode, 0);
            Sprites::drawOverwrite(46, 13, Images::Portrait::TugOfWarMode, 0);

            if (EEPROM_Utils::getLevel(0) == 0) Sprites::drawOverwrite(54, 5, Images::Portrait::Lock, 0);
            if (EEPROM_Utils::getLevel(1) == 0) Sprites::drawOverwrite(46, 5, Images::Portrait::Lock, 0);

            break;

        case GameRotation::Landscape:

            Sprites::drawOverwrite(85, 53, Images::Landscape::Rotate, 0);

            if (!renderPlayerSelection) return;

            Sprites::drawSelfMasked(71, 12 + (static_cast<uint8_t>(gameMode) * 9), Images::Landscape::LeftArrow, Constants::Arrow_Frames[arduboy.getFrameCount(48) / 12]);
            Sprites::drawOverwrite(79, 12, Images::Landscape::SurvivalMode, 0);
            Sprites::drawOverwrite(79, 21, Images::Landscape::VSMode, 0);
            Sprites::drawOverwrite(79, 30, Images::Landscape::TugOfWarMode, 0);

            if (EEPROM_Utils::getLevel(0) == 0) Sprites::drawOverwrite(71, 21, Images::Landscape::Lock, 0);
            if (EEPROM_Utils::getLevel(1) == 0) Sprites::drawOverwrite(71, 30, Images::Landscape::Lock, 0);

            break;
            
    }

}
