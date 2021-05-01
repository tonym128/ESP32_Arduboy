#include "src/utils/Arduboy2Ext.h"
#include "src/entity/Player.h"
#include "src/entity/Enemy.h"
#include "src/entity/Bullet.h"
#include "src/entity/Slot.h"
#include "src/entity/HighScore.h"
#include "src/images/Images.h"
#include "src/images/Fonts.h"
#include "src/utils/Enums.h"
#include "src/utils/Utils.h"
#include "src/utils/EEPROM_Utils.h"
#include "src/levels/Level.h"
#include "src/levels/Formations.h"
#include "src/utils/FadeEffects.h"
#include "src/sounds/Sounds.h"
#include <ArduboyTones.h>

Arduboy2Ext arduboy;
ArduboyTones sound(arduboy.audio.enabled);
GameState gameState = GameState::Intro_Init;

uint8_t introDelay = 0;
uint8_t alternate = 0;
uint8_t alternate2 = 0;
uint8_t horizonIncrement = 0;
uint8_t fuelBonusDisplay = 0;
uint8_t fuelBonus = 0;
uint8_t restart = 0;

Player player;
Bullet playerBullet;
Enemy enemies[MAX_NUMBER_OF_ENEMIES];
Bullet bullets[MAX_NUMBER_OF_BULLETS];
Level level;

HighScore highScore;
FadeInEffect fadeInEffect;



// --------------------------------------------------------------------------------------
//  Setup ..
//
void setup() {

  arduboy.boot();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.setFrameRate(DEFAULT_FRAME_RATE);
  arduboy.initRandomSeed();
  arduboy.audio.begin();

  EEPROM_Utils::initEEPROM(false);

}


// --------------------------------------------------------------------------------------
//  Main Loop ..
//
void loop() {

  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();

  switch (gameState) {

    case GameState::Intro_Init:
      arduboy.setFrameRate(DEFAULT_FRAME_RATE);
      fadeInEffect.reset();
      gameState = GameState::Intro;
      sound.tones(intro_theme);
      // break; Fall-through intentional.

    case GameState::Intro:
      Intro();
      break;

    case GameState::ScoreTable:
      sound.noTone();
      ScoreTable();
      player.resetGame();
      level.resetGame(enemies, bullets, &playerBullet);
      break;

    case GameState::Wave_Init:
      player.resetWave();
      level.resetWave(enemies, bullets, &playerBullet);
      introDelay = 12;
      alternate = 0;
      gameState = GameState::Wave;
      arduboy.setFrameRate(level.getFrameRate());
      arduboy.setRGBled(0, 0, 0);
      fuelBonusDisplay = 0;
      // break; Fall-through intentional.

    case GameState::Wave:
 
      if (arduboy.everyXFrames(20)) { introDelay--; }
    
      if (introDelay == 0) {
        level.launchFormation(enemies);
        gameState = GameState::GamePlay;
      }

      Play();
      break;

    case GameState::WaveCompleted_Init:

      for (uint8_t x = 0; x < MAX_NUMBER_OF_BULLETS; x++) {

        Bullet *bullet = &bullets[x];
        bullet->setY(0);

      }

      playerBullet.setY(0);

      introDelay = 12;
      alternate = 0;
      gameState = GameState::WaveCompleted;
      fuelBonusDisplay = 0;
      // break; Fall-through intentional.

    case GameState::WaveCompleted:
 
      if (arduboy.everyXFrames(20)) { introDelay--; }
    
      if (introDelay == 0) {
        gameState = GameState::Wave_Init;
      }

      Play();
      break;

    case GameState::Paused:
    case GameState::GamePlay:
      Play();
      break;

    case GameState::GameOver_Init:
      arduboy.setFrameRate(DEFAULT_FRAME_RATE);
      gameState = GameState::GameOver;
      fadeInEffect.reset();
      sound.tones(end_of_game);
      arduboy.setRGBled(0, 0, 0);
      // break; Fall-through intentional.

    case GameState::GameOver:
      GameOver();
      break;

    case GameState::SaveScore:
      highScore.reset();
      highScore.setSlotNumber(EEPROM_Utils::saveScore(level.getScore(), level.getWave()));
      gameState = GameState::HighScore;
      fadeInEffect.reset();
      // break; Fall-through intentional.

    case GameState::HighScore:
      HighScore();
      break;

    case GameState::Credits:
      Credits();
      break;

    default: break;

  }
  
  arduboy.display(CLEAR_BUFFER);

}


