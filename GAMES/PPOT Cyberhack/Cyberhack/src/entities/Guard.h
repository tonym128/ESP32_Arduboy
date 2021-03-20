#pragma once

#include "../utils/Constants.h"

struct Guard {

    private:

        Movement movement = Movement::IdleRight;
        int16_t x = 60;
        int16_t centre = 60;
        uint16_t span = 60;
        uint8_t steps = 60;
        uint8_t speed = 1;  // 0 - slowest, 4 Normal
        uint8_t frame = 0;
        uint8_t alertCount = 0;
        
        bool alerted = false;
        bool active = false;

    public:

        Movement getMovement()                              { return this->movement; }
        int16_t getX()                                      { return this->x; }
        uint16_t getSpan()                                  { return this->span; }
        uint8_t getSteps()                                  { return this->steps; }
        uint8_t getSpeed()                                  { return this->speed; }
        uint8_t getAlertCount()                             { return this->alertCount; }
        bool getAlerted()                                   { return this->alerted; }
        bool getActive()                                    { return this->active; }

        void setX(int16_t val)                              { this->x = val; this->centre = val; }
        void setSpan(uint16_t val)                          { this->span = val; }
        void setSteps(uint8_t val)                          { this->steps = val; }
        void setSpeed(uint8_t val)                          { this->speed = val; }
        void setActive(bool val)                            { this->active = val; }

        void setMovement(Movement val) { 
            
            this->movement = val; 
            
            if (this->movement == Movement::LookingUpRight || this->movement == Movement::LookingUpLeft) {
                this->alerted = false;
                this->alertCount = 0;
            }
        
        }

        void setAlerted(bool val) { 
            
            if (val) {
                if (!this->alerted) {
                    this->alertCount = 64;
                    this->setSteps(5);
                    if (this->speed < 4) this->speed++;
                }
            }
            else {
                this->alertCount = 0;
            }

            this->alerted = val; 
            
        }

        void incX() {  
            this->x++;  
        }

        void decX() {  
            this->x--;  
        }

        void reset() {

            this->alerted = false;
            this->active = false;

        }

        void move(int16_t playerPosRel) {

            if (this->isDazedAndConfused() && this->steps > 0) {

                this->steps--;
                return;

            } 


            // Otherwise move the guard ..

            if (this->alertCount > 0)       alertCount--;

            if (this->steps > 0) {

                this->frame++;

                
                switch (this->speed) {

                    case 0:
                        if (this->frame != 2) {
                            this->moveGuard();
                        }
                        else {
                            this->frame = 0;
                        }
                        break;

                    case 1:
                        if (this->frame != 3) {
                            this->moveGuard();
                        }
                        else {
                            this->frame = 0;
                        }
                        break;

                    case 2:
                        if (this->frame != 4) {
                            this->moveGuard();
                        }
                        else {
                            this->frame = 0;
                        }
                        break;

                    case 3:
                        if (this->frame != 5) {
                            this->moveGuard();
                        }
                        else {
                            this->frame = 0;
                        }
                        break;

                    case 4:
                        this->moveGuard();
                        break;

                }

                this->steps--;

            }
            else {


                // If the guard is alert, he should continue forward (and not turn around) ..

                if (this->alerted) {

                    if (playerPosRel <= 0) {

                        this->setMovement(Movement::WalkingLeft);
                        this->setSteps(5);

                    }
                    else {

                        this->setMovement(Movement::WalkingRight);
                        this->setSteps(5);

                    }

                }
                else {

                    uint16_t randomSpan = abs(this->x - this->centre) + (this->span / 2);
                    this->alerted = false;

                    switch (this->getMovement()) {

                        case Movement::WalkingLeft:
                            this->setMovement(Movement::IdleLeft);
                            this->setSteps(random(15, 30));
                            this->frame = 0;
                            break;

                        case Movement::IdleLeft:
                        case Movement::LookingUpLeft:
                            this->setMovement(Movement::WalkingRight);
                            this->setSteps(random(randomSpan / 2, randomSpan));
                            this->setSpeed(random(0, 4));
                            this->frame = 0;
                            break;

                        case Movement::WalkingRight:
                            this->setMovement(Movement::IdleRight);
                            this->setSteps(random(15, 30));
                            this->frame = 0;
                            break;

                        case Movement::IdleRight:
                        case Movement::LookingUpRight:
                            this->setMovement(Movement::WalkingLeft);
                            this->setSteps(random(randomSpan / 2, randomSpan));
                            this->setSpeed(random(0, 4));
                            this->frame = 0;
                            break;

                        default: break;

                    }

                }

            }

        }

        void moveGuard() {

            switch (this->movement) {

                case Movement::WalkingLeft:
                    this->x--;
                    break;

                case Movement::WalkingRight:
                    this->x++;
                    break;

                default:
                    break;
                    
            }

        }

        bool isFacingLeft() {

            return (this->movement == Movement::IdleLeft || this->movement == Movement::WalkingLeft);
            
        }

        bool isFacingRight() {

            return (this->movement == Movement::IdleRight || this->movement == Movement::WalkingRight);
            
        }

        bool isDazedAndConfused() {

            return (this->movement == Movement::LookingUpLeft || this->movement == Movement::LookingUpRight);
            
        }

};
