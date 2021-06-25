#pragma once

#include <Arduboy2.h>
#include "../images/Images.h"
#include "../utils/Consts.h"

struct Player {

    const int8_t xPositions[25] = { 0, 0, 0,  5,  10, 15, 20,
                                    26, 31, 36, 41, 46,
                                    52, 57, 62, 67, 72,
                                    83, 88, 93, 98, 101, 
                                    100, 100, 100 };
//87 102
    const int8_t xPositionsMap[25] = { 87, 87, 87, 87, 87, 87,
                                    88, 89, 90,
                                    91, 92, 93, 94, 95,
                                    96, 97, 98, 99, 100, 101, 102, 
                                    102, 102, 102 };

    const int8_t xEndPoints[25] = {  0, 1,  0,  0,  0,  0,  0,
                                     1,  0,  0,  0,  0,
                                     1,  0,  0,  0,  0,
                                     1,  0,  0,  0,  0,
                                     0, 1, 0 };

    const int8_t frames[25] =     { 2, 2, 2, 0, 1, 0, 1,
                                    0, 1, 0, 1, 0,
                                    1, 0, 1, 0, 1,
                                    0, 1, 0, 1, 0,
                                    3, 3, 3 };


    private:

        Direction direction = Direction::None;
        XPosition xPosition = XPosition::Position_10_Oil;
        uint8_t oilLevel = 0;

    public:

        XPosition getXPosition()                    { return this->xPosition; }
        uint8_t getOilLevel()                       { return this->oilLevel; }


        void setXPosition(XPosition val)            { this->xPosition = val; }
        void setOilLevel(uint8_t val)               { this->oilLevel = val; }

    public:

        void incXPosition() {

            if (this->xEndPoints[static_cast<uint8_t>(this->xPosition)] == 1 && xPosition < XPosition::Position_Outside_RH) {
                this->direction = Direction::Right;
                this->xPosition++;
            }

        }

        bool incOilLevel() {

            if (this->oilLevel < 3) {
                this->oilLevel++;
                return true;
            }

            return false;

        }

        void decXPosition() {

            if (this->xEndPoints[static_cast<uint8_t>(this->xPosition)] == 1 && xPosition > XPosition::Position_Outside_LH) {
                this->direction = Direction::Left;
                this->xPosition--;
            }

        }

        void update() {

            switch (this->direction) {

                case Direction::Left:
                    this->xPosition--;
                    break;

                case Direction::Right:
                    this->xPosition++;
                    break;

                default: break;

            }

            if (this->xEndPoints[static_cast<uint8_t>(this->xPosition)] == 1) {
                this->direction = Direction::None;
            }

        }

        int8_t getXDisplay() {

            return this->xPositions[static_cast<uint8_t>(this->xPosition)];

        }

        int8_t getXDisplayMap() {

            return this->xPositionsMap[static_cast<uint8_t>(this->xPosition)];

        }

        uint8_t getFrame() {

            return this->frames[static_cast<uint8_t>(this->xPosition)];

        }

        void reset() {

            this->direction = Direction::None;
            this->xPosition = XPosition::Position_10_Oil;
            this->oilLevel = 0;

        }

};

