#include <Arduboy2.h>


void renderPlayer() {

    uint8_t frame = player.getHealth() > 0 ? (arduboy.getFrameCount(24) / 6) : 0;
    uint8_t headImage = 255;
    uint8_t y = 36;

    switch (player.getMovement()) {

        case Movement::IdleLeft:
            headImage = 0;
            frame = 0;
            break;

        case Movement::WalkingLeft:
            headImage = 0;
            break;

        case Movement::IdleRight:
            headImage = 1;
            frame = 4;
            break;

        case Movement::WalkingRight:
            headImage = 1;
            frame = frame + 4;
            break;

        case Movement::CrouchLeft:
        case Movement::CrouchRight:
            frame = (player.getMovement() == Movement::CrouchRight);
            Sprites::drawExternalMask(player.getX(), 37, Images::Player_Crouch, Images::Player_Crouch_Mask, frame, frame);
            frame = 255;
            break;

        case Movement::Hidden:
            Sprites::drawSelfMasked(player.getX(), 37, Images::Player_Hidden, 0);
            frame = 255;
            break;

        default: break;

    }

    if (frame < 255) {

        Sprites::drawExternalMask(player.getX() + 1, y + (frame == 1 ? 1 : 0), Images::Player_Head, Images::Player_Head_Mask, headImage, headImage);
        Sprites::drawExternalMask(player.getX(), y + 9, Images::Common, Images::Common_Mask, frame, frame);

    }

}

void renderGuard(Guard &guard) {

    uint8_t frame = 0;
    uint8_t headImage = 255;

    int16_t xPosition = guard.getX() + stealthGameVars.world.buildingXPos;

    if (player.getHealth() > 0) {

        if (!player.getShowReticle()) {
            frame = arduboy.getFrameCount(24) / 6;
        }
        else {
            frame = arduboy.getFrameCount(96) / 24; 
        }

    }

    switch (guard.getMovement()) {

        case Movement::IdleLeft:
            headImage = 0;
            frame = 0;
            break;

        case Movement::WalkingLeft:
            headImage = 0;
            break;

        case Movement::IdleRight:
            headImage = 1;
            frame = 4;
            break;

        case Movement::WalkingRight:
            headImage = 1;
            frame = frame + 4;
            break;

        case Movement::LookingUpLeft:
        case Movement::LookingUpRight:
            frame = (guard.getMovement() == Movement::LookingUpRight);
            Sprites::drawExternalMask(xPosition, 36, Images::Guard_LookUp, Images::Guard_LookUp_Mask, frame, frame);
            frame = 255;
            break;

        default: break;

    }

    if (frame < 255 && headImage < 255) {
        Sprites::drawExternalMask(xPosition + 1, 36 + (frame == 1 ? 1 : 0), Images::Guard_Head, Images::Guard_Head_Mask, headImage, headImage);
        Sprites::drawExternalMask(xPosition, 45, Images::Common, Images::Common_Mask, frame, frame);
    }

    if (guard.getAlertCount() > 0) {

        Sprites::drawExternalMask(xPosition + 5, 25, Images::Guard_Alert, Images::Guard_Alert_Mask, 0, 0);

    }

    
    frame = (frame % 4) / 2;

    switch (guard.getMovement()) {

        case Movement::IdleLeft:
        case Movement::WalkingLeft:
            Sprites::drawSelfMasked(xPosition - 16, 44, Images::Guard_Flashlight, frame);
            break;

        case Movement::IdleRight:
        case Movement::WalkingRight:
            Sprites::drawSelfMasked(xPosition + 14, 44, Images::Guard_Flashlight, 2 + frame);
            break;

        case Movement::LookingUpLeft:
        case Movement::LookingUpRight:
            if (guard.getSteps() > 64 || arduboy.getFrameCountHalf(32)) {
                Sprites::drawExternalMask(xPosition + 4, 24, Images::Guard_Question, Images::Guard_Question_Mask, 0, 0);
            }                    
            break;

        default: break;

    }

}

