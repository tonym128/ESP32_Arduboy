#include "src/Utils/Arduboy2Ext.h"

void drawLevel_Intro(Level &level, uint8_t gateToRender) {

  for (uint8_t y = 0; y < level.getHeightInTiles(); y++) {

    for (uint8_t x = 0; x < level.getWidthInTiles(); x++) {

      const int16_t bitmapX = (x * TILE_SIZE);
      const int16_t bitmapY = (y * TILE_SIZE);

      uint8_t tile = level.getLevelData(x, y);

      switch (tile) {

        case EMPTY: break;

        case SIGN1:
          {
            bool found = false;
            const uint8_t numberOfPositions = level.getNumberOfCustomerPositions();
            const uint8_t *levelEndingPosition = levelEndingPositions[level.getLevelNumber()];

            for (uint8_t i = 0; i < numberOfPositions; ++i) {

              const uint8_t startPosX = pgm_read_byte(&levelEndingPosition[i * 2]);
              const uint8_t startPosY = pgm_read_byte(&levelEndingPosition[(i * 2) + 1]);

              if (x == startPosX && y == startPosY && gateToRender == i) {

                Sprites::drawOverwrite(bitmapX, bitmapY, tiles, tile);
                arduboy.fillRect(bitmapX + 2, bitmapY - 2, 5, 7, WHITE);
                Sprites::drawErase(bitmapX + 3, bitmapY - 1, font3x5_Numbers, (i + 1));
                found = true;

              }

            }

            if (!found) Sprites::drawOverwrite(bitmapX, bitmapY, tiles, GRASS);
            
          }

          break;

        default:
          Sprites::drawOverwrite(bitmapX, bitmapY, tiles, tile);
          break;
          
      }

    }

  }

}


