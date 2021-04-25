#include "src/utils/Arduboy2Ext.h"
#include "src/utils/FadeEffects.h"


IntroductionState selectedIntroItem = IntroductionState::Start;
constexpr const static uint8_t UPLOAD_DELAY = 16;


// --------------------------------------------------------------------------------------
//  Show introduction screen ..
//
void Intro() {


  // Draw logo ..

  Sprites::drawOverwrite(18, 1, logo, 0);


  if (!fadeInEffect.isComplete()) {

    fadeInEffect.draw(arduboy);
    fadeInEffect.update();

  }


  // Display 'Press A' button after a short period of time ..

  if (introDelay < INTRO_DELAY) {
    introDelay++;
  }
  else {    

    Sprites::drawOverwrite(12, 55, start_normal, 0);
    Sprites::drawOverwrite(45, 55, highscore_normal, 0);
    Sprites::drawOverwrite(88, 55, credits_normal, 0);

    if (selectedIntroItem == IntroductionState::Start)      { Sprites::drawOverwrite(12, 62, start_highlight, 0); }
    if (selectedIntroItem == IntroductionState::HighScore)  { Sprites::drawOverwrite(45, 62, highscore_highlight, 0); }
    if (selectedIntroItem == IntroductionState::Credits)    { Sprites::drawOverwrite(88, 62, credits_highlight, 0); }

  }


  // Handle keypress ..

  if (arduboy.justPressed(LEFT_BUTTON) && selectedIntroItem > IntroductionState::Start) { selectedIntroItem--; }
  if (arduboy.justPressed(RIGHT_BUTTON) && selectedIntroItem < IntroductionState::Credits) { selectedIntroItem++; }

  if (arduboy.justPressed(A_BUTTON)) { 

    switch (selectedIntroItem) {

      case IntroductionState::Start:
        gameState = GameState::ScoreTable; 
        introDelay = 0;
        break;

      case IntroductionState::HighScore:
        gameState = GameState::HighScore; 
        break;

      case IntroductionState::Credits:
        gameState = GameState::Credits; 
        break;

    }
    
  }

  // Restart ?

	if (arduboy.pressed(DOWN_BUTTON)) {

		if (restart < UPLOAD_DELAY) {
			restart++;
		}
		else {
      arduboy.exitToBootloader();
		}

	}
	else {
		restart = 0;
	}

}