

// ----------------------------------------------------------------------------
//  Initialise iinstruction screen ..
//
void instructions_Init() {

    ledDelay = 0;
    counter = 0;

}


// ----------------------------------------------------------------------------
//  Render instructoin screen ..
//
void instructions() {

    const uint8_t xCoord[53] = {  26, 28, 30, 32,   34, 36, 38, 40,   42, 44, 46, 48,   50, 52, 54, 56,   
                                  58, 60, 62, 64,   66, 68, 70, 72,   72, 72, 72, 70,   68, 66, 64, 62,  
                                  60, 58, 56, 54,   52, 50, 48, 46,   44, 42, 40, 38,   36, 34, 32, 30,
                                  28, 26, 26, 26,   26 };

    const uint8_t xPos[53]   = {   8,  9,  8,  9,    8,  9,  8,  9,    8,  9,  8,  9,    8,  9,  8,  9,    
                                   8,  9,  8,  9,    8,  9,  8, 15,   15, 15, 15, 11,   12, 11, 12, 11,
                                  12, 11, 12, 11,   12, 11, 12, 11,   12, 11, 12, 11,   12, 11, 12, 11,  
                                   12,  3,  3,  3,   3 };

    const uint8_t yCoord[53] = {  12, 12,17, 17,    17, 22, 22, 22,   27, 27, 27, 32,   32, 32, 32, 32,   
                                  32, 32, 32, 32,   32, 32, 32, 32,   32, 32, 32, 32,   32, 32, 27, 27,  
                                  27, 22, 22, 22,   17, 17, 17, 12,   12, 12, 12, 12,   12, 12, 12, 12,   
                                  12, 12, 12, 12,   12 };

    const uint8_t blink[53] = {    0,  0,  2,  0,    0,  2,  0,  0,    2,  0,  0,  0,    0,  0,  0,  0,
                                   0,  0,  0,  0,    0,  3,  0,  3,    0,  3,  0,  3,    0,  0,  1,  0,   
                                   0,  1,  0,  0,    1,  0,  0,  1,    0,  0,  0,  0,    3,  0,  3,  0,   
                                   3,  0,  3,  0,    2 };


    if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) { 

        gameState = GameState::PlayGame_Init;

    }


    ledDelay++;
    if (ledDelay == 235) ledDelay = 0;

    if (arduboy.isFrameCount(8)){
        counter++;
        if (counter == 53) counter = 0;
    }

    uint8_t counter2 = (counter + 26) % 53;

    if (blink[counter] != 1) { 
        Sprites::drawOverwrite(3, 1, Images::Button_Up, 0);
    } 
    else {
        Sprites::drawOverwrite(3, 1, Images::Button_Blank, 0);
    }

    if (blink[counter2] != 1) { 
        Sprites::drawOverwrite(115, 1, Images::Button_B, 0);
    } 
    else {
        Sprites::drawOverwrite(115, 1, Images::Button_Blank, 0);
    }

    Sprites::drawExternalMask(2, yCoord[counter], Images::Player_01, Images::Player_01_Mask, blink[counter] == 3, blink[counter] == 3);
    Sprites::drawExternalMask(102, yCoord[counter2], Images::Player_02, Images::Player_02_Mask, blink[counter2] == 3, blink[counter2] == 3);


    lion1.setXPosition(static_cast<XPosition>(xPos[counter]));
    if (counter <= 26) lion1.setDirection(Direction::Right);
    else lion1.setDirection(Direction::Left);

    lion2.setXPosition(static_cast<XPosition>(xPos[counter2]));
    if (counter2 <= 26) lion2.setDirection(Direction::Right);
    else lion2.setDirection(Direction::Left);


    Sprites::drawExternalMask(xCoord[counter] + 1, 3, Images::Lion, Images::Lion_Mask, lion1.getFrame(), lion1.getFrame());
    Sprites::drawExternalMask(xCoord[counter2] + 1, 27, Images::Lion, Images::Lion_Mask, lion2.getFrame(), lion2.getFrame());

    if (blink[counter] != 2) { 
        Sprites::drawOverwrite(3, 53, Images::Button_Down, 0);
    } 
    else {
        Sprites::drawOverwrite(3, 53, Images::Button_Blank, 0);
    }

    if (blink[counter2] != 2) { 
        Sprites::drawOverwrite(115, 53, Images::Button_A, 0);
    } 
    else {
        Sprites::drawOverwrite(115, 53, Images::Button_Blank, 0);
    }


    switch (ledDelay) {

        case 0 ... 14:
        case 75 ... 89:
        case 160 ... 174:
            break;

        case 15 ... 74:
            Sprites::drawOverwrite(23, 55, Images::Instr_00, 0);
            break;

        case 90 ... 159:
            Sprites::drawOverwrite(25, 55, Images::Instr_01, 0);
            break;

        case 175 ... 234:
            Sprites::drawOverwrite(18, 55, Images::Instr_02, 0);
            break;

    }

}