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
#include <pgmspace.h>

constexpr const static uint8_t BARREL_DELAY = 30;
constexpr const static uint8_t PRESS_A_DELAY = 130;
constexpr const static uint8_t UPLOAD_DELAY = 16;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void TitleScreenState::activate(StateMachine & machine) {

  auto & gameStats = machine.getContext().gameStats;
  auto & arduboy = machine.getContext().arduboy;
  //auto & sound = machine.getContext().sound;

  gameStats.resetGame();
  //sound.setOutputEnabled(arduboy.audio.enabled);
  //sound.volumeMode(VOLUME_ALWAYS_NORMAL);

  this->barrelPos = 4;
  this->barrelRot_LHS = 0;
  this->barrelRot_RHS = 0;
  this->pressACounter = 0;

#ifdef PLAY_SOUNDS 
  sound.tones(Sounds::unused_mischievous_cutscene_music);
#endif

}


// ----------------------------------------------------------------------------
//  Handle state updates ..
//
void TitleScreenState::update(StateMachine & machine) {

  auto & arduboy = machine.getContext().arduboy;
  auto & gameStats = machine.getContext().gameStats;
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


  // Handle barrels ..

  if (this->pressACounter > BARREL_DELAY && arduboy.everyXFrames(2)) {

    if (this->barrelPos < 38) {

      this->barrelPos++;

      if (arduboy.everyXFrames(4)) {

        this->barrelRot_RHS = wrapInc(this->barrelRot_RHS, static_cast<uint8_t>(0), static_cast<uint8_t>(2));
        this->barrelRot_LHS = wrapDec(this->barrelRot_LHS, static_cast<uint8_t>(0), static_cast<uint8_t>(2));

      }

    }

  }


	// Handle other input ..

	if (justPressed & A_BUTTON || justPressed & B_BUTTON) {
    gameStats.mode = (justPressed & A_BUTTON ? GameMode::Easy : GameMode::Hard);
		machine.changeState(GameStateType::PlayGameScreen);
	}

	if (justPressed & UP_BUTTON || justPressed & LEFT_BUTTON || justPressed & RIGHT_BUTTON) {
		machine.changeState(GameStateType::HighScoreScreen);
	}


  // Update 'Press A' counter / delay ..

  if (this->pressACounter < PRESS_A_DELAY) this->pressACounter++;

}


// ----------------------------------------------------------------------------
//  Render the state ..
//
void TitleScreenState::render(StateMachine & machine) {

  auto & arduboy = machine.getContext().arduboy;

  Sprites::drawOverwrite(20, 6, Images::Title_Kong, 0);

  for (uint8_t x = 11; x < 115; x = x + 12) {
    Sprites::drawSelfMasked(x, 50, Images::Girder_Small, 0);
  }

  Sprites::drawExternalMask(51 - this->barrelPos, 41, Images::BarrelImg, Images::Barrel_Mask, this->barrelRot_LHS, 0);
  Sprites::drawExternalMask(51 + this->barrelPos, 41, Images::BarrelImg, Images::Barrel_Mask, this->barrelRot_RHS, 0);

  if (this->pressACounter == PRESS_A_DELAY) {

    Sprites::drawExternalMask(22, 46, Images::PressAandB, Images::PressAandB_Mask, 0, 0);

  }

}
