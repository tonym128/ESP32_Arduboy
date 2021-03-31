#include "Player.h"

#include "../utils/Physics.h"
#include "../utils/Enums.h"

#define NUM_OF_ELEMENTS 4
#define TURTLE_INDEX_LAST 7

#define POSITION_LEFT_CLIFF 1
#define POSITION_TURTLE_0 2
#define POSITION_TURTLE_1 3
#define POSITION_TURTLE_2 4
#define POSITION_TURTLE_3 5
#define POSITION_TURTLE_4 6
#define POSITION_HAND_OVER_PACKAGE 8

const int8_t PROGMEM positionData[] = { 

  -4, -7, static_cast<int8_t>(Player_Positions::Standing_Right_ArmsDown), POSITION_LEFT_CLIFF,
  -3, -7, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  -2, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  -1, -10, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  0, -11, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  1, -12, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  2, -11, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  3, -11, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,

  4, -10, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  5, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  6, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  7, -8, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  8, -7, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  9, -5, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  10, -3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  11, -1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  12, 1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  13, 3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,

  14, 3, static_cast<int8_t>(Player_Positions::Standing_Right_ArmsDown), POSITION_TURTLE_0,

  15, 3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  16, 1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  17, -1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  18, -3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  19, -5, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  20, -7, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  21, -8, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  22, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  23, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  24, -10, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  25, -10, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  26, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  27, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  28, -8, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  29, -7, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  30, -5, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  31, -3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  32, -1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  33, 1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  34, 3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  
  35, 3, static_cast<int8_t>(Player_Positions::Standing_Right_ArmsDown), POSITION_TURTLE_1,

  37, 3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  38, 1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  39, -1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  40, -3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  41, -5, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  42, -7, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  43, -8, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  44, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  45, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  46, -10, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  47, -10, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  48, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  49, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  50, -8, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  51, -7, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  52, -5, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  53, -3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  54, -1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  55, 1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  56, 3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,

  56, 3, static_cast<int8_t>(Player_Positions::Standing_Right_ArmsDown), POSITION_TURTLE_2,

  56, 3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  57, 1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  57, -1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  58, -3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  59, -5, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  60, -7, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  61, -8, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  62, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  63, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  64, -10, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  65, -10, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  66, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  67, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  68, -8, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  69, -7, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  70, -5, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  71, -3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  72, -1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  73, 1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  74, 3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,

  74, 3, static_cast<int8_t>(Player_Positions::Standing_Right_ArmsDown), POSITION_TURTLE_3,

  75, 3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  76, 1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  77, -1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  78, -3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  79, -5, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  80, -7, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  81, -8, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  82, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  83, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  84, -10, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  85, -10, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  86, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  87, -9, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  88, -8, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  89, -7, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  90, -5, static_cast<int8_t>(Player_Positions::Jumping_Right_02), 0,
  91, -3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  92, -1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  93, 1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  94, 3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,

  95, 3, static_cast<int8_t>(Player_Positions::Standing_Right_ArmsDown), POSITION_TURTLE_4,

  96, 3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  97, 1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  98, -1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  99, -7, static_cast<int8_t>(Player_Positions::Giving), 0,
  100, -8, static_cast<int8_t>(Player_Positions::Giving), 0,
  100, -8, static_cast<int8_t>(Player_Positions::Giving), 0,
  100, -8, static_cast<int8_t>(Player_Positions::Giving), 0,
  100, -8, static_cast<int8_t>(Player_Positions::Giving), 0,
  100, -8, static_cast<int8_t>(Player_Positions::Giving), POSITION_HAND_OVER_PACKAGE,
  100, -8, static_cast<int8_t>(Player_Positions::Giving), 0,
  100, -8, static_cast<int8_t>(Player_Positions::Giving), 0,
  100, -8, static_cast<int8_t>(Player_Positions::Giving), 0,
  100, -8, static_cast<int8_t>(Player_Positions::Giving), 0,
  100, -8, static_cast<int8_t>(Player_Positions::Giving), 0,
  99, -7, static_cast<int8_t>(Player_Positions::Giving), 0,
  98, -1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  97, 1, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,
  95, 3, static_cast<int8_t>(Player_Positions::Jumping_Right_01), 0,

  95, 3, static_cast<int8_t>(Player_Positions::Standing_Right_ArmsDown), 7,

};

#define DEAD_INDEX_MAX 37
#define DEAD_COUNTER_MAX 100

