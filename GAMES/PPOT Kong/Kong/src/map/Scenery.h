#pragma once

#include "../utils/Enums.h"

#define SCENERY_COUNT 76
#define SCENERY_LEVEL_2_ONLY 128
#define SCENERY_PAINT_LAST 64
#define SCENERY_PAINT_FIRST 0

#define SCENERY_NUMBER_OF_ELEMENTS 3

namespace Coordinates {

  struct SceneryData {

    int8_t x;
    uint8_t y;
    Components image;
    uint8_t mode;

  };

  const uint8_t PROGMEM Scenery[] = {

    24, 30, static_cast<uint8_t>(Components::Buildings),
    0, 25, static_cast<uint8_t>(Components::Girder_OverHead),
    0, 5, static_cast<uint8_t>(Components::Dinner),

    12, 24, static_cast<uint8_t>(Components::Plate1),
    36, 24, static_cast<uint8_t>(Components::Plate2),
    60, 24, static_cast<uint8_t>(Components::Plate3),

    // X, Y, Item, Spare
    9, 67, static_cast<uint8_t>(Components::Girder),
    19, 66, static_cast<uint8_t>(Components::Girder),
    29, 65, static_cast<uint8_t>(Components::Girder),
    39, 64, static_cast<uint8_t>(Components::Girder),
    49, 63, static_cast<uint8_t>(Components::Girder),
    59, 62, static_cast<uint8_t>(Components::Girder),
    69, 61, static_cast<uint8_t>(Components::Girder),

    9, 47, static_cast<uint8_t>(Components::Ladder),

    21, 47, static_cast<uint8_t>(Components::Girder_OverHead),
    33, 47, static_cast<uint8_t>(Components::Girder_OverHead),
    45, 47, static_cast<uint8_t>(Components::Girder_OverHead),
    57, 47, static_cast<uint8_t>(Components::Girder_OverHead),
    69, 47, static_cast<uint8_t>(Components::Girder_OverHead),

    //Lower Half, 10, , 0,

    23, 75, static_cast<uint8_t>(Components::Girder_OverHead),
    35, 75, static_cast<uint8_t>(Components::Girder_OverHead),
    87, 80, static_cast<uint8_t>(Components::Girder_OverHead),
    95, 80, static_cast<uint8_t>(Components::Girder_OverHead),
    107, 80, static_cast<uint8_t>(Components::Girder_OverHead),
    119, 80, static_cast<uint8_t>(Components::Girder_OverHead),

  99, 73, static_cast<uint8_t>(Components::Girder_OverHead),
  111, 73, static_cast<uint8_t>(Components::Girder_OverHead),
  123, 73, static_cast<uint8_t>(Components::Girder_OverHead),

    107, 80, static_cast<uint8_t>(Components::EasyHard),


    87, 63, static_cast<uint8_t>(Components::Girder_OverHead),
    99, 63, static_cast<uint8_t>(Components::Girder_OverHead),
    111, 63, static_cast<uint8_t>(Components::Girder_OverHead),
    123, 63, static_cast<uint8_t>(Components::Girder_OverHead),


    static_cast<uint8_t>(-5), 108, static_cast<uint8_t>(Components::Girder_OverHead),
    22, 108, static_cast<uint8_t>(Components::Girder_OverHead),
    34, 108, static_cast<uint8_t>(Components::Girder_OverHead),
    46, 108, static_cast<uint8_t>(Components::Girder_OverHead),
    50, 108, static_cast<uint8_t>(Components::Girder_OverHead),

    79, 108, static_cast<uint8_t>(Components::Girder_OverHead),

    0, 92, static_cast<uint8_t>(Components::Girder),
    10, 93, static_cast<uint8_t>(Components::Girder),
    20, 94, static_cast<uint8_t>(Components::Girder),

    30, 95, static_cast<uint8_t>(Components::Girder),
    40, 96, static_cast<uint8_t>(Components::Girder),
    50, 97, static_cast<uint8_t>(Components::Girder),
    60, 98, static_cast<uint8_t>(Components::Girder),

    70, 99, static_cast<uint8_t>(Components::Girder),
    80, 100, static_cast<uint8_t>(Components::Girder),
    90, 101, static_cast<uint8_t>(Components::Girder),
    113, 112, static_cast<uint8_t>(Components::Girder_Small),

    125, 112, static_cast<uint8_t>(Components::Girder_Small),
    10, 133, static_cast<uint8_t>(Components::Girder),
    20, 132, static_cast<uint8_t>(Components::Girder),
    30, 131, static_cast<uint8_t>(Components::Girder),
    40, 130, static_cast<uint8_t>(Components::Girder),
    50, 129, static_cast<uint8_t>(Components::Girder),
    60, 128, static_cast<uint8_t>(Components::Girder),
    70, 127, static_cast<uint8_t>(Components::Girder),

    80, 126, static_cast<uint8_t>(Components::Girder),
    90, 125, static_cast<uint8_t>(Components::Girder),
    100, 124, static_cast<uint8_t>(Components::Girder),
    9, 74, static_cast<uint8_t>(Components::Ladder),

    91, 106, static_cast<uint8_t>(Components::Ladder),

    static_cast<uint8_t>(-4), 47, static_cast<uint8_t>(Components::Girder_OverHead),
    0, 36, static_cast<uint8_t>(Components::Lever),


    0, 68, static_cast<uint8_t>(Components::Cable2) | SCENERY_LEVEL_2_ONLY,
    20, 68, static_cast<uint8_t>(Components::Cable1) | SCENERY_LEVEL_2_ONLY,
    40, 69, static_cast<uint8_t>(Components::Cable1) | SCENERY_LEVEL_2_ONLY,
    60, 70, static_cast<uint8_t>(Components::Cable1) | SCENERY_LEVEL_2_ONLY,
    80, 71, static_cast<uint8_t>(Components::Cable1) | SCENERY_LEVEL_2_ONLY,
    100, 72, static_cast<uint8_t>(Components::Cable1) | SCENERY_LEVEL_2_ONLY,
    120, 73, static_cast<uint8_t>(Components::Cable1) | SCENERY_LEVEL_2_ONLY,


    91, 10, static_cast<uint8_t>(Components::Crane),
    85, 0, static_cast<uint8_t>(Components::Hook),
    0, 122, static_cast<uint8_t>(Components::Fire),
    0, 122, static_cast<uint8_t>(Components::Fire_Foreground) | SCENERY_PAINT_LAST,

    0, 0, static_cast<uint8_t>(Components::Girder),

  };

  inline void readSceneryData(Coordinates::SceneryData &sceneryData, uint8_t index) {

    int8_t x = pgm_read_byte(&Coordinates::Scenery[(index * SCENERY_NUMBER_OF_ELEMENTS)]);
    uint8_t y = pgm_read_byte(&Coordinates::Scenery[(index * SCENERY_NUMBER_OF_ELEMENTS) + 1]);
    uint8_t image = pgm_read_byte(&Coordinates::Scenery[(index * SCENERY_NUMBER_OF_ELEMENTS) + 2]) & 0x1F;
    uint8_t mode = pgm_read_byte(&Coordinates::Scenery[(index * SCENERY_NUMBER_OF_ELEMENTS) + 2]) & 0xC0;

    sceneryData.x = x;
    sceneryData.y = y;
    sceneryData.image = static_cast<Components>(image);
    sceneryData.mode = mode;
    
  } 

}