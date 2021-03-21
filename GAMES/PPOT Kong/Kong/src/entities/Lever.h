#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"
#include "../map/Coordinates.h"
#include "Crane.h"
#include "Base.h"
#include <ArduboyTones.h>

extern ArduboyTones sound;

class Lever : public Base {

  public:

    Lever();

 
    // Properties ..

    LeverPosition getPosition();

    void setPosition(LeverPosition position);
    void setCrane(Crane *crane);


    // Methods

    void update();
    //void setSound(ArduboyTonesExt *sound);


  protected:

    //ArduboyTonesExt *sound;
    LeverPosition position = LeverPosition::Off;
    Crane *crane;

};

