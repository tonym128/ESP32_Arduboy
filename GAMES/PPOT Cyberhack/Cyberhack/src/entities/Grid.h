#pragma once

#include "../utils/Constants.h"
#include "Objectives.h"

struct Grid {

    private:

        uint8_t size;
        uint8_t x;
        uint8_t y;
        uint8_t target;
        uint8_t objX;
        uint8_t objY;
        uint8_t selectionCount;
        uint8_t bufferSize;
        uint8_t selections[Constants::Buffer_Max_Length];
        uint8_t grid[Constants::Grid_Max_Size][Constants::Grid_Max_Size];
        uint8_t grid_Selection[Constants::Grid_Max_Size][Constants::Grid_Max_Size];
        uint8_t grid_Backup[Constants::Grid_Max_Size][Constants::Grid_Max_Size];
        SelectionType selectionType = SelectionType::Cell;

    public:

        uint8_t getX()                                       { return this->x; }
        uint8_t getY()                                       { return this->y; }
        uint8_t getTarget()                                  { return this->target; }
        uint8_t getObjX()                                    { return this->objX; }
        uint8_t getObjY()                                    { return this->objY; }
        uint8_t getSize()                                    { return this->size; }
        uint8_t getSelectionCount()                          { return this->selectionCount; }
        uint8_t getBufferSize()                              { return this->bufferSize; }
        uint8_t *getSelections()                             { return this->selections; }
        SelectionType getSelectionType()                     { return this->selectionType; }
        uint8_t getCell(uint8_t x, uint8_t y)                { return this->grid[y][x]; }
        uint8_t getCell_Selection(uint8_t x, uint8_t y)      { return this->grid_Selection[y][x]; }

        void setX(uint8_t val)                               { this->x = val; }
        void setY(uint8_t val)                               { this->y = val; }
        void setTarget(uint8_t val)                          { this->target = val; }
        void setObjX(uint8_t val)                            { this->objX = val; }
        void setObjY(uint8_t val)                            { this->objY = val; }
        void setSize(uint8_t val)                            { this->size = val; }
        void setBufferSize(uint8_t val)                      { this->bufferSize = val; }
        void setSelectionType(SelectionType val)             { this->selectionType = val; }
        void setCell(uint8_t x, uint8_t y, uint8_t val)      { this->grid[y][x] = val; }

        void decX() { 
            
            if (this->x > 0) this->x--; 
            
        }
        
        void incX() { 
            
            if (this->x < this->size - 1) this->x++; 
            
        }

        uint8_t getSelection(uint8_t idx) { 

            return selections[idx];

        }


        void placeObjectives(Objectives objectives, uint8_t contiguousCount, uint8_t maxValue) {

            bool noErrors = false;
            bool row = false;

            while (!noErrors) {    

                this->clear();

                // Migrate contiguous entries ..
                
                uint8_t x = Constants::Grid_Blank;
                uint8_t y = Constants::Grid_Blank;
                
                for (uint8_t i = 0; i < contiguousCount; i++) {
                
                    Objective &obj = objectives.getObjective(i);

                    for (uint8_t j = obj.getOverlap(); j < obj.getLength(); j++) {
            
                        uint8_t number = obj.getCell(j);
                            

                        // Handle the first cell specifically.  It must be in the first row.

                        if (x == Constants::Grid_Blank || y == Constants::Grid_Blank) {
        
                            x = random(0, this->getSize());
                            y = 0;
                            this->setCell(x, y, number);
                            noErrors = true;
        
                        }


                        // Other cells.  Can be in any row / col ..

                        else {
                            
                            
                            // Build a sequence ..
                            
                            uint8_t cellIdx[Constants::Grid_Max_Size];
                            buildASequence(cellIdx);


                            // Find a new row / column ..
                            
                            if (row) {
                                
                                noErrors = false;
                                
                                for (uint8_t k = 0; k < this->getSize(); k++) {
        
                                    if (this->getCell(cellIdx[k], y) == Constants::Grid_Blank) {
                                        x = cellIdx[k];
                                        noErrors = true;
                                        break;
                                    }
                                    
                                }
                                
                            }
                            else {
        
                                noErrors = false;
                                
                                for (uint8_t k = 0; k < this->getSize(); k++) {
        
                                    if (this->getCell(x, cellIdx[k]) == Constants::Grid_Blank) {
                                        y = cellIdx[k];
                                        noErrors = true;
                                        break;
                                    }
                                    
                                }
        
                            }
                                
                            if (noErrors) {

                                this->setCell(x, y, number);
                                row = !row;

                            }
                        
                        }
                        
                        if (!noErrors) break;
                    
                    }
                            
                    if (!noErrors) break;
            
                }
            
            }
            
            
            // Place other objectives ..
            
            noErrors = false;
            this->backup();

            while (!noErrors && contiguousCount < objectives.getCount()) {

                this->restore();
            
                uint8_t x = Constants::Grid_Blank;
                uint8_t y = Constants::Grid_Blank;
                
                
                for (uint8_t i = contiguousCount; i < objectives.getCount(); i++) {

                    Objective &obj = objectives.getObjective(i);

                    for (uint8_t j = obj.getOverlap(); j < obj.getLength(); j++) {
            
                        uint8_t number = obj.getCell(j);
                        
                        if (x == Constants::Grid_Blank || y == Constants::Grid_Blank) {
        
                            while (true) {

                                x = random(0, this->getSize());
                                y = random(0, this->getSize());
                                
                                if (this->getCell(x, y) == Constants::Grid_Blank) {
                                    
                                    this->setCell(x, y, number);
                                    noErrors = true;
                                    break;

                                }
                                
                            }                            
        
                        }
                        else {
                            
                            
                            // Build a sequence ..
                            
                            uint8_t cellIdx[Constants::Grid_Max_Size];
                            buildASequence(cellIdx);


                            // Find a new row / column ..
                            
                            if (row) {
                                
                                noErrors = false;
                                
                                for (uint8_t k = 0; k < this->getSize(); k++) {
        
                                    if (this->getCell(cellIdx[k], y) == Constants::Grid_Blank) {
                                        x = cellIdx[k];
                                        noErrors = true;
                                        break;
                                    }
                                    
                                }
                                
                            }
                            else {
        
                                noErrors = false;
                                
                                for (uint8_t k = 0; k < this->getSize(); k++) {
        
                                    // printf("Attempting col %i,%i = %i, ", x, cellIdx[k],grid[cellIdx[k]][x]  );
                                    if (this->getCell(x, cellIdx[k]) == Constants::Grid_Blank) {
                                        y = cellIdx[k];
                                        noErrors = true;
                                        break;
                                    }
                                    
                                }
        
                            }
                                
                            if (noErrors) {
                                this->setCell(x, y, number);
                                row = !row;
                            }
                        
                        }
                            
                        if (!noErrors) break;
                    
                    }

                    if (!noErrors) break;
            
                }
                
            }


            // Randomly fill the 'empty' cells
                    
            for (uint8_t y = 0; y < this->getSize(); y++) {

                for (uint8_t x = 0; x < this->getSize(); x++) {

                    if (this->getCell(x, y) == Constants::Grid_Blank) {
                        this->setCell(x, y, random(0, maxValue));
                    }

                }

            }

        }

