#include "src/utils/Arduboy2Ext.h"


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void playGame_Activate() {

    playGameVars.player.reset();

    playGameVars.clappersLowerDelay = (gameStats.mode == GameMode::Easy ? CLAPPERS_LOWER_MAX_EASY : CLAPPERS_LOWER_MAX_HARD);
    playGameVars.clappersUpperDelay = (gameStats.mode == GameMode::Easy ? CLAPPERS_UPPER_MAX_EASY : CLAPPERS_UPPER_MAX_HARD);
    playGameVars.sparksUpperDelay = (gameStats.mode == GameMode::Easy ? SPARKS_LOWER_MAX_EASY : SPARKS_LOWER_MAX_HARD);
    playGameVars.sparksUpperDelay = (gameStats.mode == GameMode::Easy ? SPARKS_UPPER_MAX_EASY : SPARKS_UPPER_MAX_HARD);
    playGameVars.birdsDelay = (gameStats.mode == GameMode::Easy ? BIRDS_MAX_EASY : BIRDS_MAX_HARD);

    playGameVars.lowerClappers.setDelayMax(playGameVars.clappersLowerDelay, true);
    playGameVars.lowerClappers.reset(107);

    playGameVars.upperClappers.setDelayMax(playGameVars.clappersUpperDelay, true);
    playGameVars.upperClappers.reset(54);

    playGameVars.lowerSparks.setDelayMax(playGameVars.sparksUpperDelay, true);
    playGameVars.lowerSparks.reset(Coordinates::LowerSpark);

    playGameVars.upperSparks.setDelayMax(playGameVars.sparksUpperDelay, true);
    playGameVars.upperSparks.reset(Coordinates::UpperSpark);

    playGameVars.birds.setDelayMax(playGameVars.birdsDelay, true);
    playGameVars.birds.reset();

    playGameVars.kong.reset();

    playGameVars.frameRate = FRAME_RATE_MIN;
    arduboy.setFrameRate(playGameVars.frameRate);

    playGame_ResetLevel(INTRO_DELAY_FROM_TITLE_SCROLL);
    playGameVars.key.setKeyLocation(KeyLocation::LowerPosition);  

    playGameVars.playing = false;
    playGameVars.exitSequence = false;
    
    playGameVars.paused = false;

}
