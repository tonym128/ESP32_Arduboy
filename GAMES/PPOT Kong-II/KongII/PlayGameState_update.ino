#include "src/utils/Arduboy2Ext.h"

// ----------------------------------------------------------------------------
//    Handle state updates .. 
//
void playGame_Update() { 

	auto pressed = arduboy.pressedButtons();
    auto justPressed = arduboy.justPressedButtons();

    if (!playGame_GetPaused() && !playGameVars.player.isDead()) {


        // Handle player movements ..

        if (playGameVars.playing && arduboy.everyXFrames(2)) {

            playGameVars.player.getXPosition(true);

            if (!playGameVars.player.isJumping() && !playGameVars.player.isFalling() && !playGameVars.exitSequence) {


                // If the player is running from right to left, the controls are reversed ..

                if (playGameVars.player.canMove(Movements::Reverse)) {

                    if ((pressed & LEFT_BUTTON) && playGameVars.player.canMove(Movements::Left)) {
                        playGameVars.player.incPlayerPosition();
                    }

                    if ((pressed & RIGHT_BUTTON) && playGameVars.player.canMove(Movements::Right)) {
                        playGameVars.player.decPlayerPosition(false);
                    }

                }
                else {

                    if ((pressed & RIGHT_BUTTON) && playGameVars.player.canMove(Movements::Right)) {
                        playGameVars.player.incPlayerPosition();
                    }

                    if ((pressed & LEFT_BUTTON) && playGameVars.player.canMove(Movements::Left)) {
                        playGameVars.player.decPlayerPosition(false);
                    }

                }

                if ((pressed & DOWN_BUTTON) && playGameVars.player.canMove(Movements::Down)) {
                    playGameVars.player.decPlayerPosition(false);
                }

                if ((pressed & UP_BUTTON) && playGameVars.player.canMove(Movements::Up)) {
                    
                    playGameVars.player.incPlayerPosition();

                    if (playGameVars.player.getPosition() == PLAYER_CHAIN_0_END - 1 && playGameVars.key.getKeyLocation() == KeyLocation::Position_00 && !playGameVars.kong.getFlashChain(0)) {

                        playGame_CheckLastKey(0);
                        
                    }
                    else if (playGameVars.player.getPosition() == PLAYER_CHAIN_1_END - 1 && playGameVars.key.getKeyLocation() == KeyLocation::Position_01 && !playGameVars.kong.getFlashChain(1)) {

                        playGame_CheckLastKey(1);
                        
                    }
                    else if (playGameVars.player.getPosition() == PLAYER_CHAIN_2_END - 1 && playGameVars.key.getKeyLocation() == KeyLocation::Position_02 && !playGameVars.kong.getFlashChain(2)) {

                        playGame_CheckLastKey(2);
                        
                    }
                    else if (playGameVars.player.getPosition() == PLAYER_CHAIN_3_END - 1 && playGameVars.key.getKeyLocation() == KeyLocation::Position_03 && !playGameVars.kong.getFlashChain(3)) {

                        playGame_CheckLastKey(3);
                        
                    }

                }


                if (playGameVars.preventJumpDelay == 0 && (pressed & A_BUTTON) && (playGameVars.player.canMove(Movements::Jump) || playGameVars.player.canMove(Movements::JumpUp))) {

                    if (playGameVars.key.getKeyLocation() == KeyLocation::LowerPosition && playGameVars.player.getPosition() >= PLAYER_POS_LOWERKEY_START && playGameVars.player.getPosition() <= PLAYER_POS_LOWERKEY_END) {

                        playGameVars.key.setPath(Coordinates::LowerKey);

                        #ifdef PLAY_SOUNDS 
                            sound.tones(Sounds::Key);
                        #endif

                    }

                    if (playGameVars.key.getKeyLocation() == KeyLocation::UpperPosition && playGameVars.player.getPosition() >= PLAYER_POS_UPPERKEY_START && playGameVars.player.getPosition() <= PLAYER_POS_UPPERKEY_END) {

                        while (true) {

                            uint8_t r = random(0, 4);

                            if (!playGameVars.kong.getChain(r)) {
                                continue;
                            }

                            if (playGameVars.kong.getChain(r)) {
                                
                                playGameVars.key.setPath((const uint8_t *)pgm_read_dword_near(&Coordinates::Positions[r]));
                        
                                #ifdef PLAY_SOUNDS 
                                    sound.tones(Sounds::Key);
                                #endif

                                break;

                            }

                        }

                    }

                    playGameVars.player.startJump();

                    #ifdef PLAY_SOUNDS 
                        sound.tones(Sounds::Jump);
                    #endif                   

                }

            }

            // Player is jumping or falling ..

            else {

                if (playGameVars.player.isJumping()) {

                    playGameVars.player.updateJump();


                    // Have we jumped a clapper or a spark?

                    if (playGameVars.player.atTopOfJump()) {

                        uint16_t position = playGameVars.player.getPosition();
                        bool points = false;

                        if (position >= PLAYER_CLAPPER_LOWER_START && position < PLAYER_CLAPPER_LOWER_END) {

                            for (uint8_t i = 0; i < CLAPPERS_LOWER_COUNT; i++) {

                                auto &clapper = playGameVars.lowerClappers.getClapper(i);

                                if (clapper.isEnabled() && absT(playGameVars.player.getXPosition(false) - clapper.getXPosition()) < JUMP_POINT_SPAN) {

                                    points = true;
                                    break;

                                } 

                            }

                        }

                        if (position >= PLAYER_SPARK_START && position < PLAYER_SPARK_END) {

                            for (uint8_t i = 0; i < SPARKS_LOWER_COUNT; i++) {

                                auto &spark = playGameVars.lowerSparks.getSpark(i);

                                if (spark.isEnabled() && absT(playGameVars.player.getXPosition(false) - spark.getXPosition()) < JUMP_POINT_SPAN) {

                                    points = true;
                                    break;

                                } 

                            }

                        }

                        if (position >= PLAYER_CLAPPER_UPPER_START && position < PLAYER_CLAPPER_UPPER_END) {

                            for (uint8_t i = 0; i < CLAPPERS_UPPER_COUNT; i++) {

                                auto &clapper = playGameVars.upperClappers.getClapper(i);

                                if (clapper.isEnabled() && absT(playGameVars.player.getXPosition(false) - clapper.getXPosition()) < JUMP_POINT_SPAN) {

                                    points = true;
                                    break;

                                } 

                            }

                        }

                        if (points) {

                            #ifdef PLAY_SOUNDS 
                                sound.tones(Sounds::JumpOver);
                            #endif

                            gameStats.score++;

                        }

                    }

                    if (playGameVars.player.isJumping() && playGameVars.player.canGrabChain()) {

                        switch (playGameVars.player.getPosition()) {

                            case PLAYER_VINE_0_MIN ... PLAYER_VINE_0:
                                playGameVars.player.setJumpPosition(0);
                                playGameVars.player.setPosition(PLAYER_VINE_0_CLIMBING);
                                break;

                            case PLAYER_VINE_1_MIN ... PLAYER_VINE_1:
                                playGameVars.player.setJumpPosition(0);
                                playGameVars.player.setPosition(PLAYER_VINE_1_CLIMBING);
                                break;

                            case PLAYER_CHAIN_0_GROUND_MIN ... PLAYER_CHAIN_0_GROUND_MAX - 1:
                                playGameVars.player.setJumpPosition(0);
                                playGameVars.player.setPosition(PLAYER_CHAIN_0_CLIMBING);
                                break;

                            case PLAYER_CHAIN_1_GROUND_MIN ... PLAYER_CHAIN_1_GROUND_MAX - 1:
                                playGameVars.player.setJumpPosition(0);
                                playGameVars.player.setPosition(PLAYER_CHAIN_1_CLIMBING);
                                break;

                            case PLAYER_CHAIN_2_GROUND_MIN ... PLAYER_CHAIN_2_GROUND_MAX - 1:
                                playGameVars.player.setJumpPosition(0);
                                playGameVars.player.setPosition(PLAYER_CHAIN_2_CLIMBING);
                                break;

                            case PLAYER_CHAIN_3_GROUND_MIN ... PLAYER_CHAIN_3_GROUND_MAX - 1:
                                playGameVars.player.setJumpPosition(0);
                                playGameVars.player.setPosition(PLAYER_CHAIN_3_CLIMBING);
                                break;
                                

                        }

                    }

                }

                if (!playGameVars.exitSequence && playGameVars.player.isFalling()) {
                    playGameVars.player.decPlayerPosition(false);
                }

            }

        }

        uint8_t yOffset = playGameVars.player.getYOffset();


        // If we are in the middle of the exit sequence, move the player and kong automatically ..

        if (playGameVars.exitSequence) {

            if (playGameVars.playing && arduboy.everyXFrames(2)) {

                playGameVars.key.updatePosition();
                playGameVars.kong.updateChains();

                uint16_t position = playGameVars.player.getPosition();

                switch (position) {

                    case PLAYER_CHAIN_0_BACKTOGROUND + 1 ... PLAYER_CHAIN_0_END:
                    case PLAYER_CHAIN_1_BACKTOGROUND + 1 ... PLAYER_CHAIN_1_END:
                    case PLAYER_CHAIN_2_BACKTOGROUND + 1 ... PLAYER_CHAIN_2_END:
                    case PLAYER_CHAIN_3_BACKTOGROUND + 1 ... PLAYER_CHAIN_3_END:
                        if (arduboy.everyXFrames(4)) {
                            playGameVars.player.decPlayerPosition(true);
                        }
                        break;

                    case PLAYER_CHAIN_MOVE_TO_EXIT_0 ... PLAYER_CHAIN_MOVE_TO_EXIT_END:
                        if (arduboy.everyXFrames(2)) {
                            playGameVars.player.incPlayerPosition();
                        }
                        break;

                }

                bool exitComplete = playGameVars.kong.updatePosition();

                if (exitComplete) {

                    playGameVars.kong.reset();
                    playGameVars.player.setPosition(0);
                    playGameVars.exitSequence = false;

                    playGame_ScrollToBottom(false);

                }

            }

        }
        else {
                

            // Update entity positions ..

            if (playGameVars.playing && arduboy.everyXFrames(2)) {

                bool updateChains = playGameVars.kong.updateChains();

                playGameVars.lowerClappers.updatePositions();
                playGameVars.upperClappers.updatePositions();
                playGameVars.key.updatePosition();
                playGameVars.birds.updatePositions();

                if (updateChains) {

                    playGameVars.player.setExit(true);
                    
                }

            }

            if (playGameVars.playing && arduboy.everyXFrames(3)) {

                playGameVars.lowerSparks.updatePositions();

            }

            if (playGameVars.playing && arduboy.everyXFrames(4)) {

                playGameVars.upperSparks.updatePositions();

            }

        }


        // Have we been hit by anything?

        #ifndef IGNORE_COLLISIONS
                
            Rect playerRect = playGameVars.player.getRect();

            for (uint8_t i = 0; i < NUMBER_OF_ENTITIES; i++) {

                auto &lowerClapper = playGameVars.lowerClappers.getClapper(i);

                if (lowerClapper.isEnabled()) {

                    Rect clapperRect = lowerClapper.getRect(yOffset, gameStats.mode);

                    if (arduboy.collide(playerRect, clapperRect)) {

                        playGame_KillPlayer();

                    }

                }

                auto &upperClapper = playGameVars.upperClappers.getClapper(i);

                if (upperClapper.isEnabled()) {

                    Rect clapperRect = upperClapper.getRect(yOffset, gameStats.mode);

                    if (arduboy.collide(playerRect, clapperRect)) {

                        playGame_KillPlayer();
                        
                    }

                }


                if (playGameVars.player.getPosition() > PLAYER_SPARK_LOWER_IGNORE) {

                    auto &lowerSpark = playGameVars.lowerSparks.getSpark(i);

                    if (lowerSpark.isEnabled()) {

                        Rect sparkRect = lowerSpark.getRect(yOffset, gameStats.mode);

                        if (arduboy.collide(playerRect, sparkRect)) {

                            playGame_KillPlayer();

                        }

                    }

                }

                auto &upperSpark = playGameVars.upperSparks.getSpark(i);

                if (upperSpark.isEnabled()) {

                    Rect sparkRect = upperSpark.getRect(yOffset, gameStats.mode);

                    if (arduboy.collide(playerRect, sparkRect)) {

                        playGame_KillPlayer();

                    }

                }

                auto &bird = playGameVars.birds.getBird(i);

                if (bird.isEnabled()) {

                    Rect birdRect = bird.getRect(yOffset, gameStats.mode);

                    if (arduboy.collide(playerRect, birdRect)) {

                        playGame_KillPlayer();
                        
                    }

                }

            }

        #endif

    }


    // End of Game?

    if (playGameVars.playing && gameStats.numberOfLivesLeft == 0) {

        gameStats.gameOver = true;
        playGameVars.playing = false;

        #ifdef PLAY_SOUNDS 
            sound.tones(Sounds::GameOver);
        #endif

    }


    // Handle the transition to a new life ..

    switch (playGameVars.introDelay) {

        case INTRO_DELAY_FROM_TITLE_SCROLL:

            playGame_ScrollToBottom(true);

            if (playGameVars.clappersLowerDelay > CLAPPERS_LOWER_MIN) {

                playGameVars.clappersLowerDelay = playGameVars.clappersLowerDelay - CLAPPERS_LOWER_INC;
                playGameVars.lowerClappers.setDelayMax(playGameVars.clappersLowerDelay, true);

            }

            if (playGameVars.clappersUpperDelay > CLAPPERS_UPPER_MIN) {

                playGameVars.clappersUpperDelay = playGameVars.clappersUpperDelay - CLAPPERS_UPPER_INC;
                playGameVars.upperClappers.setDelayMax(playGameVars.clappersUpperDelay, true);

            }

            if (playGameVars.sparksLowerDelay > SPARKS_LOWER_MIN) {

                playGameVars.sparksLowerDelay = playGameVars.sparksLowerDelay - SPARKS_LOWER_INC;
                playGameVars.lowerSparks.setDelayMax(playGameVars.sparksLowerDelay, true);

            }

            if (playGameVars.sparksUpperDelay > SPARKS_UPPER_MIN) {

                playGameVars.sparksUpperDelay = playGameVars.sparksUpperDelay - SPARKS_UPPER_INC;
                playGameVars.upperSparks.setDelayMax(playGameVars.sparksUpperDelay, true);

            }

            if (playGameVars.birdsDelay > BIRDS_MIN) {

                playGameVars.birdsDelay = playGameVars.birdsDelay - BIRDS_INC;
                playGameVars.birds.setDelayMax(playGameVars.birdsDelay, true);

            }

            playGameVars.frameRate = playGameVars.frameRate + 2;
            arduboy.setFrameRate(playGameVars.frameRate);
            
            break;

        case 1:
            if (gameStats.numberOfLivesLeft > 0) {
                playGameVars.player.reset();
                playGameVars.playing = true;
                gameStats.numberOfLivesLeft--;
            }
            break;

        default:
            break;

    }

    decToZero(playGameVars.introDelay);


    // Handle other buttons ..

    if (playGameVars.preventJumpDelay > 0) playGameVars.preventJumpDelay--;

    if (!playGameVars.playing && !gameStats.gameOver) {

        if (playGameVars.introDelay < 100 && ((justPressed & A_BUTTON) || (justPressed & B_BUTTON))) {

            playGameVars.playing = true;
            playGameVars.introDelay = 0;
            playGameVars.player.reset();
            gameStats.numberOfLivesLeft--;

            if (justPressed & A_BUTTON) {
                playGameVars.preventJumpDelay = 20;
            }

        }

    }
    else {    

        playGame_HandleCommonButtons();

    }

}


