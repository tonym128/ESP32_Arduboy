#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"
#include "../map/Coordinates.h"
#include <ArduboyTones.h>

extern ArduboyTones sound;

class Player {

  public:

    Player();
 
    // Properties ..

    uint16_t getPosition();
    uint8_t getJumpPosition();
    int8_t getXPosition(bool updatePrevPosition);
    int8_t getYPosition();
    uint8_t getMovements();
    uint8_t getYOffset();
    bool isDead();
    bool isLeaping();
    bool isFalling();

    //void setSound(ArduboyTonesExt *sound);
    void setPosition(uint16_t position);
    void setJumpPosition(uint8_t jumpPosition);
    void setDead(bool dead);
    void setLeaping(bool dead);
    void setFalling(bool falling);


    // Methods

    void incPlayerPosition();
    void decPlayerPosition();
    bool canMove(Movements movement);
    void updateJump();
    void startJump();
    bool isJumping();
    uint8_t getImage();
    Rect getRect();
    void reset();

  protected:

    uint16_t position;
    uint8_t jumpPosition;
    uint8_t movements;

    int8_t prevXPosition;
    uint8_t runCounter;
    Movements runMovement = Movements::Right;

    bool dead = false;
    bool leaping = false;
    bool falling = false;
    //ArduboyTonesExt *sound;
    Coordinates::PlayerData playerData;

};

