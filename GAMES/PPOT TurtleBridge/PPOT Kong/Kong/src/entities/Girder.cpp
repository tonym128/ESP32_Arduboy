#include "Girder.h"

#include "../utils/Enums.h"
#include "../map/Coordinates.h"

Girder::Girder() { }

uint8_t Girder::getPosition() {

  return this->position;

}

int8_t Girder::getXPosition() {

  return this->girderData.x;

}

int8_t Girder::getYPosition(uint8_t yOffset) {

  return this->girderData.y - yOffset;

}

uint8_t Girder::getImage() {

  return static_cast<uint8_t>(this->girderData.rotation);

}

bool Girder::isEnabled() {

  return this->enabled;

}

void Girder::setPosition(uint8_t position) {

  this->position = position;
  Coordinates::readGirderData(girderData, this->position);

}

void Girder::setEnabled(bool enabled) {

  this->enabled = enabled;

}

void Girder::updatePosition() {

  if (!this->enabled) return;

  this->position++;
  Coordinates::readGirderData(girderData, this->position);
 
  if (this->girderData.x == 0 && this->girderData.y == 0) {

    this->enabled = false;
    this->position = 0;

  }

}

Rect Girder::getRect(uint8_t yOffset) {

  return Rect{this->girderData.x + 1, this->girderData.y - yOffset + 4, 20, 4 };

}
