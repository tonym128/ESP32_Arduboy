#pragma once

#include "GameStateMachine.h"

template< typename ContextType, typename StateIdType >

class GameState {

  public:

    using Context = ContextType;
    using StateId = StateIdType;
    using StateMachine = GameStateMachine<Context, StateId>;

  public:
  
    //virtual ~GameState(void) {};

    virtual void activate(StateMachine & machine) {
      (void)machine;
    }

    virtual void deactivate(StateMachine & machine) {
      (void)machine;
    }
    
    virtual void update(StateMachine & machine) = 0;
    virtual void render(StateMachine & machine) = 0;
 
};