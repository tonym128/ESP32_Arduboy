#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"
#include "../map/Coordinates.h"

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
        bool isFalling();
        bool getExit();

        void setPosition(uint16_t position);
        void setJumpPosition(uint8_t jumpPosition);
        void setDead(bool dead);
        void setFalling(bool falling);
        void setExit(bool dead);


        // Methods

        uint8_t getImage();
        uint8_t getImage(bool update);

        void incPlayerPosition();
        void decPlayerPosition(bool exitSequence);

        bool canMove(Movements movement);
        void updateJump();
        void startJump();
        bool isJumping();
        bool atTopOfJump();
        bool canGrabChain();
        void reset();

        Rect getRect();

    protected:

        uint16_t position;
        uint8_t jumpPosition;
        uint8_t movements;

        int8_t prevXPosition;
        uint8_t runCounter;
        uint8_t prevImage;
        Movements runMovement = Movements::Right;

        bool dead = false;
        bool falling = false;
        bool exit = false;
        Coordinates::PlayerData playerData;

};

