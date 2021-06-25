#include <Arduboy2.h>



// ----------------------------------------------------------------------------
//  Initialise game ..
//
void playGame_Init() {

    lifeReset();

    arduboy.setFrameRate(40);
    oils.reset();
    player.reset();
    
    gameState = GameState::PlayGame;
    gameScene = GameScene::Indoors;

    frameRate = 50;
    score = 0;
    gameOver = false;
    gameOverCounter = 0;
    numberOfLives_Indoors = 3;
    numberOfLives_Outdoors = 3;
    outdoorsYOffset = 0;
    throwOil = ThrowOil::None;

}


// ----------------------------------------------------------------------------
//  Reset life at start of game and after each death ..
//
void lifeReset() {

    counter = 10;
    outdoorsYOffset = 0;
    gameOverCounter = 0;

}


// ----------------------------------------------------------------------------
//  Update game and render ..
//
void playGame(void) {

    uint8_t justPressedButton = arduboy.justPressedButtons();


    // Handle LEDS ..

    if (ledGreenCounter > 0) {

        ledGreenCounter--;

        switch (ledGreenCounter) {

            case 1 ... Constants::led_Green_Delay: 
            case (Constants::led_Green_Delay * 2) + 1 ... (Constants::led_Green_Delay * 3): 
            case (Constants::led_Green_Delay * 4) + 1 ... (Constants::led_Green_Delay * 5):  
                arduboy.setRGBled(GREEN_LED, Constants::LED_Brightness);
                break;

            default:
                arduboy.setRGBled(GREEN_LED, 0);
                break;

        }

    }

    if (ledRedCounter > 0) {

        ledRedCounter--;

        if (ledRedCounter == 0) {

            arduboy.setRGBled(RED_LED, 0);

        }
        else {

            arduboy.setRGBled(RED_LED, Constants::LED_Brightness);

        }

    }

    if (gameOverCounter > 0) {
        gameOverCounter--;
    }


    // Update entity positions ..

    if (!gameOver && arduboy.isFrameCount(4)) {
        
        catcher.update(score);

    }

    if (arduboy.isFrameCount(2)) {

        if (updateThrowOil(throwOil)) {

            gameOver = true;
            gameOverCounter = 32;

        }

    }


    // Update oil positions

    for (uint8_t x = 0; x < Constants::number_Of_Oils; x++) {

        Oil &oil = oils.getOil(x);

        switch (oil.getYPosition()) {

            case YPosition::StartDrip_00 ... YPosition::Falling_13:
                {

                    uint8_t frameCount = 0;


                    // Drop slower if player is outside ..

                    switch (player.getXPosition()) {

                        case XPosition::Position_Throwing_LH ... XPosition::Position_Tipping_LH:
                        case XPosition::Position_Tipping_RH ... XPosition::Position_Throwing_RH:
                            frameCount = 16;
                            break;

                        default:
                            frameCount = 8;
                            break;

                    }

                    if (arduboy.isFrameCount(frameCount)) {

                        #ifndef SOUNDS
                        if (oil.update()) {
                        #else
                        if (oil.update(sound)) {
                        #endif

                            if (numberOfLives_Indoors > 0) {
                                
                                #ifdef SOUNDS
                                sound.tones(Sounds::fireStarted);
                                #endif
                                numberOfLives_Indoors--;
                                ledRedCounter = Constants::led_Red_Delay;

                                if (numberOfLives_Indoors == 0) {

                                    gameOver = true;
                                    gameOverCounter = 32;

                                }

                            }

                        }

                    }

                }

                break;

            case YPosition::Fire_00 ... YPosition::Fire_07:

                if (arduboy.isFrameCount(2)) {

                    #ifndef SOUNDS
                    oil.update();
                    #else
                    oil.update(sound);
                    #endif

                }
                break;

            default: break;

        }

    }


    // Launch Oil ..

    if (!gameOver) {

        oils.launchOil(score);

    }


    switch (gameScene) {

        case GameScene::Indoors:

            if (!gameOver) {

                if (arduboy.isFrameCount(2)) {

                    player.update();

                }

                if (justPressedButton & LEFT_BUTTON)               { player.decXPosition(); }
                if (justPressedButton & RIGHT_BUTTON)              { player.incXPosition(); }
                if (justPressedButton & B_BUTTON)                  { player.incXPosition(); }

            }
            else if (gameOver && gameOverCounter == 0) {

                if (justPressedButton & A_BUTTON)                   { gameState = GameState::HighScore_Init; }

            }


            // Change scene?

            if (player.getXPosition() == XPosition::Position_Outside_LH || player.getXPosition() == XPosition::Position_Outside_RH) {

                gameScene = GameScene::Outdoors;
                throwOil = ThrowOil::None;

            }


            // Did we catch some oil ?

            switch (player.getXPosition()) {

                case XPosition::Position_5_Oil:
                case XPosition::Position_10_Oil:
                case XPosition::Position_15_Oil:
                    catchOil(player.getXPosition());
                    break;

                default: break;

            }


            break;

        case GameScene::Outdoors:

            if (!gameOver) {

                if (justPressedButton & LEFT_BUTTON) {

                    switch (player.getXPosition()) {
                        
                        case XPosition::Position_Outside_LH:

                            if (catcher.isCatching(Direction::Left)) {

                                score = score + player.getOilLevel();
                                ledGreenCounter = pgm_read_byte(&Constants::ledGreenDelays[player.getOilLevel() - 1]);

                                #ifdef SOUNDS
                                sound.tones(Sounds::scoreAdded);
                                #endif

                            }

                            if (throwOil == ThrowOil::None && player.getOilLevel() > 0) throwOil = ThrowOil::LH_Top;
                            player.setXPosition(XPosition::Position_Throwing_LH);
                            player.setOilLevel(0);
                            break;

                        case XPosition::Position_Throwing_RH:
                            player.setXPosition(XPosition::Position_Outside_RH);
                            break;

                        case XPosition::Position_Outside_RH:
                            gameScene = GameScene::Indoors; 
                            player.decXPosition();
                            break;

                        default: break;

                    }

                }

                if ((justPressedButton & RIGHT_BUTTON) || (justPressedButton & B_BUTTON)) { 

                    switch (player.getXPosition()) {
                        
                        case XPosition::Position_Outside_RH:

                            if (catcher.isCatching(Direction::Right)) {

                                score = score + player.getOilLevel();
                                ledGreenCounter = pgm_read_byte(&Constants::ledGreenDelays[player.getOilLevel() - 1]);

                                #ifdef SOUNDS
                                sound.tones(Sounds::scoreAdded);
                                #endif

                            }

                            if (throwOil == ThrowOil::None && player.getOilLevel() > 0) throwOil = ThrowOil::RH_Top;
                            player.setOilLevel(0);
                            player.setXPosition(XPosition::Position_Throwing_RH);
                            break;

                        case XPosition::Position_Throwing_LH:
                            player.setXPosition(XPosition::Position_Outside_LH);
                            break;

                        case XPosition::Position_Outside_LH:
                            gameScene = GameScene::Indoors; 
                            player.incXPosition();
                            break;

                        default: break;

                    }

                }

            }
            else if (gameOver && gameOverCounter == 0) {

                if (justPressedButton & A_BUTTON)                   { gameState = GameState::HighScore_Init; }

            }

            break;

    }



    // --------------------------------------------------------------------------
    //  Render the screen ..

    switch (gameScene) {

        case GameScene::Indoors:

            Sprites::drawOverwrite(0, 0, Images::Indoors, 0);
            renderPlayer_Indoors(0);
            renderCatcherMap(0);
            break;
            

        case GameScene::Outdoors:
    
            for (uint8_t x = 0; x < 13; x++) {

                Sprites::drawOverwrite(0, 0 - outdoorsYOffset + (x * 8), Images::Outdoors, x);

            }

            renderPlayer_Outdoors(0, outdoorsYOffset);
            renderThrowingOil(0, outdoorsYOffset);
            renderCatcher(0, outdoorsYOffset);
            renderBystanders(0, outdoorsYOffset);
            break;

    }

    renderOils(gameScene, outdoorsYOffset);
    renderScoreboard(gameScene);


    // Game over ?

    if (gameOver && gameOverCounter == 0) {

        Sprites::drawExternalMask(34, 24, Images::GameOver, Images::GameOver_Mask, 0, 0);

    }

}


void catchOil(XPosition xPosition) {

    for (uint8_t x = 0; x < Constants::number_Of_Oils; x++) {

        Oil &oil = oils.getOil(x); 

        if (oil.getYPosition() != YPosition::None && oil.getXPosition() == xPosition) {

            switch (oil.getYPosition()) {

                case YPosition::Falling_09 ... YPosition::Falling_11:
                    if (player.incOilLevel()) {

                        oil.setYPosition(YPosition::None);
                        score++;
                        #ifdef SOUNDS
                        sound.tones(Sounds::catchOil);
                        #endif

                    }
                    break;

                default: break;
                
            }
 
        }       

    }

}


bool updateThrowOil(ThrowOil &throwOil) {

    switch (throwOil) {

        case ThrowOil::LH_Top:
        case ThrowOil::RH_Top:
        case ThrowOil::LH_Miss_Bottom_Start ... ThrowOil::LH_Miss_Bottom_End:
        case ThrowOil::RH_Miss_Bottom_Start ... ThrowOil::RH_Miss_Bottom_End:
            throwOil++;
            break;

        case ThrowOil::LH_Middle:
            if (catcher.isCatching(Direction::Left)) {
                throwOil = ThrowOil::LH_Bottom;
            }
            else {

                throwOil = ThrowOil::LH_Miss_Down_Start;
                #ifdef SOUNDS
                sound.tones(Sounds::falling);
                #endif

            }
            break;
            
        case ThrowOil::LH_Bottom:
        case ThrowOil::RH_Bottom:
            throwOil = ThrowOil::None;
            break;

        case ThrowOil::LH_Miss_Down_Start ... ThrowOil::LH_Miss_Down_End:
            
            throwOil++;
            outdoorsYOffset = outdoorsYOffset + 4;
            
            if (throwOil == ThrowOil::LH_Miss_Bottom_Start) {
            
                if (numberOfLives_Outdoors > 0) {

                    numberOfLives_Outdoors--;
                    ledRedCounter = Constants::led_Red_Delay;;

                    if (numberOfLives_Outdoors == 0) {

                        return true;
                        
                    }

                }

            }
            break;

        case ThrowOil::LH_Miss_Up_Start ... ThrowOil::LH_Miss_Up_NearlyEnd:
            if (!gameOver) {
                throwOil++;
                outdoorsYOffset = outdoorsYOffset - 4;
            }
            break;

        case ThrowOil::RH_Middle:
            if (catcher.isCatching(Direction::Right)) {
                throwOil = ThrowOil::RH_Bottom;
            }
            else {

                throwOil = ThrowOil::RH_Miss_Down_Start;
                #ifdef SOUNDS
                sound.tones(Sounds::falling);
                #endif

            }
            break;

        case ThrowOil::RH_Miss_Down_Start ... ThrowOil::RH_Miss_Down_End:

            throwOil++;
            outdoorsYOffset = outdoorsYOffset + 4;

            if (throwOil == ThrowOil::RH_Miss_Bottom_Start) {

                if (numberOfLives_Outdoors > 0) {      

                    numberOfLives_Outdoors--;
                    ledRedCounter = Constants::led_Red_Delay;;

                    if (numberOfLives_Outdoors == 0) {

                        return true;
                        
                    }

                }

            }
            break;

        case ThrowOil::RH_Miss_Up_Start ... ThrowOil::RH_Miss_Up_NearlyEnd:
            if (!gameOver) {
                throwOil++;
                outdoorsYOffset = outdoorsYOffset - 4;
            }
            break;

        default: break;

    }

    return false;

}