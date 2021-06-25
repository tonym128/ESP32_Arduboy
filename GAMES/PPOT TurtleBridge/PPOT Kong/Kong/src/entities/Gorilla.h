#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../Utils/Enums.h"
#include "Barrel.h"
#include <ArduboyTones.h>

extern ArduboyTones sound;

#define GORILLA_X_POSITION_1 13
#define GORILLA_X_POSITION_2 38
#define GORILLA_X_POSITION_3 63



class Gorilla {

  public:

    Gorilla();


    // Properties ..

    uint8_t getXPosition();
    uint8_t getFallingIndex();
    uint8_t getLaunchBarrelDelay();
    int8_t getYPosition(uint8_t yOffset);
    bool getMoveToCentre();
    GorillaStance getStance();


    void setFallingIndex(uint8_t fallingIndex);
    //void setSound(ArduboyTonesExt *sound);


    // Methods ..

    void move();
    void launch(Barrel *barrel, uint8_t launchBarrelDelay);
    bool isPaused();
    uint8_t getPause();
    uint8_t isInPosition();
    void moveToCentre();
    void reset();
    void incFallingIndex();
    bool readyToLaunchNewBarrel();

  private:

    void moveLeftOrRight();
    void changeDirections();

  protected:

    uint8_t pauseAtPosition = 40;
    uint8_t launchBarrel = 0;
    uint8_t launchBarrelDelay = 0;
    uint8_t xPosition = GORILLA_X_POSITION_1;
    uint8_t fallingIndex = 0;
    Movements movement;
    Barrel *barrel;
    //ArduboyTonesExt *sound;
    bool moveCentre = false;

};

