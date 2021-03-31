#pragma once

#include <Arduboy2.h>
#include "../images/Images.h"
#include "../utils/Consts.h"

struct Player {

    private:

        const uint8_t xPositions[6]= { 13, 9, 5, 91, 95, 99 };

        uint8_t index = 0;
        YPosition y = YPosition::Level_1;
        bool running = false;

        int8_t xRunning = 0;
        int8_t yRunning = 0;
        uint8_t runningCounter = 0;
        XPosition xPosition = XPosition::Centre;


    public:

        YPosition getYPosition()                    { return this->y; }
        uint8_t getIndex()                          { return this->index; }
        XPosition getXPosition()                    { return this->xPosition; }
        bool getRunning()                           { return this->running; }

        void setY(YPosition val)                    { this->y = val; }

    public:

        void decYPosition()                         { if (this->y != YPosition::Level_1) this->y--; }
        void incYPosition()                         { if (this->y != YPosition::Level_3) this->y++; }

        void setRunning(bool val, XPosition xPosition) { 
            
            this->xRunning = this->getXDisplay(); 
            this->yRunning = this->getYDisplay(); 
            this->runningCounter = 0; 
            this->running = val;  
            this->xPosition = xPosition;
        
        }

        int8_t getYDisplay() { 

            if (!this->getRunning()) {
            
                return static_cast<uint8_t>(this->y) * 21; 
                
            }
            else {

                return this->yRunning;

            }

        }

        uint8_t getXDisplay() { 

            if (!this->getRunning()) {
                    
                switch (this->index) {

                    case Constants::Player1_Index:
                        return xPositions[static_cast<uint8_t>(this->y)];

                    case Constants::Player2_Index:
                        return xPositions[static_cast<uint8_t>(this->y) + 3];

                }
            
            }
            else {

                return this->xRunning;
                
            }

        }

        void reset(uint8_t index,  YPosition y) {

            this->index = index;
            this->y = y;
            this->running = false;
            this->xPosition = XPosition::Centre;

        }


        void updateRunning() {

            if (!this->getRunning()) return;

            switch (this->xPosition) {

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

