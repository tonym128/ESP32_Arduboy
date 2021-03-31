#include <Arduboy2.h>

#pragma once

#define SOUNDS
#define SPLASH
#define _THREE_IMAGES
#define SHOW_INSTUCTIONS
#define SHOW_ROAR
#define RESET

struct SoundVars {

    int note = 180;
    int delta = 1;
    int maximum = 201;
    int minimum = 180;

    int duration = 44;
    bool up = true;
    bool down = false;

    void reset() {

        this->note = 180;
        this->delta = 1;
        this->maximum = 201;
        this->minimum = 180;

        this->duration = 44;
        this->up = true;
        this->down = false;

    }

};

enum class GameState : uint8_t {

    #ifdef SPLASH
        Splash_Init,
        Splash,
    #endif

    Title_Init,
    Title,

    #ifdef SHOW_INSTUCTIONS
        Instructions_Init,
        Instructions,
    #endif

    PlayGame_Init,
    PlayGame

};

enum class GameMode : uint8_t {

    Easy,
    Hard,

};

enum class Direction : uint8_t {

    Left,
    Right,
    Up,
    Down,
    None

};

enum class XPosition : uint8_t {

    LH_Attacking_Left,
    LH_Attacking_Down,
    LH_Attacking_Up,
    LH_Attacking,
    LH_Attack_OutofCage,
    LH_Attack,
    LH_Position4,
    LH_Position3,
    LH_Position2,
    LH_Position1,
    Centre,
    RH_Position1,
    RH_Position2,
    RH_Position3,
    RH_Position4,
    RH_Attack,
    RH_Attack_OutofCage,
    RH_Attacking,
    RH_Attacking_Up,
    RH_Attacking_Down,
    RH_Attacking_Left,

};

inline XPosition &operator++(XPosition &c ) {
    c = static_cast<XPosition>( static_cast<uint8_t>(c) + 1 );
    return c;
}

inline XPosition operator++(XPosition &c, int ) {
    XPosition result = c;
    ++c;
    return result;
}

inline XPosition &operator--(XPosition &c ) {
    c = static_cast<XPosition>( static_cast<uint8_t>(c) - 1 );
    return c;
}

inline XPosition operator--(XPosition &c, int ) {
    XPosition result = c;
    --c;
    return result;
}

enum class YPosition : uint8_t {

    Level_1,
    Level_2,
    Level_3

};

inline YPosition &operator++(YPosition &c ) {
    c = static_cast<YPosition>( static_cast<uint8_t>(c) + 1 );
    return c;
}

inline YPosition operator++(YPosition &c, int ) {
    YPosition result = c;
    ++c;
    return result;
}

inline YPosition &operator--(YPosition &c ) {
    c = static_cast<YPosition>( static_cast<uint8_t>(c) - 1 );
    return c;
}

inline YPosition operator--(YPosition &c, int ) {
    YPosition result = c;
    --c;
    return result;
}

// ---------------------------------------------------------------------------------
// Extract individual digits of a uint16_t
//
template< size_t size >
void extractDigits(uint8_t (&buffer)[size], uint16_t value) {

    for(uint8_t i = 0; i < size; ++i) {
        buffer[i] = value % 10;
        value /= 10;
    }

}

namespace Constants {

    constexpr uint8_t Player1_Index = 1;
    constexpr uint8_t Player1_XPos = 5;
    constexpr YPosition Player1_YPos = YPosition::Level_1;
  
    constexpr uint8_t Player2_Index = 2;
    constexpr uint8_t Player2_XPos = 101;
    constexpr YPosition Player2_YPos = YPosition::Level_3;

    constexpr uint8_t Lion1_Index = 1;
    constexpr uint8_t Lion2_Index = 2;

    constexpr uint8_t Level_1 = 0;
    constexpr uint8_t Level_2 = 1;
    constexpr uint8_t Level_3 = 2;

    constexpr uint8_t Led_Delay = 8;
    constexpr uint8_t Particle_Count = 50;
    constexpr uint8_t LED_Brightness = 4;
 
}