#include "src/Utils/Arduboy2Ext.h"


// ----------------------------------------------------------------------------------------------------------
//  Can the player move left ?
// ----------------------------------------------------------------------------------------------------------

bool canMoveLeft(Level &level, uint16_t playerXPosition, uint16_t playerYPosition) {
 

  // We are not yet at the left hand edge of tile so movement is possible ..

  if (playerXPosition % TILE_SIZE != 0) return true;
	

  // Retrieve the tile from the level defintion for x, y1 ..

  uint8_t tileX = (playerXPosition / 8) - 1;
  uint8_t tileY1 = (playerYPosition / 8);
  uint8_t tile1 = level.getLevelData(tileX, tileY1);

  if (isTileSolid(tile1)) return false;


  // Are we are actually straddling two tiles vertically?  If so test the second tile ..

  if (playerYPosition % TILE_SIZE != 0) {

    uint8_t tileY2 = (playerYPosition / 8) + 1;
    uint8_t tile2 = level.getLevelData(tileX, tileY2);

    if (isTileSolid(tile2)) return false;

  }

  return true;

}


// ----------------------------------------------------------------------------------------------------------
//  Can the player move right ?
// ----------------------------------------------------------------------------------------------------------

bool canMoveRight(Level &level, uint16_t playerXPosition, uint16_t playerYPosition) {


  // We are not yet at the right hand edge of tile so movement is possible..

  if ((playerXPosition + PLAYER_WIDTH) % TILE_SIZE != TILE_SIZE - 1) return true;


  // Retrieve the tile from the level defintion for x, y1 ..
  
  uint8_t tileX = ((playerXPosition + PLAYER_WIDTH) / 8) + 1;
  uint8_t tileY1 = (playerYPosition / 8);
  uint8_t tile1 = level.getLevelData(tileX, tileY1);

  if (isTileSolid(tile1)) return false;


  // Are we are actually straddling two tiles vertically?  If so test the second tile ..

  if (playerYPosition % TILE_SIZE != 0) {

    uint8_t tileY2 = (playerYPosition / 8) + 1;
    uint8_t tile2 = level.getLevelData(tileX, tileY2);

    if (isTileSolid(tile2)) return false;

  }

  return true;
  
}


// ----------------------------------------------------------------------------------------------------------
//  Can the player move up ?
// ----------------------------------------------------------------------------------------------------------

bool canMoveUp(Level &level, uint16_t playerXPosition, uint16_t playerYPosition) {


  // We are not yet at the top edge of tile so movement is possible ..

  if (playerYPosition % TILE_SIZE != 0) return true;


  // If we are at the top of the screen then we can go no higher ..

  if (playerYPosition == 0) return false;


  // The player is 17 pixels wide so always straddles 3 tiles .. 

  uint8_t tileY = (playerYPosition / 8) - 1;
  uint8_t tileX1 = (playerXPosition / 8);
  uint8_t tileX2 = (playerXPosition / 8) + 1;
  uint8_t tileX3 = (playerXPosition / 8) + 2;


  // Retrieve the the three tiles and test them in order ..

  uint8_t tile1 = level.getLevelData(tileX1, tileY);
  if (isTileSolid(tile1)) return false;
	
  uint8_t tile2 = level.getLevelData(tileX2, tileY);
  if (isTileSolid(tile2)) return false;
	
  uint8_t tile3 = level.getLevelData(tileX3, tileY);
  if (isTileSolid(tile3)) return false;

  return true;

}



// ----------------------------------------------------------------------------------------------------------
//  Can the player move down ?
// ----------------------------------------------------------------------------------------------------------

bool canMoveDown(Level &level, uint16_t playerXPosition, uint16_t playerYPosition, uint8_t size) {


  // We are not yet at the bottom edge of tile so movement is possible ..

  if ((playerYPosition + size) % TILE_SIZE != 0) return true;


  // We are at the bottom of the level so no further movement is possible ..

  if ((playerYPosition + size) == level.getHeight()) return false;


  // The player is 17 pixels wide so always straddles 3 tiles .. 

  uint8_t tileY = ((playerYPosition + size) / 8);
  uint8_t tileX1 = (playerXPosition / 8);
  uint8_t tileX2 = (playerXPosition / 8) + 1;
  uint8_t tileX3 = (playerXPosition / 8) + 2;


  // Retrieve the three tiles and test them in order ..

  uint8_t tile1 = level.getLevelData(tileX1, tileY);
  if (isTileSolid(tile1)) return false;
	
  uint8_t tile2 = level.getLevelData(tileX2, tileY);
  if (isTileSolid(tile2)) return false;
	
  uint8_t tile3 = level.getLevelData(tileX3, tileY);
  if (isTileSolid(tile3)) return false;

  return true;

}


// ----------------------------------------------------------------------------------------------------------
//  Move the cab ..
// ----------------------------------------------------------------------------------------------------------

