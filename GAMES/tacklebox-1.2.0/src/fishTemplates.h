#pragma once


#include "nonMaskBitmaps.h"

const uint8_t NUM_U8T_PROPS = 13;
const uint8_t NUM_16T_PROPS = 6;


const uint8_t PROGMEM fish_templates_u8t[] = {

    // GOLDFISH
    0,  // minHour
    23,  // maxHour
    1,  // baitPreference WORM
    1,  // baitPreference GRUB
    0,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    5,  // ratio
    2,  // value
    8, // nameLength
    8, // bmpWidth
    3, // pull
    0, // deepWater
    54, // proWindow

    // GUPPY
    0,  // minHour
    23,  // maxHour
    1,  // baitPreference WORM
    0,  // baitPreference GRUB
    0,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    5,  // ratio
    1,  // value
    5, // nameLength
    8, // bmpWidth
    2, // pull
    0, // deepWater
    80, // proWindow

    // MINNOW
    0,  // minHour
    23,  // maxHour
    0,  // baitPreference WORM
    1,  // baitPreference GRUB
    0,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    6,  // ratio
    3,  // value
    6, // nameLength
    4, // bmpWidth
    3, // pull
    0, // deepWater
    64, // proWindow

    // BETA
    0,  // minHour
    23,  // maxHour
    1,  // baitPreference WORM
    0,  // baitPreference GRUB
    0,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    3,  // ratio
    30,  // value
    4, // nameLength
    8, // bmpWidth
    4, // pull
    0, // deepWater
    80, // proWindow

    // TROUT
    0,  // minHour
    23,  // maxHour
    1,  // baitPreference WORM
    1,  // baitPreference GRUB
    1,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    3,  // ratio
    14,  // value
    5, // nameLength
    10, // bmpWidth
    5, // pull
    0, // deepWater
    54, // proWindow

    // BASS
    3,  // minHour
    11,  // maxHour
    1,  // baitPreference WORM
    1,  // baitPreference GRUB
    0,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    10,  // ratio
    25,  // value
    4, // nameLength
    12, // bmpWidth
    5, // pull
    1, // deepWater
    50, // proWindow

    // CATFISH
    12,  // minHour
    20,  // maxHour
    1,  // baitPreference WORM
    1,  // baitPreference GRUB
    0,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    10,  // ratio
    15,  // value
    7, // nameLength
    12, // bmpWidth
    5, // pull
    1, // deepWater
    72, // proWindow

    // FLOUNDER
    0,  // minHour
    23,  // maxHour
    0,  // baitPreference WORM
    0,  // baitPreference GRUB
    1,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    8,  // ratio
    20,  // value
    8, // nameLength
    12, // bmpWidth
    4, // pull
    0, // deepWater
    50, // proWindow

    // CRAB
    10,  // minHour
    16,  // maxHour
    0,  // baitPreference WORM
    1,  // baitPreference GRUB
    1,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    2,  // ratio
    25,  // value
    4, // nameLength
    12, // bmpWidth
    3, // pull
    0, // deepWater
    70, // proWindow

    // OCTOPUS
    12,  // minHour
    23,  // maxHour
    0,  // baitPreference WORM
    1,  // baitPreference GRUB
    1,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    1,  // ratio
    80,  // value
    7, // nameLength
    14, // bmpWidth
    8, // pull
    0, // deepWater
    70, // proWindow

    // GROUPER
    0,  // minHour
    23,  // maxHour
    0,  // baitPreference WORM
    0,  // baitPreference GRUB
    1,  // baitPreference SHRIMP
    1,  // baitPreference MEAT
    4,  // ratio
    10,  // value
    7, // nameLength
    16, // bmpWidth
    6, // pull
    0, // deepWater
    62, // proWindow

    // SEAHORSE
    3,  // minHour
    12,  // maxHour
    0,  // baitPreference WORM
    1,  // baitPreference GRUB
    0,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    8,  // ratio
    4,  // value
    8, // nameLength
    4, // bmpWidth
    1, // pull
    0, // deepWater
    50, // proWindow

    // EEL
    0,  // minHour
    23,  // maxHour
    0,  // baitPreference WORM
    1,  // baitPreference GRUB
    1,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    6,  // ratio
    8,  // value
    3, // nameLength
    16, // bmpWidth
    4, // pull
    0, // deepWater
    60, // proWindow

    // SARDINE
    4,  // minHour
    11,  // maxHour
    0,  // baitPreference WORM
    1,  // baitPreference GRUB
    1,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    8,  // ratio
    2,  // value
    7, // nameLength
    5, // bmpWidth
    2, // pull
    0, // deepWater
    64, // proWindow

    // CRAWFISH
    0,  // minHour
    9,  // maxHour
    1,  // baitPreference WORM
    1,  // baitPreference GRUB
    0,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    4,  // ratio
    20,  // value
    8, // nameLength
    8, // bmpWidth
    6, // pull
    0, // deepWater
    66, // proWindow

    // FANGTOOTH
    0,  // minHour
    23,  // maxHour
    0,  // baitPreference WORM
    0,  // baitPreference GRUB
    1,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    4,  // ratio
    40,  // value
    9, // nameLength
    12, // bmpWidth
    7, // pull
    1, // deepWater
    68, // proWindow

    // LOBSTER
    0,  // minHour
    23,  // maxHour
    0,  // baitPreference WORM
    0,  // baitPreference GRUB
    1,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    4,  // ratio
    80,  // value
    7, // nameLength
    11, // bmpWidth
    5, // pull
    0, // deepWater
    80, // proWindow

    // MARLIN
    4,  // minHour
    10,  // maxHour
    0,  // baitPreference WORM
    0,  // baitPreference GRUB
    1,  // baitPreference SHRIMP
    1,  // baitPreference MEAT
    3,  // ratio
    100,  // value
    6, // nameLength
    20, // bmpWidth
    7, // pull
    1, // deepWater
    75, // proWindow

    // SHARK
    0,  // minHour
    14,  // maxHour
    0,  // baitPreference WORM
    0,  // baitPreference GRUB
    0,  // baitPreference SHRIMP
    1,  // baitPreference MEAT
    2,  // ratio
    200,  // value
    5, // nameLength
    24, // bmpWidth
    8, // pull
    1, // deepWater
    80, // proWindow

    // GRTWHITE
    13,  // minHour
    23,  // maxHour
    0,  // baitPreference WORM
    0,  // baitPreference GRUB
    0,  // baitPreference SHRIMP
    1,  // baitPreference MEAT
    1,  // ratio
    250,  // value
    8, // nameLength
    30, // bmpWidth
    8, // pull
    1, // deepWater
    95, // proWindow

    // OLD BOOT
    30,  // minHour
    30,  // maxHour
    0,  // baitPreference WORM
    0,  // baitPreference GRUB
    0,  // baitPreference SHRIMP
    0,  // baitPreference MEAT
    0,  // ratio
    0,  // value
    8, // nameLength
    6, // bmpWidth
    3, // pull
    0, // deepWater
    64, // proWindow
};


