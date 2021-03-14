#pragma once

struct Building {

    BuildingType type = BuildingType::None;
    int16_t x = 0;
    uint8_t door = 0;

    void reset() {

        this->type = BuildingType::None;
    }

    bool incDoor() { // returns closing

        if (this->door > 0 && this->door < Constants::Door_Open_Length + 4) { 
            this->door++;
        }
        else {
            this->door = 0;
        }

        return this->door >= Constants::Door_Open_Length;

    }

    uint8_t getDoorFrame() {

        switch (this->door) {

            case 1 ... 4:                                                                 return this->door - 1;

            case 5 ... Constants::Door_Open_Length:                                       return 3;
            
            case Constants::Door_Open_Length + 1 ... Constants::Door_Open_Length + 4:     return Constants::Door_Open_Length + 4 - this->door;

            default:                                                                      return 0;

        }

    }

    bool getDoorOpen() {

        return this->door > 0 && this->door < Constants::Door_Open_Length;

    }

};


struct ReticleTarget {

    int16_t x = 0;
    uint8_t y = 0;
    uint8_t buildingIdx = 0;
    TargetType type = TargetType::None;

};

struct World {

    int16_t backgroundXPos = 30;
    int16_t buildingXPos = 0;
    int16_t foregroundXPos = 60;

    void reset() {

        this->backgroundXPos = 30;
        this->buildingXPos = 0;
        this->foregroundXPos = 60;

    }

    void inc(const bool frame) {

        if (frame) backgroundXPos++;
        foregroundXPos = foregroundXPos + 2;
        buildingXPos++;           

    }

    void dec(const bool frame) {

        if (frame) backgroundXPos--;
        foregroundXPos = foregroundXPos - 2;
        buildingXPos--;   

    }

};

struct StealthGameVars {
    
    World world;
    Guard guards[Constants::Max_Guards];
    Building buildings[BUILDING_COUNT];
    ReticleTarget reticleTargets[6];
    uint8_t questionCount = 0;
    uint8_t lampPoleIdx = 0;
    uint8_t reticleTargetCount = 0;
    uint8_t reticleTargetIdx = 0;
    int8_t moveOffset = 0;

    ReticleTarget &getReticleTarget(uint8_t idx) {

        return this->reticleTargets[idx];

    }

    void reset() {

        this->questionCount = 0;
        this->moveOffset = 0;
        this->world.reset();
        this->resetReticleTargets();

        for (uint8_t i = 0; i < BUILDING_COUNT; i++) {

            this->buildings[i].type = BuildingType::None;

        }

    }

    void resetReticleTargets() {

        for (uint8_t i = 0; i < 6; i++) {

            this->reticleTargets[i].type = TargetType::None;

        }

        this->reticleTargetCount = 0;

    }

};

struct MapGameVars {

    MapMenu menu = MapMenu::SafeHouse;
    MapMenu currentLocation = MapMenu::SafeHouse;
    MapMenu nextLocation = MapMenu::SafeHouse;
    uint8_t menu_Index = Constants::Map_Closed; 
    Message message = Message::Test;
    uint8_t counter = 0;
    uint8_t threatPrice = 50;

    bool shownMapIntroText = false;

    Message commercialDistrictMessage = Message::Test;
    Message slumsDistrictMessage = Message::Test;
    Message safeHouseMessage = Message::Test;
    Message hackingMessage = Message::Test;

    void setMessage(Message message) {
        this->message = message;
        this->counter = 0;
    }
    
    Message getMessage() {
        return this->message;
    }
};


struct TVSequenceVars {

    uint8_t counter = 0;
    TVMode mode = TVMode::TurnOn;

};

struct HackGameVars {

    Grid grid;
    Objectives objectives;
    SelectionMode selectionMode = SelectionMode::Grid;

    uint8_t availableHacks = 0;
    uint8_t installedHacks = 0;
    uint8_t failedHacks = 0;
    uint16_t timer = 0;
    uint8_t ledDelay = 0;

    bool timerStart = false;
    

    void renumber(Arduboy2Ext &arduboy) {

        for (uint16_t j = 0; j < 256; j++) {

            arduboy.sBuffer[j] = j;

        }

        for (uint16_t j = 0; j < 256; j++) {

            uint16_t x = random(0, 255);
            uint16_t y = random(0, 255);

            uint8_t tmp = arduboy.sBuffer[x];
            arduboy.sBuffer[x] = arduboy.sBuffer[y];
            arduboy.sBuffer[y] = tmp;

        }

        for (uint8_t y = 0; y < grid.getSize(); y++) {

            for (uint8_t x = 0; x < grid.getSize(); x++) {

                grid.setCell(x, y, arduboy.sBuffer[grid.getCell(x, y)]);

            }

        }
      
        for (uint8_t y = 0; y < objectives.getCount(); y++) {

            Objective &objective = objectives.getObjective(y);

            for (uint8_t x = 0; x < objective.getLength(); x++) {

                objective.setCell(x, arduboy.sBuffer[objective.getCell(x)]);
                
            }

        }  
        
    }

};

