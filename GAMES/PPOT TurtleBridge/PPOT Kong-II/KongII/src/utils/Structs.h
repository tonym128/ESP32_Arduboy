#pragma once

struct HighScoreStateVars {
    
    bool newHighScore = false;
    uint8_t clearScores = 0;
    uint8_t counter = 0;
    GameMode selectedMode = GameMode::Easy;

};

struct SplashScreenStateVars {

    uint8_t counter = 0;

};

struct TitleScreenStateVars {

    uint8_t restart = 0;
    uint8_t counter = 0;
    Rotation position = Rotation::None;

};

struct PlayGameStateVars {

    uint8_t introDelay;
    uint8_t preventJumpDelay;   // Used to prevent a jump if the A button is pressed to start game.
    uint8_t frameRate = FRAME_RATE_MIN;

    bool exitSequence = false;
    
    bool playing = false;
    bool paused = false;   

    uint8_t clappersLowerDelay = CLAPPERS_LOWER_MAX_EASY;
    uint8_t clappersUpperDelay = CLAPPERS_UPPER_MAX_EASY;
    uint8_t sparksLowerDelay = SPARKS_LOWER_MAX_EASY;
    uint8_t sparksUpperDelay = SPARKS_UPPER_MAX_EASY;
    uint16_t birdsDelay = BIRDS_MAX_EASY;
    
    Player player;
    Kong kong;
    Clappers upperClappers;
    Clappers lowerClappers;
    Sparks upperSparks;
    Sparks lowerSparks;
    Birds birds;
    Key key;

};