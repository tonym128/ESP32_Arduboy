#include "Lever.h"

#include "../utils/Enums.h"
#include "../map/Coordinates.h"
#include "../sounds/Sounds.h"


Lever::Lever() : Base() { }

LeverPosition Lever::getPosition() {

  return this->position;

}
/*
void Lever::setSound(ArduboyTonesExt *sound) {

  this->sound = sound;

}
*/
void Lever::setPosition(LeverPosition position) {

  this->position = position;

  if (position == LeverPosition::On) {
    this->counter = LEVER_ON_COUNTDOWN;
  }
  else {
    this->counter = 0;
    this->crane->turnOff();
  }

}

void Lever::setCrane(Crane *crane) {

  this->crane = crane;

}

void Lever::update() {

  switch (this->counter) {

    case 0:
      break;

    case 1:
      this->counter = 0;
      this->position = LeverPosition::Off;
      this->crane->turnOff();
#ifdef PLAY_SOUNDS 
      if (!sound.playing()) sound.tones(Sounds::lever_off);
#endif
      break;

    default:
      this->counter--;
      break;

  }

}