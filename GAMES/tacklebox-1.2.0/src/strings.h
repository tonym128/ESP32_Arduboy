#ifndef strings_h
#define strings_h

const uint8_t NEW_LINE = 42;

const uint8_t PROGMEM font_tiles[] =  {
    // width, height
    4, 8,
    // ' ' and '0'
    0xf,0x6f,0x4f,0xf,
    // '1' and '2'
    0x2f,0x26,0x40,0x47,
    // '3' and '4'
    0x86,0xb4,0xb4,0x0,
    // '5' and '6'
    0x8,0x52,0x52,0x12,
    // '7' and '8'
    0xe,0x2e,0x2e,0x0,
    // '9' and 'A'
    0x8,0xaa,0xaa,0x0,
    // 'B' and 'C'
    0x0,0x64,0x64,0x61,
    // 'D' and 'E'
    0x0,0x46,0x46,0x69,
    // 'F' and 'G'
    0x0,0x6a,0x6a,0x2e,
    // 'H' and 'I'
    0x60,0xb,0x6b,0xf0,
    // 'J' and 'K'
    0x3,0xd6,0xa0,0x6e,
    // 'L' and 'M'
    0x0,0xe7,0xc7,0x7,
    // 'N' and 'O'
    0x0,0x6d,0x6b,0x0,
    // 'P' and 'Q'
    0x0,0x6a,0x2a,0x8,
    // 'R' and 'S'
    0x40,0x4a,0x22,0x28,
    // 'T' and 'U'
    0xe,0x70,0x7e,0xe,
    // 'V' and 'W'
    0x8,0x77,0x37,0x8,
    // 'X' and 'Y'
    0x86,0x39,0xb9,0x86,
    // 'Z' and '*'
    0xb6,0x72,0xb4,0xd6,
    // '.' and '/'
    0x7f,0xb7,0xdf,0xef,
    // ',' and '!'
    0xff,0x47,0xfb,0xff,
};

const uint8_t PROGMEM font_tiles_upper_mask[] = {
    0x0f, 0x0f, 0x0f, 0x0f
};

const uint8_t PROGMEM font_tiles_lower_mask[] = {
    0xf0, 0xf0, 0xf0, 0xf0
};


// "SAVE"
const uint8_t save_string[5] PROGMEM = {
    29, 11, 32, 15, 0xFF
};


// "SAVED OK"
const uint8_t gameSaved_string[9] PROGMEM = {
    29, 11, 32, 15, 14, 0, 25, 21, 0xFF
};


// "MY FISH"
const uint8_t collection_string[8] PROGMEM = {
    23, 35, 0, 16, 19, 29, 18, 0xFF
};


// "CURRENT HAUL"
const uint8_t quantity_string[13] PROGMEM = {
    13, 31, 28, 28, 15, 24, 30, 0, 18, 11, 31, 22, 0xFF
};


// "BEST LENGTH CM"
const uint8_t length_string[15] PROGMEM = {
    12, 15, 29, 30, 0, 22, 15, 24, 17, 30, 18, 0, 13, 23, 0xFF
};


// "CM"
const uint8_t cm_string[3] PROGMEM = {
    13, 23, 0xFF
};


// "*"
const uint8_t checkmark_string[2] PROGMEM = {
    37, 0xFF
};


// "DELETE"
const uint8_t delete_string[7] PROGMEM = {
    14, 15, 22, 15, 30, 15, 0xFF
};


// "REALLY"
const uint8_t really_string[7] PROGMEM = {
    28, 15, 11, 22, 22, 35, 0xFF
};


// "YES"
const uint8_t yes_string[4] PROGMEM = {
    35, 15, 29, 0xFF
};


// "NO"
const uint8_t no_string[3] PROGMEM = {
    24, 25, 0xFF
};


// "OK"
const uint8_t ok_string[3] PROGMEM = {
    25, 21, 0xFF
};


// "SFX *"
const uint8_t sfxOn_string[6] PROGMEM = {
    29, 16, 34, 0, 37, 0xFF
};


// "SFX"
const uint8_t sfxOff_string[4] PROGMEM = {
    29, 16, 34, 0xFF
};


