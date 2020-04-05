#pragma once

#include <Arduino.h>

class Worm {
    public:
        bool isSpawned;
        int16_t x;
        int16_t y;
        uint16_t duration;
        bool justWentInGround;

        Worm(int16_t x, int16_t y):
            isSpawned(false),
            x(x),
            y(y),
            duration(0),
            justWentInGround(false)
        {}

    void update(uint8_t frame, bool isActive, bool justBecameActive);
    void render(uint8_t frame);
    void onGrabbedByPlayer();
};

