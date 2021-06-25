#include "src/utils/Arduboy2Ext.h"


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void playGame_Render() {

    const uint8_t yOffset = playGameVars.player.getYOffset();


    // Draw Scenery ..

    playGame_DrawScenery(yOffset);


    // Draw player

    if (!gameStats.gameOver) {

        if (playGameVars.introDelay == 0) {

            uint8_t stance = playGameVars.player.getImage();
            int8_t x = playGameVars.player.getXPosition(false);
            int8_t y = playGameVars.player.getYPosition();

            Sprites::drawErase(x, y, (const uint8_t*)pgm_read_dword_near(&Images::Junior_Mask[stance]), 0);
            Sprites::drawSelfMasked(x, y, (const uint8_t*)pgm_read_dword_near(&Images::Junior[stance]), 0);

        }
        else {

            if ((playGameVars.introDelay / 20) % 2 == 0) {

                int8_t x = playGameVars.player.getXPosition(false);
                int8_t y = playGameVars.player.getYPosition();

                if (playGameVars.player.isDead()) {

                    uint8_t stance = playGameVars.player.getImage();
                    Sprites::drawErase(x, y, (const uint8_t*)pgm_read_dword_near(&Images::Junior_Mask[stance]), 0);
                    Sprites::drawSelfMasked(x, y, (const uint8_t*)pgm_read_dword_near(&Images::Junior[stance]), 0);

                }
                else {

                    Sprites::drawErase(x, y, Images::Junior_Idle_R_Mask, 0);
                    Sprites::drawSelfMasked(x, y, Images::Junior_Idle_R, 0);

                }

            }

        }

    }


    // Draw Kong

    if (playGameVars.kong.getEnabled()) {

        playGame_RenderKong(yOffset);

    }


    // Draw sparks / birds / clappers ..

    for (uint8_t i = 0; i < NUMBER_OF_ENTITIES; i++) {

        auto &lowerClapper = playGameVars.lowerClappers.getClapper(i);

        if (lowerClapper.isEnabled()) {

            int16_t x = lowerClapper.getXPosition();
            int8_t y = lowerClapper.getYPosition(yOffset);
            uint8_t index = lowerClapper.getImage();

            Sprites::drawErase(x, y, Images::Clappers_Mask, index);
            Sprites::drawSelfMasked(x, y, Images::Clappers, index);

        }

        auto &upperClapper = playGameVars.upperClappers.getClapper(i);

        if (upperClapper.isEnabled()) {

            int16_t x = upperClapper.getXPosition();
            int8_t y = upperClapper.getYPosition(yOffset);
            uint8_t index = upperClapper.getImage();

            Sprites::drawErase(x, y, Images::Clappers_Mask, index);
            Sprites::drawSelfMasked(x, y, Images::Clappers, index);

        }

        auto &lowerSpark = playGameVars.lowerSparks.getSpark(i);

        if (lowerSpark.isEnabled()) {

            int8_t x = lowerSpark.getXPosition();
            int8_t y = lowerSpark.getYPosition(yOffset);
            uint8_t index = lowerSpark.getImage();

            if (static_cast<SparkImage>(index) != SparkImage::None) {
                Sprites::drawErase(x, y, Images::Spark_Mask, index);
                Sprites::drawSelfMasked(x, y, Images::Spark, index);
            }

        }

        auto &upperSpark = playGameVars.upperSparks.getSpark(i);

        if (upperSpark.isEnabled()) {

            int8_t x = upperSpark.getXPosition();
            int8_t y = upperSpark.getYPosition(yOffset);
            uint8_t index = upperSpark.getImage();

            if (static_cast<SparkImage>(index) != SparkImage::None) {
                Sprites::drawErase(x, y, Images::Spark_Mask, index);
                Sprites::drawSelfMasked(x, y, Images::Spark, index);
            }

        }

        auto &bird = playGameVars.birds.getBird(i);

        if (bird.isEnabled()) {

            int8_t x = bird.getXPosition();
            int8_t y = bird.getYPosition(yOffset);
            uint8_t index = bird.getImage();

            if (static_cast<BirdImage>(index) != BirdImage::None) {

                Sprites::drawErase(x, y, (const uint8_t*)pgm_read_dword_near(&Images::Birds_Mask[index]), 0);
                Sprites::drawSelfMasked(x, y, (const uint8_t*)pgm_read_dword_near(&Images::Birds[index]), 0);

            }

        }

    }


    playGame_RenderKey(yOffset);
    playGame_RenderGameOverOrPause();


    // Render score ..

    playGame_RenderScore(yOffset);

}


// ----------------------------------------------------------------------------
//  Render Kong .. 
//
void playGame_RenderKong(uint8_t yOffset) {

    int8_t x = playGameVars.kong.getXPosition();
    int8_t y = playGameVars.kong.getYPosition(yOffset);
    KongImage image = playGameVars.kong.getImage();

    Sprites::drawErase(x, y, (const uint8_t*)pgm_read_dword_near(&Images::Kong_Mask[static_cast<uint8_t>(image)]), 0);
    Sprites::drawSelfMasked(x, y, (const uint8_t*)pgm_read_dword_near(&Images::Kong[static_cast<uint8_t>(image)]), 0);
    
    const uint8_t xPos[] = { 30, 47, 76, 83 };
    const uint8_t yPos[] = { 6, 11, 11, 6 };
    
    for (uint8_t i = 0; i < 4 ; i++) {

        if (playGameVars.kong.getDisplayChain(i)) {

            Sprites::drawErase(xPos[i], yPos[i] - yOffset, (const uint8_t*)pgm_read_dword_near(&Images::Lock_Chains_Mask[i]), 0);
            Sprites::drawSelfMasked(xPos[i], yPos[i] - yOffset, (const uint8_t*)pgm_read_dword_near(&Images::Lock_Chains[i]), 0);

        }

    }

}


// ----------------------------------------------------------------------------
//  Render key .. 
//
void playGame_RenderKey(uint8_t yOffset) {

    // Render key ..

    if (playGameVars.key.getKeyLocation() != KeyLocation::None) {

        int8_t x = playGameVars.key.getXPosition();
        int8_t y = playGameVars.key.getYPosition(yOffset);
        uint8_t index = playGameVars.key.getImage();

        if (index == NO_IMAGE) {

            index = arduboy.getFrameCount() % 59 / 15;
            Sprites::drawErase(x, y, Images::Key_Spin_Mask, index);
            Sprites::drawSelfMasked(x, y, Images::Key_Spin, index);

        }
        else {

            if (playGameVars.key.getDisplay()) {
                Sprites::drawErase(x, y, Images::Key_Rotate_Mask, index);
                Sprites::drawSelfMasked(x, y, Images::Key_Rotate, index);
            }

        }

    }

}
