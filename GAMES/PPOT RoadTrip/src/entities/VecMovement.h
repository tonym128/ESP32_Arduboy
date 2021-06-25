#pragma once

#include "../utils/Constants.h"
#include "../utils/Curves.h"


struct VecMovement {

    private: 

        MovementType movementType = MovementType::Horizontal;
        Direction direction = Direction::None;
        uint8_t count = 0;
        uint8_t curveIdx = 0;

    public:

        MovementType getMovementType()              { return this->movementType; }
        Direction getDirection()                    { return this->direction; }
        uint8_t getCount()                          { return this->count; }


        void setMovementType(MovementType val)      { this->movementType = val; }
        void setDirection(Direction val)            { this->direction = val; }
        void setCount(uint8_t val)                  { this->count = val; }


    public:

        VecMovement(MovementType movementType, Direction direction, uint8_t count, uint8_t curveIdx) {

            this->movementType = movementType;
            this->direction = direction;
            this->count = count;
            this->curveIdx = curveIdx;
            
        }

        int16_t getMovement() {

            switch (this->direction) {

                case Direction::Left:
                    {
                        uint8_t x = Curves::Roads::Horizontal::Data[this->curveIdx][this->count - 1];
                        return x * UPM / 4;
                    }
                    break;

                case Direction::Up:
                    {
                        uint8_t x = Curves::Roads::Vertical::Data[this->curveIdx][this->count - 1];
                        return x * UPM / 4;
                    }
                    break;

                case Direction::Right:
                    {
                        uint8_t x = Curves::Roads::Horizontal::Data[this->curveIdx][this->count - 1];
                        return -x * UPM / 4;
                    }
                    break;

                case Direction::Down:
                    {
                        uint8_t x = Curves::Roads::Vertical::Data[this->curveIdx][this->count - 1];
                        return -x * UPM / 4;
                    }
                    break;

                default:
                    return 0;

            }

            return 0;

        }

        void update() {

            this->count--;

            if (this->count == 0) {

                switch (this->movementType) {

                    case MovementType::Horizontal:

                        switch (this->direction) {

                            case Direction::Left:
                            case Direction::Right:
                                this->direction = Direction::None;
                                this->count = random(3, 20);
                                break;

                            default:
                                this->direction = random(0, 2) == 0 ? Direction::Left : Direction::Right;
                                this->curveIdx = random(0, Curves::Roads::Horizontal::Count);
                                this->count = Curves::Roads::Horizontal::Length[this->curveIdx];
                                break;

                        }

                        break;

                    case MovementType::Vertical:

                        switch (this->direction) {

                            case Direction::Up:
                            case Direction::Down:
                                this->direction = Direction::None;
                                this->count = random(3, 20);
                                break;

                            default:
                                this->direction = random(0, 2) == 0 ? Direction::Up : Direction::Down;
                                this->curveIdx = random(0, Curves::Roads::Vertical::Count);
                                this->count = Curves::Roads::Vertical::Length[this->curveIdx];
                                break;

                        }

                        break;

                }

            }

        }

};