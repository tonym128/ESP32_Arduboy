constexpr const static uint8_t UPLOAD_DELAY = 16;


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void titleScreen_Activate() {

    gameStats.resetGame();
    sound.volumeMode(VOLUME_ALWAYS_NORMAL);

    playGameVars.frameRate = FRAME_RATE_MIN;
    arduboy.setFrameRate(playGameVars.frameRate);

    titleScreenVars.restart = 0;
    titleScreenVars.counter = 0;
    titleScreenVars.position = Rotation::None;

    #ifdef PLAY_SOUNDS 
    sound.tones(Sounds::TitleMusic);
    #endif

}


// ----------------------------------------------------------------------------
//  Handle state updates ..
//
void titleScreen_Update() {

    auto justPressed = arduboy.justPressedButtons();
    auto pressed = arduboy.pressedButtons();


    // Restart ?

    if (pressed & DOWN_BUTTON) {

        if (titleScreenVars.restart < UPLOAD_DELAY) {
            titleScreenVars.restart++;
        }
        else {
            arduboy.exitToBootloader();
        }

    }
    else {
        titleScreenVars.restart = 0;
    }


    // Handle other input ..

    if (titleScreenVars.position == Rotation::None) {

        if (justPressed & A_BUTTON || justPressed & B_BUTTON) {
            gameStats.mode = (justPressed & A_BUTTON ? GameMode::Easy : GameMode::Hard);
            gameState = GameStateType::PlayGame_Activate; 
        }

    }
    else {

        if (justPressed & A_BUTTON) {
            gameStats.mode = (titleScreenVars.position == Rotation::Left ? GameMode::Easy : GameMode::Hard);
            gameState = GameStateType::PlayGame_Activate; 
        }

    }

    if (justPressed & UP_BUTTON) {
        gameState = GameStateType::HighScore_Activate; 
    }

    if (justPressed & LEFT_BUTTON) {
        titleScreenVars.position = Rotation::Left;
    }

    if (justPressed & RIGHT_BUTTON) {
        titleScreenVars.position = Rotation::Right;
    }

    if (arduboy.getFrameCount(24) == 0) {
        titleScreenVars.counter = (titleScreenVars.counter == 2 ? 0 : titleScreenVars.counter + 1);
    }

}


// ----------------------------------------------------------------------------
//  Render the state ..
//
void titleScreen_Render() {

    const uint8_t headPos[3] = { 18, 19, 19 };
    const uint8_t leftArm[3] = { 20, 20, 19 };
    const uint8_t rightArm[3] = { 22, 22, 21 };

    Sprites::drawOverwrite(25, 2, Images::Title_Kong_II, 0);
    Sprites::drawOverwrite(2, 0, Images::Title_Chain_Long, 0);
    Sprites::drawOverwrite(13, 0, Images::Title_Chain_Short, 0);
    Sprites::drawOverwrite(118, 0, Images::Title_Chain_Long, 0);
    Sprites::drawOverwrite(109, 0, Images::Title_Chain_Short, 0);

    if (titleScreenVars.position != Rotation::Left || (titleScreenVars.position == Rotation::Left && arduboy.getFrameCountHalf(64))) {
        Sprites::drawOverwrite(2, 56, Images::Title_Game, 0);
        Sprites::drawOverwrite(22, 53, Images::Game_Icon, 0);
    }

    if (titleScreenVars.position != Rotation::Right || (titleScreenVars.position == Rotation::Right && arduboy.getFrameCountHalf(64))) {
        Sprites::drawOverwrite(95, 56, Images::Title_Game, 0);
        Sprites::drawOverwrite(115, 53, Images::Game_Icon, 1);
    }

    Sprites::drawSelfMasked(42, 43, Images::Title_Kong_Body, 0);
    Sprites::drawSelfMasked(22, leftArm[titleScreenVars.counter], Images::Title_Kong_Arm_Left, 0);
    Sprites::drawSelfMasked(75, rightArm[titleScreenVars.counter], Images::Title_Kong_Arm_Right, 0);

    Sprites::drawSelfMasked(52, headPos[titleScreenVars.counter], Images::Title_Kong_Head, 0);

}
