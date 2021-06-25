#include "Fish.h"

#include "../utils/Enums.h"

const uint8_t positionX[5][5] = { 
  { 3, 6, 9, 11, 12 },
  { 26, 28, 30, 32, 33 },
  { 51, 52, 53, 54, 54 },
  { 91, 89, 87, 86, 85 },
  { 115, 112, 109, 107, 105 },
};

const uint8_t positionY[5] = { 
  59, 58, 57, 56, 55
};


Fish::Fish() { 
}

bool Fish::getEnabled() {

  return this->enabled;

}

uint8_t Fish::getFishIndex() {

  return this->fishIndex;

}

uint8_t Fish::getImageIndex() {

  return this->imageIndex;

}

uint8_t Fish::getDelay() {

  return this->delayValue;

}

bool Fish::isEdible() {

  return this->edible;

}

void Fish::setEnabled(bool enabled) {

  this->enabled = enabled;

  if (enabled) {
    this->counter = 120;
    this->imageIndex = 0;
    this->positionIndex = 0;
    this->edible = false;
  }

}

void Fish::setFishIndex(uint8_t value) {

  this->fishIndex = value;

}

void Fish::setImageIndex(uint8_t value) {

  this->imageIndex = value;

}

void Fish::setDelay(uint8_t value) {

  this->delayValue = value;
  this->delayValueInit = value;

}


// Methods ..

uint8_t Fish::getDisplayX() {

  return positionX[this->fishIndex][this->positionIndex];

}

uint8_t Fish::getDisplayY() {

  return positionY[this->positionIndex];

}

void Fish::updateImageIndex() {

  this->imageIndex = (this->imageIndex == 1 ? 0 : 1);

}

void Fish::updatePositionIndex() {

  switch (this->delayValue) {

    case 0:
      this->delayValue = this->delayValueInit;
      this->positionIndex++;

      if (this->positionIndex == 5) {

        this->enabled = false;
        this->edible = false;

      }
      break;

    case 10:
      if (this->positionIndex == 4) {
        this->edible = true;
      }
      this->delayValue--;
      break;

    default:  
      this->edible = false;
      this->delayValue--;
      break;

  }

}