#include <Arduboy2.h>



// ----------------------------------------------------------------------------
//  Initialise game ..
//
void playGame_Init() {

    lifeReset();

    arduboy.setFrameRate(50);
    
    gameState = GameState::PlayGame;
    frameRate = 50;
    frameRate = 50;
    counter = 159;

}


// ----------------------------------------------------------------------------
//  Reset life at start of game and after each death ..
//
void lifeReset() {

    chair.reset();

    lionAttacking = Direction::None;
    lionAttackingIndex = 0;

    player1.reset(Constants::Player1_Index, Constants::Player1_YPos); 
    player2.reset(Constants::Player2_Index, Constants::Player2_YPos); 

    lion1.reset(Direction::Left, YPosition::Level_1, 8, Constants::Lion1_Index);
    lion2.reset(Direction::Right, YPosition::Level_3, 8, Constants::Lion2_Index);

}


// ----------------------------------------------------------------------------
//  Update game and render ..
//
void playGame(void) {

    uint8_t justPressedButton = arduboy.justPressedButtons();


    // --------------------------------------------------------------------------
    //  Update entity positions ..

    if (counter > 0) counter--;

    if (ledDelay > 0) {

        ledDelay--;

        if (ledDelay == 0) {

            arduboy.setRGBled(RED_LED, 0);
            arduboy.setRGBled(GREEN_LED, 0);

        }

    }


    // Handle other movements ..

    if (arduboy.getFrameCount(4)) chair.update();

    bool finished = (lion1.getRunning() && (lion1.getYDisplay() < -40 || lion1.getYDisplay() > 100)) ||
                    (lion2.getRunning() && (lion2.getYDisplay() < -40 || lion2.getYDisplay() > 100));


    // Return to main menu ..

    if (numberOfLives == 0 && finished && lionAttacking != Direction::None) {

        if (arduboy.pressed(A_BUTTON))              { gameState = GameState::Title_Init; }

    }


    // Update player positions ..

    if (counter == 0 && lionAttacking == Direction::None/* && arduboy.everyXFrames(4)*/) {

        if (justPressedButton & A_BUTTON)               { player2.incYPosition(); }
        if (justPressedButton & B_BUTTON)               { player2.decYPosition(); }

        if (justPressedButton & UP_BUTTON)              { player1.decYPosition(); }
        if (justPressedButton & DOWN_BUTTON)            { player1.incYPosition(); }

    }


    // Handle lion movements ..

    if (!gameOver) {

        lion1.updateRunning();
        lion2.updateRunning();

    }

    if (counter == 0) {

        if ((lionAttacking == Direction::None || lionAttackingIndex == Constants::Lion1_Index) && arduboy.everyXFrames(lion1.getSpeed()))     moveLion(lion1, lion2);
        if ((lionAttacking == Direction::None || lionAttackingIndex == Constants::Lion2_Index) && arduboy.everyXFrames(lion2.getSpeed()))     moveLion(lion2, lion1);

    }


    // --------------------------------------------------------------------------
    //  Render the screen ..

    renderBackground();


    // Lions ..

    renderLion(lion1);
    renderLion(lion2);


    // Player ..

    if (!gameOver) {

        player1.updateRunning();
        player2.updateRunning();

    }
    
    renderPlayer(player1, Images::Player_01, Images::Player_01_Mask);
    if (gameMode == GameMode::Hard) renderPlayer(player2, Images::Player_02, Images::Player_02_Mask);

    if (!player1.getRunning() && !player2.getRunning()) {

        renderScoreBoards(score, numberOfLives);
        
    }


    // Chair?

    if (chair.getDirection() == Direction::Left) {

        Sprites::drawExternalMask(chair.getXDisplay(), chair.getYDisplay(), Images::Chair_LH, Images::Chair_LH_Mask, chair.getFrame(), chair.getFrame());

    }

    if (chair.getDirection() == Direction::Right) {

        Sprites::drawExternalMask(chair.getXDisplay(), chair.getYDisplay(), Images::Chair_RH, Images::Chair_RH_Mask, chair.getFrame(), chair.getFrame());

    }


    // Is the game over ?

    if (finished && lionAttacking != Direction::None) {

        if (numberOfLives == 0) {

            Sprites::drawExternalMask(34, 24, Images::GameOver, Images::GameOver_Mask, 0, 0);
            gameOver = true;
            EEPROM_Utils::saveScore(gameMode, score);
            renderScoreBoards(score, numberOfLives);

        }
        else {

            lifeReset();
            counter = 159;            

        }

    }


    // Render counter?
    {
        uint8_t frame = 255;

        switch (counter) {

            case 5 ... 39:
                frame = 3;
                break;

            case 45 ... 79:
                frame = 2;
                break;

            case 85 ... 119:
                frame = 1;
                break;

            case 125 ... 159:
                frame = 0;
                break;

            default: break;

        }
     
        if (frame != 255) {
            Sprites::drawExternalMask(55, 24, Images::Count, Images::Count_Mask, frame, 0);
        }

    }

}
