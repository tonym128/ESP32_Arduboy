#include "OtherCar.h"
#include "../utils/Physics.h"

OtherCar::OtherCar() { }

int16_t OtherCar::getX() {

  return this->x;

}

void OtherCar::setX(int16_t value) {

  this->x = value;
  if (this->x <= -RACE_OTHERCAR_WIDTH) this->enabled = false;

}

uint8_t OtherCar::getLane() {

  return this->lane;

}

void OtherCar::setLane(uint8_t value) {

  this->lane = value;

}

uint8_t OtherCar::getSpeed() {

  return this->speed;

}

void OtherCar::setSpeed(uint8_t value) {

  this->speed = value;

}

uint8_t OtherCar::getType() {

  return this->type;

}

void OtherCar::setType(uint8_t value) {

  this->type = value;

}

bool OtherCar::getEnabled() {

  return this->enabled;

}

void OtherCar::setEnabled(bool value) {

  this->enabled = value;

}

bool OtherCar::getDoNotMove() {

  return this->doNotMove;

}

void OtherCar::setDoNotMove(bool value) {

  this->doNotMove = value;

}
