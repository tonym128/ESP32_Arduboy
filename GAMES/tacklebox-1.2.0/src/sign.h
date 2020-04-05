#pragma once

#include <Arduino.h>

class Sign {
    public:
        int16_t x;
        int16_t y;
        const uint8_t* message;

        void render(void);

        Sign(int16_t x, int16_t y, const uint8_t* message):
            x(x),
            y(y),
            message(message)
        {}
};