const uint8_t GOLDFISH_string[9] PROGMEM = {
    17, 25, 22, 14, 16, 19, 29, 18, 0xFF
};

const uint8_t GUPPY_string[6] PROGMEM = {
    17, 31, 26, 26, 35, 0xFF
};

const uint8_t MINNOW_string[7] PROGMEM = {
    23, 19, 24, 24, 25, 33, 0xFF
};

const uint8_t BETA_string[5] PROGMEM = {
    12, 15, 30, 11, 0xFF
};

const uint8_t TROUT_string[6] PROGMEM = {
    30, 28, 25, 31, 30, 0xFF
};

const uint8_t BASS_string[5] PROGMEM = {
    12, 11, 29, 29, 0xFF
};

const uint8_t CATFISH_string[8] PROGMEM = {
    13, 11, 30, 16, 19, 29, 18, 0xFF
};

const uint8_t FLOUNDER_string[9] PROGMEM = {
    16, 22, 25, 31, 24, 14, 15, 28, 0xFF
};

const uint8_t CRAB_string[5] PROGMEM = {
    13, 28, 11, 12, 0xFF
};

const uint8_t OCTOPUS_string[8] PROGMEM = {
    25, 13, 30, 25, 26, 31, 29, 0xFF
};

const uint8_t GROUPER_string[8] PROGMEM = {
    17, 28, 25, 31, 26, 15, 28, 0xFF
};

const uint8_t SEAHORSE_string[9] PROGMEM = {
    29, 15, 11, 18, 25, 28, 29, 15, 0xFF
};

const uint8_t EEL_string[4] PROGMEM = {
    15, 15, 22, 0xFF
};

const uint8_t SARDINE_string[8] PROGMEM = {
    29, 11, 28, 14, 19, 24, 15, 0xFF
};

const uint8_t CRAWFISH_string[9] PROGMEM = {
    13, 28, 11, 33, 16, 19, 29, 18, 0xFF
};

const uint8_t FANGTOOTH_string[10] PROGMEM = {
    16, 11, 24, 17, 30, 25, 25, 30, 18, 0xFF
};

const uint8_t LOBSTER_string[8] PROGMEM = {
    22, 25, 12, 29, 30, 15, 28, 0xFF
};

const uint8_t MARLIN_string[7] PROGMEM = {
    23, 11, 28, 22, 19, 24, 0xFF
};

const uint8_t SHARK_string[6] PROGMEM = {
    29, 18, 11, 28, 21, 0xFF
};

const uint8_t GRTWHITE_string[9] PROGMEM = {
    17, 28, 30, 33, 18, 19, 30, 15, 0xFF
};

const uint8_t OLD_BOOT_string[9] PROGMEM = {
    25, 22, 14, 0, 12, 25, 25, 30, 0xFF
};