// --------------------------------------------------------------------------------------
//  Let's play!
//
void Play() {

  #ifdef USE_INVERT
  arduboy.invert(false);
  #endif


  // Update the ground's position ..

#if GROUND_SPEED == 1
  const uint8_t speedLookup[] = {0, 16, 8, 0, 4};
#endif
#if GROUND_SPEED == 2
  const uint8_t speedLookup[] = {0, 12, 6, 0, 3};
#endif
#if GROUND_SPEED == 3
  const uint8_t speedLookup[] = {0, 8, 4, 0, 2};
#endif

  if (gameState != GameState::Paused) {

    uint8_t speed = speedLookup[absT(player.getYDelta())];

    if (horizonIncrement == speed) {

      level.incHorizon( player.getYDelta() > 0 ? 1 : -1);
      horizonIncrement = 0;
  

      // Update the enemies and bullets relative to the player position ..

      for (uint8_t x = 0; x < MAX_NUMBER_OF_ENEMIES; x++) {

        Enemy *enemy = &enemies[x];

        if (enemy->getStatus() == EnemyStatus::Active) {

          enemy->moveRelativeToPlayer(&player);

        }

      }

      for (uint8_t x = 0; x < MAX_NUMBER_OF_BULLETS; x++) {

        Bullet *bullet = &bullets[x];

        if (bullet->getY() > 0) {

          bullet->moveRelativeToPlayer(&player);

        }

      }
      
    }

    horizonIncrement++;

  }
  

  if (gameState != GameState::Wave && gameState != GameState::WaveCompleted && gameState != GameState::Paused) {

    #ifdef INC_HEALTH
    player.incHealth();
    #endif

    level.decDoubleUpPoints();

    if (arduboy.everyXFrames(FRAME_RATE_DEC_FUEL)) {
      player.decFuel();
    }


    if (arduboy.everyXFrames(FRAME_RATE_2)) {

      for (uint8_t x = 0; x < MAX_NUMBER_OF_ENEMIES; x++) {

        Enemy *enemy = &enemies[x];

        if (enemy->getStatus() == EnemyStatus::Active) {
          if (enemy->move(&player)) level.decInPlay();
        }    

      }

    }

  }

  RenderScreen(&player, enemies);
 

  // Handle players actions ..

  if (player.getStatus() == PlayerStatus::Active && gameState != GameState::Paused) {

    if (arduboy.pressed(DOWN_BUTTON))     { if (player.decYDelta()) horizonIncrement = 0; }
    if (arduboy.pressed(UP_BUTTON))       { if (player.incYDelta()) horizonIncrement = 0; }
    if (arduboy.pressed(LEFT_BUTTON))     { player.decX(); }
    if (arduboy.pressed(RIGHT_BUTTON))    { player.incX(); }

    if (gameState != GameState::Wave && gameState != GameState::WaveCompleted && (arduboy.justPressed(A_BUTTON) || (level.inDoubleUpPhase() && arduboy.pressed(A_BUTTON)))) {

      if (playerBullet.getY() == 0) {

        playerBullet.setY(45);
        playerBullet.setX(player.getX() + 7);
        sound.tones(player_fires_bullet);

      }

    }

    if (!arduboy.pressed(DOWN_BUTTON) && !arduboy.pressed(UP_BUTTON) && arduboy.everyXFrames(FRAME_RATE_16)) { player.decelerate(); }

  }

  if (gameState != GameState::Wave && gameState != GameState::WaveCompleted && arduboy.justPressed(B_BUTTON)) gameState = (gameState == GameState::GamePlay ? GameState::Paused : GameState::GamePlay);
  if (gameState != GameState::Wave && gameState != GameState::WaveCompleted && gameState != GameState::Paused) {


    // Update player bullet position ..

    uint8_t bulletY = playerBullet.getY();
    playerBullet.setY(bulletY > 10 ? bulletY - 5 : 0);

    for (uint8_t x = 0; x < MAX_NUMBER_OF_BULLETS; x++) {

      Bullet *bullet = &bullets[x];

      if (bullet->getY() > 0) {

        bullet->move(&player);

      }

    }


    // Did our bullet hit an enemy or enemy bullet?

    if (playerBullet.getY() > 0) {

      Rect bulletRect = { playerBullet.getX() - 1, playerBullet.getY() - 5, 3, 5 };

      for (uint8_t x = 0; x < MAX_NUMBER_OF_ENEMIES; x++) {

        Enemy *enemy = &enemies[x];

        if (enemy->getStatus() == EnemyStatus::Active && enemy->getProtection() == 0) {

          Rect enemyRect = { enemy->getXDisplay(), enemy->getYDisplay(), enemy->getWidth(), enemy->getHeight() };

          if (arduboy.collide(bulletRect, enemyRect)) {

            enemy->setStatus(EnemyStatus::Explosion4);
            sound.tones(enemy_explosion);

            uint16_t scoreInc = 0;
            bool doubleUpPoints = level.inDoubleUpPhase();

            switch (enemy->getType()) {

              case EnemyType::EnemyType1:
                scoreInc = (!doubleUpPoints ? 150 : 300);
                break;

              case EnemyType::EnemyType2:
                scoreInc = (!doubleUpPoints ? 300 : 600);
                break;

              case EnemyType::EnemyType3:
                scoreInc = (!doubleUpPoints ? 450 : 900);
                break;

              case EnemyType::EnemyType4:
                scoreInc = (!doubleUpPoints ? 600 : 1200);;
                break;

              case EnemyType::AstronautCaptured:
                {
                  bool enemyLaunched = false;
                  scoreInc = 450;


                  // If possible, launch a new enemy (astronaut) in the same place as the original ..

                  for (uint8_t y = 0; y < MAX_NUMBER_OF_ENEMIES; y++) {

                    Enemy *newEnemy = &enemies[y];

                    if (newEnemy->getStatus() == EnemyStatus::Dead) {
                      newEnemy->setType(EnemyType::AstronautReleased);
                      newEnemy->setMovementSequence(MovementSequence::Sequence_4);
                      newEnemy->setStatus(EnemyStatus::Active);
                      newEnemy->setX(enemy->getX());
                      newEnemy->setY(enemy->getY());
                      newEnemy->setYDelta(-1);
                      newEnemy->setProtection(10);
                      enemyLaunched = true;
                      level.incEnemiesLaunched();
                      level.incInPlay();
                      break;
                      
                    }

                  }

                  // If no enemy launched, the re-use the current one ..

                  if (!enemyLaunched) {

                      enemy->setType(EnemyType::AstronautReleased);
                      enemy->setMovementSequence(MovementSequence::Sequence_4);
                      enemy->setStatus(EnemyStatus::Active);
                      enemy->setProtection(5);
                      enemy->setYDelta(-1);

                  }

                }
                break;

              case EnemyType::AstronautReleased:
                break;

            }

            level.setScore(level.getScore() + scoreInc);

          }

        }    

      }

      for (uint8_t x = 0; x < MAX_NUMBER_OF_BULLETS; x++) {

        Bullet *bullet = &bullets[x];

        if (bullet->getY() > 0) {

          Rect eneymBulletRect = { bullet->getX(), bullet->getY(), 2, 2 };

          if (arduboy.collide(bulletRect, eneymBulletRect)) {

            bullet->setY(0);
            Sprites::drawSelfMasked(bullet->getX() - 2, bullet->getY() - 2, alien_far, 0);
            sound.tones(bullets_collide);

          }

        }

      }

    }


    // Did any enemy hit us ..

    if (player.getStatus() == PlayerStatus::Active) {

      Rect playerRect = { player.getX() + 1, player.getY() + 4, 13, 8 };

      for (uint8_t x = 0; x < MAX_NUMBER_OF_ENEMIES; x++) {

        Enemy *enemy = &enemies[x];

        if (enemy->isVisible() && enemy->getStatus() == EnemyStatus::Active && enemy->getYDisplay() > 20) {

          Rect enemyRect = { enemy->getXDisplay() + 1, enemy->getYDisplay() + 1, static_cast<uint8_t>(enemy->getWidth() - 2), static_cast<uint8_t>(enemy->getHeight() - 2)};

          if (arduboy.collide(enemyRect, playerRect)) {

            switch (enemy->getType()) {

              case EnemyType::EnemyType1 ... EnemyType::AstronautCaptured:

                if (!enemy->getPlayerOverlap()) {

                  enemy->setPlayerOverlap(true);
                  enemy->setStatus(EnemyStatus::Explosion4);

                  #ifdef SINGLE_FLASH
                  #ifdef USE_INVERT
                  arduboy.invert(true);
                  #endif
                  arduboy.setRGBled(RED_LED, 16);
                  #else 
                  player.setHealthCountdown(HEALTH_COUNTDOWN);
                  #endif

                  uint8_t health = player.getHealth();

                  if (health > 0) {

                    player.setHealth(--health);
                    sound.tones(player_hit_by_alien);

                  }
                  else {

                    player.setStatus(PlayerStatus::Explosion4);
                    sound.tones(player_explosion);

                  }

                }
                
                break;

              case EnemyType::AstronautReleased:
                enemy->setStatus(EnemyStatus::Dead);
                sound.tones(double_up);
                level.setDoubleUpPoints(DOUBLE_UP_POINTS_DELAY);
                level.decInPlay();
                break;

            }

          }
          else {

            enemy->setPlayerOverlap(false);

          }

        }

      }


      // Did any enemy bullet hit us ..

      for (uint8_t x = 0; x < MAX_NUMBER_OF_BULLETS; x++) {

        Bullet *bullet = &bullets[x];

        if (bullet->getY() > 0) {

          Rect bulletRect = { bullet->getX(), bullet->getY(), 2, 2 };

          if (arduboy.collide(bulletRect, playerRect)) {

            #ifdef SINGLE_FLASH
            #ifdef USE_INVERT
            arduboy.invert(true);
            #endif
            arduboy.setRGBled(RED_LED, 16);
            #else 
            player.setHealthCountdown(HEALTH_COUNTDOWN);
            #endif

            uint8_t health = player.getHealth();
            bullet->setY(0);

            if (health > 0) {

              player.setHealth(--health);
              sound.tones(player_hit_by_bullet);

            }
            else {

              player.setStatus(PlayerStatus::Explosion4);
              sound.tones(player_explosion);

            }

          }

        }

      }

    }


    // Update dying enemy images ..

    if (arduboy.everyXFrames(FRAME_RATE_4)) {

      for (uint8_t x = 0; x < MAX_NUMBER_OF_ENEMIES; x++) {

        Enemy *enemy = &enemies[x];
        EnemyStatus enemyStatus = enemy->getStatus();

        if (enemyStatus > EnemyStatus::Dead && enemyStatus < EnemyStatus::Active) {

          enemy->setStatus(--enemyStatus);

          if (enemyStatus == EnemyStatus::Dead) {
            level.decInPlay();
          }

        }

      }


      // Update dying player image ..

      PlayerStatus status = player.getStatus();

      if (status > PlayerStatus::Dead && status < PlayerStatus::Active) {

        player.setStatus(--status);

        if (status == PlayerStatus::Dead) {

          player.decLives();

          if (player.getLives() > 0) {
            gameState = GameState::Wave_Init;
          }
          else {
            gameState = GameState::GameOver_Init;
            
          }

        }

      }

    }



    // Update spawning enemy images ..

    if (arduboy.everyXFrames(FRAME_RATE_3)) {

      for (uint8_t x = 0; x < MAX_NUMBER_OF_ENEMIES; x++) {

        Enemy *enemy = &enemies[x];
        EnemyStatus enemyStatus = enemy->getStatus();

        if (enemyStatus > EnemyStatus::Active) {

          enemy->setStatus(--enemyStatus);

        }

      }

    }


    // Enemy takes a shot? 

    if (random(0, level.getBulletFrequency()) == 0) {

      bool bulletLaunched = false;

      for (uint8_t x = 0; x < MAX_NUMBER_OF_BULLETS; x++) {

        Bullet *bullet = &bullets[x];

        if (bullet->getY() == 0) { 
          
          uint8_t y = random(0, MAX_NUMBER_OF_ENEMIES);

          for (uint8_t x = 0; x < MAX_NUMBER_OF_ENEMIES; x++) {
            
            uint8_t x2 = (x + y) % MAX_NUMBER_OF_ENEMIES;

            Enemy *enemy = &enemies[x2];
            EnemyStatus enemyStatus = enemy->getStatus();

            if (enemy->getType() >= EnemyType::EnemyType1 && enemy->getType() <= EnemyType::EnemyType4 && enemy->getYDelta() >= 0 && enemyStatus == EnemyStatus::Active && enemy->inShootingRange()) {

              uint8_t enemyMiddle = enemy->getXDisplay() + (enemy->getWidth() / 2);

              bullet->setX(enemyMiddle);
              bullet->setY(absT(enemy->getYDisplay() + enemy->getHeight()));
              bullet->setYDelta(enemy->getYDelta() + 1);

              switch ((player.getX() + 7 - enemyMiddle) / (player.getY() - enemy->getYDisplay())) {

                case -999 ... -1:
                  bullet->setXDelta(-1);
                  break;

                case 0:
                  bullet->setXDelta(0);
                  break;

                case 1 ... 999:
                  bullet->setXDelta(1);
                  break;

              }

              sound.tones(enemy_fires_bullet);
              bulletLaunched = true;
              break;

            }

          }

        }

        if (bulletLaunched) break;

      }

    }


    // Are we out of fuel?

    if (player.getFuel() == 0 && player.getStatus() == PlayerStatus::Active) {

      player.setStatus(PlayerStatus::Explosion4);
      sound.tones(player_explosion);

    }


    // Launch a new formation ?

    level.decCountDown();

    if (level.getCountDown() == 0 && level.getInPlay() <= MAX_NUMBER_OF_ENEMIES - MAX_NUMBER_OF_ENEMIES_PER_FORMATION && level.getEnemiesLaunchedThisWave() < level.getEnemiesInWave()) {

      uint8_t numberOfEnemies = level.launchFormation(enemies);
      sound.tones(formation_launch[numberOfEnemies - 1]);

    }


    // Have we cleared the wave ?
    
    if (gameState == GameState::GamePlay && level.getInPlay() == 0 && level.getEnemiesLaunchedThisWave() >= level.getEnemiesInWave()) {

      fuelBonus = clamp(player.getFuel(), static_cast<uint8_t>(0), static_cast<uint8_t>(9));
      fuelBonusDisplay = 0;
      level.incWave();
      gameState = GameState::WaveCompleted_Init;

    }

  }


  // Flash the red LED? 

  #ifndef SINGLE_FLASH
  uint8_t health = player.getHealthCountdown();

  arduboy.setRGBled(RED_LED, 0);

  if (health > 0) {

    player.decHealthCountdown();

    if (health % 2 == 0) {

      #ifdef USE_INVERT
      arduboy.invert(true);
      #endif
      arduboy.setRGBled(RED_LED, 16);

    }

  }
  #endif

}
