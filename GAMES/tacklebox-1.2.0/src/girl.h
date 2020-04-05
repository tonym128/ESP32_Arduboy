#pragma once

#include <Arduino.h>

struct Girl {
    static int16_t x;
    static int16_t y;

    static bool isActive;
    static uint8_t startingActiveHour;
    static uint8_t endingActiveHour;

    static void update(bool isActive, bool justBecameActive);
    static void render(uint8_t frame);
};