const int8_t PROGMEM positionData_Dead[] = { 

  5, 32, static_cast<int8_t>(Player_Positions::None), 0,

  5, 32, static_cast<int8_t>(Player_Positions::Drowning_01), 0,
  5, 32, static_cast<int8_t>(Player_Positions::Drowning_01), 0,
  5, 32, static_cast<int8_t>(Player_Positions::Drowning_01), 0,
  5, 32, static_cast<int8_t>(Player_Positions::Drowning_01), 0,

  5, 32, static_cast<int8_t>(Player_Positions::Drowning_02), 0,
  5, 32, static_cast<int8_t>(Player_Positions::Drowning_02), 0,
  5, 32, static_cast<int8_t>(Player_Positions::Drowning_02), 0,
  5, 32, static_cast<int8_t>(Player_Positions::Drowning_02), 0,

  5, 32, static_cast<int8_t>(Player_Positions::Drowning_01), 0,
  5, 31, static_cast<int8_t>(Player_Positions::Drowning_01), 0,
  5, 30, static_cast<int8_t>(Player_Positions::Drowning_01), 0,

  5, 29, static_cast<int8_t>(Player_Positions::Drowning_01), 0,
  5, 28, static_cast<int8_t>(Player_Positions::Drowning_02), 0,
  5, 27, static_cast<int8_t>(Player_Positions::Drowning_02), 0,

  5, 26, static_cast<int8_t>(Player_Positions::Drowning_02), 0,
  5, 25, static_cast<int8_t>(Player_Positions::Drowning_02), 0,
  5, 24, static_cast<int8_t>(Player_Positions::Drowning_01), 0,

  5, 23, static_cast<int8_t>(Player_Positions::Drowning_01), 0,
  5, 22, static_cast<int8_t>(Player_Positions::Drowning_01), 0,
  5, 21, static_cast<int8_t>(Player_Positions::Drowning_01), 0,

  5, 20, static_cast<int8_t>(Player_Positions::Drowning_02), 0,
  5, 19, static_cast<int8_t>(Player_Positions::Drowning_02), 0,
  5, 18, static_cast<int8_t>(Player_Positions::Drowning_02), 0,

  5, 17, static_cast<int8_t>(Player_Positions::Drowning_02), 0,
  5, 16, static_cast<int8_t>(Player_Positions::Drowning_01), 0,
  5, 15, static_cast<int8_t>(Player_Positions::Drowning_01), 0,

  5, 14, static_cast<int8_t>(Player_Positions::Drowning_01), 0,
  5, 13, static_cast<int8_t>(Player_Positions::Drowning_01), 0,
  5, 12, static_cast<int8_t>(Player_Positions::Drowning_02), 0,

  5, 11, static_cast<int8_t>(Player_Positions::Drowning_02), 0,
  5, 10, static_cast<int8_t>(Player_Positions::Drowning_02), 0,
  4, 9, static_cast<int8_t>(Player_Positions::Drowning_02), 0,

  3, 8, static_cast<int8_t>(Player_Positions::Drowning_01), 0,
  2, 7, static_cast<int8_t>(Player_Positions::Drowning_01), 0,
  1, 6, static_cast<int8_t>(Player_Positions::Drowning_01), 0,

  0, 5, static_cast<int8_t>(Player_Positions::Drowning_01), 0,

};


Player::Player() { 

}

int8_t Player::getDisplayX() {

  int8_t x = pgm_read_byte(&positionData[this->position * NUM_OF_ELEMENTS]);

  if (dead == 0) {
    return x;
  }
  else {

    int8_t xOffset = pgm_read_byte(&positionData_Dead[this->dead * NUM_OF_ELEMENTS]);
    if (this->lastDirection == Direction::Left) xOffset = xOffset * -1;
    return x + xOffset;

  }

}

int8_t Player::getDisplayY(uint8_t offset) {

  if (dead == 0) {

    return pgm_read_byte(&positionData[(this->position * NUM_OF_ELEMENTS) + 1]) - offset + 2;;

  }
  else {
   
    int8_t y = pgm_read_byte(&positionData[(this->position * NUM_OF_ELEMENTS) + 1]) + 2;
    int8_t yOffset = pgm_read_byte(&positionData_Dead[(this->dead * NUM_OF_ELEMENTS) + 1]);
    return y + yOffset + this->deadY;

  }

}

uint8_t Player::getPosition() {

  return this->position;

}

uint8_t Player::getTurtleIndexPrev() {

  return this->turtleIndexPrev;

}

Direction Player::getDirection() {

  return this->direction;

}

uint8_t Player::getImageIndex() {

  if (dead == 0) {

    uint8_t imageIndex = pgm_read_byte(&positionData[this->position * NUM_OF_ELEMENTS] + 2);
    Player_Positions playerPosition = static_cast<Player_Positions>(imageIndex);
    uint8_t leftOffset = 0;

    if((this->direction == Direction::Left) && ((playerPosition == Player_Positions::Jumping_Right_01) || (playerPosition == Player_Positions::Jumping_Right_02)))
            leftOffset = 4;

    if((this->lastDirection == Direction::Left)  && (playerPosition == Player_Positions::Standing_Right_ArmsDown))
            leftOffset = 2;

    return (imageIndex + leftOffset + (this->holdingPackage ? 1 : 0));

  }
  else {

    uint8_t image = pgm_read_byte((&positionData_Dead[this->dead * NUM_OF_ELEMENTS] + 2));
    if (this->lastDirection == Direction::Left) image = image + 2;
    return image;

  }

}

