#include <Arduboy2.h>


// ----------------------------------------------------------------------------
//  Render player indoors ..
//
void renderPlayer_Indoors(int8_t xOffset) {

    Sprites::drawExternalMask(player.getXDisplay() + xOffset, 29, Images::Player, Images::Player_Mask, player.getFrame(), player.getFrame());

    switch (player.getXPosition()) {

        case XPosition::Position_Tipping_LH:

            if (player.getOilLevel() > 0) {

                Sprites::drawOverwrite(xOffset, 38, Images::Bucket_Tilt_LH, player.getOilLevel() - 1);

            }

            break;

        case XPosition::Position_Tipping_RH:

            if (player.getOilLevel() > 0) {

                Sprites::drawOverwrite(126 + xOffset, 37, Images::Bucket_Tilt_RH, player.getOilLevel() - 1);

            }

            break;

        default:

            if (player.getOilLevel() > 0) {

                Sprites::drawErase(player.getXDisplay() + 1 + xOffset, 33, Images::Bucket, player.getOilLevel() - 1);

            }

            break;

    }            

}


// ----------------------------------------------------------------------------
//  Render player outdoors ..
//
void renderPlayer_Outdoors(int8_t xOffset, uint8_t yOffset) {

    switch (player.getXPosition()) {

        case XPosition::Position_Outside_LH:
            Sprites::drawExternalMask(15 + xOffset, 14 - yOffset, Images::Player_Outside, Images::Player_Outside_Mask, 0, 0);
            break;

        case XPosition::Position_Throwing_LH:
            Sprites::drawExternalMask(15 + xOffset, 14 - yOffset, Images::Player_Outside, Images::Player_Outside_Mask, 1, 1);
            break;

        case XPosition::Position_Outside_RH:
            Sprites::drawExternalMask(88 + xOffset, 14 - yOffset, Images::Player_Outside, Images::Player_Outside_Mask, 2, 2);
            break;

        case XPosition::Position_Throwing_RH:
            Sprites::drawExternalMask(88 + xOffset, 14 - yOffset, Images::Player_Outside, Images::Player_Outside_Mask, 3, 3);
            break;

        default: break;

    }

}


// ----------------------------------------------------------------------------
//  Render player outdoors ..
//
void renderThrowingOil(int8_t xOffset, uint8_t yOffset) {

    switch (throwOil) {

        case ThrowOil::LH_Top:
            Sprites::drawErase(13 + xOffset, 31 - yOffset, Images::ThrowOil_LH, 0);
            break;

        case ThrowOil::LH_Middle:
            Sprites::drawErase(12 + xOffset, 35 - yOffset, Images::ThrowOil_LH, 1);
            break;

        case ThrowOil::LH_Bottom:
            Sprites::drawErase(13 + xOffset, 39 - yOffset, Images::ThrowOil_LH, 2);
            break;

        case ThrowOil::LH_Miss_Down_Start ... ThrowOil::LH_Miss_Down_End:
            Sprites::drawErase(12 + xOffset - ((static_cast<uint8_t>(throwOil) - static_cast<uint8_t>(ThrowOil::LH_Miss_Down_Start)) / 4), 39 + ((static_cast<uint8_t>(throwOil) - static_cast<uint8_t>(ThrowOil::LH_Miss_Down_Start)) * 4) - yOffset, Images::ThrowOil_LH, 1);
            break;

        case ThrowOil::LH_Miss_Bottom_Start ... ThrowOil::LH_Miss_Bottom_End_Of_Splash:
            Sprites::drawErase(-1 + xOffset, 70 - yOffset, Images::Oil_Splash, static_cast<uint8_t>(throwOil) - static_cast<uint8_t>(ThrowOil::LH_Miss_Bottom_Start));
            break;

        case ThrowOil::RH_Top:
            Sprites::drawErase(112 + xOffset, 31 - yOffset, Images::ThrowOil_RH, 0);
            break;

        case ThrowOil::RH_Middle:
            Sprites::drawErase(113 + xOffset, 35 - yOffset, Images::ThrowOil_RH, 1);
            break;

        case ThrowOil::RH_Bottom:
            Sprites::drawErase(112 + xOffset, 39 - yOffset, Images::ThrowOil_RH, 2);
            break;

        case ThrowOil::RH_Miss_Down_Start ... ThrowOil::RH_Miss_Down_End:
            Sprites::drawErase(113 + xOffset + ((static_cast<uint8_t>(throwOil) - static_cast<uint8_t>(ThrowOil::RH_Miss_Down_Start)) / 4), 39 + ((static_cast<uint8_t>(throwOil) - static_cast<uint8_t>(ThrowOil::RH_Miss_Down_Start)) * 4) - yOffset, Images::ThrowOil_LH, 1);
            break;

        case ThrowOil::RH_Miss_Bottom_Start ... ThrowOil::RH_Miss_Bottom_End_Of_Splash:
            Sprites::drawErase(105 + xOffset, 70 - yOffset, Images::Oil_Splash, static_cast<uint8_t>(throwOil) - static_cast<uint8_t>(ThrowOil::RH_Miss_Bottom_Start));
            break;


        default: break;

    }

}

