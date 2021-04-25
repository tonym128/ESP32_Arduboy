#include "src/utils/Arduboy2Ext.h"

// --------------------------------------------------------------------------------------
//  Render the screen
//
void RenderScreen(Player *player, Enemy *enemies) {


  // Render the horizon ;

  const uint8_t horizon[5][7] = {
                                  { 0, 2,  6, 12, 20, 30, 42 },
                                  { 0, 2,  7, 13, 22, 32, 45 },
                                  { 0, 3,  8, 15, 24, 35, 48 },
                                  { 0, 3,  9, 16, 26, 37, 51 },
                                  { 0, 4, 10, 18, 28, 40, 54 }
                                };                        

  uint8_t row = level.getHorizon();

  for (uint8_t col = 0; col < HORIZON_COL_COUNT; col++) {

    uint8_t y = horizon[row][col] + 4;
    arduboy.drawHorizontalDottedLine((col + 2) / 2, WIDTH, y, col + 2);

  }


  // Render the enemies ..

  for (uint8_t x = 0; x < MAX_NUMBER_OF_ENEMIES; x++) {

    Enemy *enemy = &enemies[x];

    uint8_t const *imageName = nullptr;
    uint8_t const *maskName = nullptr;
    uint8_t const *spawnImageName = nullptr;
    uint8_t const *spawnMaskName = nullptr;    
    uint8_t frame = 0;

    if (enemy->isVisible()) {

      switch (enemy->getStatus()) {

        case EnemyStatus::Active:
        
          switch (enemy->getSize()) {

            case ImageSize::Horizon:

              imageName = alien_horizon;
              maskName = alien_horizon_mask;
              break;

            case ImageSize::Far:

              imageName = alien_far;
              maskName = alien_far_mask;
              break;

            case ImageSize::Medium:
       
              frame = static_cast<uint8_t>(enemy->getType());

              if (arduboy.getFrameCount(ENEMY_FRAME_COUNT) < ENEMY_FRAME_COUNT_HALF) {
                imageName = alien_medium_1;
                maskName = alien_medium_1_mask;
              }
              else {
                imageName = alien_medium_2;
                maskName = alien_medium_2_mask;
              }

              break;

            case ImageSize::Close:
     
              frame = static_cast<uint8_t>(enemy->getType());

              if (arduboy.getFrameCount(ENEMY_FRAME_COUNT) < ENEMY_FRAME_COUNT_HALF) {
                imageName = alien_close_1;
                maskName = alien_close_1_mask;
              }
              else {
                imageName = alien_close_2;
                maskName = alien_close_2_mask;
              }

              break;

          }


          // Render image ..

          Sprites::drawExternalMask(enemy->getXDisplay(), enemy->getYDisplay(), imageName, maskName, frame, frame);
          break;


        case EnemyStatus::Dead ... EnemyStatus::Explosion4:

          switch (enemy->getStatus()) {

            case EnemyStatus::Explosion1:
              imageName = alien_close_explosion_4;
              break;

            case EnemyStatus::Explosion2:
              imageName = alien_close_explosion_3;
              break;

            case EnemyStatus::Explosion3:
              imageName = alien_close_explosion_2;
              break;

            case EnemyStatus::Explosion4:
              imageName = alien_close_explosion_1;
              break;

            default: break;
              
          }

          Sprites::drawSelfMasked(enemy->getXDisplay(), enemy->getYDisplay(), imageName, 0);
          break;


        case EnemyStatus::Spawn1 ... EnemyStatus::Spawn8:
        
          uint8_t spawnIdx = static_cast<uint8_t>(enemy->getStatus()) - static_cast<uint8_t>(EnemyStatus::Spawn1);
          frame = static_cast<uint8_t>(enemy->getType());

          switch (enemy->getSize()) {

            case ImageSize::Medium:

              if (arduboy.getFrameCount(ENEMY_FRAME_COUNT) < ENEMY_FRAME_COUNT_HALF) {
                imageName = alien_medium_1;
                maskName = alien_medium_1_mask;
              }
              else {
                imageName = alien_medium_2;
                maskName = alien_medium_2_mask;
              }

              spawnImageName = spawning_medium;
              spawnMaskName = spawning_medium_mask;

              break;

            case ImageSize::Close:

              if (arduboy.getFrameCount(ENEMY_FRAME_COUNT) < ENEMY_FRAME_COUNT_HALF) {
                imageName = alien_close_1;
                maskName = alien_close_1_mask;
              }
              else {
                imageName = alien_close_2;
                maskName = alien_close_2_mask;
              }

              spawnImageName = spawning_close;
              spawnMaskName = spawning_close_mask;


              break;

          }

          Sprites::drawExternalMask(enemy->getXDisplay(), enemy->getYDisplay(), imageName, maskName, frame, frame);
          Sprites::drawExternalMask(enemy->getXDisplay() - 1, enemy->getYDisplay(), spawnImageName, spawnMaskName, spawnIdx, spawnIdx);
          break;

      }
 
    }
    else { // Enemy is not visible ..

      if (enemy->getStatus() == EnemyStatus::Active) {

        if (enemy->getYDisplay() >= 64 && enemy->getYDisplay() <= VISIBLE_SCREEN_WIDTH) {

          if (enemy->getXDisplay() < static_cast<int16_t>(0)) {

            arduboy.drawPixel(0, 59, BLACK);
            arduboy.drawFastVLine(0, 60, 3);
            arduboy.drawFastHLine(0, 63, 3);
            arduboy.drawPixel(4, 63, BLACK);

          }

          if (enemy->getXDisplay() > static_cast<int16_t>(VISIBLE_SCREEN_WIDTH)) {

            arduboy.drawPixel(113, 59, BLACK);
            arduboy.drawFastVLine(113, 60, 3);
            arduboy.drawFastHLine(110, 63, 3);
            arduboy.drawPixel(109, 63, BLACK);

          }
          else {

            arduboy.drawPixel(enemy->getXDisplay() + 2, 63, BLACK);
            arduboy.drawFastHLine(enemy->getXDisplay() + 3, 63, 6);
            arduboy.drawPixel(enemy->getXDisplay() + 9, 63, BLACK);

          }

        }
        else {

          if (enemy->getXDisplay() < static_cast<int16_t>(0)) {

            arduboy.drawPixel(0, enemy->getYDisplay() + 2, BLACK);
            arduboy.drawFastVLine(0, enemy->getYDisplay() + 3, 6, WHITE);
            arduboy.drawPixel(0, enemy->getYDisplay() + 9, BLACK);

          }

          if (enemy->getXDisplay() > static_cast<int16_t>(VISIBLE_SCREEN_WIDTH)) {

            arduboy.drawPixel(113, enemy->getYDisplay() + 2, BLACK);
            arduboy.drawFastVLine(113, enemy->getYDisplay() + 3, 6, WHITE);
            arduboy.drawPixel(113, enemy->getYDisplay() + 9, BLACK);

          }

        }

      }

    }

  }


  // Render the space ship ..  

  const uint8_t frames[] = {0, 4, 3, 3, 2};
  uint8_t frameRate = frames[absT(player->getYDelta())];

  if (player->getStatus() == PlayerStatus::Active) {
      
    switch (player->getYDelta()) {

      case -4 ... -1:

        if (arduboy.getFrameCount(frameRate) % frameRate < frameRate / 2) { 
          Sprites::drawExternalMask(player->getX(), player->getY(), spaceship_backwards_1, spaceship_backwards_1_mask, 0, 0);
        }
        else {
          Sprites::drawExternalMask(player->getX(), player->getY(), spaceship_backwards_2, spaceship_backwards_2_mask, 0, 0);
        }
        break;

      case 0:

        Sprites::drawExternalMask(player->getX(), player->getY(), spaceship_neutral, spaceship_neutral_mask, 0, 0);
        break;

      case 1 ... 4:

        if (arduboy.getFrameCount(frameRate) % frameRate < frameRate / 2) { 
          Sprites::drawExternalMask(player->getX(), player->getY(), spaceship_advance_1, spaceship_advance_1_mask, 0, 0);
        }
        else {
          Sprites::drawExternalMask(player->getX(), player->getY(), spaceship_advance_2, spaceship_advance_2_mask, 0, 0);
        }
        break;

    }

  }
  else {

    uint8_t const *imageName = nullptr;

    switch (player->getStatus()) {

      case PlayerStatus::Explosion1:
        imageName = alien_close_explosion_4;
        break;

      case PlayerStatus::Explosion2:
        imageName = alien_close_explosion_3;
        break;

      case PlayerStatus::Explosion3:
        imageName = alien_close_explosion_2;
        break;

      case PlayerStatus::Explosion4:
        imageName = alien_close_explosion_1;
        break;

      default: 
        imageName = alien_close_explosion_5;
        break;
        
    }

    Sprites::drawSelfMasked(player->getX(), player->getY(), imageName, 0);
    
  }


  // Render player bullet ..

  if (playerBullet.getY() > 0) {

    arduboy.drawFastVLine(playerBullet.getX(), playerBullet.getY(), 5, WHITE); 

  }


  // Render enemy bullets ..

  for (uint8_t x = 0; x < MAX_NUMBER_OF_BULLETS; x++) {

    Bullet *bullet = &bullets[x];

    if (bullet->getY() > 0) { 

      arduboy.drawRect(bullet->getX(), bullet->getY(), 2, 2, WHITE);

    }

  }


  if (gameState == GameState::GamePlay && arduboy.everyXFrames(2)) {
    
    alternate++;

  }


  // Render scoreboard ..

  Sprites::drawOverwrite(114, 0, blankScoreboard, 0);


  // Score ..

  uint8_t digits[6] = {};
  extractDigits(digits, level.getScore());
  
  for (int8_t i = 5, y = 1; i >= 0; --i, y += 5) {
    Sprites::drawOverwrite(123, y, numbers_vert, digits[i]);
  }


  if (gameState == GameState::Wave || gameState == GameState::WaveCompleted) {
  
    if (introDelay % 2 == 1) {

      for (int8_t i = player->getLives(), y = 64 - (player->getLives() * 6); i > 0; --i, y += 6) {
        Sprites::drawOverwrite(123, y, life_vert, 0);
      }

    }
    else {
      
      for (int8_t i = player->getLives() - 1, y = 64 - ((player->getLives() - 1) * 6); i > 0; --i, y += 6) {
        Sprites::drawOverwrite(123, y, life_vert, 0);
      }

    }

  }
  else {
      
    for (int8_t i = player->getLives() - 1, y = 64 - ((player->getLives() - 1) * 6); i > 0; --i, y += 6) {
      Sprites::drawOverwrite(123, y, life_vert, 0);
    }
    
  }

  Sprites::drawOverwrite(116, 0, fuelGauge, 0);
  arduboy.drawLine(119, 28, 119, (player->getFuel() + 6), BLACK);

  uint8_t health = player->getHealth() * 2;
  Sprites::drawOverwrite(116, 39, shieldGauge, 0);
  arduboy.drawLine(119, 61, 119, 46 + health, BLACK);


  // Wave completed ?

  if (gameState == GameState::WaveCompleted) {

    if (arduboy.everyXFrames(12) && fuelBonusDisplay < fuelBonus) {

      fuelBonusDisplay++;
      level.setScore(level.getScore() + FUEL_BONUS_INC);
      
    }

    arduboy.fillRect(19, 18, 78, 22, BLACK);
    Sprites::drawSelfMasked(20, 19, waveCompleted, 0);

    Sprites::drawSelfMasked(74, 30, numbers, (fuelBonusDisplay * FUEL_BONUS_INC_DIV) / 10);
    Sprites::drawSelfMasked(79, 30, numbers, (fuelBonusDisplay * FUEL_BONUS_INC_DIV) % 10);
    Sprites::drawSelfMasked(84, 30, numbers, 0);
    Sprites::drawSelfMasked(89, 30, numbers, 0);

  }



  // Start of game or wave?

  if (gameState == GameState::Wave) {

    if (introDelay != 2 && introDelay != 4 && introDelay != 6 && introDelay != 8) {

      if (level.getWave() == 1 && player->getLives() == MAX_NUMBER_OF_LIVES) {

        arduboy.fillRect(30, 21, 53, 13, BLACK);
        Sprites::drawSelfMasked(31, 22, getReady, 0);

      }
      else {

        arduboy.fillRect(35, 21, 46, 13, BLACK);
        Sprites::drawSelfMasked(36, 22, startOfWave, 0);
        Sprites::drawSelfMasked(67, 25, numbers, level.getWave() / 10);
        Sprites::drawSelfMasked(72, 25, numbers, level.getWave() % 10);

      }

    }
    else {

      if (introDelay == 2) {
        if (!sound.playing()) sound.tones(blib2);
      }
      else {
        if (!sound.playing()) sound.tones(blib);
      }

    }

  }

  if (gameState == GameState::Paused) {
    
    arduboy.fillRect(44, 21, 33, 13, BLACK);
    Sprites::drawSelfMasked(41, 22, pause, 0);

  }


  // Double up score ..

  alternate2++;
  if (alternate2 > 96) alternate2 = 0;

  if (level.inDoubleUpPhase()) {

    arduboy.setRGBled(0, (alternate2 < 48 ? (alternate2 + 2) / 2 : 50 - (alternate2 / 2)), 0);
   
    if (alternate2 < 48) {

      Sprites::drawSelfMasked(117, 31, x2_flashing, 0);
 
    }

  }
  else {

    arduboy.setRGBled(0, 0, 0);

  }

}
