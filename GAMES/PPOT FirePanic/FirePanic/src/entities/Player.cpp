#include "Player.h"

#include "../utils/Physics.h"
#include "../utils/Enums.h"

#define NUM_OF_ELEMENTS 5

const uint8_t PROGMEM steps[] = { // 21 steps between positions.
  3, 4, 7, 4, 3
};

Player::Player() { 

    this->x = PLAYER_MIN_X_POS;

}

uint8_t Player::getX() {

  return this->x;

}

uint8_t Player::getY() {

  return PLAYER_Y_POS;

}

uint8_t Player::getImageIndex() {

  return this->image / PLAYER_STEP_INC;

}

Direction Player::getDirection() {

  return this->direction;

}

void Player::setX(uint8_t value) {

  this->x = value;

}

void Player::setDirection(Direction value) {

  this->direction = value;

}

bool Player::canChangeDirections() {

  return this->xIdx == 0;

}

bool Player::canMoveLeft() {

  return (this->xIdx == 0) && (this->x > PLAYER_MIN_X_POS);

}

bool Player::canMoveRight() {

  return (this->xIdx == 0) && (this->x < PLAYER_MAX_X_POS);

}


void Player::move() {

  switch (this->direction) {

    case Direction::Left:

      this->x = this->x - pgm_read_byte(&steps[this->xIdx++]);
      // this->image++;
      // if (this->image == PLAYER_STEP_INC * 2) this->image = 0;

      // if (this->xIdx == NUM_OF_ELEMENTS) {
      //   this->xIdx = 0;
      //   this->direction = Direction::None;
      // }
      break;

    case Direction::Right:

      this->x = this->x + pgm_read_byte(&steps[this->xIdx++]);
      // this->image++;
      // if (this->image == PLAYER_STEP_INC * 2) this->image = 0;

      // if (this->xIdx == NUM_OF_ELEMENTS) {
      //   this->xIdx = 0;
      //   this->direction = Direction::None;
      // }
      break;
      default: break;
    
  }

  this->image++;
  if (this->image == PLAYER_STEP_INC * 2) this->image = 0;

  if (this->xIdx == NUM_OF_ELEMENTS) {
    this->xIdx = 0;
    this->direction = Direction::None;
  }

}
