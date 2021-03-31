#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../entities/Entities.h"
#include "BaseState.h"
#include <ArduboyTones.h>

extern ArduboyTones sound;

class PlayGameState : public BaseState { //GameState<GameContext, GameStateType> {

  struct Limit {
  
    uint8_t min;
    uint8_t max;
    uint8_t counter;
    uint8_t upperLimit;
  
  };

  struct Recipient {

    uint8_t x = 110;
    Limit visible; 
    Limit reappear; 

  };

  private:
 
    Player player;
    Turtle turtles[5];
    Fish fishes[5];

    Recipient recipient;
    Limit fishLaunch; 
    Limit fishSpeed; 
    Limit stickHeadUp;

    uint8_t bobWater = 0;
    uint8_t prevTurtleIndex = 0;
    uint8_t flashArrow = 0;
    
    bool newPackage = false;
    bool playing = false;

  public:

    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

  private:

    uint8_t getDisabledFish(StateMachine &machine);

};