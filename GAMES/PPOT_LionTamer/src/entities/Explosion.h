#pragma once

#include <Arduboy2.h>
#include "../images/Images.h"
#include "../utils/Consts.h"

struct Explosion {

    private:

        int16_t x = 0;
        int16_t y = 0;
        int8_t counter = -127;
        uint8_t direction = 0;

    public:

        int16_t getX()                              { return this->x; }
        int16_t getY()                              { return this->y; }
        int8_t getCounter()                         { return this->counter; }
        uint8_t getDirection()                      { return this->direction; }

        void setX(int16_t val)                      { this->x = val; }
        void setY(int16_t val)                      { this->y = val; }
        void setCounter(int8_t val)                 { this->counter = val; }
        void setDirection(uint8_t val)              { this->direction = val; }

    public:

        void update(uint16_t frame) {

            uint8_t frame2 = (frame % 2 == 0);
            uint8_t frame3 = (frame % 3 == 0);

            switch (this->counter) {

                case -126 ... -1:    
                    this->counter++;
                    break;

                case 0 ... 120:
                    this->counter++;

                    switch (this->direction) {

                        case 0:                              // NE
                            this->x = this->x + 2;
                            this->y = this->y - 2;
                            break;

                        case 1:                             
                            if (frame2) this->y = this->y - 2;
                            this->x = this->x + 2;
                            break;

                        case 2:                              // ENE
                            if (frame3) this->y = this->y - 2;
                            this->x = this->x + 2;
                            break;

                        case 3:                              
                            if (frame2) this->y = this->y - 1;
                            this->x = this->x + 2;
                            break;

                        case 4:                              // E
                            this->x = this->x + 2;
                            break;

                        case 5:                              // ENE
                            if (frame2) this->y = this->y + 1;
                            this->x = this->x + 2;
                            break;

                        case 6:                              // ESE
                            if (frame3) this->y = this->y + 2;
                            this->x = this->x + 2;
                            break;

                        case 7:                             
                            if (frame2) this->y = this->y + 2;
                            this->x = this->x + 2;
                            break;

                        case 8:                              // SE
                            this->x = this->x + 2;
                            this->y = this->y + 2;
                            break;

                    }

                    break;

            }

        }

        bool render() {

            switch (this->counter) {

                case -127 ... -1:
                    return false;

                case 0 ... 60:
                    return true;

                default:
                    return false;

            }

        }


};