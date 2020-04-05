#include "sfx.h"
#include <Arduboy2.h>

void Sfx::buzz() {
    BeepPin1::tone(BeepPin1::freq(100), 14);
}

void Sfx::purchase() {
    BeepPin1::tone(BeepPin1::freq(2000), 2);
}

void Sfx::menuTick() {
    BeepPin1::tone(BeepPin1::freq(50), 1);
}

void Sfx::gotFish() {
    BeepPin1::tone(BeepPin1::freq(1000), 4);
}

