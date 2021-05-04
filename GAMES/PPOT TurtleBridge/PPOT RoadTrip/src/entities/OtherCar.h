#pragma once

#include "Base.h"
#include <FixedPointsCommon.h>
#include "../utils/Constants.h"
#include "../utils/Curves.h"

struct OtherCar : public Base {

    private:

        MovementType movementType = MovementType::Horizontal;
        Direction direction = Direction::None;
        uint8_t count = 0;
        uint8_t curveIdx = 0;
        int16_t xWorld; // horizontal

    public:

        int16_t getXWorld()               { return this->xWorld; }

        void setXWorld(int16_t val)       { this->xWorld = val; }    

        void update() {

            this->count--;
            
            switch (this->direction) {

                case Direction::Left:

                    #ifndef DEBUG_OTHER_CAR_STEER
                    this->setX(this->getX() - (this->curveIdx * 2));
                    #endif

                    break;

                case Direction::Right:

                    #ifndef DEBUG_OTHER_CAR_STEER
                    this->setX(this->getX() + (this->curveIdx * 2));
                    #endif

                    break;

                default: break;

            }

            if (this->count == 0) {

                switch (this->direction) {

                    case Direction::Left:
                    case Direction::Right:
                        this->direction = Direction::None;
                        this->count = random(50, 200);
                        break;

                    default:

                        switch (this->getX()) {

                            case -100 ... -20:
                                this->direction = Direction::Right;
                                break;

                            case -19 ... 19:
                                this->direction = random(0, 2) == 0 ? Direction::Left : Direction::Right;
                                break;

                            case 20 ... 100:
                                this->direction = Direction::Left;
                                break;
                                
                        }

                        this->curveIdx = random(0, 2);
                        this->count = Curves::OtherCars::Length[this->curveIdx];
                        break;

                }

            }

        }

        int8_t getXMovement() {

            if (this->count > 0) {
                
                switch (this->direction) {

                    case Direction::Left:
                        return -this->curveIdx * 2;

                    case Direction::Right:
                        return this->curveIdx * 2;

                    default: 
                        return 0;

                }

            }
            else {

                return 0;

            }

        }

        void printPos() {

            Serial.print("{");
            Serial.print(this->getX() + this->getXWorld());
            Serial.print(",");
            Serial.print(this->getY());
            Serial.print(",");
            Serial.print(this->getZ());
            Serial.print("}");

        }

};