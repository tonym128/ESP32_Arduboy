#pragma once

#include "../utils/Constants.h"

struct Objective {

    private:

        uint8_t overlap;
        uint8_t length;
        uint8_t cells[Constants::Objective_Max_Length];

    public:

        uint8_t getOverlap()                        { return this->overlap; }
        uint8_t getLength()                         { return this->length; }
        uint8_t getCell(uint8_t idx)                { return this->cells[idx]; }
        uint8_t *getCells()                         { return this->cells; }

        void setOverlap(uint8_t val)                { this->overlap = val; }
        void setLength(uint8_t val)                 { this->length = val; }
        void setCell(uint8_t idx, uint8_t val)      { this->cells[idx] = val; }


    public:

        void clear() {

            this->overlap = 0;
            this->length = 0;
            
            for (uint8_t i = 0; i < Constants::Objective_Max_Length; i++) {
                cells[i] = Constants::Grid_Blank;
            }
        
        }
        
        void clone(Objective temp) {

            this->overlap = temp.getOverlap();
            this->length = temp.getLength();
            
            for (uint8_t i = 0; i < Constants::Objective_Max_Length; i++) {
                cells[i] = temp.getCell(i);
            }

        } 

        uint8_t getScore() {

            switch (this->length) {

                case 2:     return Constants::Score_Length_2;
                case 3:     return Constants::Score_Length_3;
                case 4:     return Constants::Score_Length_4;
                case 5:     return Constants::Score_Length_5;
                default:    return 0;

            }
        }

};
