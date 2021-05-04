#include <Arduboy2.h>

// ----------------------------------------------------------------------------
//  Initialise state ..
//
void title_Init() {

    #ifdef SOUNDS
        sound.tones(Sounds::Theme);
    #endif
    
    cameraPos.setX(0);
    cameraPos.setY(UPM);
    cameraPos.setZ(-40);

    world.reset();
    gamePlayVars.reset();
    titleVars.reset();

    gameState = GameState::Title;

    #ifdef CACTII
        placeCactii();
    #endif

    #ifdef DEBUG_COLLISIONS
        titleVars.offset = Constants::Title_MaxOffset - 3;
    #endif
    
}   


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void title() {

    // Update position of text ..

    if (titleVars.flyIn && titleVars.offset < Constants::Title_MaxOffset) {

        titleVars.offset = titleVars.offset + 3;

    }

    if (!titleVars.flyIn) {

        if (titleVars.offset > 0) {
            titleVars.offset = titleVars.offset - 3;
        }
        else {

            switch (titleVars.cursor) {
        
                case 0:
                    if (titleVars.seenAim)  {
                        titleVars.titleState = TitleState::SelectTransmission;
                    }
                    else {
                        titleVars.titleState = TitleState::Aim;
                        titleVars.seenAim = true;
                    }
                    titleVars.flyIn = true;
                    break;
        
                case 1:
                    gameState = GameState::HighScore_Init;
                    break;

            }

        }

    }


    draw(false, true);

    switch (titleVars.titleState) {

        case TitleState::StartGame:

            Sprites::drawPlusMask(WIDTH - titleVars.offset, 8, Images::RoadTrip_00, 0);
            Sprites::drawPlusMask(-69 + titleVars.offset, 34, Images::RoadTrip_01, 0);
        
            if (titleVars.offset == Constants::Title_MaxOffset) {

                if (arduboy.justPressed(A_BUTTON)) { 

                    titleVars.flyIn = false;
                    #ifdef DEBUG_COLLISIONS
                        titleVars.offset = 3;
                    #endif

                }         
                
                if (arduboy.justPressed(UP_BUTTON)) { 

                    titleVars.cursor = 0;

                }

                if (arduboy.justPressed(DOWN_BUTTON)) { 

                    titleVars.cursor = 1;

                }

                arduboy.fillRect(84, 8, 44, 15, WHITE);
                arduboy.drawRect(85, 10 + (titleVars.cursor == 1 ? 7 : 0), 3, 3, BLACK);

                Sprites::drawErase(90, 9, Images::Title_Start, 0);
                Sprites::drawErase(90, 16, Images::Title_HighScore, 0);

            }

            break;

        case TitleState::Aim:


            arduboy.fillRect(22, 12, 84, 42, BLACK);
            Sprites::drawOverwrite(22, 12, Images::Corner_TL, 0);
            Sprites::drawOverwrite(22, 46, Images::Corner_BL, 0);
            Sprites::drawOverwrite(103, 12, Images::Corner_TR, 0);
            Sprites::drawOverwrite(103, 46, Images::Corner_BR, 0);

            arduboy.drawFastHLine(25, 13, 78, WHITE);
            arduboy.drawFastHLine(25, 52, 78, WHITE);

            arduboy.drawFastVLine(23, 20, 32, WHITE);
            arduboy.drawFastVLine(104, 20, 32, WHITE);

            Sprites::drawOverwrite(28, 16, Images::Aim, 0);

            if (arduboy.justPressed(A_BUTTON)) { 

                titleVars.titleState = TitleState::SelectTransmission;

            }  

            break;

        case TitleState::SelectTransmission:

            Sprites::drawExternalMask(27, 16, Images::TransmissionType_Auto, Images::TransmissionType_Mask, 0, 0);
            Sprites::drawExternalMask(69, 16, Images::TransmissionType_Manual, Images::TransmissionType_Mask, 0, 0);
            Sprites::drawExternalMask(25 + (car.getTransmissionType() == TransmissionType::Manual ? 42 : 0), 14, Images::TransmissionType_Highlight, Images::TransmissionType_Highlight_Mask, 0, 0);

            if (arduboy.justPressed(A_BUTTON)) { 

                gameState = GameState::Game_Init;

            }         
            
            if (arduboy.justPressed(LEFT_BUTTON)) { 

                car.setTransmissionType(TransmissionType::Auto);

            }

            if (arduboy.justPressed(RIGHT_BUTTON)) { 

                car.setTransmissionType(TransmissionType::Manual);

            }

            break;

    }


    houseKeeping();

    #ifdef CACTII
        moveCactii();
    #endif
    
    cameraPos.setZ(cameraPos.getZ() + 6);
    cameraPos.setY(world.roadHeightAt(cameraPos.getZ() + 2 * UPM) + UPM);


    // Reset the time to ensure a white (day) screen ..

    gamePlayVars.timeOfDay = 700;

}
