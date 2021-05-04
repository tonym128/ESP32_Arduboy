#include <stdint.h>
#include "src/utils/Constants.h"
#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Utils.h"
#include "src/utils/Structs.h"
#include "src/utils/HighScoreEditor.h"
#include "src/entities/Entities.h"
#include "src/utils/EEPROM_Utils.h"
#include "src/images/Images.h"
#include <FixedPointsCommon.h>

#ifdef SOUNDS
#include <ArduboyTones.h>
#include "src/sounds/Sounds.h"
#endif

Arduboy2Ext arduboy;

#ifndef DEBUG
//ARDUBOY_NO_USB
#endif

#ifdef SOUNDS
ArduboyTones sound(arduboy.audio.enabled);
#endif

Vec3 cameraPos;

#ifdef CACTII
Vec3 cactii[Constants::NumberOfCactii];
#endif

Car car;
OtherCar otherCars[Constants::NumberOfOtherCars];
World world;
GameState gameState = GameState::Splash_Init;
HighScore highScore;


// State variables -------------------------------------------------

HighScoreVars highScoreVars;
GamePlayVars gamePlayVars;
GameOverVars gameOverVars;
TitleVars titleVars;

void setup() {

    arduboy.begin();
    arduboy.setFrameRate(30);

    #ifdef SOUNDS
    arduboy.audio.begin();
    #endif

    EEPROM_Utils::initEEPROM(false);

}

void loop() {

    if (!arduboy.nextFrame()) return;    
	arduboy.pollButtons();

    switch (gameState) {

        case GameState::Splash_Init:

            splashScreen_Init();
            splashScreen();
            arduboy.display(true);
            break;

        case GameState::Splash:

            splashScreen();
            arduboy.display(true);
            break;

        case GameState::Title_Init:

            title_Init();
            title();
            arduboy.display(true);
            break;

        case GameState::Title:

            title();
            arduboy.displayClearToPattern(gamePlayVars.getTimeOfDay_Pattern());
            break;

        case GameState::Game_Init:

            game_Init();
            game();
            arduboy.display(true);
            break;

        case GameState::Game:
            game();
            arduboy.displayClearToPattern(gamePlayVars.getTimeOfDay_Pattern());
            break;

        case GameState::GameOver_Init:

            gameOver_Init();
            gameOver();
            arduboy.displayClearToPattern(gamePlayVars.getTimeOfDay_Pattern());
            break;            

        case GameState::GameOver:

            gameOver();
            arduboy.displayClearToPattern(gamePlayVars.getTimeOfDay_Pattern());
            break;

        case GameState::SaveScore:

            highScore.reset();
            highScore.setSlotNumber(EEPROM_Utils::saveScore(gamePlayVars.distance));
            gameState = GameState::HighScore_Init;
            //[[fallthrough]];

        case GameState::HighScore_Init:

            HighScore_Init();
            //[[fallthrough]];

        case GameState::HighScore:

            renderHighScore(highScore);
            arduboy.display(true);
            break;

    }

}