// "OPEN 4AM\nTO MIDNIGHT"
const uint8_t openFromTo_string[21] PROGMEM = {
    25, 26, 15, 24, 0, 5, 11, 23, 42, 30, 25, 0, 23, 19, 14, 24, 19, 17, 18, 30, 0xFF
};


// "BUY"
const uint8_t buy_string[4] PROGMEM = {
    12, 31, 35, 0xFF
};


// "SELL"
const uint8_t sell_string[5] PROGMEM = {
    29, 15, 22, 22, 0xFF
};


// "ADVICE"
const uint8_t advice_string[7] PROGMEM = {
    11, 14, 32, 19, 13, 15, 0xFF
};


// "HOURS"
const uint8_t hours_string[6] PROGMEM = {
    18, 25, 31, 28, 29, 0xFF
};


// "SELL ALL FISH FOR"
const uint8_t sellFor_string[18] PROGMEM = {
    29, 15, 22, 22, 0, 11, 22, 22, 0, 16, 19, 29, 18, 0, 16, 25, 28, 0xFF
};


// "USEFUL ADVICE FOR"
const uint8_t adviceFor_string[18] PROGMEM = {
    31, 29, 15, 16, 31, 22, 0, 11, 14, 32, 19, 13, 15, 0, 16, 25, 28, 0xFF
};


// "YOU HAVE NO FISH\nTO SELL"
const uint8_t youHaveNoFish_string[25] PROGMEM = {
    35, 25, 31, 0, 18, 11, 32, 15, 0, 24, 25, 0, 16, 19, 29, 18, 42, 30, 25, 0, 29, 15, 22, 22, 0xFF
};


// "I AM NOT BUYING\nTHOSE BOOTS"
const uint8_t notBuyingBoots_string[28] PROGMEM = {
    19, 0, 11, 23, 0, 24, 25, 30, 0, 12, 31, 35, 19, 24, 17, 42, 30, 18, 25, 29, 15, 0, 12, 25, 25, 30, 29, 0xFF
};


// "0"
const uint8_t digit0_string[2] PROGMEM = {
    1, 0xFF
};


// "1"
const uint8_t digit1_string[2] PROGMEM = {
    2, 0xFF
};


// "2"
const uint8_t digit2_string[2] PROGMEM = {
    3, 0xFF
};


// "3"
const uint8_t digit3_string[2] PROGMEM = {
    4, 0xFF
};


// "4"
const uint8_t digit4_string[2] PROGMEM = {
    5, 0xFF
};


// "5"
const uint8_t digit5_string[2] PROGMEM = {
    6, 0xFF
};


// "6"
const uint8_t digit6_string[2] PROGMEM = {
    7, 0xFF
};


// "7"
const uint8_t digit7_string[2] PROGMEM = {
    8, 0xFF
};


// "8"
const uint8_t digit8_string[2] PROGMEM = {
    9, 0xFF
};


// "9"
const uint8_t digit9_string[2] PROGMEM = {
    10, 0xFF
};


// "GRUB"
const uint8_t grub_string[5] PROGMEM = {
    17, 28, 31, 12, 0xFF
};


// "SHRIMP"
const uint8_t shrimp_string[7] PROGMEM = {
    29, 18, 28, 19, 23, 26, 0xFF
};


// "PRO POLE"
const uint8_t proPole_string[9] PROGMEM = {
    26, 28, 25, 0, 26, 25, 22, 15, 0xFF
};


// "OARS"
const uint8_t boatOars_string[5] PROGMEM = {
    25, 11, 28, 29, 0xFF
};


// "SOME FISH ARE\nONLY IN\nDEEP WATER"
const uint8_t advice1_string[33] PROGMEM = {
    29, 25, 23, 15, 0, 16, 19, 29, 18, 0, 11, 28, 15, 42, 25, 24, 22, 35, 0, 19, 24, 42, 14, 15, 15, 26, 0, 33, 11, 30, 15, 28, 0xFF
};


// "JAKE FISHES\nTHE OCEAN AT NIGHT"
const uint8_t advice2_string[31] PROGMEM = {
    20, 11, 21, 15, 0, 16, 19, 29, 18, 15, 29, 42, 30, 18, 15, 0, 25, 13, 15, 11, 24, 0, 11, 30, 0, 24, 19, 17, 18, 30, 0xFF
};


