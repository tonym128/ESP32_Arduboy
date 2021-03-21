#pragma once

#include "../utils/Enums.h"

#define GIRDER_NUMBER_OF_ELEMENTS 3

namespace Coordinates {

  struct GirderData {

    int8_t x;
    uint8_t y;
    GirderRotation rotation;

  };

  const uint8_t PROGMEM Girder[] = {

    127, 74, static_cast<uint8_t>(GirderRotation::None),
    125, 74, static_cast<uint8_t>(GirderRotation::None),
    123, 74, static_cast<uint8_t>(GirderRotation::None),
    121, 74, static_cast<uint8_t>(GirderRotation::None),
    119, 74, static_cast<uint8_t>(GirderRotation::None),
    117, 74, static_cast<uint8_t>(GirderRotation::None),
    115, 74, static_cast<uint8_t>(GirderRotation::None),
    113, 74, static_cast<uint8_t>(GirderRotation::None),
    111, 74, static_cast<uint8_t>(GirderRotation::None),

    109, 73, static_cast<uint8_t>(GirderRotation::None),
    
    107, 73, static_cast<uint8_t>(GirderRotation::None),
    105, 73, static_cast<uint8_t>(GirderRotation::None),
    103, 73, static_cast<uint8_t>(GirderRotation::None),
    101, 73, static_cast<uint8_t>(GirderRotation::Left),
    99, 73, static_cast<uint8_t>(GirderRotation::Left),

    97, 73, static_cast<uint8_t>(GirderRotation::Left),
    95, 73, static_cast<uint8_t>(GirderRotation::Left),
    93, 73, static_cast<uint8_t>(GirderRotation::Left),
    91, 73, static_cast<uint8_t>(GirderRotation::Left),
    89, 73, static_cast<uint8_t>(GirderRotation::Left),

    87, 72, static_cast<uint8_t>(GirderRotation::Left),
    85, 72, static_cast<uint8_t>(GirderRotation::None),
    83, 72, static_cast<uint8_t>(GirderRotation::None),
    81, 72, static_cast<uint8_t>(GirderRotation::None),
    79, 72, static_cast<uint8_t>(GirderRotation::None),

    77, 72, static_cast<uint8_t>(GirderRotation::None),
    75, 72, static_cast<uint8_t>(GirderRotation::None),
    73, 72, static_cast<uint8_t>(GirderRotation::None),
    71, 72, static_cast<uint8_t>(GirderRotation::None),
    69, 72, static_cast<uint8_t>(GirderRotation::Right),

    67, 71, static_cast<uint8_t>(GirderRotation::Right),
    65, 71, static_cast<uint8_t>(GirderRotation::Right),
    63, 71, static_cast<uint8_t>(GirderRotation::Right),
    61, 71, static_cast<uint8_t>(GirderRotation::Right),
    59, 71, static_cast<uint8_t>(GirderRotation::Right),

    57, 71, static_cast<uint8_t>(GirderRotation::Right),
    55, 71, static_cast<uint8_t>(GirderRotation::Right),
    53, 71, static_cast<uint8_t>(GirderRotation::None),
    51, 71, static_cast<uint8_t>(GirderRotation::None),
    49, 71, static_cast<uint8_t>(GirderRotation::None),

    47, 70, static_cast<uint8_t>(GirderRotation::None),
    45, 70, static_cast<uint8_t>(GirderRotation::None),
    43, 70, static_cast<uint8_t>(GirderRotation::None),
    41, 70, static_cast<uint8_t>(GirderRotation::None),
    39, 70, static_cast<uint8_t>(GirderRotation::None),

    37, 70, static_cast<uint8_t>(GirderRotation::Left),
    35, 70, static_cast<uint8_t>(GirderRotation::Left),
    33, 70, static_cast<uint8_t>(GirderRotation::Left),
    31, 70, static_cast<uint8_t>(GirderRotation::Left),
    29, 70, static_cast<uint8_t>(GirderRotation::Left),

    27, 69, static_cast<uint8_t>(GirderRotation::Left),
    25, 69, static_cast<uint8_t>(GirderRotation::Left),
    23, 69, static_cast<uint8_t>(GirderRotation::Left),
    21, 69, static_cast<uint8_t>(GirderRotation::None),
    19, 69, static_cast<uint8_t>(GirderRotation::None),

    17, 69, static_cast<uint8_t>(GirderRotation::None),
    15, 69, static_cast<uint8_t>(GirderRotation::None),
    13, 69, static_cast<uint8_t>(GirderRotation::None),
    11, 69, static_cast<uint8_t>(GirderRotation::None),
    9, 69, static_cast<uint8_t>(GirderRotation::None),

    7, 68, static_cast<uint8_t>(GirderRotation::None),
    5, 68, static_cast<uint8_t>(GirderRotation::Right),
    3, 68, static_cast<uint8_t>(GirderRotation::Right),
    1, 68, static_cast<uint8_t>(GirderRotation::Right),
    static_cast<uint8_t>(-1), 68, static_cast<uint8_t>(GirderRotation::Right),

    static_cast<uint8_t>(-3), 68, static_cast<uint8_t>(GirderRotation::Right),
    static_cast<uint8_t>(-5), 68, static_cast<uint8_t>(GirderRotation::Right),
    static_cast<uint8_t>(-7), 68, static_cast<uint8_t>(GirderRotation::Right),
    static_cast<uint8_t>(-9), 68, static_cast<uint8_t>(GirderRotation::Right),
    static_cast<uint8_t>(-11), 68, static_cast<uint8_t>(GirderRotation::Right),

    static_cast<uint8_t>(-13), 67, static_cast<uint8_t>(GirderRotation::None),
    static_cast<uint8_t>(-15), 67, static_cast<uint8_t>(GirderRotation::None),
    static_cast<uint8_t>(-17), 67, static_cast<uint8_t>(GirderRotation::None),
    static_cast<uint8_t>(-19), 67, static_cast<uint8_t>(GirderRotation::None),
    static_cast<uint8_t>(-21), 67, static_cast<uint8_t>(GirderRotation::None),
    0, 0, static_cast<uint8_t>(GirderRotation::None),

  };

  inline void readGirderData(Coordinates::GirderData &girderData, uint8_t index) {

    int8_t x = pgm_read_byte(&Coordinates::Girder[(index * GIRDER_NUMBER_OF_ELEMENTS)]);
    uint8_t y = pgm_read_byte(&Coordinates::Girder[(index * GIRDER_NUMBER_OF_ELEMENTS) + 1]);
    uint8_t rotation = pgm_read_byte(&Coordinates::Girder[(index * GIRDER_NUMBER_OF_ELEMENTS) + 2]) & 0x1F;

    girderData.x = x;
    girderData.y = y;
    girderData.rotation = static_cast<GirderRotation>(rotation);
    
  } 

}	