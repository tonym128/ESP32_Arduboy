#pragma once

#include <Arduino.h>

const int16_t BEACH_DOCK_X = 16 * 35;
const int16_t ISLAND_DOCK_X = 16 * 39 - 8;

enum class Dock {
    Beach,
    Island,
    COUNT
};

struct Boat {
    static int16_t x;
    static int16_t y;
    static int8_t frame;
    static bool arrived;
    static bool traveling;
    static Dock currentDock;

    static void onEnter();
    static void update(uint8_t frame);
    static void render(uint8_t frame);
};

