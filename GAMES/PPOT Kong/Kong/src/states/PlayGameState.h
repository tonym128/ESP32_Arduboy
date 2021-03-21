#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../entities/Entities.h"
#include "BaseState.h"

class PlayGameState : public BaseState { //GameState<GameContext, GameStateType> {

  private:
 
    Gorilla gorilla;
    Barrel barrels[NUMBER_OF_BARRELS_MAX];
    Girder girders[2];
    Player player;
    Crane crane;
    Lever lever;
    Fire fire;
    Dinner dinner;
    Hook hook;
    Plate plates[3];
    
    bool playing = false;

  public:

    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

  private:

    uint8_t getActiveGirderCount();
    uint8_t getGirderMaxPosition();
    uint8_t getDisabledGirderIndex();
    uint8_t drawScenery(StateMachine & machine, uint8_t paintMode);
    void killPlayer(StateMachine & machine);
    void incPlateCounters();
    void updatePlayerAndCrane(CranePosition position);

    void resetLevel(uint8_t introDelay = INTRO_DELAY_NORMAL);
    void resetGorillaAndPlates();
    void removeLowerBarrels();
    
    uint8_t introDelay;
    uint8_t spawning;
    uint8_t preventJumpDelay;   // Used to prevent a jump if the A button is pressed to start game.
    uint8_t barrelOffset = 11;
    uint8_t numberOfBarrelsInPlay = 0;
    uint8_t frameRate = FRAME_RATE_MIN;

    bool showLivesLeft = true;
    bool firstSpawnOfGame = true;

};