#include "src/utils/Arduboy2Ext.h"
#include "src/entities/Entities.h"
#include "src/images/Images.h"
#include "src/utils/Constants.h"
#include "src/utils/EEPROM_Utils.h"
#include "src/utils/Structs.h"
#include "src/sounds/Sounds.h"

#ifdef SOUNDS
//#include <ArduboyTones.h>
#include "src/sounds/Sounds.h"
#endif

Arduboy2Ext arduboy;

#ifdef SOUNDS
ArduboyTones sound(arduboy.audio.enabled);
#endif

TitleScreenVars titleScreenVars;
GameOverScreenVars gameOverScreenVars;
GamePlayVars gamePlayVars;
Particle particles[Constants::ParticlesMax];

GameState gameState = GameState::Splash_Init;
GameMode gameMode = GameMode::Single;
GameRotation gameRotation = GameRotation::Landscape;

Player player1;
Player player2;
Mothership mothership;
Bomb bomb;

void setup() {
  
    arduboy.boot();
    arduboy.systemButtons();
    arduboy.setFrameRate(60);
    arduboy.initRandomSeed();
    arduboy.audio.begin();

    #ifdef SOUNDS
    arduboy.audio.begin();
    #endif

    EEPROM_Utils::initEEPROM(false);
    gameMode = EEPROM_Utils::getMode();
    gameRotation = EEPROM_Utils::getRotation();

}   


void loop() {

    if ( !arduboy.nextFrame() ) return;    
	arduboy.pollButtons();

    switch (gameState) {

        case GameState::Splash_Init:

            splashScreen_Init();
            splashScreen();
            break;

        case GameState::Splash:

            splashScreen();
            break;

        case GameState::Title_Init:

            title_Init();
            title();
            break;

        case GameState::Title:

            title();
            break;

        case GameState::Game_Init:

            game_Init();
            game();
            break;

        case GameState::Game:

            game();
            break;

        case GameState::TugOfWar_Init:

            tugOfWar_Init();
            tugOfWar();
            break;

        case GameState::TugOfWar:

            tugOfWar();
            break;
            
        case GameState::GameOver_Init:

            gameOver_Init();
            gameOver();
            break;

        case GameState::GameOver:

            gameOver();
            break;

    }

    arduboy.display(true);

}
