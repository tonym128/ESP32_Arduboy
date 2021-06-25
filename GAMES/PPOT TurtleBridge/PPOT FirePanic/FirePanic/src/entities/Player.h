#pragma once

#include "../Utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"

class Player {

  public:

    Player();
     
    uint8_t getX();
    uint8_t getY();
    Direction getDirection();

    void setX(uint8_t value);
    void setDirection(Direction value);

    void move();
    bool canChangeDirections();
    bool canMoveLeft();
    bool canMoveRight();
    uint8_t getImageIndex();
    
  protected:

    uint8_t x;
    uint8_t xIdx;
    uint8_t image;
    Direction direction = Direction::None;

};

