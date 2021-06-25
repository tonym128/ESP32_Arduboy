#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../images/Images.h"
#include "BaseState.h"

class TitleScreenState : public BaseState {

  private:

    uint8_t restart = 0;
    uint8_t pressACounter = 0;
    uint8_t barrelPos = 0;
    uint8_t barrelRot_LHS = 0;
    uint8_t barrelRot_RHS = 0;
    

  public:	
  
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

};

