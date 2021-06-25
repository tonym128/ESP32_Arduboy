
// ----------------------------------------------------------------------------
//  Initialise state ..
//
void highScore_Activate() {
	
    uint16_t score = EEPROM_Utils::readSaveScore(gameStats.mode);

    playGameVars.frameRate = FRAME_RATE_MIN;
    arduboy.setFrameRate(playGameVars.frameRate);
    arduboy.clearButtonState();

    highScoreVars.counter = 50;
    highScoreVars.newHighScore = false;
    highScoreVars.selectedMode = gameStats.mode;

    if (gameStats.score >= score) {
        EEPROM_Utils::saveScore(gameStats.mode, gameStats.score);
        highScoreVars.newHighScore = true;
    }

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void highScore_Update() {

    auto pressed = arduboy.pressedButtons();
    auto justPressed = arduboy.justPressedButtons();


    // Handle other input ..

    if (highScoreVars.counter > 0) {
        highScoreVars.counter--;
    }
        else {

        if ((justPressed & A_BUTTON || justPressed & B_BUTTON)) {
            gameState = GameStateType::TitleScreen_Activate; 
        }

        if ((pressed & LEFT_BUTTON) != 0) {
            highScoreVars.selectedMode = GameMode::Easy;
        }

        if ((pressed & RIGHT_BUTTON) != 0) {
            highScoreVars.selectedMode = GameMode::Hard;
        }

    }


	// Clear scores ..

	if ((pressed & LEFT_BUTTON) && (pressed & RIGHT_BUTTON)) {

		highScoreVars.clearScores++;

		switch (highScoreVars.clearScores) {

			case 21 ... 60:

				arduboy.setRGBled(128 - (highScoreVars.clearScores * 2), 0, 0);
				break;

			case 61:

				highScoreVars.clearScores = 0;
				arduboy.setRGBled(0, 0, 0);
				EEPROM_Utils::clearEEPROM();

				break;

		}

	}
	else {

		if (highScoreVars.clearScores > 0) {
		
			arduboy.setRGBled(0, 0, 0);
			highScoreVars.clearScores = 0;

		}
		
	}

}


void highScore_RenderHighScore(uint16_t score) {

    for (uint8_t j = 4; j > 0; --j) {

        uint8_t digits[4] = {};
        extractDigits(digits, score);
        Sprites::drawErase(79 - (j*7), 41, Images::Numbers, digits[j - 1]);

    }

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void highScore_Render() {

    Sprites::drawOverwrite(0, 0, Images::HighScore_Left, 0);
    Sprites::drawOverwrite(113, 0, Images::HighScore_Right, 0);
    Sprites::drawOverwrite(15, 56, Images::HighScore_Bottom, 0);
    Sprites::drawOverwrite(49, 38, Images::HighScore_Jnr, 0);
    Sprites::drawOverwrite(19, 30, Images::HighScore_Chains, 0);
    Sprites::drawOverwrite(103, 30, Images::HighScore_Chains, 0);
    Sprites::drawOverwrite(10, 10, Images::HighScore_Panel_Left, 0);
    Sprites::drawOverwrite(51, 0, Images::HighScore_Panel_Mid, 0);
    Sprites::drawOverwrite(76, 10, Images::HighScore_Panel_Right, 0);
    Sprites::drawOverwrite(9, 22, Images::HighScore_Chain_Left, 0);
    Sprites::drawOverwrite(111, 22, Images::HighScore_Chain_Right, 0);

    switch(highScoreVars.selectedMode) {

        case GameMode::Easy:
            Sprites::drawOverwrite(16, 50, Images::HighScore_Easy, 0);
            break;

        case GameMode::Hard:
            Sprites::drawOverwrite(95, 50, Images::HighScore_Hard, 0);
            break;

    }

    if (!highScoreVars.newHighScore || (highScoreVars.newHighScore & arduboy.getFrameCountHalf(64))) { 

        uint16_t score = EEPROM_Utils::readSaveScore(highScoreVars.selectedMode);
        highScore_RenderHighScore(score);

    }

}