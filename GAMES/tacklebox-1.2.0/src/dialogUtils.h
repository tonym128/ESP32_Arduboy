#pragma once

#include <Arduino.h>
#include "baitType.h"

struct DialogUtils {
    static void reset();
    static void renderFrame(const uint8_t* bmp);
    static void renderMoneyInCorner();
};