bool Player::showTombstone() {

  return (dead != 0);

}

uint8_t Player::getTombstoneY() {

  return 63 - this->deadY;

}

bool Player::isHoldingPackage() {

  return this->holdingPackage;

}

void Player::setPosition(uint8_t position) {

  this->position = position;

}

void Player::setHoldingPackage(bool value) {

  this->holdingPackage = value;

}

void Player::setDirection(Direction direction) {

  this->direction = direction;

}


// Methods ..

bool Player::canMoveLeft() {
  
  if (this->position == 0) {
    return false;
  }

  uint8_t posData = pgm_read_byte(&positionData[this->position * NUM_OF_ELEMENTS] + 3);
  return (posData > 0 && posData < POSITION_HAND_OVER_PACKAGE);

}

bool Player::canMoveRight() {

  return this->position < 114;

}

void Player::moveLeft() {

  this->position--;

}

void Player::moveRight() {

  this->position++;

}

void Player::move(bool turtle_0_Diving, bool turtle_1_Diving, bool turtle_2_Diving, bool turtle_3_Diving, bool turtle_4_Diving) {

  uint8_t turtleIndex = this->getTurtleIndex();
  this->turtleIndexPrev = (this->turtleIndexPrev != turtleIndex ? turtleIndex : TURTLE_NONE);

  if (dead == 0) {

    switch (this->direction) {

      case Direction::Left:
        this->position--;
        break;

      case Direction::Right:
        {
          this->position++;

          uint8_t posData = pgm_read_byte(&positionData[this->position * NUM_OF_ELEMENTS] + 3);

          if (posData == TURTLE_INDEX_LAST) {

            this->position = this->position - 19;//21;

          }

        }

        break;

      default:
        break;

    }

    uint8_t posData = pgm_read_byte(&positionData[this->position * NUM_OF_ELEMENTS] + 3);

    if (posData > 0 && posData < POSITION_HAND_OVER_PACKAGE) {

      if(this->direction != Direction::None)
        this->lastDirection = this->direction;

      this->direction = Direction::None;

      switch (posData - POSITION_TURTLE_0) {

        case 0:
          if (turtle_0_Diving) {
            this->dead = DEAD_INDEX_MAX;
            this->deadCounter = DEAD_COUNTER_MAX;
            this->deadY = 0;
          }
          break;

        case 1:
          if (turtle_1_Diving) {
            this->dead = DEAD_INDEX_MAX;
            this->deadCounter = DEAD_COUNTER_MAX;
            this->deadY = 0;
          }
          break;

        case 2:
          if (turtle_2_Diving) {
            this->dead = DEAD_INDEX_MAX;
            this->deadCounter = DEAD_COUNTER_MAX;
            this->deadY = 0;
          }
          break;

        case 3:
          if (turtle_3_Diving) {
            this->dead = DEAD_INDEX_MAX;
            this->deadCounter = DEAD_COUNTER_MAX;
            this->deadY = 0;
          }
          break;

        case 4:
          if (turtle_4_Diving) {
            this->dead = DEAD_INDEX_MAX;
            this->deadCounter = DEAD_COUNTER_MAX;
            this->deadY = 0;
          }
          break;
          
      }

    }

  }
  else {

    this->dead--;

    if (this->dead % 4 == 0) this->deadY++;
    if (this->deadCounter > 1) this->deadCounter--;

    if (this->dead == 0) {

      this->dead = 7;

    }

  }

}

uint8_t Player::getTurtleIndex() {

  uint8_t turtleIndex = TURTLE_NONE;
  uint8_t posData = pgm_read_byte(&positionData[this->position * NUM_OF_ELEMENTS] + 3);

  if (posData > 0 && posData < POSITION_HAND_OVER_PACKAGE) {

    turtleIndex = posData - POSITION_TURTLE_0;

  }

  return turtleIndex;

}

bool Player::isPackagePosition() {

  uint8_t posData = pgm_read_byte(&positionData[this->position * NUM_OF_ELEMENTS] + 3);
  return (posData == POSITION_HAND_OVER_PACKAGE);

}


bool Player::isLeftCliffPosition() {

  return (this->position == 0);

}

bool Player::isDead() {

  return (this->deadCounter == 1);

}

bool Player::isDying() {

  return (this->deadCounter > 1);

}

void Player::initLife() {

  this->deadCounter = 0;
  this->dead = 0;
  this->position = 0;
  this->direction = Direction::None;

}
