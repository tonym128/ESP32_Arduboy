#pragma once

// - Debug ------------------------------

#define _DEBUG
#define _DEBUG_COLLISIONS
#define _DEBUG_STRAIGHT_ROAD
#define _DEBUG_LEVEL_ROAD
#define _DEBUG_OTHER_CAR_STEER


// --------------------------------------

#define ROCKS
#define CACTII
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64
#define SOUNDS

#define UPM 64 // int16_ts per meter

namespace Constants {

    constexpr uint8_t NoCollision = 127;
    constexpr uint8_t NumberOfOtherCars = 4;
    constexpr uint8_t NumberOfCactii = 3;

    constexpr uint8_t PlayerCarWidthUnits = 78;
    constexpr uint8_t PlayerCarLengthUnits = 60;
    constexpr uint8_t PlayerCarWidthPX = 37;
    constexpr uint8_t PlayerCarHeightPX = 17;
    constexpr uint8_t OtherCarWidthUnits = 74;
    constexpr uint8_t OtherCarLengthUnits = 60;
    constexpr uint8_t CollisionTollerance_X = 65;
    constexpr uint8_t CollisionTollerance_Z = 50;

    constexpr uint16_t SpeedDiv = 1800;//3000;
    constexpr uint16_t ZReduction = 8192;
    constexpr int16_t ZMax = 8192 + 2048;
    constexpr uint8_t DistToRenewCars = 50;

    constexpr uint8_t BrakeCloud_Accelerate = 10;
    constexpr uint8_t BrakeCloud_DownGear = 10;
    constexpr uint8_t BrakeCloud_OffRoad = 4;

    constexpr uint16_t Gear0Max = 0;
    constexpr uint16_t Gear1Max = 6400;
    constexpr uint16_t Gear2Max = 11200;
    constexpr uint16_t Gear3Max = 21000;
    constexpr uint16_t Gear4Max = 40000;
    constexpr uint16_t Gear5Max = 50000;

    constexpr uint16_t Gear0Min = 0;
    constexpr uint16_t Gear1Min = Gear1Max / 3 * 4;
    constexpr uint16_t Gear2Min = Gear2Max / 3 * 4;
    constexpr uint16_t Gear3Min = Gear3Max / 3 * 4;
    constexpr uint16_t Gear4Min = Gear4Max / 3 * 4;
    constexpr uint16_t Gear5Min = Gear5Max / 3 * 4;

    constexpr uint16_t GearsMin[] = { Gear0Min, Gear1Min, Gear2Min, Gear3Min, Gear4Min, Gear5Min };
    constexpr uint16_t GearsMax[] = { Gear0Max, Gear1Max, Gear2Max, Gear3Max, Gear4Max, Gear5Max };

    constexpr uint8_t PROGMEM GearboxX[] = { 0, 5, 5, 11, 11, 17 };
    constexpr uint8_t PROGMEM GearboxY[] = { 0, 1, 13, 1, 13, 1 };

    constexpr uint8_t NewDayBannerDelay = 100;
    constexpr uint8_t NewDayBannerDelay_CountDown = 130;
    constexpr uint8_t CarsToPass_Init = 30; 
    constexpr uint8_t CarsToPass_Increase = 5;
    
    constexpr uint8_t GameOver_MaxOffset = 108;
    constexpr uint8_t Title_MaxOffset = 120;
    constexpr uint8_t NudgeDistance = 10;
    


    // Sky brightness ..
    
    constexpr uint8_t Sky_BX______D = 6;
    constexpr uint8_t Sky_B_X_____D = 4;
    constexpr uint8_t Sky_B__X____D = 3;
    constexpr uint8_t Sky_B___X___D = 2;
    constexpr uint8_t Sky_B____X__D = 1;
    constexpr uint8_t Sky_B_____X_D = 0;
    constexpr uint8_t Sky_B______XD = 5;


    constexpr uint8_t DoNotEditSlot             = 255;
    constexpr uint8_t MaxNumberOfScores         = 5;

                                       // 00, 01, 02, 03, 04, 05, 06, 07, 08, 09, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30 
    constexpr uint8_t SpeedSteering[] = {  0,  5,  5,  5,  5,  6,  7,  8,  8,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18  };

}

enum class GameState : uint8_t {
    Splash_Init, // 0
    Splash,
    Title_Init,
    Title,
    Game_Init,
    Game,
    GameOver_Init,
    GameOver,
    SaveScore,
    HighScore_Init,
    HighScore
};

enum class Direction : uint8_t {
    None,
    Up,
    Down,
    Left,
    Right,
    Both
};

enum class TimeOfDay : uint8_t {
    Day,
    Dawn,
    Night

};

enum class MovementType : uint8_t {
    Horizontal,
    Vertical
};


enum class CarMovement : uint8_t {
    Accelerate,
    Coast,
    Decelerate,
    NoMovement
};

enum class TransmissionType : uint8_t {
    Auto,
    Manual,
};

enum class TitleState : uint8_t {
    StartGame,
    Aim,
    SelectTransmission,
};
