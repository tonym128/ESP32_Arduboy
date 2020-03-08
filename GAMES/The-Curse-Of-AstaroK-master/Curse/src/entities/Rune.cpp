#include "Rune.h"

#include "../utils/Arduboy2Ext.h"
#include "../images/Images.h"
#include "../utils/Constants.h"

Rune::Rune() {}

uint8_t const spinDelay[] = { 2, 2,    
                              3, 3,  
                              4, 5, 
                              6, 7,   
                              9, 255 };

bool Rune::getReroll() {
  return this->reroll;
}

void Rune::setReroll(const bool value) {
  this->reroll = value;
  if (!value) {
    this->frame = 0;
  }
  else {
    this->index = 9;
  }
}

void Rune::alternateReroll() {
  this->reroll = !this->reroll;
  if (!this->reroll) this->frame = 0;
}

void Rune::startSpinning() {

  this->index = 0;
  this->reroll = false;

}


void Rune::update(Arduboy2Ext &arduboy) {

  if (spinDelay[this->index] != 255) {

    if (arduboy.everyXFrames(spinDelay[this->index])) {

      this->frame++;
      if (this->frame == 9) this->frame = 0;

      this->index++;


      // Have we reached the end of the sequence ?

      if (spinDelay[this->index] == 255 && frame != 0) {
        this->index--;
      }
      else {

        this->value = arduboy.randomLFSR(0, 6);

      }

    }

  }
  else {

    if (this->reroll && arduboy.everyXFrames(2)) {
      this->frame++;
      if (this->frame == 9) this->frame = 0;
    }

  }

}

bool Rune::isRotating() {

  return (spinDelay[this->index] != 255);// || this->reroll;

}

bool Rune::isRotatingOrRerolling() {

  return (spinDelay[this->index] != 255) || this->reroll;

}

uint8_t Rune::getValue() {

  return this->value;

}

uint8_t Rune::getFrame() {

  return this->frame;

}

void Rune::setValue(uint8_t value) {

  this->value = value;

}
