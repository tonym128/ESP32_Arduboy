#include "Ambulance.h"
#include "../utils/Physics.h"

Ambulance::Ambulance() { }

int16_t Ambulance::getX() {

  return this->x;

}

uint8_t Ambulance::getY() {

  return this->y;

}

uint8_t Ambulance::getPuffIndex(Direction direction) {

  return this->puffIndex[static_cast<uint8_t>(direction)];

}

uint8_t Ambulance::getPuffIndexes() {

  return this->puffIndex[static_cast<uint8_t>(Direction::Down)] + 
         this->puffIndex[static_cast<uint8_t>(Direction::Up)] +
         this->puffIndex[static_cast<uint8_t>(Direction::Right)] +
         this->puffIndex[static_cast<uint8_t>(Direction::Left)];

}

Direction Ambulance::getDirection() {

  return this->direction;

}

void Ambulance::setX(int16_t value) {

  this->x = value;

}

void Ambulance::setY(uint8_t value) {

  this->y = value;

}

void Ambulance::setPuffIndex(Direction direction, uint8_t value) {

  this->puffIndex[static_cast<uint8_t>(direction)] = value;

}

void Ambulance::setDirection(Direction value) {

  this->direction = value;

}

void Ambulance::incX(uint8_t value) {

  this->x = this->x + value;

}

void Ambulance::decX(uint8_t value) {

  this->x = this->x - value;

}

void Ambulance::incY() {

  this->y++;
  this->direction = Direction::Down;

}

void Ambulance::decY() {

  this->y--;
  this->direction = Direction::Up;

}

void Ambulance::incPuffIndex(Direction direction) {

  this->puffIndex[static_cast<uint8_t>(direction)]++;
  if (this->puffIndex[static_cast<uint8_t>(direction)] == 6) this->puffIndex[static_cast<uint8_t>(direction)] = 0;

}

void Ambulance::incPuffIndexes() {

  for (uint8_t direction = 0; direction < 4; direction++) {

    incPuffIndex(static_cast<Direction>(direction));
    // if (this->puffIndex[static_cast<uint8_t>(direction)] > 0) {
    //   this->puffIndex[static_cast<uint8_t>(direction)]++;
    //   if (this->puffIndex[static_cast<uint8_t>(direction)] == 6) this->puffIndex[static_cast<uint8_t>(direction)] = 0;
    // }

  }

}

bool Ambulance::incPuffIndexIfZero(Direction direction) {

  if (this->puffIndex[static_cast<uint8_t>(direction)] == 0) {
    this->puffIndex[static_cast<uint8_t>(direction)]++;
    return true;
  }
  else {
    return false;
  }

}

uint8_t Ambulance::getLane() {

  switch (this->y) {

    case  0 ... 14: return 0;
    case 15 ... 27: return 1;
    default:        return 2;

  }

}