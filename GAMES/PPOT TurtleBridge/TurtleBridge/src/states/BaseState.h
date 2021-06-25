#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"

class BaseState : public GameState<GameContext, GameStateType> {

  protected:

    void renderScore(StateMachine & machine, bool renderHealth, int16_t health);
    void renderCommonScenery(StateMachine & machine);
    void renderGameOverOrPause(StateMachine & machine);
    void handleCommonButtons(StateMachine & machine);
    bool getPaused();
    void setPaused(bool value);

    void initWater();
    void updateWater(StateMachine & machine);
    void renderWater();

	private:

    bool paused = false;
    bool seaweed = true; 
    uint8_t backgroundX = 0;
    int8_t waterLevel[12];

};