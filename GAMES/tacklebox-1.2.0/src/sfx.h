#ifndef sfx_h
#define sfx_h

#include <Arduino.h>
#include <Arduboy2.h>

struct Sfx {
    static void buzz();
    static void purchase();
    static void menuTick();
    static void gotFish();
};

#endif
