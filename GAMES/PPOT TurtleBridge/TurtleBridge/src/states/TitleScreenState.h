#pragma once

#include "../utils/GameContext.h"
#include "../utils/GameState.h"
#include "../utils/Enums.h"
#include "../images/Images.h"
#include "BaseState.h"

class TitleScreenState : public BaseState {

  struct Fish {

    uint8_t x;
    uint8_t y;
    bool index;

    Direction direction;

    void update() {

      switch (this->direction) {

        case Direction::Left:
          if (this->x > 12) {
            this->x--;
          }
          else {
            this->direction = Direction::Right;
          }
          break;

        case Direction::Right:
          if (this->x < 104) {
            this->x++;
          }
          else {
            this->direction = Direction::Left;
          }
          break;

      }

    }

  };

  private:

    uint8_t restart = 0;
    uint8_t pressACounter = 0;

    Fish fish1;
    Fish fish2;

  public:	
  
    void activate(StateMachine & machine) override;
    void update(StateMachine & machine) override;
    void render(StateMachine & machine) override;

};

