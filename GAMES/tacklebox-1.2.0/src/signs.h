#pragma once
#include "sign.h"

const int8_t MAX_SIGNS = 5;

Sign signs[MAX_SIGNS] = {
    // lucy's sign
    Sign(6 * 16, 18, lucyWarning_string),
    // sign at start of game with URL to manual
    Sign(16 * 26, 16 * 16, signGameManual_string),
    // on island, hinting sharks can be found
    Sign(16 * 41, 12 * 16 - 4, signSharkWarning_string),
    // second "funny" octopus warning
    Sign(16 * 41, 10 * 16 - 4, signOctoWarning_string),
    // on the beach, letting player know worms wont work in the ocean
    Sign(16 * 33 - 10, 16 * 4, signOceanFish_string)
};
