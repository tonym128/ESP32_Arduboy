#pragma once

#include "Arduboy2Ext.h"
#include "Enums.h"
#include "../characters/Player.h"
#include "../characters/Enemy.h"


/* ----------------------------------------------------------------------------
 *  Return the upper 4 bits of a byte.
 */
uint8_t leftValue(uint8_t val) {

  return val >> 4; 
      
}


/* ----------------------------------------------------------------------------
 *  Return the lower 4 bits of a byte.
 */
uint8_t rightValue(uint8_t val) {

  return val & 0x0F; 
      
}

// ---------------------------------------------------------------------------------
// Extract individual digits of a uint8_t
//
template< size_t size >
void extractDigits(uint8_t (&buffer)[size], uint8_t value) {

  for (uint8_t i = 0; i < size; ++i) {
    buffer[i] = value % 10;
    value /= 10;
  }

}

// ---------------------------------------------------------------------------------
// Extract individual digits of a uint16_t
//
template< size_t size >
void extractDigits(uint8_t (&buffer)[size], uint16_t value) {

  for(uint8_t i = 0; i < size; ++i) {
    buffer[i] = value % 10;
    value /= 10;
  }

}

// ---------------------------------------------------------------------------------
// Can be stood on by the player ?
//
// The player cannot stand on brick that has been dug out or is repairing itself.
//
boolean canBeStoodOn(LevelElement levelElement, Enemy *enemies, uint16_t positionX, uint16_t positionY) {

  switch (levelElement) {

    case LevelElement::Brick:
    case LevelElement::Solid:
    case LevelElement::Ladder:
      return true;

    default:


	  // Is the player standing on top of an enemy?

      for (uint8_t x = 0; x < NUMBER_OF_ENEMIES; x++) {

        Enemy *enemy = &enemies[x];

        if (enemy->getEnabled()) {

          if (enemy->getX() == positionX * GRID_SIZE && enemy->getY() == positionY * GRID_SIZE) {

            return true; 

          }

        }

      }

	    return false;

  }

}


// ---------------------------------------------------------------------------------
// Can be stood on by the enemy ?
//
boolean isSolid(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Brick:
    case LevelElement::Solid:
      return true;

    default:
      return false;

  }

}

// ---------------------------------------------------------------------------------
// Can the player fall onto these elements ?
//
// Elements should be the same as the 'canBeStoodOn' with the addition of the rail.
//
boolean canBeFallenOn(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Brick:
    case LevelElement::Solid:
    case LevelElement::Rail:
    case LevelElement::Ladder:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can the space be climbed on by the player ?
//
boolean canBeClimbedOn(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Ladder:
    case LevelElement::Rail:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can the space be occupied by the player ?
//
// Can the player enter the square - ie. is it empty or something that can be climbed on?
//
boolean canBeOccupied(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Blank:
    case LevelElement::Ladder:
    case LevelElement::Rail:
    case LevelElement::Gold:
    case LevelElement::FallThrough:
    case LevelElement::Brick_1 ... LevelElement::Brick_Close_4:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can the space be fallen into by the player ?
//
boolean canBeFallenInto(LevelElement levelElement, Enemy *enemies, uint16_t positionX, uint16_t positionY) {

  switch (levelElement) {

    case LevelElement::Blank:
    case LevelElement::FallThrough:
    case LevelElement::Rail:
    case LevelElement::Gold:
      return true;

    case LevelElement::Brick_1:
    case LevelElement::Brick_2:
    case LevelElement::Brick_3:
    case LevelElement::Brick_4:
    case LevelElement::Brick_Transition:
    case LevelElement::Brick_Close_1:
    case LevelElement::Brick_Close_2:
    case LevelElement::Brick_Close_3:
    case LevelElement::Brick_Close_4:

      for (uint8_t x = 0; x < NUMBER_OF_ENEMIES; x++) {

        Enemy *enemy = &enemies[x];

        if (enemy->getEnabled()) {

          if (enemy->getStance() == PlayerStance::Falling && enemy->getX() == positionX * GRID_SIZE && enemy->getY() == positionY * GRID_SIZE) {

            return false; 

          }

        }

      }

      return true;

    default:
      return false;

  }

}

// ---------------------------------------------------------------------------------
// Can be stood on by the enemy ?
//
boolean canBeStoodOnBasic_Enemy(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Brick:
    case LevelElement::Solid:
    case LevelElement::Ladder:
      return true;

    default:
      return false;

  }

}

// ---------------------------------------------------------------------------------
// Can be stood on by the enemy ?
//
// The enemy can stand on bricks that have been dug out or are reforming.  Of 
// course they will fall into these if they do step on them.
//
boolean canBeStoodOn_Enemy(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Brick:
    case LevelElement::Solid:
    case LevelElement::Ladder:
    case LevelElement::Brick_1:
    case LevelElement::Brick_2:
    case LevelElement::Brick_3:
    case LevelElement::Brick_4:
    case LevelElement::Brick_Transition:
    case LevelElement::Brick_Close_1:
    case LevelElement::Brick_Close_2:
    case LevelElement::Brick_Close_3:
    case LevelElement::Brick_Close_4:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can the space be occupied by the enemy ?
//
// Can the player enter the square - ie. is it empty or something that can be climbed on?
//
boolean canBeOccupiedBasic_Enemy(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Blank:
    case LevelElement::Ladder:
    case LevelElement::Rail:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can the space be occupied by the enemy ?
//
// Can the player enter the square - ie. is it empty or something that can be climbed on?
//
boolean canBeOccupied_Enemy(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Blank:
    case LevelElement::Ladder:
    case LevelElement::Rail:
    case LevelElement::FallThrough:
    case LevelElement::Gold:
    case LevelElement::Brick_1:
    case LevelElement::Brick_2:
    case LevelElement::Brick_3:
    case LevelElement::Brick_4:
    case LevelElement::Brick_Transition:
    case LevelElement::Brick_Close_1:
    case LevelElement::Brick_Close_2:
    case LevelElement::Brick_Close_3:
    case LevelElement::Brick_Close_4:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can a falling enemy continue to fall ?  Should stop if they have fallen into a dug hole.
//
boolean canContinueToFall_Enemy(LevelElement levelElement) {

  switch (levelElement) {

    case LevelElement::Blank:
    case LevelElement::FallThrough:
      return true;

    default:
      return false;

  }

}


// ---------------------------------------------------------------------------------
// Can the space be fallen into by the enemy ?
//
boolean canBeFallenInto_Enemy(LevelElement levelElement, Enemy *enemies, uint16_t positionX, uint16_t positionY) {

  switch (levelElement) {

    case LevelElement::Blank:
    case LevelElement::FallThrough:
      return true;

    case LevelElement::Brick_1:
    case LevelElement::Brick_2:
    case LevelElement::Brick_3:
    case LevelElement::Brick_4:
    case LevelElement::Brick_Transition:
    case LevelElement::Brick_Close_1:
    case LevelElement::Brick_Close_2:
    case LevelElement::Brick_Close_3:
//  case LevelElement::Brick_Close_4:


      // Is another enemy in the position already ?

      for (uint8_t x = 0; x < NUMBER_OF_ENEMIES; x++) {

        Enemy *enemy = &enemies[x];

        if (enemy->getEnabled()) {

          if (enemy->getX() == positionX * GRID_SIZE && enemy->getY() == positionY * GRID_SIZE) {

            return false; 

          }

        }

      }

      return true;

    default:
      return false;

  }

}