// ----------------------------------------------------------------------------
//  Render catcher ..
//
void renderCatcher(int8_t xOffset, uint8_t yOffset) {

    Sprites::drawExternalMask(catcher.getXDisplay() + xOffset, 41 - yOffset, Images::Catcher, Images::Catcher_Mask, catcher.getFrame(), catcher.getFrame());

}


// ----------------------------------------------------------------------------
//  Render catcher map ..
//
void renderCatcherMap(int8_t xOffset) {

    arduboy.drawFastVLine(player.getXDisplayMap() - 4 + xOffset, 21, 3, BLACK);
    arduboy.drawFastVLine(catcher.getXDisplayMap() - 4 + xOffset, 27, 3, BLACK);

}


// ----------------------------------------------------------------------------
//  Render bystanders
//
void renderBystanders(int8_t xOffset, uint8_t yOffset) {

    uint8_t bystander1 = 0;
    uint8_t bystander2 = 0;
    
    switch (throwOil) {

        case ThrowOil::LH_Miss_Bottom_Start ... ThrowOil::LH_Miss_Bottom_End:
            bystander1 = 1;
            bystander2 = 2;
            break;

        case ThrowOil::RH_Miss_Bottom_Start ... ThrowOil::RH_Miss_Bottom_End:
            bystander1 = 0;
            bystander2 = 3;
            break;

        default: 
            bystander1 = 0;
            bystander2 = 2;
            break;
    
    }

    Sprites::drawExternalMask(2 + xOffset, 76 - yOffset, Images::Bystanders, Images::Bystanders_Mask, bystander1, bystander1);
    Sprites::drawExternalMask(108 + xOffset, 76 - yOffset, Images::Bystanders, Images::Bystanders_Mask, bystander2, bystander2);

}

// ----------------------------------------------------------------------------
//  Render oil droplets ..
//
void renderOils(GameScene &gameScene, uint8_t yOffset) {

    for (uint8_t x = 0; x < Constants::number_Of_Oils; x++) {

        Oil oil = oils.getOil(x); 

        renderOil(gameScene, oil, 0, outdoorsYOffset);

    }

}

