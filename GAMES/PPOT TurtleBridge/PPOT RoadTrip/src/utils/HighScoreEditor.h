#pragma once

#include "Arduboy2Ext.h"
#include "Constants.h"

struct HighScore {

    private:
   
        uint8_t slotNumber;
        uint8_t charIndex;
        uint8_t chars[3];

    public: 

        HighScore() {

            this->slotNumber = Constants::DoNotEditSlot;
            this->charIndex = 0;
            this->chars[0] = 0;
            this->chars[1] = 0;
            this->chars[2] = 0;

        };

        uint8_t getSlotNumber()                    { return this->slotNumber; }
        uint8_t getCharIndex()                     { return this->charIndex; }
        uint8_t getChar(uint8_t idx)               { return this->chars[idx]; }

        void setSlotNumber(uint8_t val)            { this->slotNumber = val; }
        void setCharIndex(uint8_t val)             { this->charIndex = val; }
        void setChar(uint8_t idx, uint8_t val)     { this->chars[idx] = val; }


        void incCharIndex() {
            if (this->charIndex < 2) ++this->charIndex;
        }

        void decCharIndex() {
            if (this->charIndex > 0) --this->charIndex;
        }

        void incChar(uint8_t idx) {  
            if (this->chars[idx] == 25) {
                this->chars[idx] = 0;
            }
            else {
                ++this->chars[idx]; 
            }
        }

        void decChar(uint8_t idx) {
    
            if (this->chars[idx] == 0) {
                this->chars[idx] = 25;
            }
            else {
                --this->chars[idx]; 
            }
    
        }

        void disableEditting() {
            this->slotNumber = Constants::DoNotEditSlot;
        }

        void reset() {

            this->slotNumber = Constants::DoNotEditSlot;
            this->charIndex = 0;
            this->chars[0] = 0;
            this->chars[1] = 0;
            this->chars[2] = 0;

        }

};
