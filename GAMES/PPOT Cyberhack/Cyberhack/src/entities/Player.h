#pragma once

#include "../utils/Constants.h"

struct Player {

    private:

        const uint16_t deckPrice[4]         = { Constants::Deck_Price_L0, Constants::Deck_Price_L1, Constants::Deck_Price_L2, Constants::Deck_Price_L3 };
        const uint8_t hackSuccess_Max[4]    = { Constants::Hack_Max_Success_L0, Constants::Hack_Max_Success_L1, Constants::Hack_Max_Success_L2, Constants::Hack_Max_Success_L3 };


        Movement movement = Movement::IdleRight;
        uint16_t dollars = 0;
        uint8_t health = 192;
        uint8_t memory = 6;
        uint8_t threatLevel = 0;
        int16_t x = 60;

        int16_t xReticle = 60;
        int16_t yReticle = 10;
        bool showReticle = false;
        bool deck[4] = { true, false, false, false };
        uint8_t deckIndex = 0;
        uint8_t hackSuccess[4] = { 0, 0, 0, 0 };
        uint8_t hackIndex = 0;

    public:

        Movement getMovement()                              { return this->movement; }
        uint16_t getDollars()                               { return this->dollars; }
        uint8_t getHealth()                                 { return this->health; }
        uint8_t getThreatLevel()                            { return this->threatLevel; }
        uint8_t getMemory()                                 { return this->memory; }
        int16_t getX()                                      { return this->x; }
        bool getDeck(uint8_t i)                             { return this->deck[i]; }
        uint16_t getDeckPrice(uint8_t i)                    { return this->deckPrice[i]; }
        uint16_t getHackSuccess(uint8_t i)                  { return this->hackSuccess[i]; }
        uint16_t getHackSuccess_Max(uint8_t i)              { return this->hackSuccess_Max[i]; }
        uint8_t getDeckIndex()                              { return this->deckIndex; }
        uint8_t getHackIndex()                              { return this->hackIndex; }

        int16_t getXReticle()                              { return this->xReticle; }
        int16_t getYReticle()                              { return this->yReticle; }
        bool getShowReticle()                              { return this->showReticle; }

        void setMovement(Movement val)                      { this->movement = val; }
        void setDollars(uint16_t val)                       { this->dollars = val; }
        void setHealth(uint8_t val)                         { this->health = val; }
        void setMemory(uint8_t val)                         { this->memory = val; }
        void setX(int16_t val)                              { this->x = val; }
        void setDeck(uint8_t i, bool val)                   { this->deck[i] = val; }

        void setXReticle(int16_t val)                      { this->xReticle = val; }
        void setYReticle(int16_t val)                      { this->yReticle = val; }
        void setShowReticle(bool val)                      { this->showReticle = val; }
        void setDeckIndex(uint8_t val)                      { this->deckIndex = val; }


        void setThreatLevel(uint8_t val) { 
            this->threatLevel = val; 
            #ifdef BYTE_BEAT_SOUNDS
                #ifdef BYTE_BEAT_SOUNDS_1
                    hpISR = 30 + (this->threatLevel >> 2);
                #endif
            #endif
        }

        void resetStealth() {
            this->x = 60;
            this->health = 192;
            this->memory = 6;
        }

        void reset() {

            for (uint8_t i = 0; i < 4; i++) {
                this->deck[i] = (i == 0);
                this->hackSuccess[i] = 0;
            }

            this->dollars = 0;
            this->setThreatLevel(0);

        }

        void incX() {  
            this->x++;  
        }

        void decX() {  
            this->x--;  
        }

        void decHealth() {  
            if (this->health > 0) this->health--;  
        }

        void incHealth() {  
            if (this->health < 192) this->health++;  
        }

        void decMemory(uint8_t val) {
            this->memory = this->memory - val;
        }

        bool incMemory() {
            if (this->memory < 6) this->memory++;
            return this->memory == 6;
        }

        void incThreatLevel(uint8_t val) {  

            this->setThreatLevel(this->getThreatLevel() + val);

        }
        
        void decThreatLevel(uint16_t val) {  
            if (this->threatLevel > val) {
                this->setThreatLevel(this->getThreatLevel() - val);
            }
            else {
                this->setThreatLevel(0);
            }
        }

        void decDollars(uint16_t val) {  
            if (this->dollars > val) {
                this->dollars = this->dollars - val;
            }
            else {
                this->dollars = 0;
            }
        }

        uint8_t getFirstAvailDeck(bool returnNonIndex) {

            for (uint8_t i = 1; i < Constants::Max_Deck_Count; i++) {
            
                if (!this->deck[i]) return i;

            }

            return (returnNonIndex ? Constants::No_Deck_Avail : 1);

        }

        uint8_t getAvailDeckCount() {

            uint8_t count = 0;

            for (uint8_t i = 0; i < Constants::Max_Deck_Count; i++) {
            
                if (!this->deck[i]) count++;

            }

            return count;

        }

        uint8_t getDeckIndex_ForDisplay() {

            uint8_t count = 0;

            for (uint8_t i = 0; i < this->deckIndex; i++) {
            
                if (this->deck[i]) count++;

            }

            return count;

        }

        void decDeckIndex() {

            for (uint8_t i = this->deckIndex; i > 0; i--) {
            
                if (this->deck[i - 1]) {
                    this->deckIndex = i - 1;
                    break;
                }

            }

        }

        void incDeckIndex() {

            for (uint8_t i = this->deckIndex + 1; i < Constants::Max_Deck_Count; i++) {
            
                if (this->deck[i]) {
                    this->deckIndex = i;
                    break;
                }

            }

        }

        uint8_t getRandomPuzzle(bool anyDeck) {

            bool otherDeck = false;
            bool notAllDecksOwned = false;

            if (hackSuccess[deckIndex] < hackSuccess_Max[deckIndex]) {

                this->hackIndex = (this->deckIndex * 5) + random(0, 5);
                return this->hackIndex;
            
            }
            else {

                for (uint8_t x = Constants::Max_Deck_Count; x > 0; x--) {

                    if (this->deck[x - 1]) { // If the player owns the deck ..

                        if (hackSuccess[x - 1] < hackSuccess_Max[x - 1]) {

                            if (anyDeck || (!anyDeck && x - 1 < deckIndex)) {
                                this->hackIndex = ((x - 1) * 5) + random(0, 5);
                                return this->hackIndex;
                            }

                            otherDeck = true;
                        
                        }

                    }
                    else {

                        notAllDecksOwned = true;
                    }

                }

            }

            if (otherDeck || notAllDecksOwned) {

                this->hackIndex = Constants::Hack_None_Available;
                return this->hackIndex;

            }
            else {

                this->hackIndex = Constants::Hack_Game_Over;
                return this->hackIndex;

            }

        } 

        void incHackSuccess() {

            this->hackSuccess[this->hackIndex / 5]++;

        }

        bool canPurchaseADeck() {

            for (uint8_t i = 1; i < 4; i++) {

                if (!this->deck[i] && this->dollars >= deckPrice[i]) {
                    return true;

                }

            }

            return false;

        }

        bool isHiddenOrCrouching() {

            switch (this->movement) {

                case Movement::Hidden:
                case Movement::CrouchRight:
                case Movement::CrouchLeft:
                    return true;

                default: return false;

            }

        }

};