void drawLevel(Level &level) {

  for (uint8_t y = 0; y < level.getHeightInTiles(); y++) {

    for (uint8_t x = 0; x < level.getWidthInTiles(); x++) {

      const int16_t tileX = (x * TILE_SIZE);
      const int16_t tileY = (y * TILE_SIZE);
      const int16_t bitmapX = tileX + level.getXOffsetDisplay();
      const int16_t bitmapY = tileY + level.getYOffsetDisplay();
      

      // Do we really need to render the tile?

      if (bitmapX < -8 || bitmapX >= WIDTH || bitmapY < -8 || bitmapY >= HEIGHT)
        continue;

      uint8_t tile = level.getLevelData(x, y);

      switch (tile) {

        case EMPTY: 
          {
            uint16_t tileFrame = ((y * level.getHeightInTiles()) + x + 3) % 27;
#ifdef SPARKLE
            if (tileFrame % 8 == 0 && arduboy.getFrameCount() % 4 != (y * level.getHeightInTiles() + x) % 4) {
#else
            if (tileFrame % 8 == 0) {
#endif              
              Sprites::drawOverwrite(bitmapX, bitmapY, Empty_With_Stars, tileFrame >> 3);
            }
          }
          break;

        case SIGN1:

          Sprites::drawOverwrite(bitmapX, bitmapY, tiles, tile);
          arduboy.fillRect(bitmapX + 2, bitmapY - 2, 5, 7, WHITE);

          if (flashingCounter < (FLASH_MAX / 2)) {

            const uint8_t numberOfPositions = level.getNumberOfCustomerPositions();
            const uint8_t *levelEndingPosition = levelEndingPositions[level.getLevelNumber()];

            for (uint8_t i = 0; i < numberOfPositions; ++i) {

              const uint8_t startPosX = pgm_read_byte(&levelEndingPosition[i * 2]);
              const uint8_t startPosY = pgm_read_byte(&levelEndingPosition[(i * 2) + 1]);

              if (x == startPosX && y == startPosY) {

                Sprites::drawErase(bitmapX + 3, bitmapY - 1, font3x5_Numbers, (i + 1));

              }

            }

          }

          break;


        case FUEL1:
          {
            Fuel *fuel = level.getFuel(x, y);
            Sprites::drawOverwrite(bitmapX, bitmapY, FuelImgs, fuel->getFuelLeftPerCent());
          }
          break;

        default: 
          Sprites::drawOverwrite(bitmapX, bitmapY, tiles, tile);
          break;
          
      }

    }

  }
             
}

#define HUD_OFFSET 10
#define HUD_WIDTH 66

//__attribute__((noinline)) 
void drawHUD(Font4x6 &font4x6, Player &player, Customer &customer) {

  Sprites::drawOverwrite(0, 56, SpaceCabHUD, 0);
  font4x6.setCursor(13, 57);

  {
    uint8_t digits[5];
    extractDigits(digits, player.getScore());
    for (uint8_t i = 5; i > 0; --i) 
    font4x6.print(digits[i - 1]);
  }

  if (player.getFuel() > 0 && player.getFuel() < PLAYER_FUEL_MIN_BLINK && flashingCounter < (FLASH_MAX / 2)) {
  
    arduboy.fillRect(44, 57, 30, 8 , BLACK);
  
  }
  else {

    uint8_t digitsFuel[3];
    font4x6.setCursor(57, 57);
    extractDigits(digitsFuel, player.getFuel());
    for(uint8_t i = 3; i > 0; --i) 
    font4x6.print(digitsFuel[i - 1]);

  }

  {
    uint8_t digitsLives[1];
    font4x6.setCursor(94, 57);
    extractDigits(digitsLives, player.getNumberOfLives());
    for(uint8_t i = 1; i > 0; --i) 
    font4x6.print(digitsLives[i - 1]);
  }

  {
    uint8_t digitsFare[2];
    font4x6.setCursor(115, 57);
    extractDigits(digitsFare, customer.getFare());
    for(uint8_t i = 2; i > 0; --i)
    font4x6.print(digitsFare[i - 1]);
  }

}

void scrollingBackground(bool scrollLeft) {

  uint8_t xCount = 0;

  arduboy.drawFastHLine(0, 0, WIDTH, WHITE);

  for (int16_t x = backdropx; x < 128; x = x + 8) {

    for (uint8_t y = 0; y < 7; ++y) {

      if ((xCount % 2 == 0 && y % 2 == 0) || (xCount % 2 == 1 && y % 2 == 1)) {
        Sprites::drawOverwrite(x, 4 + (y * 8), checkeredBG_White, 0);
      }

    }

    ++xCount;

  }

  arduboy.drawFastHLine(0, HEIGHT - 1, WIDTH, WHITE);

  if (arduboy.everyXFrames(2)) { // when running at 60fps

    if (scrollLeft) {
      --backdropx;
      if (backdropx <= -WIDTH) {
        backdropx = 0;
      }
    }
    else {
      ++backdropx;
      if (backdropx >= 0) {
        backdropx = -WIDTH;
      }
    }
  
  }

}

void playerDisplay(Player &player) {

  uint8_t const *imageName = nullptr;
  uint8_t const *maskName = nullptr;
  int8_t xOffset = 0;
  int8_t yOffset = 0;
  uint8_t frameNumber = player.getFrame();

  switch (player.getStatus()) {

    case PlayerStatus::OutOfFuel_Img1_Start ... PlayerStatus::OutOfFuel_Img1_End:
      imageName = SpaceTaxi_OutOfFuel_1;
      maskName = SpaceTaxi_OutOfFuel_1_Mask;
      xOffset = -5;
      yOffset = -5;
      frameNumber = 0;
      break;

    case PlayerStatus::OutOfFuel_Img2_Start ... PlayerStatus::OutOfFuel_Img2_End:
      imageName = SpaceTaxi_OutOfFuel_2;
      maskName = SpaceTaxi_OutOfFuel_2_Mask;
      xOffset = -5;
      yOffset = -5;
      frameNumber = 0;
      break;

    case PlayerStatus::OutOfFuel_Img3_Start ... PlayerStatus::OutOfFuel_Img3_End:
      imageName = SpaceTaxi_OutOfFuel_3;
      maskName = SpaceTaxi_OutOfFuel_3_Mask;
      xOffset = -5;
      yOffset = -5;
      break;

    case PlayerStatus::OutOfFuel_Img4_Start ... PlayerStatus::OutOfFuel_Img4_End:
      imageName = SpaceTaxi_OutOfFuel_4;
      maskName = SpaceTaxi_OutOfFuel_4_Mask;
      xOffset = -5;
      yOffset = -5;
      break;

    case PlayerStatus::Active:

      if (player.isLandingGearDown()) {
        imageName = SpaceTaxi_LandingGear;
        maskName = SpaceTaxi_LandingGear_Mask;
      }
      else {
        imageName = SpaceTaxi;
        maskName = SpaceTaxi_Mask;
      }
      break;

    default: break;

  }

  if (imageName != nullptr) {
    Sprites::drawExternalMask(player.getXDisplay() + xOffset, player.getYDisplay() + yOffset, imageName, maskName, frameNumber, frameNumber);
  }

}


void customerDisplay(Level &level, Player &player, Customer &customer, int16_t customerXVal, int16_t customerYVal) {

  uint8_t frame = customer.getFrame();

  if (!player.isCarryingCustomer() && customerXVal >= -CUSTOMER_WIDTH && customerXVal < WIDTH && customerYVal >= -CUSTOMER_HEIGHT && customerYVal < HEIGHT) {

    switch (customer.getStatus()) {

      case CustomerStatus::Dead:

        Sprites::drawExternalMask(customerXVal, customerYVal, Customer_Flat_Img, Customer_Flat_Mask, 0, 0);
        break;

      case CustomerStatus::Alive:

        Sprites::drawExternalMask(customerXVal, customerYVal, Customer_Img, Customer_Img_Mask, frame, frame);
        if (arduboy.everyXFrames(15)) {
          customer.incFrame();
        }

        break;

      case CustomerStatus::BoardingCab:
        {
          int8_t walkingOffset = customer.getXWalkingOffset();

          if (customer.getWalkingDirection() == Direction::Left) {

            Sprites::drawExternalMask(customerXVal + walkingOffset, customerYVal, Customer_WalkingLeft, Customer_WalkingLeft_Mask, frame, frame);

          }
          else {

            Sprites::drawExternalMask(customerXVal + walkingOffset, customerYVal, Customer_WalkingRight, Customer_WalkingRight_Mask, frame, frame);

          }

          if (arduboy.everyXFrames(8)) {

            uint16_t customerXPosition = customer.getX();

            if (customerXPosition + CUSTOMER_WIDTH_HALF + walkingOffset != player.getXDisplay() - level.getXOffsetDisplay() + PLAYER_WIDTH_HALF) {

              customer.incFrame();

              if (customer.getWalkingDirection() == Direction::Left) {
                customer.decXWalkingOffset();
              }
              else {
                customer.incXWalkingOffset();
              }

            }
            else {

              player.setCarryingCustomer(true);
              player.setPickingUpCustomer(false);
              if (player.getFaresCompleted() >= level.getFaresRequired())  level.openLevelGates(); 

              sound.tone(NOTE_E6, 50, NOTE_E3, 50, NOTE_E2, 50);

            }

          }

        }

        break;

    }

  }
  else if (state == GameState::PlayGame)  { 

    if (player.isCarryingCustomer() && customer.getDestinationPosition() == GO_TO_GATE) { return; }

    uint16_t customerX = 0;
    uint16_t customerY = 0;
    uint8_t arrowX = 0;
    uint8_t arrowY = 0;
    uint8_t playerX = player.getXDisplay();
    uint8_t playerY = player.getYDisplay();

    Direction direction = Direction::Up;


    // Render arrows.

    if (flashingCounter < (FLASH_MAX / 2)) { 

      if (!player.isCarryingCustomer()) {

        customerX = customer.getX();
        customerY = customer.getY();

      }
      else {

        customerX = customer.getXDestinationTile() * TILE_SIZE;
        customerY = customer.getYDestinationTile() * TILE_SIZE;

      }


      // If the customer location is on screen then do not render ..
      {
        int16_t customerXDisplay = customerX + level.getXOffsetDisplay();
        int16_t customerYDisplay = customerY + level.getYOffsetDisplay();

        if (customerXDisplay >= -CUSTOMER_WIDTH && customerXDisplay < WIDTH && customerYDisplay >= -CUSTOMER_HEIGHT && customerYDisplay < HEIGHT) {
          return;
        } 

      }

      int16_t dX = customerX - (player.getX() - level.xOffset).getInteger();
      int16_t dY = customerY - (player.getY() - level.yOffset).getInteger();

      if (dX < -playerX) {

        if (dY < -playerY)                            { direction = Direction::UpLeft; arrowX = 0; arrowY = 0; }
        if (dY >= -playerY && dY < -playerY + 56)     { direction = Direction::Left; arrowX = 0; arrowY = playerY + dY - 4; }
        if (dY >= -playerY + 56)                      { direction = Direction::DownLeft; arrowX = 0; arrowY = 48; }

      }

      if (dX >= -playerX && dX < -playerX + WIDTH) {

        if (dY < 0)                                   { direction = Direction::Up; arrowX = playerX + dX; arrowY = 0; }
        if (dY > 0)                                   { direction = Direction::Down; arrowX = playerX + dX; arrowY = 48; }

      }

      if (dX >= -playerX + WIDTH) {

        if (dY < -playerY)                            { direction = Direction::UpRight; arrowX = 120; arrowY = 0; }
        if (dY >= -playerY && dY < -playerY + 56)     { direction = Direction::Right; arrowX = 120; arrowY = playerY + dY - 4; }
        if (dY >= -playerY + 56)                      { direction = Direction::DownRight; arrowX = 120; arrowY = 48; }

      }

      Sprites::drawExternalMask(arrowX, arrowY, ArrowImgs, ArrowMasks, static_cast<uint8_t>(direction), static_cast<uint8_t>(direction));

    }

  }

}


void drawDollars(Player &player) {

  if (dollarsCount > 0) {

    uint8_t idx = (DOLLARS_COUNT_MAX - dollarsCount) / DOLLARS_COUNT_MULT;

    Sprites::drawExternalMask(player.getXDisplay() - 5, player.getYDisplay() - 10, dollars, dollars_Mask, idx, idx);
    dollarsCount--;
  
  }

}


void drawGoto(Level &level, Player &player, Customer &customer, int16_t customerXVal, int16_t customerYVal) {

  if (gotoCounter > 0) {

    int8_t walkingOffset = customer.getXWalkingOffset() + 1;
    
    if (player.getFaresCompleted() >= level.getFaresRequired()) {
      Sprites::drawExternalMask(customerXVal + walkingOffset, customerYVal - 13, Hail_GoGate, Hail_Mask, 0, 0);
    }
    else {
      Sprites::drawExternalMask(customerXVal + walkingOffset, customerYVal - 13, Hail, Hail_Mask, 0, 0);
      Sprites::drawErase(customerXVal + walkingOffset + 25, customerYVal - 9, font4x6_Full, customer.getDestinationPosition() + 53);
    }

  }

}

void drawOuch(Level &level, Customer &customer, int16_t customerXVal, int16_t customerYVal) {

  if (ouchCounter > 0) {

    Sprites::drawExternalMask(customerXVal + customer.getXWalkingOffset() + 1, customerYVal - 13, Ouch, Hail_Mask, 0, 0);

  }

}
