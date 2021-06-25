#pragma once

#include "../utils/Constants.h"
#include "Objective.h"

struct Objectives {

    private:

        uint8_t count;
        Objective objectives[Constants::Objectives_Max_Number];

    public:

        uint8_t getCount()                                        { return this->count; }
        Objective & getObjective(uint8_t idx)                     { return this->objectives[idx]; }

    public:

        #ifdef SHUFFLE_RANDOM
        void shuffle() {

            for (uint8_t j = 0; j < this->count * 2; j++) {

                uint8_t idx1 = random(1, this->count);
                uint8_t idx2 = random(1, this->count);

                if (idx1 != idx2) {
                    
                    Objective o;
                    o.clone(this->objectives[idx1]);
                    objectives[idx1].clone(this->objectives[idx2]);
                    objectives[idx2].clone(o);

                }

            }
            
        }
        #endif

        #ifdef SHUFFLE_BY_SIZE

        void shuffle() {

            for (uint8_t i = 0; i < this->count - 1; i++) {

                for (uint8_t j = i + 1; j < this->count; j++) {

                    if(objectives[j].getLength() < objectives[i].getLength()) {

                        Objective o;
                        o.clone(this->objectives[i]);
                        objectives[i].clone(this->objectives[j]);
                        objectives[j].clone(o);
                        
                    }

                }

            }

        }

        #endif    
        void clear() {
            
            this->count = 0;
            
            for (uint8_t i = 0; i < Constants::Objectives_Max_Number; i++) {
                objectives[i].clear();
            }
            
        }

        void generate(uint8_t objectiveId, uint8_t objectiveLength, uint8_t maxValue, uint8_t templateObjectiveId, uint8_t templateStartIdx) {
            
            Objective tempObjective;
            uint8_t randomIdx = 0;
            
            bool hackComplete = false;
            

            // Zero out the temp variable ..
              
            tempObjective.clear();
            
            
            // Copy any template numbers across ..
            
            if (templateObjectiveId != NO_HACK_ID) {
            
                for (uint8_t i = templateStartIdx; i < Constants::Objective_Max_Length; i++) {
                    
                    if (objectives[templateObjectiveId].getCell(i) != Constants::Grid_Blank) {

                        tempObjective.setCell(randomIdx, this->objectives[templateObjectiveId].getCell(i));
                        randomIdx++;
                    
                    }
                    
                }
                
            }

            if (objectiveLength == randomIdx) hackComplete = true;


            // Generate a new hack ..    
            
            while (!hackComplete) {
                
                
                // Assign random numbers ..
                
                for (uint8_t i = randomIdx; i < objectiveLength; i++) {
                    
                    tempObjective.setCell(i, random(1, maxValue + 1));

                }
       
                
                // Is the hack unique?
                
                for (uint8_t i = 0; i < Constants::Objectives_Max_Number; i++) {
                
                    bool unique = false;
                    
                    if (i == objectiveId) continue;
                    
                    for (uint8_t j = 0; j < Constants::Objective_Max_Length; j++) {
                        
                        if (tempObjective.getCell(j) != this->objectives[objectiveId].getCell(j)) {
                            unique = true;
                            break;
                        }
                        
                    }
                    
                    if (!unique) {

                        hackComplete = false;
                        break;

                    }
                    else {//if (i == Constants::Objectives_Max_Number - 1) {

                        hackComplete = true;
                        
                    }
                    
                }

            }
            
            
            // Copy hack to list ..
            
            for (uint8_t j = 0; j < Constants::Objective_Max_Length; j++) {
                
                this->objectives[objectiveId].setCell(j, tempObjective.getCell(j));
                
            }

            this->objectives[objectiveId].setOverlap(randomIdx);
            this->objectives[objectiveId].setLength(objectiveLength);
            this->count++;
            
        }


        void print() {
            
            #ifdef DEBUG
                
                Serial.println("\nObjectives\n----------------------");
                
                for (uint8_t j = 0; j < this->count; j++) {
                    
                    Objective &obj = objectives[j];
                                
                    for (uint8_t i = 0; i < Constants::Objective_Max_Length; i++) {
                        
                        Serial.print(obj.getCell(i));
                        Serial.print(" ");
                        
                    }
                    
                    Serial.print(": ");
                    Serial.println(obj.getOverlap());

                }

            #endif

        }

};