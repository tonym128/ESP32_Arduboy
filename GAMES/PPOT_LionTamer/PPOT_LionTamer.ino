#include "src/utils/Arduboy2Ext.h"
#include "src/images/Images.h"
#include "src/utils/Consts.h"
#include "src/utils/EEPROM_Utils.h"
#include "src/entities/Entities.h"
#include "src/sounds/Sounds.h"
#include <ArduboyTones.h>

Arduboy2Ext arduboy;

#ifdef SOUNDS
    ArduboyTones sound(arduboy.audio.enabled);
#endif

Player player1;
Player player2;
Lion lion1;
Lion lion2;
Chair chair;

#ifdef SHOW_ROAR
    Explosions explosions;
    #ifdef SOUNDS
        SoundVars soundVars;
    #endif
#endif

Direction lionAttacking = Direction::None;
uint8_t lionAttackingIndex = 0;

#ifdef SPLASH
    GameState gameState = GameState::Splash_Init;
#else
    GameState gameState = GameState::Title_Init;
#endif

GameMode gameMode = GameMode::Easy;

uint8_t frameRate = 50;
int16_t counter = 10;
uint16_t score = 0;
uint8_t numberOfLives = 3;
uint8_t ledDelay = 0;
uint8_t marqueeCounter = 0;
uint8_t resetCounter = 0;

bool gameOver = false;

#ifdef SOUNDS
    uint8_t soundCounter = 0;
    bool sounds = arduboy.audio.enabled();
#endif

void setup(void) {

	arduboy.boot();
	arduboy.flashlight();
	arduboy.systemButtons();

    #ifdef SOUNDS    
	    arduboy.audio.begin();
    #endif

	arduboy.initRandomSeed();
	arduboy.setFrameRate(40);
    arduboy.setRGBled(0, 0, 0);

    EEPROM_Utils::initEEPROM(false);
	
}

void loop(void) {

	if (!arduboy.nextFrame()) return;

	arduboy.pollButtons();

    switch (gameState) {

        #ifdef SPLASH

            case GameState::Splash_Init:

                splashScreen_Init();
                gameState = GameState::Splash;
                [[fallthrough]]

            case GameState::Splash:

                splashScreen();
                arduboy.display();
                break;

        #endif

        #ifdef SHOW_INSTUCTIONS

            case GameState::Instructions_Init:
                instructions_Init();
                gameState = GameState::Instructions;
                [[fallthrough]]

            case GameState::Instructions:
                instructions();
                arduboy.displayClearToWhite();
                break;

        #endif

        case GameState::Title_Init:

            gameState = GameState::Title;
            counter = -1;
            marqueeCounter = 0;
            numberOfLives = 3;
            gameOver = false;
            arduboy.setFrameRate(60);

            #ifdef SOUNDS
                soundCounter = 0;
                sound.tones(Sounds::Title);
            #endif

            #ifdef SHOW_ROAR
                explosions.reset();
            #endif
            [[fallthrough]]

        case GameState::Title:

            title();
            arduboy.displayClearToWhite();
            break;

        case GameState::PlayGame_Init:

            arduboy.setFrameRate(40);
            playGame_Init();
            score = 0;
            [[fallthrough]]

        case GameState::PlayGame:

            playGame();

            if (frameRate != 40 + (score / 8)) {
                frameRate = 40 + (score / 8);
                arduboy.setFrameRate(frameRate);
            }
            arduboy.displayClearToWhite();
            break;

    }


}
