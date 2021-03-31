#include "TitleScreenState.h"
#include "../images/Images.h"
#include "../utils/EEPROM_Utils.h"
#include "../utils/Enums.h"
#include "../utils/Utils.h"
#include "../sounds/Sounds.h"
#include <ArduboyTones.h>

extern ArduboyTones sound;

// For size_t
#include <stddef.h>

// For pgm_read_ptr
#include <avr/pgmspace.h>

constexpr const static uint8_t PRESS_A_DELAY = 200;
constexpr const static uint8_t UPLOAD_DELAY = 16;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void TitleScreenState::activate(StateMachine & machine) {

  auto & gameStats = machine.getContext().gameStats;
  auto & arduboy = machine.getContext().arduboy;
  //auto & sound = machine.getContext().sound;  
	
  this->fish1.x = 16;
  this->fish1.y = 41;
  this->fish1.direction = Direction::Right;
	
  this->fish2.x = 104;
  this->fish2.y = 48;
  this->fish2.direction = Direction::Left;

  gameStats.resetGame();
  //sound.setOutputEnabled(arduboy.audio.enabled);

  BaseState::initWater();

  auto scoreIndex = static_cast<size_t>(random(getSize(Sounds::Scores)));
  auto score = static_cast<const uint16_t *>(pgm_read_ptr(&Sounds::Scores[scoreIndex]));

  sound.tones(score);

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void TitleScreenState::update(StateMachine & machine) {

  auto & arduboy = machine.getContext().arduboy;
  auto justPressed = arduboy.justPressedButtons();
  auto pressed = arduboy.pressedButtons();
  auto & gameStats = machine.getContext().gameStats;


	// Restart ?

	if (pressed & DOWN_BUTTON) {

		if (this->restart < UPLOAD_DELAY) {
			this->restart++;
		}
		else {
			arduboy.exitToBootloader();
		}

	}
	else {
		this->restart = 0;
	}


	// Handle other input ..

	if (justPressed & A_BUTTON || justPressed & B_BUTTON) {
    gameStats.mode = (justPressed & A_BUTTON ? GameMode::Easy : GameMode::Hard);
		machine.changeState(GameStateType::PlayGameScreen, GameStateType::None); 
	}

	if (justPressed & UP_BUTTON || justPressed & LEFT_BUTTON || justPressed & RIGHT_BUTTON) {
		machine.changeState(GameStateType::HighScoreScreen, GameStateType::None); 
	}


  BaseState::updateWater(machine);


  // Update fishes ..

  if (arduboy.everyXFrames(6)) {

    this->fish1.update();
    if (arduboy.everyXFrames(12)) {
      this->fish1.index = !this->fish1.index;
    }
    
    this->fish2.update();
    if (arduboy.everyXFrames(12)) {
      this->fish2.index = !this->fish2.index;
    }

  }


  // Update 'Press A' counter / delay ..

  if (this->pressACounter < PRESS_A_DELAY) this->pressACounter++;

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void TitleScreenState::render(StateMachine & machine) {

  auto & arduboy = machine.getContext().arduboy;

  BaseState::renderCommonScenery(machine);
  BaseState::renderWater();

  Sprites::drawExternalMask(26, 9, Images::Title, Images::Title_Mask, 0, 0);


  // Render fish ..

  uint8_t index = (this->fish1.direction == Direction::Left ? 0 : 2) + (this->fish1.index ? 0 : 1);
  Sprites::drawExternalMask(this->fish1.x, this->fish1.y, Images::Fish, Images::Fish_Mask, index, index);

  index = (this->fish2.direction == Direction::Left ? 0 : 2) + (this->fish2.index ? 0 : 1);
  Sprites::drawExternalMask(this->fish2.x, this->fish2.y, Images::Fish, Images::Fish_Mask, index, index);



  if (this->pressACounter == PRESS_A_DELAY) {

    Sprites::drawExternalMask(35, 42, Images::PressAandB, Images::PressAandB_Mask, 0, 0);

  }

  arduboy.displayWithBackground();

}
