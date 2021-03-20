#pragma once

// - Debug ------------------------------

#define _DEBUG
#define _PUZZLE_LOAD_TEST
#define _SHOW_PUZZLE_NUMBER


// - Inclusions -------------------------

#ifdef DEBUG
    //#define BYTE_BEAT_SOUNDS
    #define _ARDUBOY_TONES_SOUNDS
    #define _FLICKERING
    #define _USE_LED_ANALOGUE
    #define _TV_TRANSITION
#else
    //#define BYTE_BEAT_SOUNDS
    #define ARDUBOY_TONES_SOUNDS
    #define FLICKERING
    #define USE_LED_ANALOGUE
    #define TV_TRANSITION   
#endif


// ---------------------------------------

#define CREDITS
#define NEW_TOTALS
#define _OLD_TOTALS
//#define _BYTE_BEAT_SOUNDS_1
//#define BYTE_BEAT_SOUNDS_3

#define NO_HACK_ID 255
#define BUILDING_COUNT 14
#define BUILDING_LOW_WIDTH 59
#define BUILDING_LOW_DOOR_X 6
#define BUILDING_MED_WIDTH 48
#define BUILDING_MED_DOOR_X 6
#define BUILDING_HIGH_WIDTH 46
#define BUILDING_HIGH_DOOR_X 4
#define BUILDING_DOOR_WIDTH 16
#define BUILDING_LIGHTPOLE_WIDTH 20
#define BUILDING_LIGHTPOLE_BULB_X 11
#define BUILDING_LIGHTPOLE_BULB_WIDTH 8
#define BUILDING_TERMINAL_WIDTH 13

#define _SHUFFLE_RANDOM
#define SHUFFLE_BY_SIZE

#ifdef BYTE_BEAT_SOUNDS
    #ifdef BYTE_BEAT_SOUNDS_1
        extern uint8_t hpISR;
    #endif
    #ifdef BYTE_BEAT_SOUNDS_3
        extern uint8_t byteBeatIdx;
    #endif
#endif

namespace Constants {

    constexpr uint8_t Hack_Max_Success_L0 = 8;
    constexpr uint8_t Hack_Max_Success_L1 = 10;
    constexpr uint8_t Hack_Max_Success_L2 = 12;
    constexpr uint8_t Hack_Max_Success_L3 = 14;

    constexpr uint16_t Deck_Price_L0 = 0;
    constexpr uint16_t Deck_Price_L1 = 100;
    constexpr uint16_t Deck_Price_L2 = 150;
    constexpr uint16_t Deck_Price_L3 = 200;

    constexpr uint8_t Player_CentrePos = 60;
    constexpr int16_t Town_RightPos = -600;
    constexpr uint8_t Score_Length_2 = 3;
    constexpr uint8_t Score_Length_3 = 8;
    constexpr uint8_t Score_Length_4 = 15;
    constexpr uint8_t Score_Length_5 = 30;
    constexpr uint8_t Map_Closed = 255;
    constexpr uint8_t Max_Guards = 4;
    constexpr uint8_t Upload_Delay = 16;
    constexpr uint8_t Door_Open_Length = 240;
    constexpr uint8_t Max_Deck_Count = 4;
    constexpr uint8_t Dist_To_Torch = 27;
    constexpr uint8_t Hack_None_Available = 255;
    constexpr uint8_t Hack_Game_Over = 254;
    constexpr uint8_t No_Deck_Avail = 255;
    constexpr uint8_t Objectives_Max_Number = 6;
    constexpr uint8_t Objective_Max_Length = 5;
    constexpr uint8_t Buffer_Max_Length = 7;
    constexpr uint8_t Grid_Max_Size = 6;
    constexpr uint8_t Grid_Blank = 255;

}

namespace Words {

    constexpr uint8_t No_Deck = 130;
    constexpr uint8_t Deck_V1 = No_Deck + 1;
    constexpr uint8_t Deck_V2 = Deck_V1 + 1;
    constexpr uint8_t Deck_V3 = Deck_V2 + 1;
    constexpr uint8_t Safe_House_Opts = Deck_V3 + 1;
    constexpr uint8_t Move_Here = Safe_House_Opts + 1;
    constexpr uint8_t Sold_Out = Move_Here + 1;

}

