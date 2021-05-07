#pragma once

// - Debug ------------------------------

#define _DEBUG
#define SOUNDS

// ---------------------------------------

#define MOTHERSHIP_SIZE_LANDSCAPE SIZE_3X3
#define MOTHERSHIP_SIZE_PORTRAIT SIZE_3X3

namespace Constants {

    constexpr uint8_t ParticlesMax = 30;
    constexpr uint8_t MaskWidth = 1;

    constexpr uint8_t FlashSpeed = 64;
    constexpr uint8_t TugOfWarRowAdjustment = 7;
    constexpr uint8_t WaveIncrease = 50;
    

    constexpr uint8_t PlayerHeight = 8;
    constexpr uint8_t PlayerWidth = 11;
    constexpr uint8_t PlayerHeightNoMask = 7;
    constexpr uint8_t PlayerWidthNoMask = 9;

    constexpr uint8_t BombHeight = 8;
    constexpr uint8_t BombWidth = 5;
    constexpr uint8_t BulletHeight = 7;
    constexpr uint8_t BulletWidth = 5;

    namespace Portrait {
        constexpr int8_t PlayerMinPos = -1;
        constexpr int8_t PlayerMaxPos = 53;
    }

    namespace Landscape {
        constexpr int8_t PlayerMinPos = -1;
        constexpr int8_t PlayerMaxPos = 117;
    }

    constexpr uint8_t MothershipHeight = 9;
    constexpr uint8_t MothershipWidth = 14;
    constexpr uint8_t MothershipCounterMax = 80;
    constexpr uint8_t MothershipExplosionMax = 8;
    constexpr uint8_t MothershipRowHeight = 8;

    namespace Portrait {
        constexpr int16_t MothershipMinPos = -2800;
        constexpr int16_t MothershipMaxPos = 7500;
        constexpr int16_t MothershipStartHeight = 104;
    }

    namespace Landscape {
        constexpr int16_t MothershipMinPos = -2800;
        constexpr int16_t MothershipMaxPos = 13800;
        constexpr int16_t MothershipStartHeight = 4; 
    }
    
    constexpr uint8_t Mothership_Frames[] = { 0, 1, 2, 3, 2, 1 };
    constexpr uint8_t Arrow_Frames[] = { 0, 1, 2, 1 };
}

enum class GameState : uint8_t {
    Splash_Init, // 0
    Splash,
    Title_Init,
    Title,
    Game_Init,
    Game,
    TugOfWar_Init,
    TugOfWar,
    GameOver_Init,
    GameOver,
};

enum class GameMode : uint8_t {
    Single,
    Double,
    TugOfWar
};

enum class GameRotation : uint8_t {
    Portrait,
    Landscape
};

enum class Movement : uint8_t {
    Up,
    Down,
    Left,
    Right
};


// --------------------------------------------------------------------

inline GameMode &operator++(GameMode &c ) {
    c = static_cast<GameMode>( static_cast<uint8_t>(c) + 1 );
    return c;
}

inline GameMode operator++(GameMode &c, int ) {
    GameMode result = c;
    ++c;
    return result;
}

inline GameMode &operator--(GameMode &c ) {
    c = static_cast<GameMode>( static_cast<uint8_t>(c) - 1 );
    return c;
}

inline GameMode operator--(GameMode &c, int ) {
    GameMode result = c;
    --c;
    return result;
}

