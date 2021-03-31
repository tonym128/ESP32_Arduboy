#include "BaseState.h"

#include "../images/Images.h"
#include "../utils/Utils.h"
#include "../utils/Enums.h"

void BaseState::renderScore(StateMachine & machine, bool renderHealth, int16_t health) {
  	
  auto & gameStats = machine.getContext().gameStats;
  auto & arduboy = machine.getContext().arduboy;

  Sprites::drawExternalMask(52, 1, Images::Scoreboard, Images::Scoreboard_Mask, 0, 0);

  if (!renderHealth) {

    uint8_t digits[4] = {};
    extractDigits(digits, gameStats.score);

    for (uint8_t j = 4; j > 0; --j) {

      Sprites::drawSelfMasked(75 - (j*5), 4, Images::Scoreboard_Numbers, digits[j - 1]);

    }

  }
  else {

    for (uint8_t i = 0; i < health; i = i + 2) {
      
      arduboy.drawFastVLine(94 + i, 3, 8);

    }

  }

}


void BaseState::renderCommonScenery(StateMachine & machine) {

  auto & gameStats = machine.getContext().gameStats;
	auto & arduboy = machine.getContext().arduboy;

  if (arduboy.everyXFrames(64)) {
  
    this->backgroundX++;
    if (this->backgroundX >= 64) this->backgroundX = 0;

  }

  Sprites::drawOverwrite(this->backgroundX - 64, 0, Images::SkyBackground, 0);
  Sprites::drawOverwrite(this->backgroundX, 0, Images::SkyBackground, 0);
  
  Sprites::drawExternalMask(0, 15, Images::Ground_Left, Images::Ground_Left_Mask, 0, 0);
  Sprites::drawExternalMask(117, 15, Images::Ground_Right, Images::Ground_Right_Mask, 0, 0);

  Sprites::drawExternalMask(12, 48, Images::Seaweed, Images::Seaweed_Mask, this->seaweed ? 1 : 0, 0);
  Sprites::drawExternalMask(112, 48, Images::Seaweed, Images::Seaweed_Mask, this->seaweed ? 1 : 0, 0);
  Sprites::drawExternalMask(64, 54, Images::Seaweed, Images::Seaweed_Mask, this->seaweed ? 1 : 0, 0);

  Sprites::drawExternalMask(11, 48, Images::Ground_Bottom, Images::Ground_Bottom_Mask, 0, 0);

  if (arduboy.everyXFrames(32)) {

    this->seaweed = !this->seaweed;

  }

}


bool BaseState::getPaused() {

  return this->paused;

}

void BaseState::setPaused(bool value) {
  
  this->paused = value;

}

void BaseState::handleCommonButtons(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
  auto & gameStats = machine.getContext().gameStats;
  auto justPressed = arduboy.justPressedButtons();

  if (gameStats.gameOver) {

    if (justPressed & A_BUTTON) {
      machine.changeState(GameStateType::HighScoreScreen, GameStateType::None); 
    }

  }
  else {

    if (justPressed & B_BUTTON) {
      this->paused = !this->paused; 
    }

  }

}

void BaseState::renderGameOverOrPause(StateMachine & machine) {

  auto & gameStats = machine.getContext().gameStats;


  // Game Over?

  if (gameStats.gameOver) {

    Sprites::drawExternalMask(25, 20, Images::GameOver, Images::GameOver_Mask, 0, 0); 

  }

  // Pause?

  if (this->paused) {

    Sprites::drawExternalMask(39, 20, Images::Pause, Images::Pause_Mask, 0, 0); 

  }

}


void BaseState::initWater() {

  int8_t y = 0;

  for (uint8_t x = 0; x < 12; x++) {

    switch (y) {
      
      case -1 ... 1:
        this->waterLevel[x] = y;
        break;

      case 2:
        this->waterLevel[x] = 0;
        break;

    }

    y++;
    if (y==3) y = -1;

  }

}

void BaseState::updateWater(StateMachine & machine) {

  auto & arduboy = machine.getContext().arduboy;

  if (arduboy.everyXFrames(16)) {

    uint8_t firstElement = this->waterLevel[0];
  
    for (uint8_t x = 0; x < 11; x++) {

      this->waterLevel[x] = this->waterLevel[x + 1];

    }

    this->waterLevel[11] = firstElement;

  }

}

void BaseState::renderWater() {

  Sprites::drawExternalMask(0, 33 + this->waterLevel[0], Images::Water_01, Images::Water_01_Mask, 0, 0);
  for (uint8_t x = 1; x < 11; x++) {
    Sprites::drawExternalMask(4 + (x * 10), 33 + this->waterLevel[x], Images::Water_02, Images::Water_02_Mask, 0, 0);
  }
  Sprites::drawExternalMask(114, 33 + this->waterLevel[11], Images::Water_03, Images::Water_03_Mask, 0, 0);

}