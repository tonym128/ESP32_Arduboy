#include "Clapper.h"

#include "../utils/Enums.h"

Clapper::Clapper() { }

int16_t Clapper::getXPosition() {

    return this->x;

}

int8_t Clapper::getYPosition(uint8_t yOffset) {

    return this->y - yOffset;

}

uint8_t Clapper::getImage() {

    return (this->x % 8 < 4 ? 1 : 0);

}

bool Clapper::isEnabled() {

    return this->enabled;

}

void Clapper::setEnabled(bool enabled) {

    this->enabled = enabled;
    this->x = 128;

}

void Clapper::setY(uint8_t y) {

    this->y = y;

}

void Clapper::updatePosition() {

    if (x > -9) x--;
    if (x == -9) enabled = false;

}

Rect Clapper::getRect(uint8_t yOffset, GameMode mode) {

    if (mode == GameMode::Easy) {
        return Rect{this->x + 2, this->y - yOffset + 2, 5, 3 };
    }
    else {
        return Rect{this->x + 1, this->y - yOffset + 1, 7, 5 };
    }

}