        void select() {
     
            this->selections[this->selectionCount] = this->grid[y][x];
            this->grid_Selection[y][x] = this->selectionCount;
            this->selectionCount++;

            switch (this->selectionType) {
                
                case SelectionType::Cell:
                    this->selectionType = SelectionType::Column;
                    break;
                
                case SelectionType::Column:
                    this->selectionType = SelectionType::Row;
                    break;
                
                case SelectionType::Row:
                    this->selectionType = SelectionType::Column;
                    break;

            }

        }

        void print() {
                
            #ifdef DEBUG 

                Serial.println("\nGrid\n---------");
                
                for (uint8_t j = 0; j < Constants::Grid_Max_Size; j++) {
                    
                    for (uint8_t i = 0; i < Constants::Grid_Max_Size; i++) {

                        Serial.print(grid[j][i]);
                        Serial.print(" ");
                        
                    }
                    
                    Serial.println("\n");

                }

            #endif
            
        }

        // Clear grid ..

        void clear() { 

            this->x = 0;
            this->y = 0;
            this->selectionCount = 0;
            this->selectionType = SelectionType::Cell;

            for (uint8_t i = 0; i < 6; i++) {

                this->selections[i] = Constants::Grid_Blank;

            }

            for (uint8_t y = 0; y < Constants::Grid_Max_Size; y++) {
                
                for (uint8_t x = 0; x < Constants::Grid_Max_Size; x++) {
                    
                    this->grid[y][x] = Constants::Grid_Blank;
                    this->grid_Selection[y][x] = Constants::Grid_Blank;
                    
                }
                
            }
            
        }


    private:

        void backup() {
                    
            for (uint8_t y = 0; y < Constants::Grid_Max_Size; y++) {
                
                for (uint8_t x = 0; x < Constants::Grid_Max_Size; x++) {
                    
                    this->grid_Backup[y][x] = this->grid[y][x];
                    
                }
                
            }
            
        }

        void restore() {
                    
            for (uint8_t y = 0; y < Constants::Grid_Max_Size; y++) {
                
                for (uint8_t x = 0; x < Constants::Grid_Max_Size; x++) {
                    
                    this->grid[y][x] = this->grid_Backup[y][x];
                    
                }
                
            }
            
        }

        // populates the cellIdx array with a randomly placed 0 ... 6 - takes into account the grid size.
        void buildASequence(uint8_t (&cellIdx)[Constants::Grid_Max_Size]) {
                                
            for (uint8_t i = 0; i < Constants::Grid_Max_Size; i++) {
                cellIdx[i] = i;
            }
                    
            for (uint8_t i = 0; i < Constants::Grid_Max_Size * 2; i++) {

                uint8_t r1 = random(0, this->getSize());
                uint8_t r2 = random(0, this->getSize());
                uint8_t temp = cellIdx[r1];
                cellIdx[r1] = cellIdx[r2];
                cellIdx[r2] = temp;

            }
            
        }        

};