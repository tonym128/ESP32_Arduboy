#pragma once

#include <Arduboy2.h>
#include <ArduboyTones.h>
#include "../images/Images.h"
#include "../utils/Consts.h"

struct Oil {

    const uint8_t xDisplay[3] =            { 29, 56, 87 };
    const uint8_t xDisplay_Map[3] =        { 51, 59, 76 };
    const XPosition xPositions[3] =        { XPosition::Position_5_Oil, XPosition::Position_10_Oil, XPosition::Position_15_Oil };

    const uint8_t yPositions[16] =         {  7, 7, 7, 7, 11, 15,
                                             19, 23, 27, 31,
                                             35, 39, 43, 47, 51, 0};

    const uint8_t yPositions_Map_01[16] =  { 18, 18, 18, 18, 19, 20,
                                             21, 22, 23, 24,
                                             25, 26, 27, 28, 29, 0};

    const uint8_t yPositions_Map_2[16] =   { 18, 18, 19, 20, 21, 22,
                                             23, 24, 25, 26,
                                             27, 28, 29, 30, 31, 0};

    private:

        uint8_t x = 0;
        YPosition yPosition = YPosition::None;

    public:

        uint8_t getX()                              { return this->x; }
        YPosition getYPosition()                    { return this->yPosition; }

        void setX(uint8_t var)                      { this->x = var; }
        void setYPosition(YPosition var)            { this->yPosition = var; }

        XPosition getXPosition() { 
            
            return this->xPositions[this->x];
            
        }

        void incYPosition() {

            if (yPosition < YPosition::None) {
                this->yPosition++;
            }

        }

        uint8_t getXDisplay() {

            return this->xDisplay[this->x];

        }

        uint8_t getYDisplay() {

            return this->yPositions[static_cast<uint8_t>(this->yPosition)];

        }

        uint8_t getXDisplayMap() {

            return this->xDisplay_Map[this->x];

        }

        uint8_t getYDisplayMap() {

            switch (this->x) {

                case 0 ... 1:

                    if (this->yPosition < YPosition::Falling_12) {
                        return this->yPositions_Map_01[static_cast<uint8_t>(this->yPosition)];
                    }
                    break;

                case 2:
                    return this->yPositions_Map_2[static_cast<uint8_t>(this->yPosition)];
                    
            }

            return 255;

        }

        #ifndef SOUNDS    
        bool update() {
        #else
        bool update(ArduboyTones &sound) {
        #endif

            if (this->yPosition < YPosition::None) {

                this->yPosition++;
                
                if (this->yPosition < YPosition::Fire_00) {

                    #ifdef SOUNDS    
                    sound.tone(NOTE_A3, 20);
                    #endif

                }

            }

            return this->yPosition == YPosition::Fire_00;

        }

        void reset() {

            this->x = 0;
            this->yPosition = YPosition::None;

        }

};