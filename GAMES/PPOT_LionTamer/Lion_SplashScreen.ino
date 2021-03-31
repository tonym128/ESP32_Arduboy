
// ----------------------------------------------------------------------------
//  Initialise state ..
//
void splashScreen_Init() {

    arduboy.setFrameRate(60);
    counter = 0;

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void splashScreen() { 

    auto justPressed = arduboy.justPressedButtons();

    if (justPressed > 0 && counter > 0) {

    counter = 32;  

    }

    if (justPressed > 0 && counter == 0) {

        #ifdef SOUNDS    
        sound.tone(NOTE_A2, 45);
        sound.tone(NOTE_F1, 45);
        #endif

        counter = 1;

    }

    if (counter > 0) {

        counter++;

        if (counter == 33) gameState = GameState::Title_Init; 

    }


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
