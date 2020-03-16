#include <ESP8266WiFi.h>
#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Enums.h"
#include "src/utils/Utils.h"
#include "src/utils/EEPROM_Utils.h"
#include "src/entities/Entities.h"
#include "src/map/Coordinates.h"
#include "src/images/Images.h"
#include "src/sounds/Sounds.h"
#include "src/utils/Structs.h"
//#include <ArduboyTones.h>

#ifdef NO_USB
//ARDUBOY_NO_USB 
#endif


Arduboy2Ext arduboy;
ArduboyTones sound(arduboy.audio.enabled);

SplashScreenStateVars splashScreenVars;
TitleScreenStateVars titleScreenVars;
PlayGameStateVars playGameVars;
HighScoreStateVars highScoreVars;

GameStats gameStats;
GameStateType gameState = GameStateType::SplashScreen_Activate; 

void setup(void) {
  WiFi.mode(WIFI_OFF);
	arduboy.boot();
	arduboy.flashlight();
	arduboy.systemButtons();
	arduboy.audio.begin();
	arduboy.initRandomSeed();
	arduboy.setFrameRate(70);

	EEPROM_Utils::initialiseEEPROM();
	
}

void loop(void) {

	if (!arduboy.nextFrame()) return;

	arduboy.pollButtons();

		switch (gameState) {

		case GameStateType::SplashScreen_Activate:
			splashScreen_Activate();
			gameState = GameStateType::SplashScreen;
			[[fallthrough]];

		case GameStateType::SplashScreen:
			splashScreen_Update();
			splashScreen_Render();
			arduboy.displayWithBackground(Background::Black);
			break;

		case GameStateType::TitleScreen_Activate:
			titleScreen_Activate();
			gameState = GameStateType::TitleScreen;
			[[fallthrough]];

		case GameStateType::TitleScreen:
			titleScreen_Update();
			titleScreen_Render();
			arduboy.displayWithBackground(Background::Black);
			break;

		case GameStateType::PlayGame_Activate:
			playGame_Activate();
			gameState = GameStateType::PlayGame;
			[[fallthrough]];

		case GameStateType::PlayGame:
			playGame_Update();
			playGame_Render();
			arduboy.displayWithBackground(Background::None);
			break;

		case GameStateType::HighScore_Activate:
			highScore_Activate();
			gameState = GameStateType::HighScore;
			[[fallthrough]];

		case GameStateType::HighScore:
			highScore_Update();
			highScore_Render();
			arduboy.displayWithBackground(Background::White);
			break;

		default: break;

	}

}
