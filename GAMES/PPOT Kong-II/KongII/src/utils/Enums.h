#pragma once

#include "Arduboy2Ext.h"

#define NO_USB
#define _DEBUG

// Comment out IGNORE_COLLISIONS to include colllision detection ..
#define _IGNORE_COLLISIONS
#define PLAY_SOUNDS


// - Game play stuff ---------------------

#define FRAME_RATE_MIN 67
#define FRAME_RATE_MAX 90

#define INTRO_DELAY_FROM_TITLE_SCROLL 121
#define INTRO_DELAY_FROM_TITLE 120

#define NO_IMAGE 255

#define NUMBER_OF_ENTITIES 4
#define NUMBER_OF_CHAINS 4

#define CLAPPERS_LOWER_MAX_EASY 250
#define CLAPPERS_LOWER_MAX_HARD 220
#define CLAPPERS_LOWER_MIN 125
#define CLAPPERS_LOWER_INC 5
#define CLAPPERS_LOWER_COUNT NUMBER_OF_ENTITIES

#define CLAPPERS_UPPER_MAX_EASY 250
#define CLAPPERS_UPPER_MAX_HARD 220
#define CLAPPERS_UPPER_MIN 125
#define CLAPPERS_UPPER_INC 5
#define CLAPPERS_UPPER_COUNT NUMBER_OF_ENTITIES

#define SPARKS_LOWER_MAX_EASY 250
#define SPARKS_LOWER_MAX_HARD 220
#define SPARKS_LOWER_MIN 125
#define SPARKS_LOWER_INC 5
#define SPARKS_LOWER_COUNT NUMBER_OF_ENTITIES

#define SPARKS_UPPER_MAX_EASY 250
#define SPARKS_UPPER_MAX_HARD 220
#define SPARKS_UPPER_MIN 125
#define SPARKS_UPPER_INC 5
#define SPARKS_UPPER_COUNT NUMBER_OF_ENTITIES

#define BIRDS_MAX_EASY 450
#define BIRDS_MAX_HARD 400
#define BIRDS_MIN 220
#define BIRDS_INC 10
#define BIRDS_COUNT NUMBER_OF_ENTITIES

#define JUMP_POINT_SPAN 12


enum class GameStateType : uint8_t {
	SplashScreen_Activate,
	SplashScreen,
	TitleScreen_Activate,
	TitleScreen,
    PlayGame_Activate,
    PlayGame,
    HighScore_Activate,
    HighScore,
};

enum class Background : uint8_t {
    None,
    Black,
    White
};

enum class Stance : uint8_t {
    Normal,                     // 0
    Normal_RHS,                 // 1
    Running_01,                 // 2
    Running_02,                 // 3
    Running_01_RHS,             // 4
    Running_02_RHS,             // 5
    OnRope_01,                  // 6
    OnRope_02,                  // 7
    OnRope_03,                  // 8
    OnRope_04,                  // 9
    Climbing_Vine_01,           // 10
    Climbing_Vine_02,           // 11
    Jump,                       // 12
    Falling_01,                 // 13
    Falling_02,                 // 14
};

enum class KeyLocation : uint8_t {
    LowerPosition,
    UpperPosition,
    Position_00,
    Position_01,
    Position_02,
    Position_03,
    None,
};

enum class Movements : uint8_t {
    None,
    Up = 1,
    Down = 2, 
    Left = 4,
    Right = 8,
    Jump = 16,
    Reverse = 32,
    JumpUp = 128,
    ThrowKey = 128,  
};

enum class Rotation : uint8_t {
    Left = 1,
    Right = 2,
    None = 4
};

enum class SparkImage : uint8_t {
    Left,
    Right,
    None
};

enum class KongImage : uint8_t {
    Normal,
    Falling_00,
    Falling_01,
    Walking_00,
    Walking_01,
    Walking_02,
    Walking_03,
};

enum class KeyImage : uint8_t {
    Position_00,
    Position_01,
    Position_02,
    Position_03,
    Position_04,
    Position_05,
    Position_06,
    Position_07,
    None
};

enum class BirdImage : uint8_t {
    Bird_00,
    Bird_01,
    Bird_02,
    Bird_03,
    Bird_Upright,
    Bird_Turn_00,
    Bird_Turn_01,
    Bird_Turn_02,
    Bird_Turn_03,
    Bird_Turn_04,
    Bird_Turn_05,
    None
};

enum class SparkPath : uint8_t {
    Lower,
    Upper
};


enum class GameMode : uint8_t {
    Easy,
    Hard
};

struct GameStats {

    public: 

        GameStats() { };

        uint16_t score = 0;
        uint8_t numberOfLivesLeft = 4; 
        uint8_t level = 1;

        GameMode mode = GameMode::Easy;

        bool gameOver = false;

        void resetGame() {

            this->numberOfLivesLeft = 4; 
            this->score = 0; 
            this->level = 1;
            this->gameOver = false;

        }

};


