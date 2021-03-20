#include <Arduboy2.h>

#pragma once

#define SOUNDS

enum class GameState : uint8_t {
    Splash_Init,
    Splash,
    Title_Init,
    Title,
    Instructions_Init,
    Instructions_Scene1,
    Instructions_Scene2,
    Instructions_Scene3,
    Instructions_Scene4,
    Instructions_Scene5,
    Instructions_Scene6,
    PlayGame_Init,
    PlayGame,
    HighScore_Init,
    HighScore
};

enum class ThrowOil : uint8_t {
    LH_Top,
    LH_Middle,
    LH_Bottom,
    LH_Miss_Down_Start,
    LH_Miss_Down_01,
    LH_Miss_Down_02,
    LH_Miss_Down_03,
    LH_Miss_Down_04,
    LH_Miss_Down_05,
    LH_Miss_Down_06,
    LH_Miss_Down_07,
    LH_Miss_Down_08,
    LH_Miss_Down_End,
    LH_Miss_Bottom_Start,
    LH_Miss_Bottom_01,
    LH_Miss_Bottom_02,
    LH_Miss_Bottom_03,
    LH_Miss_Bottom_End_Of_Splash,
    LH_Miss_Bottom_05,
    LH_Miss_Bottom_06,
    LH_Miss_Bottom_07,
    LH_Miss_Bottom_08,
    LH_Miss_Bottom_End,
    LH_Miss_Up_Start,
    LH_Miss_Up_01,
    LH_Miss_Up_02,
    LH_Miss_Up_03,
    LH_Miss_Up_04,
    LH_Miss_Up_05,
    LH_Miss_Up_06,
    LH_Miss_Up_07,
    LH_Miss_Up_08,
    LH_Miss_Up_NearlyEnd,
    LH_Miss_Up_End,
    RH_Top,
    RH_Middle,
    RH_Bottom,
    RH_Miss_Down_Start,
    RH_Miss_Down_01,
    RH_Miss_Down_02,
    RH_Miss_Down_03,
    RH_Miss_Down_04,
    RH_Miss_Down_05,
    RH_Miss_Down_06,
    RH_Miss_Down_07,
    RH_Miss_Down_08,
    RH_Miss_Down_09,
    RH_Miss_Down_End,
    RH_Miss_Bottom_Start,
    RH_Miss_Bottom_01,
    RH_Miss_Bottom_02,
    RH_Miss_Bottom_03,
    RH_Miss_Bottom_End_Of_Splash,
    RH_Miss_Bottom_05,
    RH_Miss_Bottom_06,
    RH_Miss_Bottom_07,
    RH_Miss_Bottom_08,
    RH_Miss_Bottom_End,
    RH_Miss_Up_Start,
    RH_Miss_Up_01,
    RH_Miss_Up_02,
    RH_Miss_Up_03,
    RH_Miss_Up_04,
    RH_Miss_Up_05,
    RH_Miss_Up_06,
    RH_Miss_Up_07,
    RH_Miss_Up_08,
    RH_Miss_Up_NearlyEnd,
    RH_Miss_Up_End,
    None    
};

enum class GameScene : uint8_t {
    Indoors,
    Outdoors,
};

enum class Direction : uint8_t {
    Left,
    Right,
    Up,
    Down,
    None
};

enum class XPosition : uint8_t {
    Position_Throwing_LH,
    Position_Outside_LH,
    Position_Tipping_LH,
    Position_1,
    Position_2,
    Position_3,
    Position_4,
    Position_5_Oil,
    Position_6,
    Position_7,
    Position_8,
    Position_9,
    Position_10_Oil,
    Position_11,
    Position_12,
    Position_13,
    Position_14,
    Position_15_Oil,
    Position_16,
    Position_17,
    Position_18,
    Position_19,
    Position_Tipping_RH,
    Position_Outside_RH,
    Position_Throwing_RH,
};

enum class YPosition : uint8_t {
    StartDrip_00,
    StartDrip_01,
    StartDrip_02,
    StartDrip_03,
    Falling_04,
    Falling_05,
    Falling_06,
    Falling_07,
    Falling_08,
    Falling_09,
    Falling_10,
    Falling_11,
    Falling_12,
    Falling_13,
    Fire_00,
    Fire_01,
    Fire_02,
    Fire_03,
    Fire_04,
    Fire_05,
    Fire_06,
    Fire_07,
    None,
};

// --------------------------------------------------------------------

inline ThrowOil &operator++(ThrowOil &c ) {
    c = static_cast<ThrowOil>( static_cast<uint8_t>(c) + 1 );
    return c;
}

inline ThrowOil operator++(ThrowOil &c, int ) {
    ThrowOil result = c;
    ++c;
    return result;
}

inline ThrowOil &operator--(ThrowOil &c ) {
    c = static_cast<ThrowOil>( static_cast<uint8_t>(c) - 1 );
    return c;
}

inline ThrowOil operator--(ThrowOil &c, int ) {
    ThrowOil result = c;
    --c;
    return result;
}

// --------------------------------------------------------------------

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

// --------------------------------------------------------------------

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

    constexpr uint8_t number_Of_Oils = 6;

    constexpr uint8_t turn_Delay_Score_0        = 40;     
    constexpr uint8_t turn_Delay_Score_1        = 80;
    constexpr uint8_t turn_Delay_Score_2        = 120;
    constexpr uint8_t turn_Delay_Min_0          = 20;
    constexpr uint8_t turn_Delay_Min_1          = 17;
    constexpr uint8_t turn_Delay_Min_2          = 14;
    constexpr uint8_t turn_Delay_Min_3          = 11;

    constexpr uint8_t LED_Brightness            = 8;

    constexpr uint8_t name_Length               = 3;
    constexpr uint8_t name_Length_Plus_Term     = name_Length + 1;
    constexpr uint8_t no_Winner                 = 255;
    constexpr uint8_t led_Red_Delay             = 40;
    constexpr uint8_t led_Green_Delay           = 5;

    const uint8_t PROGMEM ledGreenDelays[] = { led_Green_Delay + 1, (led_Green_Delay * 3) + 1, (led_Green_Delay * 5) + 1 };

}