// ----------------------------------------------------------------------------
//  Render oil droplets indoors ..
//
void renderOil(GameScene gameScene, Oil &oil, int8_t xOffset, uint8_t yOffset) {

    if (oil.getYPosition() != YPosition::None) {

        if (gameScene == GameScene::Indoors) {

            switch (oil.getYPosition()) {

                case YPosition::StartDrip_00 ... YPosition::StartDrip_01:
                    Sprites::drawExternalMask(oil.getXDisplay() + xOffset, oil.getYDisplay(), Images::Oil_Drop, Images::Oil_Drop_Mask, 0, 0);
                    break;

                case YPosition::StartDrip_02 ... YPosition::StartDrip_03:
                    Sprites::drawExternalMask(oil.getXDisplay() + xOffset, oil.getYDisplay(), Images::Oil_Drop, Images::Oil_Drop_Mask, 1, 1);
                    break;

                case YPosition::Falling_04 ... YPosition::Falling_13:
                    Sprites::drawExternalMask(oil.getXDisplay() + xOffset, oil.getYDisplay(), Images::Oil_Drop, Images::Oil_Drop_Mask, 2, 2);
                    break;

                case YPosition::Fire_00 ... YPosition::Fire_03:
                    Sprites::drawExternalMask(oil.getXDisplay() - 1 + xOffset, 24, Images::Oil_Fire, Images::Oil_Fire_Mask, static_cast<uint8_t>(oil.getYPosition()) - static_cast<uint8_t>(YPosition::Fire_00), static_cast<uint8_t>(oil.getYPosition()) - static_cast<uint8_t>(YPosition::Fire_00));
                    break;

                case YPosition::Fire_04 ... YPosition::Fire_07:
                    Sprites::drawExternalMask(oil.getXDisplay() - 1 + xOffset, 24, Images::Oil_Fire, Images::Oil_Fire_Mask, static_cast<uint8_t>(YPosition::Fire_07) - static_cast<uint8_t>(oil.getYPosition()), static_cast<uint8_t>(YPosition::Fire_07) - static_cast<uint8_t>(oil.getYPosition()));
                    break;
                
                default: break;

            }

        }
        else {

            switch (oil.getYPosition()) {

                case YPosition::StartDrip_00 ... YPosition::Falling_13:
                    arduboy.drawPixel(oil.getXDisplayMap() + xOffset, oil.getYDisplayMap() - yOffset, BLACK);
                    break;

                case YPosition::Fire_00 ... YPosition::Fire_03:
                    if (oil.getX() == 2) {
                        Sprites::drawErase(oil.getXDisplayMap() + xOffset - 1, 20 - yOffset + 8, Images::Oil_Fire_Map, static_cast<uint8_t>(oil.getYPosition()) - static_cast<uint8_t>(YPosition::Fire_00));
                    }
                    break;

                case YPosition::Fire_04 ... YPosition::Fire_07:
                    if (oil.getX() == 2) {
                        Sprites::drawErase(oil.getXDisplayMap() + xOffset - 1, 20 - yOffset + 8, Images::Oil_Fire_Map, static_cast<uint8_t>(YPosition::Fire_07) - static_cast<uint8_t>(oil.getYPosition()));
                    }
                    break;
                
                default: break;

            }

        }

    }    

}

// ----------------------------------------------------------------------------
//  Render scoreboards ..
//
void renderScoreboard(GameScene gameScene) {

    uint8_t digits[3] = {};
    extractDigits(digits, score);
    
    switch (gameScene) {

        case GameScene::Indoors:

            for (uint8_t i = 3, x = 111; i > 0; i--, x = x + 4) {

                Sprites::drawErase(x, 10, Images::Font, digits[i - 1]);

            }

            if (numberOfLives_Indoors < 3) {

                for (uint8_t i = 0, x = 120; i < 3 - numberOfLives_Indoors; i++, x = x - 8) {

                    Sprites::drawErase(x, 16, Images::Life, 0);

                }

                Sprites::drawErase(108, 24, Images::Miss, 0);

            }

            break;

        case GameScene::Outdoors:

            for (uint8_t i = 3, x = 1; i > 0; i--, x = x + 4) {

                Sprites::drawErase(x, 1, Images::Font, digits[i - 1]);

            }

            if (numberOfLives_Outdoors < 3) {

                for (uint8_t i = 0, x = 123; i < 3 - numberOfLives_Outdoors; i++, x = x - 5) {

                    Sprites::drawExternalMask(x, 1, Images::outsideMiss, Images::outsideMiss_Mask, 0, 0);

                }

            }

            break;

    }

}

