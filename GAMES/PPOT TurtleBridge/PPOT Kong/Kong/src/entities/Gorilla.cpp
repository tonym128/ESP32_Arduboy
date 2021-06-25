#include "Gorilla.h"
#include "../utils/Utils.h"
#include "../utils/Enums.h"
#include "../sounds/Sounds.h"

const uint8_t  Offsets[] = {

//  0, 0, 0, 0,0, 0, 0, 0, 0, 3, 6, 9, 12, 15, 18, 20 // 16 elements
  0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 6, 8, 11, 14, 17, 19 // 16 elements

};

Gorilla::Gorilla() { 
}

uint8_t Gorilla::getXPosition() {

  switch (this->fallingIndex) {

    case 11 ... 15:
      return this->xPosition - 1;

    default:
      return this->xPosition;

  }

}

int8_t Gorilla::getYPosition(uint8_t yOffset) {

  return 3 - yOffset + (this->moveCentre ? pgm_read_byte(&Offsets[fallingIndex]) : 0);

}

bool Gorilla::getMoveToCentre() {

  return this->moveCentre;

}

uint8_t Gorilla::getFallingIndex() {

  return this->fallingIndex;

}

uint8_t Gorilla::getLaunchBarrelDelay() {

  return this->launchBarrelDelay;

}

GorillaStance Gorilla::getStance() {

  switch (this->fallingIndex) {

    case 11 ... 12:
      return GorillaStance::Dead1;

    case 13 ... 14:
      return GorillaStance::Dead2;

    case 15:
      return GorillaStance::Dead3;

    default:

      switch (this->xPosition) {

        case GORILLA_X_POSITION_1:
        case GORILLA_X_POSITION_2:
        case GORILLA_X_POSITION_3:

          if (this->launchBarrel > 0) {

            return static_cast<GorillaStance>( static_cast<uint8_t>(GorillaStance::ThrowingBarrel1) + (3 - (this->launchBarrel / 10)) );

          }
          else {
            return GorillaStance::Normal;
          }

        case GORILLA_X_POSITION_1 + 1 ... GORILLA_X_POSITION_2 - 1:
        case GORILLA_X_POSITION_2 + 1 ... GORILLA_X_POSITION_3 - 1:

          switch (this->movement) {

            case Movements::Left:
              return GorillaStance::Left;

            case Movements::Right:
              return GorillaStance::Right;

            default:
              return GorillaStance::Left;

          }

          return GorillaStance::Normal;

      }

  }

}

void Gorilla::move() {

  if (this->moveCentre) {

    switch (this->xPosition) {

      case GORILLA_X_POSITION_2:
        break;

      case GORILLA_X_POSITION_1:
        this->movement = Movements::Right;
        moveLeftOrRight();
        break;

      case GORILLA_X_POSITION_3:
        this->movement = Movements::Left;
        moveLeftOrRight();
        break;

      case GORILLA_X_POSITION_1 + 1 ... GORILLA_X_POSITION_2 - 1:
      case GORILLA_X_POSITION_2 + 1 ... GORILLA_X_POSITION_3 - 1:
        moveLeftOrRight();
        break;    

    }

  }
  else {

    if (this->launchBarrelDelay > 0) {

      this->launchBarrelDelay--;

      if (this->launchBarrelDelay == 0) {
        this->launchBarrel = 40;
      }

    }
    else if (this->launchBarrel > 0) {

      this->launchBarrel--;

      if (this->launchBarrel == 0) {
        
        uint8_t lauchPosition = 0;
        if (this->xPosition == GORILLA_X_POSITION_2) lauchPosition = 1;
        if (this->xPosition == GORILLA_X_POSITION_3) lauchPosition = 2;
        this->barrel->launch(lauchPosition);
#ifdef PLAY_SOUNDS 
        if (!sound.playing()) sound.tones(Sounds::drop_barrel);
#endif
      }

    }


    // Pause in the current position or move?

    if (this->pauseAtPosition > 0) {

      this->pauseAtPosition--;

    }
    else {

      switch (this->xPosition) {

        case GORILLA_X_POSITION_1:
        case GORILLA_X_POSITION_2:
        case GORILLA_X_POSITION_3:
          changeDirections();

        case GORILLA_X_POSITION_1 + 1 ... GORILLA_X_POSITION_2 - 1:
        case GORILLA_X_POSITION_2 + 1 ... GORILLA_X_POSITION_3 - 1:
          moveLeftOrRight();

          switch (this->xPosition) {

            case GORILLA_X_POSITION_1:
            case GORILLA_X_POSITION_2:
            case GORILLA_X_POSITION_3:
              this->pauseAtPosition = random(70, 200);
              break;

            default:            
              break;    

          }

          break;    

      }

    }

  }

}

void Gorilla::moveLeftOrRight() {

  switch (this->movement) {

    case Movements::Left:
      this->xPosition--;
      break;

    case Movements::Right:
      this->xPosition++;
      break;

    default: break;

  }

}

void Gorilla::changeDirections() {

  switch (this->xPosition) {

    case GORILLA_X_POSITION_1:
      this->movement = Movements::Right;
      break;

    case GORILLA_X_POSITION_2:
      this->movement = (random(0, 2) == 0 ? Movements::Left : Movements::Right);
      break;
      
    case GORILLA_X_POSITION_3:
      this->movement = Movements::Left;
      break;

  }
  
}

/*
void Gorilla::setSound(ArduboyTonesExt *sound) {

  this->sound = sound;

}
*/
void Gorilla::launch(Barrel *barrel, uint8_t launchBarrelDelay) {

  this->barrel = barrel;
  this->launchBarrel = 0;
  this->launchBarrelDelay = launchBarrelDelay;
  this->pauseAtPosition = launchBarrelDelay + 60;

}

bool Gorilla::isPaused() {

  return (this->pauseAtPosition > 0);

}

uint8_t Gorilla::getPause() {

  return this->pauseAtPosition;

}

uint8_t Gorilla::isInPosition() {

  switch (this->xPosition) {

    case GORILLA_X_POSITION_1:
      return 0;

    case GORILLA_X_POSITION_2:
      return 1;
      
    case GORILLA_X_POSITION_3:
      return 2;

    default: 
      return NOT_IN_A_POSITION;

  }

}

void Gorilla::moveToCentre() {

  this->moveCentre = true;
  this->pauseAtPosition = 100;
  this->launchBarrel = 0;
  this->launchBarrelDelay = 0;

}

void Gorilla::incFallingIndex() {
  
  if (this->fallingIndex < 15) this->fallingIndex++;

}

void Gorilla::reset() {

    this->pauseAtPosition = 0;
    this->launchBarrel = 0;
    this->launchBarrelDelay = 0;
    this->xPosition = GORILLA_X_POSITION_1;
    this->moveCentre = false;
    this->fallingIndex = 0;

}

bool Gorilla::readyToLaunchNewBarrel() {
  
  return (this->isInPosition() != NOT_IN_A_POSITION /* && this->pauseAtPosition > 10 */ && this->launchBarrelDelay == 0 && this->launchBarrel == 0);

}
