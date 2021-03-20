#include "Coin.h"
#include "../utils/Enums.h"


Coin::Coin() {}

int16_t Coin::getX() {

  return this->x;

}

uint8_t Coin::getLane() {

  return this->lane;

}

uint8_t Coin::getCountdown() {

  return this->countdown;

}

bool Coin::getEnabled() {

  return this->enabled;

}

void Coin::setX(int16_t value) {

  this->x = value;

}

void Coin::setLane(uint8_t value) {

  this->lane = value;

}

void Coin::setEnabled(bool value) {

  this->enabled = value;

}

void Coin::decCountdown() {

  this->countdown--;

}

void Coin::move() {

  this->x--;

  if (x == -8) {

    this->enabled = false;
    this->countdown = random(10, 100);

  }

}
