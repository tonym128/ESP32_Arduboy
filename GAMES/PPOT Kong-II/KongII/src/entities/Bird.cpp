#include "Bird.h"

#include "../utils/Enums.h"

Bird::Bird() { }

uint16_t Bird::getPosition() {

    return this->position;

}

int8_t Bird::getXPosition() {

    return pgm_read_byte(&Coordinates::Bird[(this->position * 3)]);

}

int8_t Bird::getYPosition(uint8_t yOffset) {

    uint8_t y = pgm_read_byte(&Coordinates::Bird[(this->position * 3) + 1]);
    return y - yOffset;

}

uint8_t Bird::getImage() {

    return pgm_read_byte(&Coordinates::Bird[(this->position * 3) + 2]);

}

bool Bird::isEnabled() {

    return this->enabled;

}

void Bird::setEnabled(bool enabled) {

    this->enabled = enabled;
    this->position = 0;

}

void Bird::setPosition(uint16_t position) {

    this->position = position;

}

void Bird::updatePosition() {

    if (!this->enabled) return;

    this->position++;

    uint8_t x = pgm_read_byte(&Coordinates::Bird[(this->position * 3)]);
    uint8_t y = pgm_read_byte(&Coordinates::Bird[(this->position * 3) + 1]);

    if (x == 0 && y == 0) {

    this->enabled = false;
    this->position = 0;

    }

}

Rect Bird::getRect(uint8_t yOffset, GameMode mode) {

    int16_t x = pgm_read_byte(&Coordinates::Bird[(this->position * 3)]);
    int16_t y = pgm_read_byte(&Coordinates::Bird[(this->position * 3) + 1]) - yOffset;

    switch (static_cast<BirdImage>(this->getImage())) {

        case BirdImage::Bird_Upright:
            if (mode == GameMode::Easy) {
                return Rect{x + 2, y + 2, 4, 7 };
            }
            else {
                return Rect{x + 1, y + 1, 6, 9 };
            }

        case BirdImage::Bird_00:
        case BirdImage::Bird_02:
            if (mode == GameMode::Easy) {
                return Rect{x + 2, y + 2, 8, 3 };
            }
            else {
                return Rect{x + 1, y + 1, 10, 5 };
            }

        case BirdImage::Bird_01:
        case BirdImage::Bird_03:
            if (mode == GameMode::Easy) {
                return Rect{x + 1, y + 3, 10, 4 };
            }
            else {
                return Rect{x + 2, y + 4, 8, 2 };
            }

        case BirdImage::Bird_Turn_00:
        case BirdImage::Bird_Turn_05:
            if (mode == GameMode::Easy) {
                return Rect{x + 2, y + 2, 7, 5 };
            }
            else {
                return Rect{x + 1, y + 1, 9, 7 };
            }

        case BirdImage::Bird_Turn_01:
            if (mode == GameMode::Easy) {
                return Rect{x + 5, y + 2, 3, 8 };
            }
            else {
                return Rect{x + 4, y + 1, 5, 10 };
            }

        case BirdImage::Bird_Turn_02:
        case BirdImage::Bird_Turn_03:
            if (mode == GameMode::Easy) {
                return Rect{x + 2, y + 2, 7, 5 };
            }
            else {
                return Rect{x + 1, y + 1, 9, 7 };
            }

        case BirdImage::Bird_Turn_04:
            if (mode == GameMode::Easy) {
                return Rect{x + 5, y + 2, 3, 8 };
            }
            else {
                return Rect{x + 3, y + 1, 5, 10 };
            }

        default: return Rect{ 0, 0, 0, 0 };

    }

}