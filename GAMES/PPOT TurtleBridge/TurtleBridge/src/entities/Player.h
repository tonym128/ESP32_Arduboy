#pragma once

#include "../Utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"

class Player {

  public:

    Player();
     
    int8_t getDisplayX();
    int8_t getDisplayY(uint8_t offset);
    uint8_t getPosition();
    Direction getDirection();
    bool isHoldingPackage();
    
    void setPosition(uint8_t position);
    void setHoldingPackage(bool value);
    void setDirection(Direction direction);


    // Methods ..

    bool canMoveLeft();
    bool canMoveRight();
    void moveLeft();
    void moveRight();
    void move(bool turtle_0_Diving, bool turtle_1_Diving, bool turtle_2_Diving, bool turtle_3_Diving, bool turtle_4_Diving);
    uint8_t getImageIndex();
    uint8_t getTurtleIndex();
    uint8_t getTurtleIndexPrev();

    bool isPackagePosition();
    bool isLeftCliffPosition();
    bool isDead();
    bool isDying();
    void initLife();

    bool showTombstone();
    uint8_t getTombstoneY();

  protected:

    uint8_t position;
    uint8_t turtleIndexPrev = TURTLE_NONE;
    bool holdingPackage;
    
    uint8_t dead;
    uint8_t deadCounter;
    uint8_t deadY;

    Direction direction = Direction::None;
    Direction lastDirection = Direction::None;

};

