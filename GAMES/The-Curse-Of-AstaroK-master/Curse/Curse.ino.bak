#include "src/utils/Arduboy2Ext.h"
#include "src/entities/Entities.h"
#include "src/fonts/Font3x6.h"
#include "src/images/Images.h"
#include "src/utils/Utils.h"
#include "src/utils/Constants.h"
#include "src/utils/Enums.h"
#include "src/utils/Structs.h"
#include "src/utils/FadeEffects.h"
#include "src/ardBitmap/ArdBitmap.h"

#include <ESP8266WiFi.h>
#include <ArduboyTones.h>
#include "src/sounds/Sounds.h"

#define SOUND_BUTTONS
#ifndef DEBUG
//ARDUBOY_NO_USB
#endif

FadeInEffect fadeInEffect;
Arduboy2Ext arduboy;
ArdBitmap<WIDTH, HEIGHT> ardBitmap;
Font3x6 font3x6;

ArduboyTones sound(arduboy.audio.enabled);

GameStateType gameState = GameStateType::SplashScreen_Activate; 

SplashScreenStateVars splashScreenVars;
TitleScreenStateVars titleScreenVars;
PlayGameStateVars playGameVars;
GameOverStateVars gameOverVars;
int8_t mrBlinky = 127;

void setup() {
  WiFi.mode(WIFI_OFF);
  EEPROM.begin(100);
	arduboy.boot();
	#ifndef SOUND_BUTTONS
	arduboy.systemButtons();
	#endif
	arduboy.setFrameRate(40);
	arduboy.audio.begin();

}

void loop() {

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
			break;

		case GameStateType::TitleScreen_Activate:
			titleScreen_Activate();
			gameState = GameStateType::TitleScreen;
			[[fallthrough]];

		case GameStateType::TitleScreen:
			titleScreen_Update();
			titleScreen_Render();
			break;

		case GameStateType::PlayGame_Activate:
			playGame_Activate();
			gameState = GameStateType::PlayGame;
			[[fallthrough]];

		case GameStateType::PlayGame:
			playGame_Update();
			playGame_Render();
			break;

		default: break;

	}

}
