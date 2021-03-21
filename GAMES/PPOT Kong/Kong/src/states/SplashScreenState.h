#pragma once

#include "BaseState.h"
#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"

class SplashScreenState : public BaseState {

  private:
    uint8_t counter = 0;

  public:
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

};