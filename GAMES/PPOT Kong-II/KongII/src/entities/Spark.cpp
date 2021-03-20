#include "Spark.h"

#include "../utils/Enums.h"

Spark::Spark() { }

uint8_t Spark::getPosition() {

    return this->position;

}

int8_t Spark::getXPosition() {

    return pgm_read_byte(&this->path[(this->position * 3)]);

}

int8_t Spark::getYPosition(uint8_t yOffset) {

    uint8_t y = pgm_read_byte(&this->path[(this->position * 3) + 1]);
    return y - yOffset;

}

uint8_t Spark::getImage() {

    return pgm_read_byte(&this->path[(this->position * 3) + 2]);

}

bool Spark::isEnabled() {

    return this->enabled;

}

void Spark::setEnabled(bool enabled) {

    this->enabled = enabled;
    this->position = 0;

}

void Spark::setPath(uint8_t const *path) {

    this->path = path;

}

void Spark::setPosition(uint8_t position) {

    this->position = position;

}

void Spark::updatePosition() {

    if (!this->enabled) return;

    this->position++;

    uint8_t x = pgm_read_byte(&this->path[(this->position * 3)]);
    uint8_t y = pgm_read_byte(&this->path[(this->position * 3) + 1]);

    if (x == 0 && y == 0) {

        this->enabled = false;
        this->position = 0;

    }
  
}

Rect Spark::getRect(uint8_t yOffset, GameMode mode) {


  int8_t x = pgm_read_byte(&this->path[(this->position * 3)]);
  uint8_t y = pgm_read_byte(&this->path[(this->position * 3) + 1]) - yOffset;

    if (mode == GameMode::Easy) {
        return Rect{x + 3, y + 2, 2, 3 };
    }
    else {
        return Rect{x + 2, y + 1, 4, 5 };
    }

}
