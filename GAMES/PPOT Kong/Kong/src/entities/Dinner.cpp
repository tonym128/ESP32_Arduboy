#include "Dinner.h"

#include "../utils/Utils.h"
#include "../utils/Enums.h"
#include "../map/Coordinates.h"

Dinner::Dinner() : Base() { }


bool Dinner::isVisible() {

  return this->visible;

}

uint8_t Dinner::getFood() {

  return this->food;

}

void Dinner::setVisible(bool visible) {

  this->visible = visible;

}

void Dinner::setFood(uint8_t food) {

  this->food = food;

}

void Dinner::incFood() {

  this->food = wrapInc(this->food, static_cast<uint8_t>(0), static_cast<uint8_t>(2));

}

void Dinner::update() {

  this->counter = wrapInc(this->counter, static_cast<uint8_t>(0), static_cast<uint8_t>(2));

}