#pragma once

#include <Arduboy2.h>
#include "../images/Images.h"
#include "../utils/Consts.h"

struct Catcher {

    const int8_t xPositions[2][25] = {    
                                        {  6, 14, 14, 22, 22, 
                                          30, 30, 38, 38, 46, 
                                          46, 54, 54, 62, 62,
                                          70, 70, 72, 72, 78, 
                                          78, 78, 86, 86, 90 }
                                        ,
                                        {  6,  9,  9,  9, 13, 
                                          13, 21, 21, 29, 29,  
                                          35, 35, 43, 43, 51, 
                                          51, 59, 59, 67, 67, 
                                          75, 75, 83, 83, 90 }
                                     };


    const int8_t frames[2][25] =     { 
                                        { 0, 1, 1, 0, 0, 
                                          1, 1, 0, 0, 1, 
                                          1, 0, 0, 1, 1, 
                                          0, 0, 2, 3, 4, 
                                          5, 5, 6, 6, 5 }
                                        ,
                                        { 0, 1, 1, 2, 3, 
                                          4, 6, 6, 5, 5, 
                                          6, 6, 5, 5, 6, 
                                          6, 5, 5, 6, 6, 
                                          5, 5, 6, 6, 5 }
                                     };


    const int8_t xPositionsMap[25] =  { 87, 88, 89, 90, 90, 
                                        91, 91, 92, 92, 93, 
                                        93, 94, 94, 95, 96, 
                                        97, 97, 98, 98, 99, 
                                        99, 100, 100, 101, 102,  };


    private:

        Direction direction = Direction::Left;
        uint8_t x = 8;
        uint8_t turnDelay = 0;
        uint8_t minTurnDelay = 0;
        uint8_t maxTurnDelay = 0;

    public:

        uint8_t getX()                              { return this->x; }

        void setX(uint8_t val)                      { this->x = val; }
        void setDirection(Direction val)            { this->direction = val; }

    public:

        void incXPosition() {

            if (x < 24) {
                this->direction = Direction::Right;
                this->x++;
            }

        }

        void decXPosition() {

            if (x > 0) {
                this->direction = Direction::Left;
                this->x--;
            }

        }

        void update(uint16_t score) {


            // Decrease average turn delay based on score ..

            switch (score) {

                case 0 ... Constants::turn_Delay_Score_0:
                    this->minTurnDelay = Constants::turn_Delay_Min_0;
                    this->maxTurnDelay = Constants::turn_Delay_Min_0 * 2;
                    break;

                case (Constants::turn_Delay_Score_0 + 1) ... Constants::turn_Delay_Score_1:
                    this->minTurnDelay = Constants::turn_Delay_Min_1;
                    this->maxTurnDelay = Constants::turn_Delay_Min_1 * 2;
                    break;

                case (Constants::turn_Delay_Score_1 + 1) ... Constants::turn_Delay_Score_2:
                    this->minTurnDelay = Constants::turn_Delay_Min_2;
                    this->maxTurnDelay = Constants::turn_Delay_Min_2 * 2;
                    break;

                default:
                    this->minTurnDelay = Constants::turn_Delay_Min_3;
                    this->maxTurnDelay = Constants::turn_Delay_Min_3 * 2;
                    break;
                    
            }

            switch (this->direction) {

                case Direction::None:

                    this->turnDelay--;

                    if (this->turnDelay == 0) {

                        switch (this->x) {

                            case 0:
                                this->direction = Direction::Right;
                                this->x++;
                                break;

                            case 24:
                                this->direction = Direction::Left;
                                this->x--;
                                break;
                            
                        }

                    }
                    break;

                case Direction::Left:
                    this->x--;
                    if (x == 0) {
                        this->direction = Direction::None;
                        this->turnDelay = random(this->minTurnDelay, this->maxTurnDelay);
                    }
                    break;

                case Direction::Right:
                    this->x++;
                    if (x == 24) {
                        this->direction = Direction::None;
                        this->turnDelay = random(this->minTurnDelay, this->maxTurnDelay);
                    }
                    break;

                default: break;

            }

        }

        int8_t getXDisplay() {

            return this->xPositions[this->direction == Direction::Right][x];

        }

        int8_t getXDisplayMap() {

            return this->xPositionsMap[x];

        }

        uint8_t getFrame() {

            return this->frames[this->direction == Direction::Right][x];

        }

        bool isCatching(Direction direction) {

            switch (direction) {
                
                case Direction::Left:
                    return x == 0;
                
                case Direction::Right:
                    return x == 24;

                default: break;
                    
            }

            return false;

        }

        void reset() {

            this->direction = Direction::Left;
            this->x = 8;
            this->turnDelay = 0;
            this->minTurnDelay = 0;
            this->maxTurnDelay = 0;

        }

};

