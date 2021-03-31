#include "Turtle.h"

#include "../utils/Enums.h"


Turtle::Turtle() { 
}

uint8_t Turtle::getX() {

  return this->x;

}

uint8_t Turtle::getY() {

  return this->y;

}

TurtleMode Turtle::getMode() {

  return this->mode;

}

TurtleType Turtle::getType() {

  return this->type;

}

Direction Turtle::getDirection() {

  return this->direction;

}

void Turtle::setX(uint8_t x) {

  this->x = x;

}

void Turtle::setY(uint8_t y) {

  this->y = y;

}

void Turtle::setMode(TurtleMode mode) {

  this->mode = mode;

  switch (this->mode) {

    case TurtleMode::LookingUp:
      this->returnToLookingDown = RETURN_TO_LOOKING_DOWN_FROM_UP;
      break;

    case TurtleMode::LookingDown:
      this->returnToLookingDown = 0;
      break;

    case TurtleMode::Diving:
      this->returnToLookingDown = RETURN_TO_LOOKING_DOWN_FROM_DIVING;
      break;
      
  }

}

void Turtle::setType(TurtleType type) {
  this->type = type;
}

void Turtle::setDirection(Direction direction) {

  this->direction = direction;

}

void Turtle::setBobUpDown(uint8_t bobUpDown) {

  this->bobUpDown = bobUpDown;

}


// Methods ..

uint8_t Turtle::getImageIndex() {

  return static_cast<uint8_t>(this->mode) + (this->direction == Direction::Right ? 3 : 0);

}

uint8_t Turtle::getDisplayX() {

  return this->x;

}

uint8_t Turtle::getDisplayY() {

  switch (this->mode) {

    case TurtleMode::LookingUp:
      return this->y - 4;

    case TurtleMode::LookingDown:
      return this->y + (this->bobUpDown < 50 ? -1 : 0);

    case TurtleMode::Diving:

      switch (this->returnToLookingDown) {

        // case 0 ... 3:
        //   return this->y + 2;
        // case 4 ... 6:
        //   return this->y + 4;
        // case 7 ... 12:
        //   return this->y + 7;
        // case 13 ... 15:
        //   return this->y + 4;
        // case 16 ... 18:
        //   return this->y + 2;

        case 0 ... 3:
          return this->y + 2;
        case 4 ... 6:
          return this->y + 4;
        case 7 ... 12:
          return this->y + 6;
        case 13 ... 15:
          return this->y + 8;
        case 16 ... 18:
          return this->y + 10;
        case 19 ... 21:
          return this->y + 10;
        case 22 ... 24:
          return this->y + 8;
        case 25 ... 27:
          return this->y + 6;
        case 28 ... 30:
          return this->y + 4;
        case 31 ... 33:
          return this->y + 2;
        
        default : return this->y;

      }

    default:
      return this->y;

  }

}

void Turtle::updateMode() {

  if (this->bobUpDown > 0) {
    this->bobUpDown--;
  }
  else {
    this->bobUpDown = 100;
  }


  if (this->returnToLookingDown > 0) {
    this->returnToLookingDown--;
  }
  else {
    this->mode = TurtleMode::LookingDown;
  }

}


bool Turtle::getBobUp() {

  return (this->bobUpDown < 50);

}