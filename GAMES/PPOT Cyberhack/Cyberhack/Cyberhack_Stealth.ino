#include <Arduboy2.h>

const uint8_t PROGMEM offsets[] = { 0, 8, 8, 6 };

const uint8_t PROGMEM coords[] = { 
    0, 29, 17, 
    17, 12, 18,    
    17, 12, 14,
    31, 12, 14,
    31, 21, 14, 
    45, 21, 6,
    45, 27, 10,
    55, 5, 23,
    55, 5, 23,
    78, 5, 26,
    78, 26, 18,
    96, 14, 13,
    96, 14, 19,
    115, 14, 20,
};


void stealthGame_Init() {
                
    gameState = GameState::StealthGame;
    stealthGameVars.reset();
    player.resetStealth();

    uint8_t max = BUILDING_COUNT / 2;
    uint16_t x = 10;

    for (uint8_t i = 0; i < max; i++) {

        BuildingType buildingType = static_cast<BuildingType>(random(static_cast<uint8_t>(BuildingType::Min), static_cast<uint8_t>(BuildingType::Max) + 1));
        Building &building = stealthGameVars.buildings[i];
        building.type = buildingType;
        building.x = x;

        x = x + random(BUILDING_LOW_WIDTH + 10, BUILDING_LOW_WIDTH + 80);

    }

    x = 30;

    for (uint8_t i = max; i < BUILDING_COUNT; i++) {

        Building &building = stealthGameVars.buildings[i];
        building.type = BuildingType::Terminal;
        building.x = x;

        x = x + random(100, 300);

    }

    uint16_t span = -(Constants::Town_RightPos) / Constants::Max_Guards;
    uint16_t walkSpan = span + 60;

    for (uint8_t i = 0; i < Constants::Max_Guards; i++) {

        Guard &guard = stealthGameVars.guards[i];
        guard.setSpan(walkSpan);
        guard.setMovement(i % 2 == 0 ? Movement::WalkingRight : Movement::WalkingLeft);
        guard.setX(70 + (span / 2) + (span * i));
        guard.setSteps(random((walkSpan / 4), (walkSpan / 2)));
        guard.setSteps(random((walkSpan / 4), (walkSpan / 2)));
        guard.setAlerted(false);

    }

    #ifdef BYTE_BEAT_SOUNDS_3
    byteBeatIdx = ByteBeatIndex::Street;
    #endif

}

