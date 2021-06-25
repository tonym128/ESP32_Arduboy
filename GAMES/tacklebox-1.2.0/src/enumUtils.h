#pragma once
#include "sfx.h"

template <class T>
T next(T current) {
    uint8_t c = static_cast<uint8_t>(current);
    c = (c + 1) % static_cast<uint8_t>(T::COUNT);

    Sfx::menuTick();

    return static_cast<T>(c);
}

template <class T>
T prev(T current) {
    uint8_t c = static_cast<uint8_t>(current);

    Sfx::menuTick();

    if (c == 0) {
        return static_cast<T>(static_cast<uint8_t>(T::COUNT) - 1);
    } else {
        return static_cast<T>(c - 1);
    }
}
