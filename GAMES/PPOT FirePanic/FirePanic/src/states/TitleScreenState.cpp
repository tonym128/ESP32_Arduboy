#include "TitleScreenState.h"
#include "../images/Images.h"
#include "../utils/EEPROM_Utils.h"
#include "../sounds/Sounds.h"

extern ArduboyTones sound;

constexpr const static uint8_t PRESS_A_DELAY = 200;
constexpr const static uint8_t UPLOAD_DELAY = 16;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void TitleScreenState::activate(StateMachine & machine) {

  auto & gameStats = machine.getContext().gameStats;
  auto & arduboy = machine.getContext().arduboy;
  //auto & sound = machine.getContext().sound;  
	
  this->sloganCaption = 0;
  this->sloganDisplay = false;

  gameStats.resetGame();
  //sound.setOutputEnabled(arduboy.audio.enabled);
  sound.tones(Sounds::Score);

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void TitleScreenState::update(StateMachine & machine) {

  auto & arduboy = machine.getContext().arduboy;
  auto justPressed = arduboy.justPressedButtons();
  auto pressed = arduboy.pressedButtons();


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

	if (justPressed & A_BUTTON) {
		machine.changeState(GameStateType::GameIntroScreen, GameStateType::PlayGameScreen); 
	}

	if (justPressed & B_BUTTON) {
		machine.changeState(GameStateType::HighScoreScreen, GameStateType::None); 
	}


  // Update flame counter.

  if (arduboy.everyXFrames(15)) {

    this->flameCounter++;
    if (this->flameCounter == 3) this->flameCounter = 0;

  }


  // Help !

  if (arduboy.everyXFrames(80) && this->pressACounter > 75) {

    this->sloganDisplay = !this->sloganDisplay;
    this->sloganCaption++;

    if (this->sloganCaption == 16) this->sloganCaption = 0;

  }


  // Update 'Press A' counter / delay ..

  if (this->pressACounter < PRESS_A_DELAY) this->pressACounter++;

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void TitleScreenState::render(StateMachine & machine) {

  auto & arduboy = machine.getContext().arduboy;

  BaseState::renderCommonScenery(machine, false, true);
  BaseState::renderLowerGrass();
  
  SpritesB::drawExternalMask(33, 1, Images::FirePanic_Logo, Images::FirePanic_Logo_Mask, 0, 0);
  SpritesB::drawSelfMasked(42, 13, Images::FirePanic_Logo_Anim_1, this->flameCounter);
  SpritesB::drawSelfMasked(54, 13, Images::FirePanic_Logo_Anim_2, this->flameCounter); 
  SpritesB::drawSelfMasked(63, 13, Images::FirePanic_Logo_Anim_3, this->flameCounter);
  SpritesB::drawSelfMasked(71, 13, Images::FirePanic_Logo_Anim_4, this->flameCounter);
  SpritesB::drawSelfMasked(79, 13, Images::FirePanic_Logo_Anim_5, this->flameCounter); 

  if (this->pressACounter == PRESS_A_DELAY) {

    SpritesB::drawOverwrite(43, 52, Images::PressA, 0);

  }

  uint8_t slogan = this->sloganCaption / 4;

  switch (slogan) {

    case 0:
      SpritesB::drawExternalMask(0, 14, Images::Victim_OnEdge_01, Images::Victim_OnEdge_01_Mask, 0, 0);
      break;

    case 1:
      SpritesB::drawExternalMask(112, 14, Images::Victim_OnEdge_01_RH, Images::Victim_OnEdge_01_RH_Mask, 0, 0);
      break;

    case 2:
      SpritesB::drawExternalMask(0, 31, Images::Victim_OnEdge_01, Images::Victim_OnEdge_01_Mask, 0, 0);
      break;

    case 3:
      SpritesB::drawExternalMask(112, 31, Images::Victim_OnEdge_01_RH, Images::Victim_OnEdge_01_RH_Mask, 0, 0);
      break;
      
  }

  if (this->sloganDisplay) {

    switch (slogan) {
      
      case 0:
        SpritesB::drawExternalMask(18, 3, Images::Help, Images::Help_Mask, 0, 0);
        break;
      
      case 1:
        SpritesB::drawExternalMask(91, 3, Images::Help, Images::Help_Mask, 1, 1);
        break;
      
      case 2:
        SpritesB::drawExternalMask(18, 16, Images::Filmote, Images::Filmote_Mask, 0, 0);
        break;
      
      case 3:
        SpritesB::drawExternalMask(71, 16, Images::Vampirics, Images::Vampirics_Mask, 0, 0);
        break;

    }

  }

	arduboy.displayWithBackground(TimeOfDay::Day);

}
