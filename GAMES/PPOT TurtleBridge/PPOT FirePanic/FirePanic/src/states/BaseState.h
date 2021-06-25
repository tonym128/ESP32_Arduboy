#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"

class BaseState : public GameState<GameContext, GameStateType> {

  protected:

    void renderScore(StateMachine & machine, bool renderHealth, int16_t health);
    void renderMisses(StateMachine & machine, bool renderLatest);
    void renderCommonScenery(StateMachine & machine, bool incSmoke, bool incRHSBuilding);
    void renderLowerGrass();
    void renderAmbulance(StateMachine & machine, int8_t x, int8_t y, bool doorOpen);
    void renderPuff(int8_t x, int8_t y, uint8_t puffIndex, uint8_t puffIndex_Mask);
    void renderGameOverOrPause(StateMachine & machine);
    void handleCommonButtons(StateMachine & machine);
    bool getPaused();
    void setPaused(bool value);

    LightsState lights = LightsState::Lights_1;

	private:

    uint8_t smokeIndex;
    bool paused = false;

};