#pragma once

#include "../entities/Coin.h"
#include "../entities/OtherCar.h"
#include "../entities/Ambulance.h"
#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../images/Images.h"
#include "BaseState.h"


class RaceState : public BaseState {

  private:

    Ambulance ambulance;
    Coin coin;

    int8_t xScenery = 0;
    int16_t xHospital = 0;
    int8_t xGrass = 0;
    int8_t xLine1 = 0;
    int8_t xLine2 = 0;
    int16_t distance = 0;
    uint8_t slowDown = 0;
    uint8_t coin_index = 0;

    uint8_t healthShowCountdown = 0;
    uint8_t deathCountdown = 0;
    uint8_t ledCountdown = 0;

    uint16_t carLaunch_RandomMax;
    uint16_t carLaunch_RandomMin;

    OtherCar otherCars[3];
    
    bool showHospital = false;

    bool checkLaunchCollisions(int16_t x, uint8_t lane);
    uint8_t checkForCollisions(Arduboy2Ext & arduboy, int16_t x, uint8_t y);
    bool checkForCollision(Arduboy2Ext & arduboy, int16_t ambulanceX, uint8_t ambulanceY, int16_t carX, uint8_t carLane);
    void updateSceneryPositions(Arduboy2Ext & arduboy);
    void decHealth(StateMachine & machine);
    void checkRoadExtents();


  public:	
  
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

};

