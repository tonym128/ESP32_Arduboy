#pragma once

#include <Arduboy2.h>
#include "../images/Images.h"
#include "../utils/Consts.h"

struct Lion {

    // Images                               0 = LH0, 1 = LH1, 2 = LHA, 3 = RH0, 4 = RH1, 5 = RHA, 6 = FWD, 7 = BWD
    //                                     AL  AD  AU  AT  OC  AT   4   3   2   1     C     1   2   3   4  AT  OC  AT  AU  AD  AL
    const uint8_t xPositions[3][21] =   {{ 12, 12, 12, 36, 38, 40, 42, 44, 46, 48,   52,   56, 58, 60, 62, 64, 66, 90, 90, 90, 90 },
                                         {  8,  8,  8, 30, 33, 36, 39, 42, 45, 48,   52,   56, 59, 62, 67, 68, 71, 74, 94, 94, 94 },
                                         {  4,  4,  4, 20, 26, 32, 36, 40, 44, 48,   52,   56, 60, 64, 68, 72, 76, 80, 97, 97, 97 }};
    const uint8_t leftImages[21] =      {   2,  6,  7,  2,  2,  2,  0,  1,  0,  1,    0,    1,  0,  1,  0,  1,  0,  0,  0,  0, 0 };
    const uint8_t rightImages[21] =     {   0,  0,  0,  0,  4,  3,  4,  3,  4,  3,    4,    3,  4,  3,  4,  5,  5,  5,  7,  6, 5 };

    private:

        XPosition x = XPosition::Centre;
        YPosition y = YPosition::Level_1;
        Direction direction = Direction::Left;
        Direction nextDirection = Direction::Left;
        uint8_t speed = 8;
        uint8_t steps = 0;
        uint8_t stepsOrig = 0;
        uint8_t index = 0;

        bool running = false;
        int8_t xRunning = 0;
        int8_t yRunning = 0;
        uint8_t runningCounter = 0;
        uint8_t runningDelay = 0;

    public:

        XPosition getXPosition()                    { return this->x; }
        YPosition getYPosition()                    { return this->y; }
        Direction getDirection()                    { return this->direction; }
        Direction getNextDirection()                { return this->nextDirection; }
        uint8_t getSpeed()                          { return this->speed; }
        uint8_t getSteps()                          { return this->steps; }
        uint8_t getIndex()                          { return this->index; }
        bool getRunning()                           { return this->running; }
        bool getRunningDelay()                      { return this->runningDelay; }

        void setXPosition(XPosition val)            { this->x = val; }
        void setYPosition(YPosition val)            { this->y = val; }
        void setDirection(Direction val)            { this->direction = val; this->setSpeed(random(6, 11)); }
        void setNextDirection(Direction val)        { this->nextDirection = val; }
        void setSpeed(uint8_t val)                  { this->speed = val; }
        void setSteps(uint8_t val)                  { this->steps = val; this->stepsOrig = val; }
        void setIndex(uint8_t val)                  { this->index = val; }
        void setRunningDelay(uint8_t val)           { this->runningDelay = val; }

    public:

        void decXPosition()                         { this->x--; }
        void incXPosition()                         { this->x++; }
        void decYPosition()                         { if (this->y != YPosition::Level_1) this->y--; }
        void incYPosition()                         { if (this->y != YPosition::Level_3) this->y++; }

        void setRunning(bool val, bool includeDelay) { 
            
            this->xRunning = this->getXDisplay(); 
            this->yRunning = this->getYDisplay(); 
            this->runningCounter = 0; 
            this->running = val; 
            this->runningDelay = includeDelay ? 16 : 0;  
            
        }

        int8_t getXDisplay() { 
        
            if (!this->getRunning()) {
                return this->xPositions[static_cast<uint8_t>(this->y)][static_cast<uint8_t>(this->x)]; 
            }
            else {
                return this->xRunning;
            }
            
        }
        
        bool decSteps() { 

            this->steps--; 

            if (this->steps % 3 == 0) {

                this->stepsOrig = this->stepsOrig - 3;

                switch (this->direction) {

                    case Direction::Up:
                        this->y--;
                        break;

                    case Direction::Down:
                        this->y++;
                        break;

                    default: break;
                
                }

            }

            if (this->steps == 0) {
                this->stepsOrig = 0;
                this->direction = this->nextDirection;                
            }

            return (this->steps == 0);

        }
        
        int8_t getYDisplay() { 

            const int8_t yPositions[] = { -4, 16, 37 };            
            
            if (!this->getRunning()) {

                int8_t yPos = yPositions[static_cast<uint8_t>(this->y)]; 

                switch (this->direction) {

                    case Direction::Up:
                        yPos = yPos - ((this->stepsOrig - this->steps) * 7);
                        return yPos;

                    case Direction::Down:
                        yPos = yPos + ((this->stepsOrig - this->steps) * 7);
                        return yPos;

                    default:
                        return yPos;

                }

            }
            else {

                return this->yRunning;

            }
        
        }

        uint8_t getFrame() {

            switch (this->direction) {

                case Direction::Left:
                    return this->leftImages[static_cast<uint8_t>(this->x)];

                case Direction::Right:
                    return this->rightImages[static_cast<uint8_t>(this->x)];

                case Direction::Up:
                    return 7;

                case Direction::Down:
                    return 6;

                default: return 0;

            }

        }

        bool isMovingUpDown() {

            return this->direction == Direction::Up || this->direction == Direction::Down;

        }

        bool isAttacking() {

            return (this->x < XPosition::LH_Attacking || this->x > XPosition::RH_Attacking);

        }

        void reset(Direction direction, YPosition yPosition, uint8_t speed, uint8_t index) {

            this->setXPosition(XPosition::Centre);
            this->setSteps(0);
            this->setDirection(direction);
            this->setYPosition(yPosition);
            this->setIndex(index);   
            this->setSpeed(speed);
            this->setRunning(false, false);
            this->setRunningDelay(0);

        }

        void updateRunning() {

            if (!this->getRunning()) return;

            if (this->getRunningDelay() > 0) {

                this->runningDelay--;
                return;
            }

            switch (this->getXPosition()) {

                case XPosition::LH_Attacking_Left:
                    this->xRunning--;
                    break;

                case XPosition::LH_Attacking_Down:
                    this->yRunning++;
                    if (this->runningCounter == 6) this->xRunning--;
                    break;

                case XPosition::LH_Attacking_Up:
                    this->yRunning--;
                    if (this->runningCounter == 6) this->xRunning++;
                    break;

                case XPosition::RH_Attacking_Left:
                    this->yRunning++;
                    break;

                case XPosition::RH_Attacking_Down:
                    this->yRunning++;
                    if (this->runningCounter == 6) this->xRunning++;
                    break;

                case XPosition::RH_Attacking_Up:
                    this->yRunning--;
                    if (this->runningCounter == 6) this->xRunning--;
                    break;

                default: break;

            }

            runningCounter++;
            if (runningCounter == 7) runningCounter = 0;

        }

};