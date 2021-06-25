#include "PlayGameState.h"

#include "../utils/Arduboy2Ext.h"
#include "../utils/Utils.h"
#include "../images/Images.h"
#include "../sounds/Sounds.h"


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void PlayGameState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
  auto & gameStats = machine.getContext().gameStats;
  
  const uint8_t yOffset = this->player.getYOffset();
  const bool hidePlayer = (this->playing ? false : (this->firstSpawnOfGame) ? true : arduboy.getFrameCountHalf(FLASH_FRAME_COUNT));


  // Draw Scenery ..
  
  this->drawScenery(machine, SCENERY_PAINT_FIRST);


  // Draw Barrels
  {

    for (auto &barrel : this->barrels) {

      if (barrel.isEnabled()) {

        int8_t x = barrel.getXPosition();
        int8_t y = barrel.getYPosition(yOffset);
        uint8_t r = barrel.getRotation();
        Sprites::drawExternalMask(x, y, Images::BarrelImg, Images::Barrel_Mask, r, 0);

      }

    }

  }


  // Draw gorilla ..
  {
    GorillaStance stance = this->gorilla.getStance();
    uint8_t x = this->gorilla.getXPosition();
    int8_t y = this->gorilla.getYPosition(yOffset);

    switch (stance) {

      case GorillaStance::Dead1:
        Sprites::drawExternalMask(x, y, Images::Gorilla_Dead_01, Images::Gorilla_Dead_01_Mask, 0, 0);
        break;

      case GorillaStance::Dead2:
        Sprites::drawExternalMask(x, y, Images::Gorilla_Dead_02, Images::Gorilla_Dead_02_Mask, 0, 0);
        break;

      case GorillaStance::Dead3:
        Sprites::drawExternalMask(x, y, Images::Gorilla_Dead_03, Images::Gorilla_Dead_03_Mask, 0, 0);
        break;

      default:
        uint8_t stance = static_cast<uint8_t>(this->gorilla.getStance());
        Sprites::drawSelfMasked(x, y, Images::Gorilla, stance );
        break;


    }

  }


  // Draw player

  if (this->playing || this->introDelay > 0) {

    uint8_t const *imageName = nullptr;
    uint8_t const *mask = nullptr;
    uint8_t image = this->player.getImage();

    int8_t x = this->player.getXPosition(true);
    int8_t y = this->player.getYPosition();

    bool maskTop = false;
    int8_t maskTop_XOffset = 0;

    switch (image) {

      case static_cast<uint8_t>(Stance::Normal) ... static_cast<uint8_t>(Stance::OnCrane_RHS):

        if (y == 36 && x >= 13 && x <= 75) {

          imageName = Images::Mario;
          maskTop = true;

          switch (image) {

            case static_cast<uint8_t>(Stance::Normal):
            case static_cast<uint8_t>(Stance::Running_01):
            case static_cast<uint8_t>(Stance::Running_02):
            case static_cast<uint8_t>(Stance::Running_03):
            case static_cast<uint8_t>(Stance::Running_04):
            case static_cast<uint8_t>(Stance::Jump):
              mask = Images::Mario_Upper_Mask_LHS;
              break;

            default:
              mask = Images::Mario_Upper_Mask_RHS;
              maskTop_XOffset = -1;
              break;
            
          }

        }
        else {
          imageName = Images::Mario;
          mask = Images::Mario_Mask;
        }
        break;

      case static_cast<uint8_t>(Stance::Dead_01) ... static_cast<uint8_t>(Stance::Dead_03):
        imageName = Images::Mario_Dying;
        image = image - static_cast<uint8_t>(Stance::Dead_01);
        break;

      case static_cast<uint8_t>(Stance::OnCrane_EmptyHand) ... static_cast<uint8_t>(Stance::OnCrane_HoldingHook):
        imageName = Images::Mario_Leaping;
        image = image - static_cast<uint8_t>(Stance::OnCrane_EmptyHand);
        break;

    }


    if (!maskTop) {

      if (this->introDelay < 10 || this->introDelay > INTRO_MOVE_TO_BOTTOM || gameStats.gameOver) {

        if (!hidePlayer) {

          if (mask == nullptr) {
            Sprites::drawSelfMasked(x, y, imageName, image);
          }
          else {
            Sprites::drawExternalMask(x, y, imageName, mask, image, image);
          }

        }

      }
      else {

        if (this->spawning != NO_IMAGE) {
          Sprites::drawSelfMasked(x, y, Images::Mario_Spawn, this->spawning - 1);
        }

      }

    }
    else {

      Sprites::drawErase(x + maskTop_XOffset, y - 1, mask, 0);
      Sprites::drawSelfMasked(x, y, imageName, image);

    }

  }


  // Draw girders ..

  if (gameStats.mode == GameMode::Hard) {

    for (auto &girder : this->girders) {

      if (girder.isEnabled()) {

        int8_t x = girder.getXPosition();
        int8_t y = girder.getYPosition(yOffset);
        uint8_t index = girder.getImage();
        Sprites::drawExternalMask(x, y, Images::Girder_Moving, Images::Girder_Moving_Mask, index, index);
  
      }

    }

  }

  this->drawScenery(machine, SCENERY_PAINT_LAST);

  if (!gameStats.gameOver && this->showLivesLeft) {

    Sprites::drawExternalMask(2, 30, Images::Level, Images::Level_Mask, 0, 0);

    uint8_t digits[3] = {};
    extractDigits(digits, gameStats.level);

    for (uint8_t j = 3; j > 0; --j) {

      Sprites::drawSelfMasked(21 - (j*5), 39, Images::Scoreboard_Numbers, digits[j - 1]);

    }
    
  }
  else {

    BaseState::renderGameOverOrPause(machine);

  }


  // Render score ..

  BaseState::renderScore(machine, 0);

}