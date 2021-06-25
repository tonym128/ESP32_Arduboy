#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../entities/Victim.h"
#include "../entities/Player.h"
#include "BaseState.h"

class GameIntroState : public BaseState { //GameState<GameContext, GameStateType> {

  private:

    uint8_t xAmbulance = 156;
    uint8_t xPlayer = 0;
    uint8_t counter = 0;
    uint8_t speedInc = 0;    
    uint8_t ledBrightness = 0;    
    
    bool ambulanceDoor = false;
    bool playerImageIndex = false;

  public:
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

};