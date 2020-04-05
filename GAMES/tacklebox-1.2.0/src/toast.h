#pragma once

#include <Arduino.h>

struct Toast {
    static const uint8_t* msg;
    static uint8_t toastCount;

    static void toast(const uint8_t* msg, uint8_t count = 120);
    static void render();
};

