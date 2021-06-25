#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../utils/SaveEntry.h"
#include "../images/Images.h"
#include "BaseState.h"

class HighScoreState : public BaseState {

  private:

    uint8_t winnerIdx = 0;
    uint8_t charIdx = 0;
    uint8_t clearScores = 0;
    uint8_t pressACounter = HS_PRESS_A_DELAY;

	SaveEntry players[eepromSaveEntriesCount];

    void renderHighScore(uint8_t y, const SaveEntry & saveEntry);

  public:	
  
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

};

