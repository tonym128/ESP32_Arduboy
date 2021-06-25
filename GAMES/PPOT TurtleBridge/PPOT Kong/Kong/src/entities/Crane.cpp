#include "Crane.h"

#include "../utils/Enums.h"
#include "../map/Coordinates.h"

Crane::Crane() : Base() { }

CranePosition Crane::getPosition() {

  return this->position;

}

bool Crane::getLiftPlayer() {

  return this->liftPlayer;

}

void Crane::setPosition(CranePosition position) {

  this->position = position;

}

void Crane::setLiftPlayer(bool liftPlayer) {

  this->liftPlayer = liftPlayer;

}

void Crane::turnOn() {

  this->liftPlayer = false;
  this->state = CraneState::TurningOn;
  this->counter = 20;

}

void Crane::turnOff() {

  this->state = CraneState::TurningOff;
  this->counter = 20;

}

void Crane::update() {

  switch (this->state) {

    case CraneState::Dormant:
      break;

    case CraneState::TurningOn:

      if (this->counter > 0) {

        this->counter--;

      }
      else {

        switch (this->position) {

          case CranePosition::Declined:
            this->position = CranePosition::Flat;
            this->counter = 20;
            break;

          case CranePosition::Flat:
            this->position = CranePosition::Inclined_03;
            this->counter = 20;
            break;

          case CranePosition::Inclined_01 ... CranePosition::Inclined_05:
            this->hook = 2;
            this->counter = 10;
            this->state = CraneState::Swinging;
            break;

          default: break;

        }

      }

      break;

    case CraneState::TurningOff:

      if (this->counter > 0) {

        this->counter--;

        if (this->position >= CranePosition::Inclined_01 && this->position <= CranePosition::Inclined_05 && this->hook != 2) {
          this->swingHook();
        }

      }
      else {

        switch (this->position) {

          case CranePosition::Inclined_01 ... CranePosition::Inclined_05:
            this->position = CranePosition::Flat;
            this->counter = 20;
            break;

          case CranePosition::Flat:
            this->position = CranePosition::Declined;
            this->counter = 20;
            break;

          case CranePosition::Declined:
            this->hook = 2;
            this->counter = 10;
            this->state = CraneState::Dormant;
            break;

          default: break;

        }

      }

      break;

    case CraneState::Swinging:

      if (this->counter > 0) {

        this->counter--;

      }
      else {

        this->swingHook();
        this->counter = 10;

      }

      break;

    default: break;

  }

}

void Crane::swingHook() {

  switch (this->hookDirection) {

    case Movements::Left: 

      switch (this->hook) {
        
        case 0:
          this->hook = 1;
          this->hookDirection = Movements::Right;
          break;

        case 1 ... 4:
          this->hook--;
          break;

      }

      break;

    case Movements::Right: 

      switch (this->hook) {
        
        case 0 ... 3:
          this->hook++;
          break;

        case 4:
          this->hook = 3;
          this->hookDirection = Movements::Left;
          break;

      }

      break;

    default: break;

  }

  if (this->position >= CranePosition::Inclined_01 && this->position <= CranePosition::Inclined_05) {

    uint8_t cranePos = static_cast<uint8_t>(CranePosition::Inclined_01) + this->hook;
    this->position = static_cast<CranePosition>(cranePos);
    
  }

}