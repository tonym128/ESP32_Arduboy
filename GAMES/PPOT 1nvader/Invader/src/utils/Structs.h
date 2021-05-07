#pragma once

#include "Constants.h"

struct GamePlayVars {

    uint16_t bombCounter;
    uint8_t waveCounter;
    bool waveCleared;

};

struct TitleScreenVars {

    int16_t counter = 0;
    uint8_t levels = 0;

    void reset(GameRotation gameRotation) {
        
        switch (gameRotation) {

            case GameRotation::Portrait:
            
                this->counter = 89;
                break;

            case GameRotation::Landscape:
            
                this->counter = 0;
                break;

        }

    }

};

struct GameOverScreenVars {

    int16_t counter = 0;
    bool newHighScore = false;
    bool flashPlayer1 = false;
    bool flashPlayer2 = false;

    void reset() {

        this->counter = 128;
        newHighScore = false;
        flashPlayer1 = false;
        flashPlayer2 = false;

    }

};