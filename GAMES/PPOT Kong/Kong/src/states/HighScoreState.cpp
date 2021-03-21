#include "HighScoreState.h"
#include "../map/Coordinates.h"
#include "../images/Images.h"
#include "../fonts/Font4x6.h"
#include "../utils/Enums.h"
#include "../utils/Utils.h"

// ----------------------------------------------------------------------------
//  Initialise state ..
//
void HighScoreState::activate(StateMachine & machine) {
	
	auto & arduboy = machine.getContext().arduboy;
	auto & gameStats = machine.getContext().gameStats;

  arduboy.setFrameRate(FRAME_RATE_MIN);
  this->charIdx = 0;
  this->clearScores = 0;
  this->pressACounter = HS_PRESS_A_DELAY;
  this->selectedMode = gameStats.mode;
  this->winnerIdx = (gameStats.score > 0 ? EEPROM_Utils::saveScore(this->selectedMode, gameStats.score) : NO_WINNER);

  arduboy.clearButtonState();

  // Retrieve existing names and scores ..
  EEPROM_Utils::readSaveEntries(GameMode::Easy, this->easyEntries);
  EEPROM_Utils::readSaveEntries(GameMode::Hard, this->hardEntries);

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void HighScoreState::update(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
  auto justPressed = arduboy.justPressedButtons();
  auto pressed = arduboy.pressedButtons();
  auto & entries = (this->selectedMode == GameMode::Easy) ? this->easyEntries : this->hardEntries;


  // Is the new score a high score ?

  if (this->winnerIdx < NO_WINNER) {

    if (arduboy.everyXFrames(12)) {

      char * player = entries[this->winnerIdx].name;

      if (pressed & UP_BUTTON) {

        player[this->charIdx]++;
        if (player[this->charIdx] > 90)  player[this->charIdx] = 65;
        if (player[this->charIdx] == 64) player[this->charIdx] = 65;

      }

      if (pressed & DOWN_BUTTON) {

        player[this->charIdx]--;
        if (player[this->charIdx] < 65)  player[this->charIdx] = 90;

      }

      if (pressed & LEFT_BUTTON && this->charIdx > 0) {
        this->charIdx--;
      }

      if (pressed & RIGHT_BUTTON && this->charIdx < 2) {
        this->charIdx++;
      }

      if (pressed & A_BUTTON) {

        if (player[0] != 63 && player[1] != 63 && player[2] != 63) {
          
          for (uint8_t i = 0; i < 3; i++) {
            EEPROM_Utils::saveChar(this->selectedMode, this->winnerIdx, i, player[i]);
          }
          
          this->winnerIdx = NO_WINNER;
          this->pressACounter = HS_PRESS_A_DELAY;

        }

      }

    }

  }
  else {


    // Handle other input ..

    if ((justPressed & A_BUTTON || justPressed & B_BUTTON) && this->pressACounter == 0) {

      machine.changeState(GameStateType::TitleScreen); 
    }

    if ((pressed & LEFT_BUTTON) != 0) {
      this->selectedMode = GameMode::Easy;
    }

    if ((pressed & RIGHT_BUTTON) != 0) {
      this->selectedMode = GameMode::Hard;
    }

  }


	// Clear scores ..

	if ((pressed & LEFT_BUTTON) && (pressed & RIGHT_BUTTON)) {

		clearScores++;

		switch (clearScores) {

			case 21 ... 60:

				arduboy.setRGBled(128 - (clearScores * 2), 0, 0);
				break;

			case 61:

				clearScores = 0;
				arduboy.setRGBled(0, 0, 0);
				EEPROM_Utils::clearEEPROM();
				EEPROM_Utils::readSaveEntries(GameMode::Easy, this->easyEntries);
				EEPROM_Utils::readSaveEntries(GameMode::Hard, this->hardEntries);

				break;

		}

	}
	else {

		if (clearScores > 0) {
		
			arduboy.setRGBled(0, 0, 0);
			clearScores = 0;

		}
		
	}


  // Decrement the 'Press A' counter if it has been set ..

  if (this->pressACounter > 0) this->pressACounter--;

}


void HighScoreState::renderHighScore(uint8_t y, const SaveEntry & saveEntry) {

  for (uint8_t i = 0; i < 3; i++) {
    Sprites::drawOverwrite(HS_NAME_LEFT + (i * 6), y, font_images, saveEntry.name[i] == 63 ? 0 : saveEntry.name[i] - 64);
  }

  for (uint8_t j = 6, x2 = HS_SCORE_LEFT - 4; j > 0; --j, x2 += 5) {
    
    uint8_t digits[6] = {};
    extractDigits(digits, saveEntry.score);
    Sprites::drawOverwrite(x2, y, font_images, digits[j - 1] + 27);

  }

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void HighScoreState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
	auto & entries = (this->selectedMode == GameMode::Easy) ? this->easyEntries : this->hardEntries;

	const bool flash = arduboy.getFrameCountHalf(FLASH_FRAME_COUNT);

  for (int x=3, y=10; x < 105; x = x + 10, y++) {

    Sprites::drawSelfMasked(x, y, Images::Girder, 0);
  }

  for (int x=15, y=54 ; x < 120; x = x + 10, y--) {

    Sprites::drawSelfMasked(x, y, Images::Girder, 0);
  }


  // Barrels
  
  this->renderBarrel(machine, this->barrel1, barrel1_rot);
  this->renderBarrel(machine, this->barrel2, barrel2_rot);
  this->renderBarrel(machine, this->barrel3, barrel3_rot);

  if (arduboy.everyXFrames(5)) {

    this->barrel1 = wrapInc(this->barrel1, static_cast<uint8_t>(0), static_cast<uint8_t>(BARREL_POSITION_SPLASH_END - 1));
    this->barrel2 = wrapInc(this->barrel2, static_cast<uint8_t>(0), static_cast<uint8_t>(BARREL_POSITION_SPLASH_END - 1));
    this->barrel3 = wrapInc(this->barrel3, static_cast<uint8_t>(0), static_cast<uint8_t>(BARREL_POSITION_SPLASH_END - 1));

  }

  Sprites::drawExternalMask(29, 18, Images::HighscorePanel, Images::HighscorePanel_Mask, 0, 0);
  Sprites::drawExternalMask(22, 3, Images::HighscoreText, Images::HighscoreText_Mask, 0, 0);


  // Render scores ..

  for (uint8_t index = 0; index < EEPROM_Utils::saveEntriesCount; ++index) {

    renderHighScore(HS_CHAR_TOP + (HS_CHAR_V_SPACING * index), entries[index]);

  }


  // Render edit field if the slot is being editted ..

  if (this->winnerIdx < NO_WINNER && flash) {

    char *player = entries[this->winnerIdx].name;

    arduboy.fillRect(HS_NAME_LEFT + (this->charIdx * 6) - 1, HS_CHAR_TOP + (winnerIdx * HS_CHAR_V_SPACING) - 1, 6, 8, WHITE);
    Sprites::drawErase(HS_NAME_LEFT + (this->charIdx * 6), HS_CHAR_TOP + (HS_CHAR_V_SPACING * this->winnerIdx), font_images, player[this->charIdx] == 63 ? 0 : player[this->charIdx] - 64);

  }


  // Display Press A message?

  if (this->winnerIdx == NO_WINNER && this->pressACounter == 0) {

    Sprites::drawExternalMask(48, 53, Images::Highscore_PressA, Images::Highscore_PressA_Mask, 0, 0);

  }

  const size_t panelIndex = static_cast<size_t>(this->selectedMode);

  Sprites::drawExternalMask(2, 27, Images::DifficultyPanel, Images::DifficultyPanel_Mask, panelIndex, 0);

}

void HighScoreState::renderBarrel(StateMachine & machine, uint8_t barrelPosition, uint8_t &barrelRot) {

	auto & arduboy = machine.getContext().arduboy;

  uint8_t x = pgm_read_byte(&Coordinates::Barrel_Splash[(barrelPosition * 3)]);
  int8_t y = pgm_read_byte(&Coordinates::Barrel_Splash[(barrelPosition * 3) + 1]);
  Rotation r = static_cast<Rotation>(pgm_read_byte(&Coordinates::Barrel_Splash[(barrelPosition * 3) + 2]));

  if (arduboy.everyXFrames(5)) {

    switch (r) {
      
      case Rotation::None:
        break;
      
      case Rotation::Right:
        barrelRot = wrapInc(barrelRot, static_cast<uint8_t>(0), static_cast<uint8_t>(2));
        break;
      
      case Rotation::Left:
        barrelRot = wrapDec(barrelRot, static_cast<uint8_t>(0), static_cast<uint8_t>(2));
        break;

    }

  }

  Sprites::drawExternalMask(x, y, Images::BarrelImg, Images::Barrel_Mask, (r == Rotation::None ? 0 : barrelRot), 0);

}