void stealthGame() {

	auto justPressed = arduboy.justPressedButtons();
	auto pressed = arduboy.pressedButtons();


    //SJH
    // if (arduboy.justPressed(B_BUTTON)) {

    //     gameState = GameState::ShowMap;
    //     mapGameVars.currentLocation = mapGameVars.nextLocation;
    //     // arduboy.invert(false); 

    //     if (mapGameVars.currentLocation == MapMenu::CommercialDistrict) {
    //         mapGameVars.menu_Index = player.getFirstAvailDeck(false) - 1;
    //     }

    // }


    uint16_t distToGuard = 512;

    arduboy.invert(false); 
    
    if (player.getHealth() > 0 && (!player.getShowReticle() || arduboy.getFrameCount(4) == 0)) { 

        if (arduboy.isFrameCount(128)) {

            if (player.incMemory()) {
                #ifdef ARDUBOY_TONES_SOUNDS
                sound.tones(Sounds::MemoryFull);
                #endif
            }

        }

        if (arduboy.isFrameCount(64)) {

            player.incHealth();

        }


        if (arduboy.isFrameCount(2)) {


            // Close doors ..

            for (uint8_t i = 0; i < BUILDING_COUNT; i++) {

                bool closing = stealthGameVars.buildings[i].incDoor();

                if (closing) {
                    player.setMovement(Movement::IdleRight);
                }

            }


            // Move background if necessary ..

            if (stealthGameVars.moveOffset < 0) {

                stealthGameVars.world.inc(0);
                stealthGameVars.moveOffset++;

            }
            else if (stealthGameVars.moveOffset > 0) {

                stealthGameVars.world.dec(0);
                stealthGameVars.moveOffset--;

            }

        }


        // Handle guard movements ..

        for (uint8_t i = 0; i < Constants::Max_Guards; i++) {
            
            Guard &guard = stealthGameVars.guards[i];

            int16_t guardXPos = guard.getX() + stealthGameVars.world.buildingXPos;
            int16_t distToGuard_Test = guardXPos - player.getX();

            if (abs(distToGuard_Test) < distToGuard && !guard.isDazedAndConfused()) {

                distToGuard = abs(distToGuard_Test);

            }

            if (distToGuard_Test <= Constants::Dist_To_Torch) {

                if (!player.isHiddenOrCrouching()) {

                    if (!guard.isDazedAndConfused()) guard.setAlerted(true);

                }

            }

            guard.move(-distToGuard_Test);


        }

    }

    if (distToGuard <= Constants::Dist_To_Torch && !player.isHiddenOrCrouching()) {

        player.decHealth();

    }


    // Handle player movements (if not mission over!)..

    if (player.getHealth() > 0) {
            
        if (justPressed & A_BUTTON || player.getShowReticle()) {

            if (player.getMemory() >= 3) {

                switch (player.getMovement()) {

                    case Movement::WalkingLeft:
                        player.setMovement(Movement::IdleLeft);
                        break;

                    case Movement::WalkingRight:
                        player.setMovement(Movement::IdleRight);
                        break;

                    default: break;
                        
                }

                if (!player.getShowReticle()) {
                    player.setShowReticle(true);
                    arduboy.invert(true);
                    justPressed = 0;
                    #ifdef ARDUBOY_TONES_SOUNDS
                    sound.tones(Sounds::ReticleShown);
                    #endif
                }

                if (justPressed & LEFT_BUTTON) {
                    if (stealthGameVars.reticleTargetIdx > 0) {
                        stealthGameVars.reticleTargetIdx--;
                    }
                    else {
                        stealthGameVars.reticleTargetIdx = stealthGameVars.reticleTargetCount - 1;
                    }
                }

                else if (justPressed & RIGHT_BUTTON) {
                    if (stealthGameVars.reticleTargetIdx < stealthGameVars.reticleTargetCount - 1) {
                        stealthGameVars.reticleTargetIdx++;
                    }
                    else {
                        stealthGameVars.reticleTargetIdx = 0;
                    }
                }

                if (justPressed & A_BUTTON) {
                    
                    arduboy.invert(true);
                    player.decMemory(3);
                    player.setShowReticle(false);
                    
                    ReticleTarget &reticleTarget = stealthGameVars.getReticleTarget(stealthGameVars.reticleTargetIdx);

                    if (reticleTarget.type == TargetType::Door) {

                        stealthGameVars.buildings[reticleTarget.buildingIdx].door = 1;

                    }

                    if (reticleTarget.type == TargetType::LightPole) {

                        stealthGameVars.questionCount = 255;
                        stealthGameVars.lampPoleIdx = reticleTarget.buildingIdx;

                        for (uint8_t i = 0; i < Constants::Max_Guards; i++) {

                            Guard &guard = stealthGameVars.guards[i];
                            int16_t guardXPos = guard.getX() + stealthGameVars.world.buildingXPos;

                            if (guardXPos >= -30 && guardXPos < WIDTH + 30) {

                                if (guardXPos < reticleTarget.x) {

                                    guard.setMovement(Movement::LookingUpRight);

                                }
                                else {

                                    guard.setMovement(Movement::LookingUpLeft);

                                }

                                guard.setSteps(255);

                            }

                        }

                    }

                    #ifdef ARDUBOY_TONES_SOUNDS
                        sound.tones(Sounds::ReticleSelect);
                    #endif


                }

                if (justPressed & B_BUTTON) {
                    
                    arduboy.invert(true);
                    player.setShowReticle(false);

                }

            }
            else {

                #ifdef ARDUBOY_TONES_SOUNDS
                    sound.tones(Sounds::InvalidChoice);
                #endif


            }

        }
        else {

            player.setShowReticle(false);
                
            if (justPressed & LEFT_BUTTON) {
                player.setMovement(Movement::IdleLeft);
            }

            else if (pressed & LEFT_BUTTON) {

                player.setMovement(Movement::WalkingLeft);

                if (player.getX() > Constants::Player_CentrePos) {

                    player.decX();

                }
                else if (stealthGameVars.world.buildingXPos < 130) {

                    stealthGameVars.world.inc(arduboy.getFrameCount(2));

                }
                else if (stealthGameVars.world.buildingXPos == 130) {

                    if (player.getX() > 2) {
                        player.decX();
                    }

                }

            }

            else if (player.getMovement() == Movement::WalkingLeft) {

                player.setMovement(Movement::IdleLeft);

            }
            


            // Right ..

            if (justPressed & RIGHT_BUTTON) {

                player.setMovement(Movement::IdleRight);

            }
            else if (pressed & RIGHT_BUTTON) {

                player.setMovement(Movement::WalkingRight);

                if (player.getX() < Constants::Player_CentrePos) {

                    player.incX();

                }
                else {

                    if (stealthGameVars.world.buildingXPos > Constants::Town_RightPos) {
                    
                        stealthGameVars.world.dec(arduboy.getFrameCount(2));

                    }
                    else {

                        if (player.getX() < 128) {
                            player.incX();
                        }
                        else {

                            #ifdef TV_TRANSITION
                                gotoMapNextState(GameState::TV_Init);
                            #else
                                gotoMap();
                            #endif
                            mapGameVars.currentLocation = mapGameVars.nextLocation;


                            // Prime menu index if moving to commerical district ..

                            if (mapGameVars.currentLocation == MapMenu::CommercialDistrict) {
                                mapGameVars.menu_Index = player.getFirstAvailDeck(false) - 1;
                            }


                        }

                    }

                }

            }

            else if (player.getMovement() == Movement::WalkingRight) {

                player.setMovement(Movement::IdleRight);

            }


            // Up button ..

            if (justPressed & UP_BUTTON) {

                int16_t playerXPos = player.getX() - stealthGameVars.world.buildingXPos;
                for (uint8_t i = 0; i < BUILDING_COUNT / 2; i++) {

                    Building &building = stealthGameVars.buildings[i];

                    switch (building.type) {

                        case BuildingType::LowBuilding:
                        case BuildingType::MedBuilding:
                        case BuildingType::HighBuilding:

                            if (building.x - 2 < playerXPos && building.x + 12 > playerXPos && building.getDoorOpen()) {

                                player.setMovement(Movement::Hidden);
                                clearGuardAlerts();
                                stealthGameVars.moveOffset = building.x - playerXPos + pgm_read_byte(&offsets[static_cast<uint8_t>(building.type)]);

                            }
                            break;

                        default: break;

                    }

                }

            }

            // else if (!(pressed & UP_BUTTON)) {

            //     player.setMovement(Movement::IdleRight);

            // }


            // Down button ..

            if (justPressed & DOWN_BUTTON) {

                int16_t playerXPos = player.getX() - stealthGameVars.world.foregroundXPos;

                for (uint8_t i = BUILDING_COUNT / 2; i < BUILDING_COUNT; i++) {
                    
                    Building &building = stealthGameVars.buildings[i];

                    if (building.x - 20 < playerXPos && building.x - 10 > playerXPos) {

                        player.setMovement(Movement::CrouchRight);
                        stealthGameVars.moveOffset = (building.x - playerXPos - 12) / 2;
                        clearGuardAlerts();

                    }

                    else if ((building.x + BUILDING_TERMINAL_WIDTH - 5 < playerXPos) && (building.x + BUILDING_TERMINAL_WIDTH + 5 > playerXPos)) {

                        player.setMovement(Movement::CrouchLeft);
                        stealthGameVars.moveOffset = (building.x - playerXPos + 10) / 2;
                        clearGuardAlerts();

                    }                
        
                }

            }

            // else if (!(pressed & DOWN_BUTTON)) {

            //     switch (player.getMovement()) {

            //         case Movement::CrouchLeft:
            //             player.setMovement(Movement::IdleLeft);
            //             break;

            //         case Movement::CrouchRight:
            //             player.setMovement(Movement::IdleRight);
            //             break;

            //         default: break;

            //     }

            // }

        }

    }

    // Mission over ..

    else {
                    
        if (justPressed & A_BUTTON) {

            switch (mapGameVars.nextLocation) {

                case MapMenu::CommercialDistrict:
                    player.incThreatLevel(10);
                    break;

                default:
                    player.incThreatLevel(5);
                    break;

            }

            if (player.getThreatLevel() == 100) {
                #ifdef TV_TRANSITION
                    mapGameVars.setMessage(Message::Game_Over);
                    gotoMapNextState(GameState::TV_Init);
                    nextState = GameState::Message_Game_Over_Init;
                #else
                    mapGameVars.setMessage(Message::Game_Over);
                    gameState = GameState::Message_Game_Over_Init;
                #endif
            }
            else {
                #ifdef TV_TRANSITION
                    gotoMapNextState(GameState::TV_Init);
                #else
                    gotoMap();
                #endif
            }

        }

    }

    stealthGameVars.resetReticleTargets();

    {
        #define SPACING_BACKGROUND 116
        int16_t x = stealthGameVars.world.backgroundXPos % SPACING_BACKGROUND;
        
        renderBackground(x - SPACING_BACKGROUND);
        renderBackground(x);
        renderBackground(x + SPACING_BACKGROUND);

    }

    for (uint8_t i = 0; i < BUILDING_COUNT / 2; i++)  {

        Building building = stealthGameVars.buildings[i];
        int16_t xPos = building.x + stealthGameVars.world.buildingXPos;

        switch (building.type) {

            case BuildingType::LowBuilding:

                if (xPos >= - BUILDING_LOW_WIDTH && xPos <= WIDTH) {
                    renderLowBuilding(xPos, building.getDoorFrame());
                }

                if (xPos >= - BUILDING_LOW_DOOR_X - (BUILDING_DOOR_WIDTH / 2) && xPos <= WIDTH - BUILDING_LOW_DOOR_X - (BUILDING_DOOR_WIDTH / 2)) {

                    createReticleTarget(i, xPos + BUILDING_LOW_DOOR_X - 1);

                }

                break;

            case BuildingType::MedBuilding:

                if (xPos >= - BUILDING_MED_WIDTH && xPos <= WIDTH) {
                    renderMedBuilding(xPos, building.getDoorFrame());
                }

                if (xPos >= - BUILDING_MED_DOOR_X - (BUILDING_DOOR_WIDTH / 2) && xPos <= WIDTH - BUILDING_MED_DOOR_X - (BUILDING_DOOR_WIDTH / 2)) {

                    createReticleTarget(i, xPos + BUILDING_MED_DOOR_X - 1);

                }

                break;

            case BuildingType::HighBuilding:

                if (xPos >= - BUILDING_HIGH_WIDTH && xPos <= WIDTH) {
                    renderTallBuilding(xPos, building.getDoorFrame());
                }

                if (xPos >= - BUILDING_HIGH_DOOR_X - (BUILDING_DOOR_WIDTH / 2) && xPos <= WIDTH - BUILDING_HIGH_DOOR_X - (BUILDING_DOOR_WIDTH / 2)) {

                    createReticleTarget(i, xPos + BUILDING_HIGH_DOOR_X - 1);

                }

                break;

            default: break;

        }

    }


    {
        #define SPACING_FOREGROUND 243
        int16_t x = (stealthGameVars.world.foregroundXPos % SPACING_FOREGROUND) - SPACING_FOREGROUND;
        renderForeground(x);
        
    }


    renderGauges();


    // Player + Guards

    for (uint8_t i = 0; i < Constants::Max_Guards; i++) {
        renderGuard(stealthGameVars.guards[i]);
    }

    renderPlayer();
    renderTerminals();


    // If the reticle is being diplayed ..

    if (player.getShowReticle() && arduboy.getFrameCountHalf(16)) {

        ReticleTarget &reticleTarget = stealthGameVars.getReticleTarget(stealthGameVars.reticleTargetIdx);
        Sprites::drawExternalMask(reticleTarget.x, reticleTarget.y, Images::Reticle, Images::Reticle_Mask, 0, 0);

    }


    // Mission over?

    if (player.getHealth() == 0) {

        arduboy.fillRect(16, 20, 96, 19, BLACK);
        arduboy.drawRect(17, 21, 94, 17, WHITE);
        font3x5.setCursor(22, 26);
        font3x5.print(F("YOU~HAVE~BEEN~CAPTURED!"));

    }
    
    #ifdef USE_LED_ANALOGUE
        if (distToGuard < 48) {
            arduboy.setRGBled(RED_LED, 48 - distToGuard);
        }
        else {
            arduboy.setRGBled(RED_LED, 0);
        }
    #endif

}

void clearGuardAlerts() {

    for (uint8_t i = 0; i < Constants::Max_Guards; i++) {
        
        Guard &guard = stealthGameVars.guards[i];
        guard.setAlerted(false);

    }

}

void createReticleTarget(uint8_t idx, int16_t x) {

    ReticleTarget &target = stealthGameVars.getReticleTarget(stealthGameVars.reticleTargetCount);
    target.x = x;
    target.y = 36;
    target.buildingIdx = idx;
    target.type = TargetType::Door;
    stealthGameVars.reticleTargetCount++;

}

void gotoMap() {

    gameState = GameState::ShowMap;

    #ifdef BYTE_BEAT_SOUNDS_3
    byteBeatIdx = ByteBeatIndex::Map;
    #endif

}

void gotoMapNextState(GameState next) {

    gameState = next;
    nextState = GameState::ShowMap;

    #ifdef BYTE_BEAT_SOUNDS_3
    byteBeatIdx = ByteBeatIndex::Map;
    #endif

}