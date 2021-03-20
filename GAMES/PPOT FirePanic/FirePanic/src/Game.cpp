/*
	Copyright (C) 2018 Pharap (@Pharap)

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.

	Modified by Filmote and Vampirics
*/

#include "Game.h"

#include "states/States.h"
#include "utils/Utils.h"
#include "utils/EEPROM_Utils.h"

void Game::setup(void) {

	auto & arduboy = this->context.arduboy;

	arduboy.boot();
	arduboy.flashlight();
	arduboy.systemButtons();
	arduboy.audio.begin();
	arduboy.initRandomSeed();
	arduboy.setFrameRate(75);


	EEPROM_Utils::initialiseEEPROM();

	this->currentState = GameStateType::SplashScreen; 
	this->splashScreenState.activate(*this);
	
}

void Game::loop(void) {

	auto &arduboy = this->context.arduboy;
	if (!arduboy.nextFrame()) return;

	arduboy.pollButtons();

	switch (this->currentState) {

		case GameStateType::SplashScreen:

			if (currentState != savedCurrentState) {
				this->context.gameState = this->currentState;
				this->context.nextState = this->nextState;
        this->splashScreenState.activate(*this);
				this->savedCurrentState = this->currentState;
			}
			this->splashScreenState.update(*this);
			this->splashScreenState.render(*this);
			break;

		case GameStateType::TitleScreen: 

			if (currentState != savedCurrentState) {
				this->context.gameState = this->currentState;
				this->context.nextState = this->nextState;
				this->titleScreenState.activate(*this);
				this->savedCurrentState = this->currentState;
			}
			this->titleScreenState.update(*this);
			this->titleScreenState.render(*this);
			break;

		case GameStateType::PlayGameScreen: 

			if (currentState != savedCurrentState) {
				this->context.gameState = this->currentState;
				this->context.nextState = this->nextState;
				this->playGameState.activate(*this);
				this->savedCurrentState = this->currentState;
			}
			this->playGameState.update(*this);
			this->playGameState.render(*this);
			break;

		case GameStateType::GameIntroScreen: 
		case GameStateType::GameIntroScreen_ChangeDay: 

      if (currentState == GameStateType::GameIntroScreen_ChangeDay) {
        this->currentState = GameStateType::GameIntroScreen;
        this->context.gameStats.timeOfDay = (this->context.gameStats.timeOfDay == TimeOfDay::Day ? TimeOfDay::Night : TimeOfDay::Day);
      }

			if (currentState != savedCurrentState) {
				this->context.gameState = this->currentState;
				this->context.nextState = this->nextState;
				this->gameIntroState.activate(*this);
				this->savedCurrentState = this->currentState;
			}
			this->gameIntroState.update(*this);
			this->gameIntroState.render(*this);
			break;

		case GameStateType::HighScoreScreen: 

			if (currentState != savedCurrentState) {
				this->context.gameState = this->currentState;
				this->context.nextState = this->nextState;
				this->highScoreState.activate(*this);
				this->savedCurrentState = this->currentState;
			}
			this->highScoreState.update(*this);
			this->highScoreState.render(*this);
			break;

		case GameStateType::PlayRaceScreen: 

			if (currentState != savedCurrentState) {
				this->context.gameState = this->currentState;
				this->context.nextState = this->nextState;
				this->raceState.activate(*this);
				this->savedCurrentState = this->currentState;
			}
			this->raceState.update(*this);
			this->raceState.render(*this);
			break;

		default: break;	

	}

}