void moveCab(Level &level, Player &player, Customer &customer) {

  SQ15x16 playerYDeltaVal = player.getYDeltaVal();
  SQ15x16 playerXDeltaVal = player.getXDeltaVal();

  uint16_t playerXPosition = player.getXDisplay() - level.getXOffsetDisplay();
  uint16_t playerYPosition = player.getYDisplay() - level.getYOffsetDisplay();


  // --  Moving up -----------------------------------------------------------------------------------------

  if (player.getYDelta() < 0) { 

    if (canMoveUp(level, playerXPosition, playerYPosition)) {

      player.decRetractLandingGear();
      player.setJustLanded(false);
      fuelSound[0] = 0;

      if (level.yOffset < 0) {

        if (player.getY() < PLAYER_Y_CENTRE) {
          if (level.yOffset < playerYDeltaVal) {
            level.yOffset = level.yOffset - playerYDeltaVal;
          }
          else {
            level.yOffset = 0;
          }
        }
        else if (player.getY() == PLAYER_Y_CENTRE) {
          if (level.yOffset < PLAYER_Y_CENTRE && (level.getWidthInTiles() != 16 || level.getHeightInTiles() != 8)) {
            level.yOffset = level.yOffset - playerYDeltaVal;
          }
          else {
            player.setY(player.getY() + playerYDeltaVal - level.yOffset);
            level.yOffset = 0;
          }
        }
        else if (player.getY() > PLAYER_Y_CENTRE) {
          if (player.getY() + playerYDeltaVal > PLAYER_Y_CENTRE) {
            player.setY(player.getY() + playerYDeltaVal);
          }
          else {
            level.yOffset = level.yOffset + PLAYER_Y_CENTRE - player.getY() - playerYDeltaVal;
            player.setY(PLAYER_Y_CENTRE);
          }
        }

      }
      else {

        if (player.getY() > -playerYDeltaVal) {
          player.setY(player.getY() + playerYDeltaVal);
        }
        else {
          player.setY(0);
        }

      }

    }
    else {

      // Are we at the top of the screen ?

      if (player.getY().getInteger() == 0) {

        player.setScore(player.getScore() + customer.getFare());
        sound.tones(coinsSound);

        if (levelNumber == MAX_NUMBER_OF_LEVELS) {
          //TODO End of GameState
          state = GameState::MaxLevelCompleted;
        }
        else {
          if (state == GameState::SplashScreen) {
            state = GameState::PlayGame_InitGame;
          }
          else {
            state = GameState::PlayGame_LevelIntroduction_Init;
            levelNumber++;
          }
          player.setFaresCompleted(0);
        }

      }

    }
    
  }


  // --  Moving down --------------------------------------------------------------------------------------------

  if (player.getYDelta() > 0) { 
    
    if (canMoveDown(level, playerXPosition, playerYPosition, player.getHeight())) {
      if (player.getY() < PLAYER_Y_CENTRE) {                         
        if (player.getY() + playerYDeltaVal < PLAYER_Y_CENTRE) {
          player.setY(player.getY() + playerYDeltaVal);
        }
        else {
          if (level.getWidthInTiles() != 16 || level.getHeightInTiles() != 8) {
            level.yOffset = PLAYER_Y_CENTRE - player.getY() - playerYDeltaVal;
          }
          player.setY(PLAYER_Y_CENTRE);
        }
      }
      else if (player.getY() == PLAYER_Y_CENTRE) {    
        
        if (level.yOffset - playerYDeltaVal > -level.getHeight() + HEIGHT && (level.getWidthInTiles() != 16 || level.getHeightInTiles() != 8)) {
          level.yOffset = level.yOffset - playerYDeltaVal;
        }
        else {
          // player.setY(player.getY() + ((level.getHeight() - HEIGHT) + level.yOffset) + playerYDeltaVal);
          player.setY(player.getY() + playerYDeltaVal);
          if (level.getWidthInTiles() != 16 || level.getHeightInTiles() != 8) {
            level.yOffset = -level.getHeight() + HEIGHT;
          }
        }
      }
      else if (player.getY() > PLAYER_Y_CENTRE) {
        if (player.getY() + playerYDeltaVal + player.getHeight() < HEIGHT) {
          player.setY(player.getY() + playerYDeltaVal);
        }
        else {
          player.setY(HEIGHT - player.getHeight());
        }

      }

    }
    else {

      player.setXDelta(0);
      player.setJustLanded(true);


      // If we have landed within 8 pixels of a customer then pick up the fare ..

      if (!player.getPickingUpCustomer() && !player.isCarryingCustomer() && customer.getStatus() != CustomerStatus::Dead) {

        uint16_t customerXPosition = customer.getX();
        uint16_t customerYPosition = customer.getY() + CUSTOMER_HEIGHT;

        int16_t left = customerXPosition - CUSTOMER_PICKUP_RANGE - PLAYER_WIDTH;
        int16_t right = customerXPosition + CUSTOMER_WIDTH + CUSTOMER_PICKUP_RANGE;

        if ((left <= static_cast<int16_t>(playerXPosition) && static_cast<int16_t>(playerXPosition) <= right) &&
            (customerYPosition - 2 <= playerYPosition + player.getHeight() && playerYPosition <= customerYPosition + player.getHeight() + 2)) {

          customer.setStatus(CustomerStatus::BoardingCab);
          customer.setXWalkingOffset(0);
          player.setPickingUpCustomer(true);
          gotoCounter = GOTO_COUNTER_MAX;

          if (customerXPosition + CUSTOMER_WIDTH_HALF < player.getXDisplay() - level.getXOffsetDisplay() + PLAYER_WIDTH_HALF) {
            customer.setWalkingDirection(Direction::Right);
          }
          else {
            customer.setWalkingDirection(Direction::Left);
          }

        }

      }


      // If we have landed within 8 pixels of a flag then drop off the fare ..

      else if (player.isCarryingCustomer() && customer.getStatus() != CustomerStatus::Dead) {

        uint8_t x = ((player.getXDisplay() - level.getXOffsetDisplay()) / 8) + 1; // Move over one to account for 17 pixels
        uint8_t y = ((player.getYDisplay() - level.getYOffsetDisplay()) / 8) + 1;

        if (player.isCarryingCustomer() && diffT(customer.getXDestinationTile(), x) < 3 && customer.getYDestinationTile() == y) {

          player.setCarryingCustomer(false);
          player.setScore(player.getScore() + customer.getFare());
          player.incFaresCompleted();
          sound.tones(coinsSound);
          dollarsCount = DOLLARS_COUNT_MAX;

          if (player.getFaresCompleted() >= level.getFaresRequired()) {
            launchCustomer(level, customer, RANDOM_START_POSITION, GO_TO_GATE);
          }
          else {
            launchCustomer(level, customer, RANDOM_START_POSITION, RANDOM_END_POSITION);
          }

        }

      }

    }

    player.setRetractLandingGear(PLAYER_RETRACT_LANDING_GEAR);

  }


  // --  Moving left --------------------------------------------------------------------------------------------

  if (player.getXDelta() < 0) { 

    if (canMoveLeft(level, playerXPosition, playerYPosition)) {

      if (level.xOffset < 0) {

        if (player.getX() < PLAYER_X_CENTRE) {
          if (level.xOffset < playerXDeltaVal) {
            level.xOffset = level.xOffset - playerXDeltaVal;
          }
          else {
            level.xOffset = 0;
          }
        }
        else if (player.getX() == PLAYER_X_CENTRE) {
          if (level.xOffset < playerXDeltaVal && (level.getWidthInTiles() != 16 || level.getHeightInTiles() != 8)) {
            level.xOffset = level.xOffset - playerXDeltaVal;
          }
          else {
            player.setX(player.getX() + playerXDeltaVal - level.xOffset);
            level.xOffset = 0;
          }
        }
        else if (player.getX() > PLAYER_X_CENTRE) {
          if (player.getX() + playerXDeltaVal > PLAYER_X_CENTRE) {
            player.setX(player.getX() + playerXDeltaVal);
          }
          else {
            level.xOffset = level.xOffset + PLAYER_X_CENTRE - player.getX() - playerXDeltaVal;
            player.setX(PLAYER_X_CENTRE);
          }
        }

      }
      else {

        if (player.getX() > -playerXDeltaVal) {
          player.setX(player.getX() + playerXDeltaVal);
        }
        else {
          player.setX(0);
        }

      }

    }
    
  }


  // --  Moving Right -------------------------------------------------------------------------------------------

  if (player.getXDelta() > 0) {

    if (canMoveRight(level, playerXPosition, playerYPosition)) {

      if (player.getX() < PLAYER_X_CENTRE) {
        if (player.getX() + playerXDeltaVal < PLAYER_X_CENTRE) {
          player.setX(player.getX() + playerXDeltaVal);
        }
        else {
          if (level.getWidthInTiles() != 16 || level.getHeightInTiles() != 8) {
            level.xOffset = PLAYER_X_CENTRE - player.getX() - playerXDeltaVal;
          }
          player.setX(PLAYER_X_CENTRE);
        }
      }
      else if (player.getX() == PLAYER_X_CENTRE) {                            
        if (level.xOffset - playerXDeltaVal > -level.getWidth() + WIDTH && (level.getWidthInTiles() != 16 || level.getHeightInTiles() != 8)) {
          level.xOffset = level.xOffset - playerXDeltaVal;
        }
        else {
          player.setX(player.getX() + ((level.getWidth() - WIDTH) + level.xOffset) + playerXDeltaVal);
          if (level.getWidthInTiles() != 16 || level.getHeightInTiles() != 8) {
            level.xOffset = -level.getWidth() + WIDTH;
          }
        }
      }
      else if (player.getX() > PLAYER_X_CENTRE) {

        if (player.getX() + playerXDeltaVal + PLAYER_WIDTH < WIDTH) {
          player.setX(player.getX() + playerXDeltaVal);
        }
        else {
          player.setX(WIDTH - PLAYER_WIDTH);
        }

      }

    }

  }

}
