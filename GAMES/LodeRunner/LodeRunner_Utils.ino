#include "src/utils/Arduboy2Ext.h"
//#include <ArduboyTones.h>


uint8_t getNearestX(int8_t margin) {

  return (player.getX() - level.getXOffset() + margin) / GRID_SIZE;
  
}

uint8_t getNearestY(int8_t margin) {

  return (player.getY() - level.getYOffset() + margin) / GRID_SIZE;
  
}

boolean inCellX() {

  return ((player.getX() - level.getXOffset()) % GRID_SIZE == 0);

}

boolean inCellX(uint8_t margin) {

  uint8_t rem = (player.getX() - level.getXOffset()) % GRID_SIZE;
  return (rem > (GRID_SIZE / 2) ? GRID_SIZE - rem : rem) <= margin;

}

boolean inCellY() {

  return ((player.getY() - level.getYOffset()) % GRID_SIZE == 0);

}

boolean inCellY(uint8_t margin) {

  uint8_t rem = (player.getY() - level.getYOffset()) % GRID_SIZE;
  return ((rem > (GRID_SIZE / 2) ? GRID_SIZE - rem : rem) <= margin);

}

const PlayerStance nextStances[] PROGMEM = {
	PlayerStance::Burn_Left, // -12
	static_cast<PlayerStance>(-11),
  PlayerStance::Swinging_Left1,
  PlayerStance::Swinging_Left4,
  PlayerStance::Swinging_Left3,
  PlayerStance::Swinging_Left2,
  PlayerStance::Climbing_Down1,
  PlayerStance::Climbing_Down2,
  PlayerStance::Running_Left1,
  PlayerStance::Running_Left4,
  PlayerStance::Running_Left3,
  PlayerStance::Running_Left2,
	PlayerStance::StandingStill,	
  PlayerStance::Running_Right2,
  PlayerStance::Running_Right3,
  PlayerStance::Running_Right4,
  PlayerStance::Running_Right1,
  PlayerStance::Climbing_Up2,
  PlayerStance::Climbing_Up1,
  PlayerStance::Swinging_Right2,
  PlayerStance::Swinging_Right3,
  PlayerStance::Swinging_Right4,
  PlayerStance::Swinging_Right1,
  PlayerStance::Falling,
  PlayerStance::Burn_Right,
  PlayerStance::Rebirth_2,
  PlayerStance::Rebirth_3,
  PlayerStance::StandingStill,
};


PlayerStance getNextStance(PlayerStance stance) {

  uint8_t index = static_cast<uint8_t>(stance) + 12;
  if (index > 27) return stance;
  return static_cast<PlayerStance>(pgm_read_byte(&nextStances[index]));

}


Direction getDirection_4Directions(Direction direction) {

  switch (direction) {

    case Direction::RightUp2 ... Direction::RightDown:    return Direction::Right;
    case Direction::RightDown2 ... Direction::LeftDown:   return  Direction::Down;
    case Direction::LeftDown2 ... Direction::LeftUp:      return  Direction::Left;
    default:                                              return Direction::Up;

  }

}


Direction getDirection_16Directions(int16_t xDiff, int16_t yDiff) {

  if (xDiff < 0) {
  
    if (yDiff > 0) {
    
      if (absT(xDiff) > absT(yDiff))          { return Direction::RightUp2; }
      else if (absT(xDiff) < absT(yDiff))     { return Direction::RightUp1; }
      else                                    { return Direction::RightUp; }
      
    }
    else if (yDiff < 0) {
    
      if (absT(xDiff) < absT(yDiff))          { return Direction::RightDown2; }
      else if (absT(xDiff) > absT(yDiff))     { return Direction::RightDown1; }
      else                                    { return Direction::RightDown; }
    
    }
    else {
    
      return Direction::Right;
      
    }
  
  } 
  else if (xDiff > 0) {
  
    if (yDiff > 0) {
    
      if (absT(xDiff) > absT(yDiff))          { return Direction::LeftUp1; }
      else if (absT(xDiff) < absT(yDiff))     { return Direction::LeftUp2; }
      else                                    { return Direction::LeftUp; }
      
    }
    else if (yDiff < 0) {
    
      if (absT(xDiff) < absT(yDiff))          { return Direction::LeftDown1; }
      else if (absT(xDiff) > absT(yDiff))     { return Direction::LeftDown2; }
      else                                    { return Direction::LeftDown; }
    
    }
    else {
    
      return Direction::Left;
      
    }
  
  }
  else {  
  
    if (yDiff < 0) {
    
      return Direction::Down;
      
    }
    else if (yDiff > 0) {
    
      return Direction::Up;
      
    }
    else {
    
      return Direction::Up;   // Player should be dead !
      
    }
  
  }

  return Direction::Up;       // Default, should never get here!

}
