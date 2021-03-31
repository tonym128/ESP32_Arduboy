#include <Arduboy2.h>


// ----------------------------------------------------------------------------
//  Render background ..
//
void renderBackground() {

    Sprites::drawOverwrite(0, 0, Images::Scenery_LH, 0);
    Sprites::drawOverwrite(106, 0, Images::Scenery_RH, 0);
    Sprites::drawOverwrite(27, 0 , Images::Cage, 0);

    if (gameMode == GameMode::Easy) {

        Sprites::drawExternalMask(91, 0, Images::FenceRight, Images::FenceRight_Mask, 0, 0);
        Sprites::drawExternalMask(112, 30, Images::Player_Sitting, Images::Player_Sitting_Mask, 0, 0);

    }

}


// ----------------------------------------------------------------------------
//  Render scoreboards ..
//
void renderScoreBoards(uint16_t score, uint8_t numberOfLives) {

    Sprites::drawOverwrite(-1, 56, Images::Scoreboard_LH, 0);
    Sprites::drawOverwrite(114, 56, Images::Scoreboard_RH, 0);

    uint8_t digits[3] = {};
    extractDigits(digits, score);
    
    for (uint8_t i = 3, x = 1; i > 0; i--, x = x + 4) {

        Sprites::drawErase(x, 58, Images::Font, digits[i - 1]);

    }

    for (uint8_t x = 126, y = numberOfLives; y > 0; x = x - 2, y--) {

        arduboy.drawFastVLine(x, 58, 5, BLACK);

    }

}


// ----------------------------------------------------------------------------
//  Render player ..
//
void renderPlayer(Player &player, const uint8_t *playerImages, const uint8_t *playerMask) {

    uint8_t playerFrame = 0;

    if (arduboy.frameCount % 8 < 4) {

        YPosition playerY = player.getYPosition();
        playerFrame = ((playerY == lion1.getYPosition() && lion1.getXPosition() <= XPosition::LH_Attack) || (playerY == lion2.getYPosition() && lion2.getXPosition() >= XPosition::RH_Attack));

    }

    if (!player.getRunning()) {

        Sprites::drawExternalMask(player.getXDisplay(), player.getYDisplay(), playerImages, playerMask, playerFrame, playerFrame);

    }
    else {

        uint8_t frame = 0;

        switch (player.getXPosition()) {

            case XPosition::LH_Attacking_Up:
            case XPosition::RH_Attacking_Up:
                frame = 0;
                break;

            case XPosition::LH_Attacking_Down:
            case XPosition::RH_Attacking_Down:
                frame = 1;
                break;

            default: break;

        }

        Sprites::drawExternalMask(player.getXDisplay() + (player.getIndex() == Constants::Player2_Index ? 12 : 0), player.getYDisplay(), Images::Player_Up_Down, Images::Player_Up_Down_Mask, frame, 0);

    }

}



// ----------------------------------------------------------------------------
//  Render lion ..
//
void renderLion(Lion &lion) {

    Sprites::drawExternalMask(lion.getXDisplay(), lion.getYDisplay(), Images::Lion, Images::Lion_Mask, lion.getFrame(), lion.getFrame());

    if (lion.getRunning() && (lion.getXPosition() == XPosition::LH_Attacking_Up || lion.getXPosition() == XPosition::RH_Attacking_Up)) {
        uint8_t frame = (arduboy.frameCount % 4 < 2);
        Sprites::drawExternalMask(lion.getXDisplay(), lion.getYDisplay() + 24, Images::Lion_Cloud, Images::Lion_Cloud_Mask, frame, frame);

    }

}