#include "Player.h"

#include "../utils/Enums.h"
#include "../map/Coordinates.h"
#include "../sounds/Sounds.h"

#define JUMP_POSITIONS 37

const uint8_t  jumpPositions[JUMP_POSITIONS] = { 
0, 2, 4, 6,
8, 9, 10, 10,
11, 11, 12, 12, 

12, 13, 13, 13, 
13, 13, 13, 13, 
13, 13, 13, 13, 

//13, 13, 13, 13, 
13, 12, 12, 12, 
11, 11, 10, 9, 

8, 6, 4, 2, 
0
};

Player::Player() { 

  this->setPosition(0);

}

uint16_t Player::getPosition() {

  return this->position;

}

uint8_t Player::getJumpPosition() {

  return this->jumpPosition;

}

int8_t Player::getXPosition(bool updatePrevPosition) {

  if (updatePrevPosition) {
    this->prevXPosition = this->playerData.x;
  }

  Coordinates::readPlayerData(this->playerData, this->getPosition());
  return this->playerData.x;

}

int8_t Player::getYPosition() {

  int8_t y = this->playerData.y - this->playerData.yOffset - pgm_read_byte(&jumpPositions[this->jumpPosition]);
  return y;

}

uint8_t Player::getMovements() {

  return this->playerData.movements;

}

uint8_t Player::getYOffset() {

  return this->playerData.yOffset;

}

bool Player::isDead() {

  return this->dead;

}

bool Player::isLeaping() {

  return this->leaping;

}

bool Player::isFalling() {

  return this->falling;

}
/*
void Player::setSound(ArduboyTonesExt *sound) {

  this->sound = sound;

}
*/
void Player::setPosition(uint16_t position) {

  this->position = position;
  this->jumpPosition = 0;
  Coordinates::readPlayerData(this->playerData, this->getPosition());

}

void Player::setJumpPosition(uint8_t jumpPosition) {

  this->jumpPosition = jumpPosition;

}

void Player::setDead(bool dead) {

  this->dead = dead;

}

void Player::setLeaping(bool leaping) {

  this->leaping = leaping;

}

void Player::setFalling(bool falling) {

  this->falling = falling;

}

void Player::incPlayerPosition() {

  this->position++;
  Coordinates::readPlayerData(this->playerData, this->getPosition());

}

void Player::decPlayerPosition() {

  this->position--;
  Coordinates::readPlayerData(this->playerData, this->getPosition());
  
}

bool Player::canMove(Movements movement) {

  return (this->playerData.movements & static_cast<uint8_t>(movement));

}

void Player::updateJump() {

  if (this->jumpPosition < JUMP_POSITIONS - 1) {
    this->jumpPosition++;
  }
  else {
    this->jumpPosition = 0;
  }

}

void Player::startJump() {

#ifdef PLAY_SOUNDS 
  sound.tones(Sounds::jump);
#endif
  this->jumpPosition = 1;

}

bool Player::isJumping() {

  return this->jumpPosition > 0;

}

uint8_t Player::getImage() {

  if (this->jumpPosition == 0) {

    switch (this->playerData.stance) {

      case Stance::Normal:

        if (this->playerData.x == this->prevXPosition) {

          if (this->runMovement == Movements::Left) {
            return static_cast<uint8_t>(Stance::Normal);
          }
          else {
            return static_cast<uint8_t>(Stance::Normal_RHS);
          }

        }

        if (this->playerData.x > this->prevXPosition) {

          this->runCounter++;
          this->runMovement = Movements::Right;
          if (this->runCounter == 16) this->runCounter = 0;
          return static_cast<uint8_t>(Stance::Normal) + ((this->runCounter / 4) * 2) + 3;

        }

        if (this->playerData.x < this->prevXPosition) {

          this->runCounter++;
          this->runMovement = Movements::Left;
          if (this->runCounter == 16) this->runCounter = 0;
          return static_cast<uint8_t>(Stance::Normal) + ((this->runCounter / 4) * 2) + 2;

        }

        break;

      default:

        return static_cast<uint8_t>(this->playerData.stance);

    }

  }
  else {

    if (this->playerData.x == this->prevXPosition) {

      if (this->runMovement == Movements::Left) {
        return static_cast<uint8_t>(Stance::Jump);
      }
      else {
        return static_cast<uint8_t>(Stance::Jump_RHS);
      }

    }

  }

  return static_cast<uint8_t>(Stance::Normal);

}


Rect Player::getRect() {

  int8_t y = this->playerData.y - this->playerData.yOffset - pgm_read_byte(&jumpPositions[this->jumpPosition]);
  return Rect{this->playerData.x, y, 7, 11 };

}

void Player::reset() {

  this->setPosition(0);
  this->setJumpPosition(0);
  this->dead = false;
  this->leaping = false;
  this->falling = false;
  this->prevXPosition = -1;
  this->runMovement = Movements::Right;

}