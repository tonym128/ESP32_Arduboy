#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"
#include "Base.h"
#include <ArduboyTones.h>

extern ArduboyTones sound;

class Hook : public Base {

  public:

    Hook();


    // Methods ..

    void decCounter();
    //void setSound(ArduboyTonesExt *sound);

  protected:

    //ArduboyTonesExt *sound;

};

