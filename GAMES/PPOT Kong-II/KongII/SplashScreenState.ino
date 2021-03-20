
// ----------------------------------------------------------------------------
//  Initialise state ..
//
void splashScreen_Activate() {

  splashScreenVars.counter = 0;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void splashScreen_Update() { 

    auto justPressed = arduboy.justPressedButtons();

    if (justPressed > 0 && splashScreenVars.counter > 0) {

    splashScreenVars.counter = 32;  

    }

    if (justPressed > 0 && splashScreenVars.counter == 0) {

        #ifdef PLAY_SOUNDS    
        sound.tone(NOTE_A2, 45);
        sound.tone(NOTE_F1, 45);
        #endif

        splashScreenVars.counter = 1;

    }

    if (splashScreenVars.counter > 0) {

        splashScreenVars.counter++;

        if (splashScreenVars.counter == 33) gameState = GameStateType::TitleScreen_Activate; 

    }

}


// ----------------------------------------------------------------------------
//  Render the state ..
//
void splashScreen_Render() {

    Sprites::drawOverwrite(32, 17, Images::PPOT, 0);

    switch (arduboy.getFrameCount(96)) {

        case 0 ... 23:
            Sprites::drawOverwrite(91, 25, Images::PPOT_Arrow, 0);
            break;

        case 24 ... 47:
            Sprites::drawOverwrite(45, 28, Images::PPOT_Spindle, 0);
            Sprites::drawOverwrite(91, 25, Images::PPOT_Arrow, 0);
            arduboy.drawPixel(52, 30, WHITE);
            break;

        case 48 ... 71:
            arduboy.drawPixel(52, 31, WHITE);
            break;

        case 72 ... 95:
            Sprites::drawOverwrite(45, 28, Images::PPOT_Spindle, 0);
            arduboy.drawPixel(52, 32, WHITE);
            break;

    }

}