void playGame_CheckLastKey(uint8_t chain) {

    playGameVars.kong.setFlashChain(chain, true);
    playGameVars.key.setFlash(true);

    gameStats.score = gameStats.score + random(8, 16);

    if (gameStats.mode == GameMode::Easy) {

        playGameVars.upperClappers.reset(54);
        playGameVars.birds.reset();                            
        
    }

    #ifdef PLAY_SOUNDS 
        sound.tones(Sounds::Unlock);
    #endif

    for (uint8_t i = 0; i < 4 ; i++) {

        if (playGameVars.kong.getDisplayChain(i)) return;

    }

    playGameVars.lowerClappers.reset(107);
    playGameVars.upperClappers.reset(54);
    playGameVars.lowerSparks.reset(Coordinates::LowerSpark);
    playGameVars.upperSparks.reset(Coordinates::UpperSpark);
    playGameVars.birds.reset();
    playGameVars.exitSequence = true;

}

void playGame_ScrollToBottom(bool renderKong) {

    playGame_DrawScenery(0);
    if (renderKong) playGame_RenderKong(0);
    arduboy.displayWithBackground(Background::None);
    delay(1000);

    for (uint8_t i = 0; i < 64 ; i++) {

        playGame_DrawScenery(i);
        playGame_RenderKey(i);
        playGame_RenderScore(i);
        if (renderKong) playGame_RenderKong(i);
        arduboy.displayWithBackground(Background::None);
        delay(20);
    
    }

}
