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

  const uint8_t  Player[] = {

    11, 121, 74, static_cast<uint8_t>(Stance::Normal_RHS), static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    13, 121, 74, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    15, 121, 74, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    17, 120, 73, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    19, 120, 73, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    21, 120, 73, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    23, 120, 73, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    25, 120, 73, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    27, 119, 72, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    29, 119, 72, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    31, 119, 72, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    33, 119, 72, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    35, 119, 72, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    37, 118, 71, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    39, 118, 71, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    41, 118, 71, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    43, 118, 71, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    45, 118, 71, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    47, 117, 70, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    49, 117, 70, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    51, 117, 70, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    53, 117, 70, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    55, 117, 70, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    57, 116, 69, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    59, 116, 69, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    61, 116, 69, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    63, 116, 69, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    65, 116, 69, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    67, 115, 67, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    69, 115, 67, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    71, 115, 67, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    73, 115, 67, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    75, 115, 67, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    77, 114, 66, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    79, 114, 66, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    81, 114, 66, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    83, 114, 66, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    85, 114, 66, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    87, 113, 65, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    89, 113, 65, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    91, 113, 65, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Up),  // Soft corner up

    // Bottom of first ladder ..
    93, 113, 65, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Left),
    93, 111, 63, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    93, 109, 61, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    93, 107, 59, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    93, 105, 57, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    93, 103, 55, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    93, 101, 53, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    93, 99, 51, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    93, 97, 49, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    93, 95, 47, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    93, 93, 45, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    93, 91, 43, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    93, 89, 41, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Down),
    91, 89, 41, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Down), // Soft corner down
    89, 88, 40, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    87, 88, 40, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    85, 88, 40, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    83, 88, 40, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    81, 88, 40, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    79, 87, 39, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    77, 87, 39, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    75, 87, 39, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    73, 87, 39, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    71, 87, 39, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    69, 86, 38, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    67, 86, 38, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    65, 86, 38, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    63, 86, 38, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    61, 86, 38, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    59, 85, 37, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    57, 85, 37, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    55, 85, 37, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    53, 85, 37, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),

    51, 85, 37, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    49, 84, 36, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    47, 84, 36, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Jump),
    45, 84, 36, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    43, 84, 36, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),

    41, 84, 36, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    39, 83, 35, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    37, 83, 35, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    35, 83, 35, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    33, 83, 35, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),

    31, 83, 35, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    29, 82, 34, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    27, 82, 34, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    25, 82, 34, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    23, 82, 34, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),

    21, 82, 34, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    19, 81, 33, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    17, 81, 33, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    15, 81, 33, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    13, 81, 33, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Up),  // soft corner up

    11, 81, 33, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Reverse) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Up),
    11, 79, 31, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 77, 29, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 75, 27, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 73, 25, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),

    11, 71, 23, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 69, 21, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 67, 19, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 65, 17, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 63, 15, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 61, 13, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 59, 11, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 57, 9, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 55, 7, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 53, 5, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 51, 3, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 49, 1, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 47, 0, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 45, 0, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 43, 0, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 41, 0, static_cast<uint8_t>(Stance::Ladder_02), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    11, 39, 0, static_cast<uint8_t>(Stance::Ladder_01), static_cast<uint8_t>(Movements::Up) | static_cast<uint8_t>(Movements::Down),
    13, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Down) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Lever), // Top of upper ladder

    13, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::Down),  // Soft Corner down
    15, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    17, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    19, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    21, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    23, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    25, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    27, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    29, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    31, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    33, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    35, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    37, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    39, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),

    41, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    43, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    45, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    47, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    49, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    51, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    53, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    55, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    57, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    59, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    61, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    63, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    65, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    67, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    69, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    71, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    73, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpToCrane),
    75, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right) | static_cast<uint8_t>(Movements::JumpToCrane),
    #define END_OF_TOP_LEVEL static_cast<uint16_t>(148)
    //148 above

    // Jump properly ..

    77, 33, 0, static_cast<uint8_t>(Stance::Jump_RHS), static_cast<uint8_t>(Movements::Left),
    79, 31, 0, static_cast<uint8_t>(Stance::Jump_RHS), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    81, 30, 0, static_cast<uint8_t>(Stance::Jump_RHS), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    83, 29, 0, static_cast<uint8_t>(Stance::Jump_RHS), static_cast<uint8_t>(Movements::Left) | static_cast<uint8_t>(Movements::Right),
    #define LEAP_DECISION_POINT END_OF_TOP_LEVEL + static_cast<uint16_t>(4)

    #define PLAYER_CRANE_START LEAP_DECISION_POINT + static_cast<uint16_t>(1)
    85, 29, 0, static_cast<uint8_t>(Stance::OnCrane_RHS), 0,
    85, 29, 0, static_cast<uint8_t>(Stance::OnCrane_RHS), 0,
    85, 29, 0, static_cast<uint8_t>(Stance::OnCrane_RHS), 0,

    85, 29, 0, static_cast<uint8_t>(Stance::OnCrane_RHS), 0,
    85, 29, 0, static_cast<uint8_t>(Stance::OnCrane_RHS), 0,
    85, 29, 0, static_cast<uint8_t>(Stance::OnCrane_RHS), 0,
    85, 29, 0, static_cast<uint8_t>(Stance::OnCrane_RHS), 0,

    #define PLAYER_CRANE_LIFT_01 PLAYER_CRANE_START + static_cast<uint16_t>(7)
    95, 21, 0, static_cast<uint8_t>(Stance::OnCrane_RHS), 0,
    95, 21, 0, static_cast<uint8_t>(Stance::OnCrane_RHS), 0,
    91, 21, 0, static_cast<uint8_t>(Stance::OnCrane_EmptyHand), 0,
    91, 21, 0, static_cast<uint8_t>(Stance::OnCrane_EmptyHand), 0,

    #define PLAYER_CRANE_LIFT_02 PLAYER_CRANE_LIFT_01 + static_cast<uint16_t>(4)
    95, 23, 0, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,
    #define PLAYER_CRANE_POINTS PLAYER_CRANE_LIFT_02 + static_cast<uint16_t>(1)
    95, 23, 0, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,

    #define PLAYER_CRANE_LIFT_03 PLAYER_CRANE_LIFT_02 + static_cast<uint16_t>(2)
    100, 25, 0, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,
    100, 25, 0, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,
    100, 25, 0, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,

    #define PLAYER_CRANE_LIFT_04 PLAYER_CRANE_LIFT_03 + static_cast<uint16_t>(3)
    88, 29, 1, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,
    88, 29, 2, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,
    88, 29, 3, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,

    #define PLAYER_CRANE_LIFT_05 PLAYER_CRANE_LIFT_04 + static_cast<uint16_t>(3)
    // Crane Flat
    84, 42, 4, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,
    84, 42, 5, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,
    84, 42, 6, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,

    #define PLAYER_CRANE_LIFT_06 PLAYER_CRANE_LIFT_05 + static_cast<uint16_t>(3)
    // Crane Declined
    88, 50, 7, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,
    88, 50, 8, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,
    88, 50, 9, static_cast<uint8_t>(Stance::OnCrane_HoldingHook), 0,
    89, 52, 9, static_cast<uint8_t>(Stance::Normal), 0,

    89, 52, 9, static_cast<uint8_t>(Stance::Normal_RHS), 0,
    91, 52, 9, static_cast<uint8_t>(Stance::Running_01_RHS), 0,
    93, 52, 9, static_cast<uint8_t>(Stance::Running_02_RHS), 0,
    95, 52, 9, static_cast<uint8_t>(Stance::Running_03_RHS), 0,

    97, 52, 9, static_cast<uint8_t>(Stance::Running_04_RHS), 0,
    99, 52, 9, static_cast<uint8_t>(Stance::Running_01_RHS), 0,
    #define PLAYER_VICTORY_RUN_DECISION PLAYER_CRANE_LIFT_06 + static_cast<uint16_t>(9)

    101, 52, 9, static_cast<uint8_t>(Stance::Running_02_RHS), 0,
    103, 52, 9, static_cast<uint8_t>(Stance::Running_03_RHS), 0,
    105, 52, 9, static_cast<uint8_t>(Stance::Running_04_RHS), 0,
    107, 52, 9, static_cast<uint8_t>(Stance::Running_01_RHS), 0,

    109, 52, 9, static_cast<uint8_t>(Stance::Running_02_RHS), 0,
    111, 52, 9, static_cast<uint8_t>(Stance::Running_03_RHS), 0,
    113, 52, 9, static_cast<uint8_t>(Stance::Running_04_RHS), 0,
    115, 52, 9, static_cast<uint8_t>(Stance::Running_01_RHS), 0,

    117, 52, 9, static_cast<uint8_t>(Stance::Running_02_RHS), 0,
    119, 52, 9, static_cast<uint8_t>(Stance::Running_03_RHS), 0,
    121, 52, 9, static_cast<uint8_t>(Stance::Running_04_RHS), 0,
    123, 52, 9, static_cast<uint8_t>(Stance::Running_01_RHS), 0,

    125, 52, 9, static_cast<uint8_t>(Stance::Running_02_RHS), 0,
    127, 52, 9, static_cast<uint8_t>(Stance::Running_03_RHS), 0,
    129, 52, 9, static_cast<uint8_t>(Stance::Running_03_RHS), 0,

    #define PLAYER_CRANE_END PLAYER_VICTORY_RUN_DECISION + static_cast<uint16_t>(15)


    // Die Sequence
    #define PLAYER_DIE_START PLAYER_CRANE_END + static_cast<uint16_t>(1)
    85, 30, 0, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    87, 31, 1, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    91, 33, 3, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    93, 35, 5, static_cast<uint8_t>(Stance::Jump_RHS), 0,

    95, 38, 8, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    97, 41, 11, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    99, 44, 14, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    101, 48, 18, static_cast<uint8_t>(Stance::Jump_RHS), 0,

    103, 51, 21, static_cast<uint8_t>(Stance::Dead_01), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_02), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,

    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,

    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,
    #define PLAYER_DIE_END PLAYER_DIE_START + static_cast<uint16_t>(17)

    #define PLAYER_RUNOFF_START PLAYER_DIE_END + static_cast<uint16_t>(1)
    77, 36, 0, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    79, 36, 0, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    81, 36, 0, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    83, 37, 0, static_cast<uint8_t>(Stance::Jump_RHS), 0,

    85, 37, 0, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    87, 37, 1, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    91, 38, 3, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    93, 38, 5, static_cast<uint8_t>(Stance::Jump_RHS), 0,

    95, 39, 8, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    97, 40, 11, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    99, 41, 14, static_cast<uint8_t>(Stance::Jump_RHS), 0,
    101, 43, 18, static_cast<uint8_t>(Stance::Jump_RHS), 0,

    103, 46, 21, static_cast<uint8_t>(Stance::Dead_01), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_02), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,

    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,

    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,
    105, 51, 21, static_cast<uint8_t>(Stance::Dead_03), 0,    
    #define PLAYER_RUNOFF_END PLAYER_RUNOFF_START + static_cast<uint16_t>(21)

    #define PLAYER_VICTORY_RUN_START PLAYER_RUNOFF_END + static_cast<uint16_t>(1)
    199, 52, 9, static_cast<uint8_t>(Stance::Normal_RHS), 0,
    99, 52, 9, static_cast<uint8_t>(Stance::Normal_RHS), 0,
    99, 52, 9, static_cast<uint8_t>(Stance::Normal), 0,
    99, 52, 9, static_cast<uint8_t>(Stance::Normal), 0,

    97, 51, 9, static_cast<uint8_t>(Stance::Running_01), 0,
    95, 49, 9, static_cast<uint8_t>(Stance::Running_02), 0,
    93, 46, 9, static_cast<uint8_t>(Stance::Running_03), 0,
    91, 44, 9, static_cast<uint8_t>(Stance::Running_04), 0,

    89, 42, 9, static_cast<uint8_t>(Stance::Running_01), 0,
    87, 40, 9, static_cast<uint8_t>(Stance::Running_02), 0,
    85, 38, 9, static_cast<uint8_t>(Stance::Running_03), 0,
    83, 36, 9, static_cast<uint8_t>(Stance::Running_04), 0,

    81, 34, 9, static_cast<uint8_t>(Stance::Running_01), 0,
    79, 33, 9, static_cast<uint8_t>(Stance::Running_02), 0,
    77, 34, 9, static_cast<uint8_t>(Stance::Running_03), 0,
    75, 36, 9, static_cast<uint8_t>(Stance::Running_04), 0,

    73, 36, 9, static_cast<uint8_t>(Stance::Running_01), 0,
    71, 36, 9, static_cast<uint8_t>(Stance::Running_02), 0,
    69, 36, 9, static_cast<uint8_t>(Stance::Running_03), 0,
    67, 36, 9, static_cast<uint8_t>(Stance::Running_04), 0, //20

    65, 36, 9, static_cast<uint8_t>(Stance::Running_01), 0,
    63, 36, 9, static_cast<uint8_t>(Stance::Running_02), 0,
    61, 36, 9, static_cast<uint8_t>(Stance::Running_03), 0,
    59, 36, 9, static_cast<uint8_t>(Stance::Running_04), 0,

    57, 36, 9, static_cast<uint8_t>(Stance::Running_01), 0,
    55, 36, 9, static_cast<uint8_t>(Stance::Running_02), 0,
    53, 36, 9, static_cast<uint8_t>(Stance::Running_03), 0,
    51, 36, 9, static_cast<uint8_t>(Stance::Running_04), 0,

    49, 36, 9, static_cast<uint8_t>(Stance::Running_01), 0,
    47, 36, 9, static_cast<uint8_t>(Stance::Running_02), 0,
    45, 36, 9, static_cast<uint8_t>(Stance::Running_03), 0,
    43, 36, 9, static_cast<uint8_t>(Stance::Running_04), 0,

    41, 36, 9, static_cast<uint8_t>(Stance::Running_01), 0,
    39, 36, 9, static_cast<uint8_t>(Stance::Running_02), 0,
    37, 36, 9, static_cast<uint8_t>(Stance::Running_03), 0,
    35, 36, 9, static_cast<uint8_t>(Stance::Running_04), 0,

    33, 36, 9, static_cast<uint8_t>(Stance::Running_01), 0,
    31, 36, 9, static_cast<uint8_t>(Stance::Running_02), 0,
    29, 36, 9, static_cast<uint8_t>(Stance::Running_03), 0,
    27, 34, 9, static_cast<uint8_t>(Stance::Running_04), 0, //20

    25, 32, 9, static_cast<uint8_t>(Stance::Running_01), 0,
    23, 30, 8, static_cast<uint8_t>(Stance::Running_02), 0,
    21, 28, 7, static_cast<uint8_t>(Stance::Running_03), 0,
    19, 26, 6, static_cast<uint8_t>(Stance::Running_04), 0,

    17, 24, 5, static_cast<uint8_t>(Stance::Jump), 0,
    15, 22, 4, static_cast<uint8_t>(Stance::Jump), 0,
    15, 20, 3, static_cast<uint8_t>(Stance::Jump), 0,
    15, 19, 2, static_cast<uint8_t>(Stance::Jump), 0,

    15, 17, 1, static_cast<uint8_t>(Stance::Jump), 0,
    13, 15, 0, static_cast<uint8_t>(Stance::Jump), 0,
    11, 14, 0, static_cast<uint8_t>(Stance::Running_01), 0,
    9, 14, 0, static_cast<uint8_t>(Stance::Running_02), 0,

    7, 14, 0, static_cast<uint8_t>(Stance::Running_03), 0,
    5, 14, 0, static_cast<uint8_t>(Stance::Running_04), 0,
    3, 14, 0, static_cast<uint8_t>(Stance::Running_01), 0,
    3, 14, 0, static_cast<uint8_t>(Stance::Ladder_01), 0,
    #define PLAYER_VICTORY_RUN_POINTS PLAYER_VICTORY_RUN_START + static_cast<uint16_t>(56)

    3, 14, 0, static_cast<uint8_t>(Stance::Ladder_01), 0,
    3, 14, 0, static_cast<uint8_t>(Stance::Ladder_02), 0,
    3, 14, 0, static_cast<uint8_t>(Stance::Ladder_02), 0,
    3, 14, 0, static_cast<uint8_t>(Stance::Ladder_01), 0, //20

    3, 14, 0, static_cast<uint8_t>(Stance::Ladder_01), 0,
    3, 14, 0, static_cast<uint8_t>(Stance::Ladder_02), 0,
    3, 14, 0, static_cast<uint8_t>(Stance::Ladder_02), 0,
    3, 14, 0, static_cast<uint8_t>(Stance::Ladder_01), 0,

    3, 14, 0, static_cast<uint8_t>(Stance::Ladder_01), 0,
    3, 14, 0, static_cast<uint8_t>(Stance::Ladder_02), 0,
    3, 14, 0, static_cast<uint8_t>(Stance::Ladder_02), 0,
    3, 14, 0, static_cast<uint8_t>(Stance::Ladder_01), 0,
    #define PLAYER_VICTORY_RUN_EAT_DINNER PLAYER_VICTORY_RUN_START + static_cast<uint16_t>(68)

    3, 14, 0, static_cast<uint8_t>(Stance::Running_01), 0,
    1, 14, 0, static_cast<uint8_t>(Stance::Running_02), 0,
    static_cast<uint8_t>(-1), 14, 0, static_cast<uint8_t>(Stance::Running_03), 0,
    static_cast<uint8_t>(-3), 14, 0, static_cast<uint8_t>(Stance::Running_04), 0,

    static_cast<uint8_t>(-5), 14, 0, static_cast<uint8_t>(Stance::Running_01), 0,
    static_cast<uint8_t>(-7), 14, 0, static_cast<uint8_t>(Stance::Running_02), 0,
    static_cast<uint8_t>(-9), 14, 0, static_cast<uint8_t>(Stance::Running_03), 0,
    static_cast<uint8_t>(-11), 14, 0, static_cast<uint8_t>(Stance::Running_04), 0,

    #define PLAYER_VICTORY_RUN_END PLAYER_VICTORY_RUN_START + static_cast<uint16_t>(75) 

    121, 36, 0, static_cast<uint8_t>(Stance::Normal), static_cast<uint8_t>(Movements::Left),

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