enum class GameState : uint8_t {
    Splash_Init, // 0
    Splash,
    Title_Init,
    Title,
    HackGame_Init, // 4
    HackGame,
    HackOver_Disconnected,
    HackOver_BufferFull,
    HackOver_CannotMove, //8
    HackOver_AllHacksPlayed,
    HackScore,
    ShowMap_Init,
    ShowMap, //12
    StealthGame_Init,
    StealthGame,
    #ifdef CREDITS
    Credits_Init,
    Credits,
    #endif
    #ifdef TV_TRANSITION
    TV_Init,
    TV,
    #endif
    Message_Intro_Init,
    Message_Move_To_CommercialDistrict_Init, // 16
    Message_Move_To_SafeHouse_Init,
    Message_Move_To_SlumsDistrict_Init,
    Message_Hack_Target_Not_Achieved_Init,
    Message_Hacks_Achieved_Init, // 20
    Message_Move_To_Hacking_Init,
    Message_Hacks_Game_Over_Init,
    Message_Game_Over_Success_Init,
    Message_Game_Over_Init,
    Message_Intro,
    Message_Move_To_CommercialDistrict,
    Message_Move_To_SafeHouse, // 24
    Message_Move_To_SlumsDistrict, 
    Message_Hack_Target_Not_Achieved,
    Message_Hacks_Achieved,
    Message_Move_To_Hacking,
    Message_Hacks_Game_Over,
    Message_Game_Over_Success,
    Message_Game_Over,
    Messages_Start_Init = Message_Intro_Init,
    Messages_End_Init = Message_Game_Over_Init,
    Messages_Start = Message_Intro,
    Messages_End = Message_Game_Over,
};

enum class Message : uint8_t {
    Test,
    SafeHouse_1,
    SafeHouse_2,
    SafeHouse_3,
    Hacking_1,
    Hacking_2,
    Hacking_3,
    SlumsDistrict_1,
    SlumsDistrict_2,
    SlumsDistrict_3,
    CommercialDistrict_1,
    CommercialDistrict_2,
    CommercialDistrict_3,
    Intro_1,
    Intro_2,
    Intro_3,
    Hack_Fail,
    Hack_Success,
    Hack_None_Available,
    Hacks_Game_Over,
    Game_Over,
    Game_Over_Success,
    Face_Player_Start = Test,
    Face_Player_End = Hacks_Game_Over,
};

enum class TargetType : uint8_t {
    None,
    Door,
    LightPole,
};

enum class TVMode : uint8_t {
    TurnOff,
    TurnOn
};

enum class Movement : uint8_t {
    IdleLeft,
    IdleRight,
    WalkingLeft,
    WalkingRight,
    CrouchLeft,
    CrouchRight,
    Hidden,
    LookingUpLeft,
    LookingUpRight,
    RandomMin = IdleLeft,
    RandomMax = WalkingRight,
};

enum class SelectionType : uint8_t {
    Cell,
    Row,
    Column,
};

enum class SelectionMode : uint8_t {
    Grid,
    Objectives
};

enum class MapMenu : uint8_t {
    CommercialDistrict,
    SafeHouse,
    SlumsDistrict,
    CyberDeck
};

enum class BuildingType : uint8_t {
    None,
    LowBuilding,
    MedBuilding,
    HighBuilding,
    Terminal,
    Min = LowBuilding,
    Max = HighBuilding,
};

enum ByteBeatIndex {
    Map = 0,
    Hack = 1,
    Street = 2
};

// --------------------------------------------------------------------

inline MapMenu &operator++(MapMenu &c ) {
    if (c == MapMenu::SlumsDistrict) {
        c = MapMenu::CommercialDistrict;
    }
    else {
        c = static_cast<MapMenu>( static_cast<uint8_t>(c) + 1 );
    }
    return c;
}

inline MapMenu operator++(MapMenu &c, int ) {
    MapMenu result = c;
    ++c;
    return result;
}

inline MapMenu &operator--(MapMenu &c ) {
    if (c == MapMenu::CommercialDistrict) {
        c = MapMenu::SlumsDistrict;
    }
    else {
       c = static_cast<MapMenu>( static_cast<uint8_t>(c) - 1 );
    }
    return c;
}

inline MapMenu operator--(MapMenu &c, int ) {
    MapMenu result = c;
    --c;
    return result;
}