const uint32_t PROGMEM fish_templates_16t[] = {

    // GOLDFISH
    80, // min X
    432, // max X
    reinterpret_cast<uint32_t>(GOLDFISH_string),
    reinterpret_cast<uint32_t>(GOLDFISH_tiles),
    3, // minLength;
    20, // maxLength

    // GUPPY
    80, // min X
    432, // max X
    reinterpret_cast<uint32_t>(GUPPY_string),
    reinterpret_cast<uint32_t>(GUPPY_tiles),
    1, // minLength;
    4, // maxLength

    // MINNOW
    560, // min X
    800, // max X
    reinterpret_cast<uint32_t>(MINNOW_string),
    reinterpret_cast<uint32_t>(MINNOW_tiles),
    2, // minLength;
    3, // maxLength

    // BETA
    352, // min X
    432, // max X
    reinterpret_cast<uint32_t>(BETA_string),
    reinterpret_cast<uint32_t>(BETA_tiles),
    2, // minLength;
    4, // maxLength

    // TROUT
    0, // min X
    288, // max X
    reinterpret_cast<uint32_t>(TROUT_string),
    reinterpret_cast<uint32_t>(TROUT_tiles),
    15, // minLength;
    25, // maxLength

    // BASS
    0, // min X
    288, // max X
    reinterpret_cast<uint32_t>(BASS_string),
    reinterpret_cast<uint32_t>(BASS_tiles),
    20, // minLength;
    40, // maxLength

    // CATFISH
    0, // min X
    64, // max X
    reinterpret_cast<uint32_t>(CATFISH_string),
    reinterpret_cast<uint32_t>(CATFISH_tiles),
    10, // minLength;
    40, // maxLength

    // FLOUNDER
    560, // min X
    800, // max X
    reinterpret_cast<uint32_t>(FLOUNDER_string),
    reinterpret_cast<uint32_t>(FLOUNDER_tiles),
    20, // minLength;
    40, // maxLength

    // CRAB
    560, // min X
    800, // max X
    reinterpret_cast<uint32_t>(CRAB_string),
    reinterpret_cast<uint32_t>(CRAB_tiles),
    5, // minLength;
    45, // maxLength

    // OCTOPUS
    608, // min X
    800, // max X
    reinterpret_cast<uint32_t>(OCTOPUS_string),
    reinterpret_cast<uint32_t>(OCTOPUS_tiles),
    100, // minLength;
    200, // maxLength

    // GROUPER
    608, // min X
    800, // max X
    reinterpret_cast<uint32_t>(GROUPER_string),
    reinterpret_cast<uint32_t>(GROUPER_tiles),
    50, // minLength;
    250, // maxLength

    // SEAHORSE
    560, // min X
    608, // max X
    reinterpret_cast<uint32_t>(SEAHORSE_string),
    reinterpret_cast<uint32_t>(SEAHORSE_tiles),
    1, // minLength;
    4, // maxLength

    // EEL
    560, // min X
    800, // max X
    reinterpret_cast<uint32_t>(EEL_string),
    reinterpret_cast<uint32_t>(EEL_tiles),
    20, // minLength;
    100, // maxLength

    // SARDINE
    560, // min X
    608, // max X
    reinterpret_cast<uint32_t>(SARDINE_string),
    reinterpret_cast<uint32_t>(SARDINE_tiles),
    1, // minLength;
    4, // maxLength

    // CRAWFISH
    0, // min X
    64, // max X
    reinterpret_cast<uint32_t>(CRAWFISH_string),
    reinterpret_cast<uint32_t>(CRAWFISH_tiles),
    4, // minLength;
    8, // maxLength

    // FANGTOOTH
    608, // min X
    800, // max X
    reinterpret_cast<uint32_t>(FANGTOOTH_string),
    reinterpret_cast<uint32_t>(FANGTOOTH_tiles),
    10, // minLength;
    30, // maxLength

    // LOBSTER
    560, // min X
    800, // max X
    reinterpret_cast<uint32_t>(LOBSTER_string),
    reinterpret_cast<uint32_t>(LOBSTER_tiles),
    10, // minLength;
    30, // maxLength

    // MARLIN
    608, // min X
    800, // max X
    reinterpret_cast<uint32_t>(MARLIN_string),
    reinterpret_cast<uint32_t>(MARLIN_tiles),
    80, // minLength;
    170, // maxLength

    // SHARK
    608, // min X
    800, // max X
    reinterpret_cast<uint32_t>(SHARK_string),
    reinterpret_cast<uint32_t>(SHARK_tiles),
    100, // minLength;
    200, // maxLength

    // GRTWHITE
    608, // min X
    800, // max X
    reinterpret_cast<uint32_t>(GRTWHITE_string),
    reinterpret_cast<uint32_t>(GRTWHITE_tiles),
    180, // minLength;
    250, // maxLength

    // OLD BOOT
    352, // min X
    432, // max X
    reinterpret_cast<uint32_t>(OLD_BOOT_string),
    reinterpret_cast<uint32_t>(OLD_BOOT_tiles),
    1, // minLength;
    10, // maxLength
};
