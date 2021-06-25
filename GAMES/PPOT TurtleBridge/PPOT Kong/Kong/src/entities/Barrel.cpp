#include "Barrel.h"

#include "../utils/Utils.h"
#include "../utils/Enums.h"
#include "../map/Coordinates.h"

Barrel::Barrel() { 
}

uint8_t Barrel::getPosition() {

  return this->position;

}

uint8_t Barrel::getAisle() {

  return this->aisle;

}

uint8_t Barrel::getRotation() {

  return this->rotation;

}

uint8_t Barrel::getEnabledCountdown() {

  return this->enabledCountdown;

}

bool Barrel::isEnabled() {

  return this->enabled;

}

bool Barrel::isEnabledOrPending() {

  return this->enabled || (this->enabledCountdown > 0);

}

void Barrel::setPosition(uint8_t position) {

  this->position = position;
  Coordinates::readBarrelData(barrelData, this->position);

}

void Barrel::setAisle(uint8_t aisle) {

  this->aisle = aisle;

}

void Barrel::setRotation(uint8_t rotation) {

  this->rotation = rotation;

}

void Barrel::setEnabledCountdown(uint8_t enabledCountdown) {

  this->enabledCountdown = enabledCountdown;

}

void Barrel::setEnabled(bool enabled) {

  this->enabled = enabled;
  if (!enabled) this->enabledCountdown = 0;

}


// Methods ..

int8_t Barrel::getXPosition() {

  return barrelData.x;

}

int8_t Barrel::getYPosition(uint8_t yOffset) {

  return barrelData.y - yOffset;

}

void Barrel::updatePosition() {

  // Retrieve sjip value from current position (if it exists) ..

  this->position = this->position + 1 + barrelData.skip;
  Coordinates::readBarrelData(barrelData, this->position);

  if (barrelData.x == 0 && barrelData.y == 0) {

    this->position = 0;
    this->enabled = false;

  }

}

void Barrel::rotate() {

  switch (barrelData.rotation) {

    case Rotation::Right:
      this->rotation = wrapInc(this->rotation, static_cast<uint8_t>(0), static_cast<uint8_t>(2));
      break;

    case Rotation::Left:
      this->rotation = wrapDec(this->rotation, static_cast<uint8_t>(0), static_cast<uint8_t>(2));
      break;

    case Rotation::None:
      break;
      
  }

}

void Barrel::launch(uint8_t startingPosition) {

  const uint8_t startingPositions[3] = { BARREL_POSITION_1_START, BARREL_POSITION_2_START, BARREL_POSITION_3_START };

  this->setPosition(startingPositions[startingPosition]);
  this->aisle = startingPosition;
  this->enabled = true;

}

void Barrel::decEnabledCountdown() {

  if (this->enabledCountdown > 0) {
    
    this->enabledCountdown--;

  }

}

Rect Barrel::getRect(uint8_t yOffset, GameMode mode) {

  switch (mode) {
    
    case GameMode::Easy:
      return Rect{barrelData.x + 2, barrelData.y - yOffset + 2, 5, 5 };

    case GameMode::Hard:
      return Rect{barrelData.x + 1, barrelData.y - yOffset + 1, 7, 7 };

  }

}
