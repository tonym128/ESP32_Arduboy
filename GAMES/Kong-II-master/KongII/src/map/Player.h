#pragma once

#include "../utils/Enums.h"

#define PLAYER_NUMBER_OF_ELEMENTS 5

namespace Coordinates {

    struct PlayerData {

        int8_t x;
        int8_t y;
        uint8_t yOffset;
        Stance stance;
        uint8_t movements;

    };

    #define PLAYER_POS_LOWERKEY_START 14
    #define PLAYER_POS_LOWERKEY_END 16

    #define PLAYER_POS_UPPERKEY_START 220
    #define PLAYER_POS_UPPERKEY_END 222

    const uint8_t PROGMEM Player[] = {

    // 0
    11, 116, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Up),
    11, 115, 64, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 114, 64, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 113, 64, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),

    11, 112, 64, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 111, 64, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 110, 64, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 109, 64, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),

    11, 108, 64, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 107, 64, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 106, 64, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 105, 64, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),

    11, 104, 64, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 103, 64, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 102, 64, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 101, 64, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Jump),

    #define PLAYER_CLAPPER_LOWER_START 16
    12, 100, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    13, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    14, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    15, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    // 20
    16, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    17, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    18, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    19, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    #define PLAYER_SPARK_LOWER_IGNORE 24
    20, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    21, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    22, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    23, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    24, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    25, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    26, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    27, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    28, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    29, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    30, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    31, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    32, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    33, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    34, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    35, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    // 40
    36, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    37, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    38, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    39, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    40, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    41, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    42, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    43, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    44, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    45, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    46, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    47, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    48, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    49, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    50, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    51, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    52, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    53, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    54, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    55, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    // 60
    56, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    57, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    58, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    59, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    60, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    61, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    62, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    63, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    64, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    65, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    66, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    67, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    68, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    69, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    70, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    71, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    72, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    73, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    74, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    75, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    // 80
    76, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    77, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    78, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    79, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    80, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    81, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    82, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    83, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    84, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    #define PLAYER_VINE_0_MIN         89
    85, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    86, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    87, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    88, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),

    #define PLAYER_CLAPPER_LOWER_END  PLAYER_VINE_0_MIN + 4
    #define PLAYER_VINE_0             PLAYER_VINE_0_MIN + 4
    89, 101, 64, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::JumpUp),
    #define PLAYER_VINE_0_MAX         PLAYER_VINE_0 + 1
    89, 100, 64, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    89, 99, 64, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    89, 98, 64, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    89, 97, 64, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    89, 96, 64, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    89, 95, 64, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),

    #define PLAYER_VINE_0_CLIMBING    PLAYER_VINE_0_MAX + 6
    // 89, 97, 64, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    // 89, 96, 64, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    // 89, 95, 63, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    // 100
    89, 94, 62, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    89, 93, 61, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    89, 92, 60, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    89, 91, 59, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    89, 90, 58, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    89, 89, 57, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    89, 88, 56, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    89, 87, 55, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    89, 86, 54, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    89, 85, 53, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    89, 84, 52, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    89, 83, 51, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    89, 82, 51, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    //    88, 82, 50, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    89, 81, 49, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),


    //    #define PLAYER_SPARK_START 111
    #define PLAYER_SPARK_START 111
    87, 80, 48, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    86, 80, 47, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    85, 80, 46, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    84, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    83, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    82, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    // 120
    81, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    80, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    79, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    78, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    77, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    76, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    75, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    74, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    73, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    72, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    71, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    70, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    69, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    68, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    67, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    66, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    65, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    64, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    63, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    62, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    // 140
    61, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    60, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    59, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    58, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    57, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    56, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    55, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    54, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    53, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    52, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    51, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    50, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    49, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    48, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    47, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    46, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    45, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    44, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    43, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    42, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    // 160
    41, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    40, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    39, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    38, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    37, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    36, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    35, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    34, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    33, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    32, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    31, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    30, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    29, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    28, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    27, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    26, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    25, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    24, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    23, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    22, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    // 180
    21, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    20, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    #define PLAYER_VINE_1_MIN  182
    19, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    18, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    17, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    // 185
    16, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    #define PLAYER_SPARK_END    PLAYER_VINE_1_MIN + 4
    #define PLAYER_VINE_1       PLAYER_VINE_1_MIN + 4
    15, 80, 45, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    #define PLAYER_VINE_1_MAX   PLAYER_VINE_1 + 1
    15, 80, 45, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::Reverse),
    15, 79, 45, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::Reverse),
    15, 78, 45, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::Reverse),
    15, 77, 45, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::Reverse),
    15, 76, 45, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::Reverse),
    15, 75, 45, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::Reverse),
    15, 74, 45, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::Reverse),
    15, 73, 45, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::Reverse),
    #define PLAYER_VINE_1_CLIMBING  PLAYER_VINE_1_MAX + 8

    15, 72, 45, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 71, 45, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    15, 70, 45, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 69, 45, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 68, 43, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    // 200
    15, 67, 41, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 66, 39, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 65, 37, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 64, 35, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    15, 63, 33, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 62, 31, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 61, 29, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 60, 27, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    15, 59, 25, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 58, 23, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 57, 21, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 56, 19, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    15, 55, 16, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 54, 13, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 53, 10, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    15, 52, 7, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    15, 51, 4, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    15, 50, 2, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    15, 51, 0, static_cast<uint8_t>(Stance::Climbing_Vine_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    15, 49, 0, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),

    // 220
    15, 48, 0, static_cast<uint8_t>(Stance::Climbing_Vine_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Right),

    #define PLAYER_CLAPPER_UPPER_START 221
    15, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    16, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    17, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    18, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    19, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    20, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    21, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    22, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    23, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    #define PLAYER_CHAIN_0_GROUND_MIN     230    
    24, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    25, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    26, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    27, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    28, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    29, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    #define PLAYER_CHAIN_0_GROUND         PLAYER_CHAIN_0_GROUND_MIN + 6
    30, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    32, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    33, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    34, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    35, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    36, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    #define PLAYER_CHAIN_0_GROUND_MAX     PLAYER_CHAIN_0_GROUND + 7    

    37, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    38, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    39, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    40, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    41, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    42, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    43, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    #define PLAYER_CHAIN_1_GROUND_MIN     PLAYER_CHAIN_0_GROUND_MAX + 7    
    44, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    45, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    46, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    47, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    48, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    49, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    #define PLAYER_CHAIN_1_GROUND         PLAYER_CHAIN_1_GROUND_MIN + 6
    50, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    51, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    52, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    53, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    54, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    55, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    56, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    #define PLAYER_CHAIN_1_GROUND_MAX     PLAYER_CHAIN_1_GROUND + 7

    57, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    58, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    59, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    60, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    61, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    62, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    63, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    #define PLAYER_CHAIN_2_GROUND_MIN     PLAYER_CHAIN_1_GROUND_MAX + 7    
    64, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    65, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    66, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    67, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    68, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    69, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    #define PLAYER_CHAIN_2_GROUND         PLAYER_CHAIN_2_GROUND_MIN + 6
    70, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    71, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    72, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    73, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    74, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    75, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    76, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    #define PLAYER_CHAIN_2_GROUND_MAX     PLAYER_CHAIN_2_GROUND + 7

    77, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    78, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    79, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    80, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    81, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    82, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    83, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    #define PLAYER_CHAIN_3_GROUND_MIN     PLAYER_CHAIN_2_GROUND_MAX + 7    
    84, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    85, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    86, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    87, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    88, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpUp),
    #define PLAYER_CHAIN_3_GROUND         PLAYER_CHAIN_3_GROUND_MIN + 5
    89, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::JumpUp),
    #define PLAYER_CHAIN_3_GROUND_MAX     PLAYER_CHAIN_3_GROUND + 1
    #define PLAYER_CLAPPER_UPPER_END    PLAYER_CHAIN_3_GROUND
    0, 0, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),


    // --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


    #define PLAYER_CHAIN_0_BACKTOGROUND   PLAYER_CHAIN_3_GROUND_MAX + 1

    30, 48, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    30, 47, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    30, 46, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    30, 45, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),

    30, 44, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    30, 43, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    30, 42, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    30, 41, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),

    30, 40, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),

    #define PLAYER_CHAIN_0_CLIMBING     PLAYER_CHAIN_0_BACKTOGROUND + 9

    30, 39, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 38, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 37, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 36, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    30, 35, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 34, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 33, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 32, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    30, 31, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 30, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 29, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 28, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    30, 27, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 26, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 25, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 24, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    30, 23, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 22, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 21, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    30, 20, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down),

    #define PLAYER_CHAIN_0_END              PLAYER_CHAIN_0_CLIMBING + 20


    // --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


    #define PLAYER_CHAIN_1_BACKTOGROUND     PLAYER_CHAIN_0_END

    50, 48, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    50, 47, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    50, 46, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    50, 45, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),

    50, 44, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    50, 43, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    50, 42, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),
    50, 41, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),

    50, 40, 0, static_cast<uint8_t>(Stance::Falling_01), static_cast<uint8_t>(Movements::None),

    #define PLAYER_CHAIN_1_CLIMBING     PLAYER_CHAIN_1_BACKTOGROUND + 9

    50, 39, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 38, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 37, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 36, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    50, 35, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 34, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 33, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    50, 32, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 31, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 30, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 29, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    50, 28, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 27, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 26, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 25, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    50, 24, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 23, 0, static_cast<uint8_t>(Stance::OnRope_02), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 22, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    50, 21, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    50, 20, 0, static_cast<uint8_t>(Stance::OnRope_01), static_cast<uint8_t>(Movements::Down),

    #define PLAYER_CHAIN_1_END              PLAYER_CHAIN_1_CLIMBING + 20


    // --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


    #define PLAYER_CHAIN_2_BACKTOGROUND     PLAYER_CHAIN_1_END

    70, 48, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    70, 47, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    70, 46, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    70, 45, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),

    70, 44, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    70, 43, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    70, 42, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    70, 41, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),

    70, 40, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),

    #define PLAYER_CHAIN_2_CLIMBING     PLAYER_CHAIN_2_BACKTOGROUND + 9

    70, 39, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 38, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 37, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    70, 36, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 35, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 34, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 33, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    70, 32, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 31, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 30, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 29, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    70, 28, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 27, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 26, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 25, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    70, 24, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 23, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 22, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    70, 21, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    70, 20, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down),

    #define PLAYER_CHAIN_2_END              PLAYER_CHAIN_2_CLIMBING + 20


    // --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


    #define PLAYER_CHAIN_3_BACKTOGROUND     PLAYER_CHAIN_2_END

    90, 48, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    90, 47, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    90, 46, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    90, 45, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),

    90, 44, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    90, 43, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    90, 42, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    90, 41, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),

    90, 40, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    // 90, 39, 0, static_cast<uint8_t>(Stance::Falling_02), static_cast<uint8_t>(Movements::None),
    // 90, 38, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    // 90, 37, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    #define PLAYER_CHAIN_3_CLIMBING     PLAYER_CHAIN_3_BACKTOGROUND + 9

    90, 39, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 38, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 37, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    90, 36, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 35, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 34, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 33, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    90, 32, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 31, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 30, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 29, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    90, 28, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 27, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 26, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 25, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),

    90, 24, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 23, 0, static_cast<uint8_t>(Stance::OnRope_04), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 22, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 21, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Up),
    90, 20, 0, static_cast<uint8_t>(Stance::OnRope_03), static_cast<uint8_t>(Movements::Down),

    #define PLAYER_CHAIN_3_END              PLAYER_CHAIN_3_CLIMBING + 20


    // --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    #define PLAYER_CHAIN_MOVE_TO_EXIT_0     PLAYER_CHAIN_3_END + 1
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    31, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    32, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    33, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    34, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    35, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //4
    36, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    37, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    38, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    39, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //8
    40, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    41, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    42, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    43, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //12
    44, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    45, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    46, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    47, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //16
    48, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    49, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    #define PLAYER_CHAIN_MOVE_TO_EXIT_1     PLAYER_CHAIN_MOVE_TO_EXIT_0 + 37
    50, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    51, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //20
    52, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    53, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    54, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    55, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //24
    56, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    57, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    58, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    59, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //28
    60, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    61, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    62, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    63, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //32
    64, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    65, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    66, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    67, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //36
    68, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    69, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    #define PLAYER_CHAIN_MOVE_TO_EXIT_2     PLAYER_CHAIN_MOVE_TO_EXIT_1 + 20
    70, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    71, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //40
    72, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    73, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    74, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    75, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //44
    76, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    77, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    78, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    79, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //48
    80, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    81, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    82, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    83, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //52
    84, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    85, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    86, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    87, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //56
    88, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    89, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    #define PLAYER_CHAIN_MOVE_TO_EXIT_3     PLAYER_CHAIN_MOVE_TO_EXIT_2 + 20
    90, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    91, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //60
    92, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    93, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    94, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    95, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //64
    96, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    97, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    98, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    99, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //68
    100, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    101, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    102, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    103, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //72
    104, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    105, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    106, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    107, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //76
    108, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    109, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    110, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    111, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //80
    112, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    113, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    114, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    115, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //84
    116, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    117, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    118, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    119, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //88
    120, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    121, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    122, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    123, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //92
    124, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    125, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    126, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    127, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None), //96
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    // Give Kong a chance to walk out
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),
    128, 48, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    #define PLAYER_CHAIN_MOVE_TO_EXIT_END     PLAYER_CHAIN_MOVE_TO_EXIT_3 + 39

    0, 0, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::None),

    };


    inline void readPlayerData(Coordinates::PlayerData &playerData, uint16_t index) {

        int8_t x = static_cast<int8_t>(pgm_read_byte(&Coordinates::Player[(index * PLAYER_NUMBER_OF_ELEMENTS)]));
        int8_t y = pgm_read_byte(&Coordinates::Player[(index * PLAYER_NUMBER_OF_ELEMENTS) + 1]);
        uint8_t yOffset = pgm_read_byte(&Coordinates::Player[(index * PLAYER_NUMBER_OF_ELEMENTS) + 2]);
        Stance stance = static_cast<Stance>(pgm_read_byte(&Coordinates::Player[(index * PLAYER_NUMBER_OF_ELEMENTS) + 3]));
        uint8_t movements = pgm_read_byte(&Coordinates::Player[(index * PLAYER_NUMBER_OF_ELEMENTS) + 4]);

        playerData.x = x;
        playerData.y = y;
        playerData.yOffset = yOffset;
        playerData.stance = stance;
        playerData.movements = movements;

    } 

}