#include "SplashScreenState.h"
#include "../utils/Arduboy2Ext.h"
#include "../images/Images.h"


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void SplashScreenState::activate(StateMachine & machine) {

  (void)machine;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void SplashScreenState::update(StateMachine & machine) { 

	auto & arduboy = machine.getContext().arduboy;
	auto justPressed = arduboy.justPressedButtons();

  if (justPressed > 0 && this->counter > 0) {

    this->counter = 124;

  }

  if (justPressed > 0 && this->counter == 0) {

    this->counter = 1;

  }

  if (this->counter > 0) {

    this->counter++;

    if (this->counter == 125) machine.changeState(GameStateType::TitleScreen, GameStateType::None); 

  }

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void SplashScreenState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;

  SpritesB::drawOverwrite(47, 17, Images::Ppot_Buttons, 0);
  SpritesB::drawOverwrite(43, 26, Images::Ppot_ButtonUp, 0);
  SpritesB::drawOverwrite(73, 26, Images::Ppot_ButtonUp, 0);

  if (this->counter == 0) {

    SpritesB::drawOverwrite(58, 26, Images::Ppot_ButtonUp, 0);
    SpritesB::drawOverwrite(26, 46, Images::Ppot_Caption, 0);

  }
  else {

    SpritesB::drawOverwrite(58, 26, Images::Ppot_ButtonDown, 0);
    SpritesB::drawOverwrite(44, 46, Images::Ppot_Loading, 0);

    uint8_t i = (this->counter / 15) % 4;

    for (uint8_t j = 0; j < i; j++) {
      
        arduboy.drawPixel(79 + (j * 2), 49, WHITE);

    }

  }

	arduboy.display(true);

}
