#pragma once

#include <Arduboy2.h>
#include "../images/Images.h"
#include "../utils/Consts.h"

struct Chair {

    private:

        uint8_t x;
        uint8_t y;
        uint8_t frame;
        uint8_t counter;
        Direction direction = Direction::None;

    public:

        Direction getDirection()        { return this->direction; }

        uint8_t getFrame()              { return this->frame; }
        uint8_t getXDisplay()           { return this->x; }
        uint8_t getYDisplay()           { return this->y; }

        void setPosition(uint8_t x, uint8_t y, Direction directionToMove) {

            this->x = x;
            this->y = y;
            this->direction = directionToMove;
            this->frame = 0;
            this->counter = 8;

        }

        void update() {

            if (this->counter == 0) return;

            this->x = this->x + (this->direction == Direction::Right ? -2 : 2);
            if (this->counter % 2 == 0) { this->y++; }
            this->counter--;
            this->frame++;
            if (this->frame == 4) this->frame = 0;

        }

        void reset() {

            this->direction = Direction::None;

        }

};