// "VISIT\nCITY41.GAMES/FG"
const uint8_t advice3_string[22] PROGMEM = {
    32, 19, 29, 19, 30, 42, 13, 19, 30, 35, 5, 2, 38, 17, 11, 23, 15, 29, 39, 16, 17, 0xFF
};


// "YOU KNOW ALL"
const uint8_t noMoreAdvice_string[13] PROGMEM = {
    35, 25, 31, 0, 21, 24, 25, 33, 0, 11, 22, 22, 0xFF
};


// "HELP AT CITY41.GAMES/TBOX"
const uint8_t signGameManual_string[26] PROGMEM = {
    18, 15, 22, 26, 0, 11, 30, 0, 13, 19, 30, 35, 5, 2, 38, 17, 11, 23, 15, 29, 39, 30, 12, 25, 34, 0xFF
};


// "WARNING SHARKS!"
const uint8_t signSharkWarning_string[16] PROGMEM = {
    33, 11, 28, 24, 19, 24, 17, 0, 29, 18, 11, 28, 21, 29, 41, 0xFF
};


// "OCTOPUS TOO"
const uint8_t signOctoWarning_string[12] PROGMEM = {
    25, 13, 30, 25, 26, 31, 29, 0, 30, 25, 25, 0xFF
};


// "OCEAN FISH HATE WORMS"
const uint8_t signOceanFish_string[22] PROGMEM = {
    25, 13, 15, 11, 24, 0, 16, 19, 29, 18, 0, 18, 11, 30, 15, 0, 33, 25, 28, 23, 29, 0xFF
};


// "BRING ME THREE\nLOBSTERS AND I WILL\nSELL YOU NICE BAIT"
const uint8_t girlQuest_string[54] PROGMEM = {
    12, 28, 19, 24, 17, 0, 23, 15, 0, 30, 18, 28, 15, 15, 42, 22, 25, 12, 29, 30, 15, 28, 29, 0, 11, 24, 14, 0, 19, 0, 33, 19, 22, 22, 42, 29, 15, 22, 22, 0, 35, 25, 31, 0, 24, 19, 13, 15, 0, 12, 11, 19, 30, 0xFF
};


// "BUY PRIMO BAIT FOR"
const uint8_t girlSellMeat_string[19] PROGMEM = {
    12, 31, 35, 0, 26, 28, 19, 23, 25, 0, 12, 11, 19, 30, 0, 16, 25, 28, 0xFF
};


// "BRING ME FOUR BOOTS\nFOR A NEW WAY\nTO FISH"
const uint8_t guyQuest_string[42] PROGMEM = {
    12, 28, 19, 24, 17, 0, 23, 15, 0, 16, 25, 31, 28, 0, 12, 25, 25, 30, 29, 42, 16, 25, 28, 0, 11, 0, 24, 15, 33, 0, 33, 11, 35, 42, 30, 25, 0, 16, 19, 29, 18, 0xFF
};


// "SET FISHING MODE\nPRO"
const uint8_t guyTurnOnProMode_string[21] PROGMEM = {
    29, 15, 30, 0, 16, 19, 29, 18, 19, 24, 17, 0, 23, 25, 14, 15, 42, 26, 28, 25, 0xFF
};


// "SET FISHING MODE\nCASUAL"
const uint8_t guyTurnOffProMode_string[24] PROGMEM = {
    29, 15, 30, 0, 16, 19, 29, 18, 19, 24, 17, 0, 23, 25, 14, 15, 42, 13, 11, 29, 31, 11, 22, 0xFF
};


// "THIS IS MY SPOT, BACK OFF!"
const uint8_t lucyWarning_string[27] PROGMEM = {
    30, 18, 19, 29, 0, 19, 29, 0, 23, 35, 0, 29, 26, 25, 30, 40, 0, 12, 11, 13, 21, 0, 25, 16, 16, 41, 0xFF
};


// "YOU HAVE NO BAIT!"
const uint8_t noBait_string[18] PROGMEM = {
    35, 25, 31, 0, 18, 11, 32, 15, 0, 24, 25, 0, 12, 11, 19, 30, 41, 0xFF
};

#endif