void renderForeground(const int16_t xOffset) {


    // Ground ..

    for (int16_t x = 6 + xOffset; x < 243; x = x + 32) {
        Sprites::drawOverwrite(x, 53, Images::Footpath, 0);
    }


    // LightPoles

    uint8_t lpi = 0;
    bool flickeringLight = false;

    for (int16_t x = 6 + xOffset; x < (SPACING_FOREGROUND * 3); x = x + 81) {

        if (x > -BUILDING_LIGHTPOLE_WIDTH && x < WIDTH) {

            Sprites::drawExternalMask(x, 12, Images::LightPole_Top, Images::LightPole_Top_Mask, 0, 0);
            Sprites::drawExternalMask(x, 20, Images::LightPole_Base, Images::LightPole_Base_Mask, 0, 0);

            if (stealthGameVars.questionCount > 0) {

                if (lpi == stealthGameVars.lampPoleIdx) {

                    uint8_t flicker = random(18, 40);

                    if (arduboy.getFrameCountHalf(flicker)) {

                        arduboy.drawFastHLine(x + 10, 16, 8, BLACK);
                        flickeringLight = true;

                        #ifdef USE_LED_ANALOGUE
                        arduboy.setRGBled(BLUE_LED, flicker - 18);
                        #endif

                    }

                }

            }

        }

        if (x >= - BUILDING_LIGHTPOLE_WIDTH + BUILDING_LIGHTPOLE_BULB_X && x <= WIDTH - BUILDING_LIGHTPOLE_BULB_X - BUILDING_LIGHTPOLE_BULB_WIDTH) {

            ReticleTarget &target = stealthGameVars.getReticleTarget(stealthGameVars.reticleTargetCount);
            target.x = x + 7;
            target.y = 10;
            target.type = TargetType::LightPole;
            target.buildingIdx = lpi;
            stealthGameVars.reticleTargetCount++;

        }

        lpi++;

    }

    if (!flickeringLight) {
        #ifdef USE_LED_ANALOGUE
        arduboy.setRGBled(BLUE_LED, 0);
        #endif
    }

    if (stealthGameVars.questionCount > 0) {
        stealthGameVars.questionCount--;
    }

}


void renderBackground(const int16_t xOffset) {

    for (uint8_t i = 0; i < 14; i++) {

        int16_t x = pgm_read_byte(&coords[i * 3]) + xOffset;
        int16_t y = pgm_read_byte(&coords[i * 3 + 1]);
        int16_t z = pgm_read_byte(&coords[i * 3 + 2]);

        if (i % 2 == 0) {
            arduboy.drawFastHLine(x, y, z);
        }
        else {
            arduboy.drawFastVLine(x, y, z);
        }

    }

}

void renderTallBuilding(const int16_t x, const int8_t door) {

    arduboy.drawFastVLine(x, 0, 52, BLACK);    
    arduboy.fillRect(x + 1, 0, 23, 52, WHITE);
    arduboy.drawFastVLine(x + 24, 0, 52, BLACK);    
    arduboy.drawFastVLine(x + 25, 0, 52, WHITE);    
    arduboy.fillRect(x + 26, 0, 19, 52, BLACK);
    arduboy.drawFastVLine(x + 45, 0, 52, WHITE);    

    Sprites::drawOverwrite(x + 3, 23, Images::Building_Sign_00, 0);
    Sprites::drawOverwrite(x + 4, 33, Images::Building_Door, door);
}

void renderMedBuilding(const int16_t x, const int8_t door) {

    Sprites::drawExternalMask(x, 18, Images::Building_Med, Images::Building_Med_Mask, 0, 0);
    Sprites::drawOverwrite(x + 6, 33, Images::Building_Door, door);

}

void renderLowBuilding(const int16_t x, const int8_t door) {

    Sprites::drawOverwrite(x, 18, Images::Building_Roof, 0);
    arduboy.fillRect(x + 3, 25, 20, 27, WHITE);
    arduboy.fillRect(x, 22, 59, 3, BLACK);
    arduboy.fillRect(x + 23, 26, 33, 26, BLACK);
    arduboy.drawFastHLine(x + 1, 23, 57);    
    arduboy.drawFastHLine(x + 23, 25, 33);    
    arduboy.drawFastVLine(x + 56, 25, 27);    
    Sprites::drawOverwrite(x + 25, 32, Images::Building_DarkSide_00, 0);
    Sprites::drawOverwrite(x + 39, 32, Images::Building_DarkSide_01, 0);
    Sprites::drawOverwrite(x + 6, 27, Images::Building_Sign_01, 0);
    Sprites::drawOverwrite(x + 6, 33, Images::Building_Door, door);

}

void renderGauges() {


    // Health ..
    
    arduboy.fillRect(0, 1, 14 + (player.getHealth() / 32), 7, BLACK);    
    Sprites::drawSelfMasked(1, 1, Images::Alert, 0);
    
    for (uint8_t x = 0; x < player.getHealth() / 32; x++) {

        arduboy.drawFastVLine(8 + (x * 2), 2, 5);

    }



    // Memory ..
    
    arduboy.fillRect(106, 1, 9 + (player.getMemory() * 2), 7, BLACK);    
    Sprites::drawSelfMasked(107, 2, Images::Memory, 0);
    for (uint8_t x = 0; x < player.getMemory(); x++) {

        arduboy.drawFastVLine(115 + (x * 2), 2, 5);

    }

}

void renderTerminals() {


    // Terminals ..

    for (uint8_t i = BUILDING_COUNT / 2; i < BUILDING_COUNT; i++)  {

        Building building = stealthGameVars.buildings[i];
        int16_t xPos = building.x + stealthGameVars.world.foregroundXPos;

        if (xPos >= - BUILDING_TERMINAL_WIDTH && xPos <= WIDTH) {

            Sprites::drawOverwrite(xPos, 39, Images::Terminal, 0);

        }

    }

}