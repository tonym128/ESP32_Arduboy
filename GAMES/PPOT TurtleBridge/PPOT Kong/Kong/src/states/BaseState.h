#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"

class BaseState : public GameState<GameContext, GameStateType> {

  protected:

    void renderScore(StateMachine & machine, int16_t health);
    void renderGameOverOrPause(StateMachine & machine);
    void handleCommonButtons(StateMachine & machine);
    bool getPaused();
    void setPaused(bool value);

	private:

    bool paused = false;
    bool seaweed = true; 
    uint8_t backgroundX = 0;
    int8_t waterLevel[12];

};