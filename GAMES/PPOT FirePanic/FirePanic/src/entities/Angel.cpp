#include "Angel.h"
#include "../utils/Physics.h"

Angel::Angel() { }

uint8_t Angel::getX() {

  return this->x;

}

uint8_t Angel::getY() {

  return this->y;

}

uint8_t Angel::getEnabled() {

  return this->enabled;

}

void Angel::setEnabled(bool value) {

  this->enabled = value;
  this->posIndex = 0;

}

void Angel::setSequence(uint8_t sequence) {

  this->sequence = sequence;

}

uint8_t Angel::getImageIndex() { // Seq 0 1 2 3 2 1 .. 0 1 2 3 2 1 ..

  uint8_t imageIndex = (this->posIndex / 2) % 6;

  if (imageIndex == 4) imageIndex = 2;
  if (imageIndex == 5) imageIndex = 1;

  return imageIndex;

}

bool Angel::renderImage() {
  return this->posIndex >= 0;
}

void Angel::init(uint8_t sequence, uint8_t missNumber) {

  this->sequence = sequence;
  this->enabled = true;
  this->posIndex = -6;  

  if (missNumber > 2) missNumber = 2; // on the third miss, use the second pattern ..

  switch (this->sequence) {

    case 0:
      this->sequenceLen = pgm_read_byte(&Angel_Arc_1[missNumber - 1]);
      break;

    case 1:
      this->sequenceLen = pgm_read_byte(&Angel_Arc_2[missNumber - 1]);
      break;

    case 2:
      this->sequenceLen = pgm_read_byte(&Angel_Arc_3[missNumber - 1]);
      break;

  }
  
}

bool Angel::move() {
  
  this->posIndex++;  

  if (this->posIndex >= 0 && this->posIndex < this->sequenceLen) {

    switch (this->sequence) {

      case 0:
        this->x = pgm_read_byte(&Angel_Arc_1[(this->posIndex * 2) + 2]);
        this->y = pgm_read_byte(&Angel_Arc_1[(this->posIndex * 2) + 3]);
        break;

      case 1:
        this->x = pgm_read_byte(&Angel_Arc_2[(this->posIndex * 2) + 2]);
        this->y = pgm_read_byte(&Angel_Arc_2[(this->posIndex * 2) + 3]);
        break;

      case 2:
        this->x = pgm_read_byte(&Angel_Arc_3[(this->posIndex * 2) + 2]);
        this->y = pgm_read_byte(&Angel_Arc_3[(this->posIndex * 2) + 3]);
        break;

    }

  }

  if (this->posIndex == this->sequenceLen) {
    this->enabled = false;
  }

  return (this->sequenceLen - this->posIndex < 8) ;

}
