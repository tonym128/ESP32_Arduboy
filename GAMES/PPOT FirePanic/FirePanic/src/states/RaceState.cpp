#include "RaceState.h"
#include "../images/Images.h"
#include "../utils/EEPROM_Utils.h"
#include "../utils/Enums.h"
#include "../sounds/Sounds.h"

extern ArduboyTones sound;

// ----------------------------------------------------------------------------
//  Initialise state ..
//
void RaceState::activate(StateMachine & machine) {

  auto & gameStats = machine.getContext().gameStats;
  auto & arduboy = machine.getContext().arduboy;  
  //auto & sound = machine.getContext().sound;  
  
  //sound.setOutputEnabled(arduboy.audio.enabled);

  this->ambulance.setX(-40);
  this->ambulance.setY(31);
  this->distance = DIST_MAXIMUM;
  this->slowDown = 3;
  this->showHospital = false;
  gameStats.health = RACE_PLAYER_HEALTH_MAX;

  this->carLaunch_RandomMax = OTHER_CAR_LAUNCH_MAX - (gameStats.level * OTHER_CAR_LAUNCH_MAX_DEC * 3);
  this->carLaunch_RandomMin = OTHER_CAR_LAUNCH_MIN - (gameStats.level * OTHER_CAR_LAUNCH_MIN_DEC * 3);

  if (this->carLaunch_RandomMax > OTHER_CAR_LAUNCH_MAX_FLOOR) this->carLaunch_RandomMax = OTHER_CAR_LAUNCH_MAX_FLOOR; 
  if (this->carLaunch_RandomMin > OTHER_CAR_LAUNCH_MIN_FLOOR) this->carLaunch_RandomMin = OTHER_CAR_LAUNCH_MIN_FLOOR; 

  for (auto &car : this->otherCars) {

    car.setEnabled(false);

  }

  BaseState::setPaused(false);

}


// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void RaceState::update(StateMachine & machine) {
	
  auto & arduboy = machine.getContext().arduboy;
  auto & gameStats = machine.getContext().gameStats;
  //auto & sound = machine.getContext().sound; 

  auto pressed = arduboy.pressedButtons();


  // Turn led off?

  if (this->ledCountdown > 0) {

    this->ledCountdown--;
    if (this->ledCountdown == 0)   arduboy.setRGBled(0, 0, 0);

  }

  if (!BaseState::getPaused()) {


    // Update coin index ..

    if (arduboy.everyXFrames(8)) {
      this->coin_index++;
      if (this->coin_index == 4) this->coin_index = 0;
    }


    if (gameStats.misses < 3) { if (this->distance > 0) this->distance--; }
    if (this->healthShowCountdown > 0) this->healthShowCountdown--;
    if (this->deathCountdown > 0) this->deathCountdown--;



    // Make the game a little harder ..

    if (this->distance % DIST_INTERVAL == 0) {

      this->carLaunch_RandomMax = OTHER_CAR_LAUNCH_MAX - (gameStats.level * OTHER_CAR_LAUNCH_MAX_DEC);
      this->carLaunch_RandomMin = OTHER_CAR_LAUNCH_MIN - (gameStats.level * OTHER_CAR_LAUNCH_MIN_DEC);

      if (this->carLaunch_RandomMax > OTHER_CAR_LAUNCH_MAX_FLOOR) this->carLaunch_RandomMax = OTHER_CAR_LAUNCH_MAX_FLOOR; 
      if (this->carLaunch_RandomMin > OTHER_CAR_LAUNCH_MIN_FLOOR) this->carLaunch_RandomMin = OTHER_CAR_LAUNCH_MIN_FLOOR; 

    }


    // Game over?

    if (gameStats.misses >= 3) {

      if (this->ambulance.getX() >- 120) {

        if (arduboy.everyXFrames(5)) {
          this->ambulance.decX(2);
        }

      }
      else {
        gameStats.gameOver = true;
      }
      
    }

        
    // Ambulance is still entering screen ..

    else if (this->ambulance.getX() < 0) {

      if (arduboy.everyXFrames(5)) {
        this->ambulance.incX(2);
      }

    }


    // Have we completed the race?  If so, move the ambulance off to the right ..

    else if (this->distance == 0 && this->ambulance.getX() < DIST_DELAY_AFTER_AMBULANCE_LEAVES_SCREEN) {

      if (arduboy.everyXFrames(5)) {
        this->ambulance.incX(2);
      }

    }


    // If the game is over and the ambulance has left the screen, then move to the other game ..

    else if (this->distance == 0 && this->ambulance.getX() >= DIST_DELAY_AFTER_AMBULANCE_LEAVES_SCREEN) {

      gameStats.level++;
      machine.changeState(GameStateType::GameIntroScreen_ChangeDay, GameStateType::PlayGameScreen); 

    }


    // Play game ..

    else {


      // Clear 'do notmove flag'.  This is used to not move a car if it is being pushed by the ambulance ..

      for (auto &car : this->otherCars) {
        car.setDoNotMove(false);
      }


      // Left --------------------------------------------------------------------------------------------

      if ((pressed & LEFT_BUTTON) && this->ambulance.getX() > 0)           { this->ambulance.decX(); }


      // Right -------------------------------------------------------------------------------------------

      if (arduboy.getFrameCount(3) < 2 && (pressed & RIGHT_BUTTON) && this->ambulance.getX() < 70) { 

        uint8_t carCollision = checkForCollisions(arduboy, this->ambulance.getX() + 1, this->ambulance.getY());

        if (carCollision < CAR_COLLISION_NONE) {
          
          if (this->ambulance.incPuffIndexIfZero(Direction::Right)) {
            decHealth(machine);
            sound.tones(Sounds::Crash);
          }

          this->otherCars[carCollision].setX(this->ambulance.getX() + RACE_AMBULANCE_WIDTH);
          this->otherCars[carCollision].setDoNotMove(true);

        }
        else {
          this->ambulance.incX();
        }
        
      }

 
      // Up ----------------------------------------------------------------------------------------------
 
      if (pressed & UP_BUTTON && this->ambulance.getY() > 7) {

        if (checkForCollisions(arduboy, this->ambulance.getX(), this->ambulance.getY() - 1) < CAR_COLLISION_NONE) {

          if (this->ambulance.incPuffIndexIfZero(Direction::Up)) {
            decHealth(machine);
            sound.tones(Sounds::Crash);
          }

        }
        else {
          this->ambulance.decY();
        }

      }

      if (this->ambulance.getDirection() == Direction::Up) {

        if (!(pressed & UP_BUTTON)) {
          if (checkForCollisions(arduboy, this->ambulance.getX(), this->ambulance.getY() - 1) < CAR_COLLISION_NONE) {

            if (this->ambulance.incPuffIndexIfZero(Direction::Up)) {
              decHealth(machine);
              sound.tones(Sounds::Crash);
            }

          }
          else {
            this->ambulance.decY();
          }
        }

        checkRoadExtents();

      }


      // Down ----------------------------------------------------------------------------------------------

      if (pressed & DOWN_BUTTON && this->ambulance.getY() < 33) {

        if (checkForCollisions(arduboy, this->ambulance.getX(), this->ambulance.getY() + 1) < CAR_COLLISION_NONE) {

          if (this->ambulance.incPuffIndexIfZero(Direction::Down)) {
            decHealth(machine);
            sound.tones(Sounds::Crash);
          }

        }
        else {
          this->ambulance.incY();
        }

      }

      if (this->ambulance.getDirection() == Direction::Down) {

        if (!(pressed & DOWN_BUTTON)) {
          if (checkForCollisions(arduboy, this->ambulance.getX(), this->ambulance.getY() + 1) < CAR_COLLISION_NONE) {

            if (this->ambulance.incPuffIndexIfZero(Direction::Down)) {
              decHealth(machine);
              sound.tones(Sounds::Crash);
            }

          }
          else {
            this->ambulance.incY();
          }
        }

        checkRoadExtents();

      }


      // Launch another car?  Not if we are about to finish the race ..

      if (this->distance > DIST_NO_NEW_CARS) {

        for (auto &car : this->otherCars) {

          if (!car.getEnabled()) {

            do {
              car.setX(random(128, 300));
              car.setLane(random(0, 3));
              car.setType(random(0, 2));
              car.setSpeed(random(2, 4)); // rnadom(1,4) for fast!
              car.setEnabled(true);
            }
            while (checkLaunchCollisions(car.getX(), car.getLane()));

          }

        }

      }


      // Launch the coin?  Not if we are about to finish the race ..

      if (this->distance > DIST_NO_NEW_CARS && coin.getCountdown() == 0) {

        if (!coin.getEnabled()) {

          this->coin.setX(random(128, 340));
          this->coin.setLane(random(0, 3));
          this->coin.setEnabled(true);

        }

      }
      else {
        coin.decCountdown();
      }


      // Update positions ..

      if (arduboy.getFrameCount(3) < 2) {        

        if (this->coin.getEnabled()) {
          
          this->coin.move();

          Rect coinRect = { this->coin.getX(), 27 + (this->coin.getLane() * 14), 8, 8 };
          Rect ambulanceRect = { this->ambulance.getX(), this->ambulance.getY() + 21, RACE_AMBULANCE_WIDTH, 10 };

          if (arduboy.collide(coinRect, ambulanceRect)) {

            arduboy.setRGBled(0, LED_BRIGHTNESS, 0);
            this->ledCountdown = 10;

            gameStats.score = gameStats.score + 2;
            this->coin.setEnabled(false);
            sound.tones(Sounds::Coin);

          }

        }

      }


      // Update car positions ..

      for (auto &car : this->otherCars) {
        
        if (car.getEnabled() && !car.getDoNotMove()) {

          if (checkForCollision(arduboy, this->ambulance.getX(), this->ambulance.getY(), car.getX(), car.getLane())) {
            car.setX(ambulance.getX() + RACE_AMBULANCE_WIDTH);

            if (this->ambulance.incPuffIndexIfZero(Direction::Right)) {
              decHealth(machine);
              sound.tones(Sounds::Crash);
            }
            
          }
          else {
            car.setX(car.getX() - (arduboy.getFrameCount(car.getSpeed()) == 0/*< car.speed - 1*/ ? 2 : 1));
          }

        }

      }


      // Make sure one car is not going through another ..

      bool retest = true;

      while (retest) {

        retest = false;

        for (uint8_t i = 0; i < RACE_OTHERCARS_MAX; i++) {

          for (uint8_t j = 0; j < RACE_OTHERCARS_MAX; j++) {
    
            if (i != j) {

              OtherCar &car1 = this->otherCars[i];
              OtherCar &car2 = this->otherCars[j];

              if (car1.getEnabled() && car2.getEnabled() && car1.getLane() == car2.getLane() && absT(car1.getX() - car2.getX()) <= RACE_OTHERCAR_WIDTH) {

                if (car1.getX() > car2.getX()) {
                  car1.setX(car2.getX() + RACE_OTHERCAR_WIDTH + 1);
                }
                else {
                  car2.setX(car1.getX() + RACE_OTHERCAR_WIDTH + 1);
                }

                retest = true;
                break;

              }

            }

          }

          if (retest) break;

        }

      }

    }

    updateSceneryPositions(arduboy);

  }

  if (arduboy.everyXFrames(6)) {
    this->ambulance.incPuffIndexes();
  }


  // Handle other buttons ..

  BaseState::handleCommonButtons(machine);

}


// ----------------------------------------------------------------------------
//  Check to see if hte ambulance is at the centre of a lane.  If so stop .. 
//
void RaceState::checkRoadExtents() {

  if (this->ambulance.getY() == 7 || this->ambulance.getY() == 20 || this->ambulance.getY() == 33) {
    this->ambulance.setDirection(Direction::None);
  }

}


// ----------------------------------------------------------------------------
//  Decrease player health if they have had an accident .. 
//
void RaceState::decHealth(StateMachine & machine) {
  
	auto & arduboy = machine.getContext().arduboy;
  auto & gameStats = machine.getContext().gameStats;
  //auto & sound = machine.getContext().sound;

  arduboy.setRGBled(LED_BRIGHTNESS, 0, 0);
  this->ledCountdown = 10;

  gameStats.health = gameStats.health - RACE_PLAYER_HEALTH_DEC;
  this->healthShowCountdown = 100;

  if (gameStats.health <= 0) {

    gameStats.misses++;
    sound.tones(Sounds::VictimDead);

    if (gameStats.misses < 3) {

      gameStats.health = RACE_PLAYER_HEALTH_MAX;
      this->deathCountdown = 200;

    }

  }

}


// ----------------------------------------------------------------------------
//  Update positions of hte scenery elements .. 
//
void RaceState::updateSceneryPositions(Arduboy2Ext & arduboy) {

  if (arduboy.getFrameCount(2) < 1) {

    this->xScenery--;
    if (this->xScenery == -64) this->xScenery = 0;
    if (this->showHospital) this->xHospital--;

    if (this->distance == 0 && this->showHospital == false) {
      this->showHospital = true;
      this->xHospital = this->xScenery + (64 * 3);
    }

  }

  if (arduboy.getFrameCount(3) < 2) {

    this->xGrass--;
    if (this->xGrass == -16) this->xGrass = 0;

  }

  if (arduboy.getFrameCount(4) < 3) {        

    this->xLine1 = this->xLine1 - 2;
    if (this->xLine1 <= -32) this->xLine1 = 0;
  }

  if (arduboy.getFrameCount(5) < 3) {        

    this->xLine2 = this->xLine2 - 3;
    if (this->xLine2 <= -32) this->xLine2 = 0;

  }

}


// ----------------------------------------------------------------------------
//  Check for a collision between the car and others when launching .. 
//
bool RaceState::checkLaunchCollisions(int16_t x, uint8_t lane) {

  uint8_t collision = 0;

  for (auto &car : this->otherCars) {

    if (car.getEnabled() && car.getLane() == lane && absT(car.getX() - x) <= (RACE_OTHERCAR_WIDTH + 20)) {
      collision++;
    }

  }

  return collision != 1;

}


// ----------------------------------------------------------------------------
//  Check for a collision between the ambulance and any car .. 
//
uint8_t RaceState::checkForCollisions(Arduboy2Ext & arduboy, int16_t x, uint8_t y) {

  Rect ambulanceRect = { x, y + 21, RACE_AMBULANCE_WIDTH, 10 };

  for (uint8_t i = 0; i < 3; i++) {

    auto &car = this->otherCars[i];

    if (car.getEnabled()) {

      Rect carRect = { car.getX(), 27 + (car.getLane() * 14), RACE_OTHERCAR_WIDTH, 10 };

      if (arduboy.collide(ambulanceRect, carRect)) return i;

    }

  }

  return CAR_COLLISION_NONE;

}


// ----------------------------------------------------------------------------
//  Check for a front collision between the ambulance and a nominated car .. 
//
bool RaceState::checkForCollision(Arduboy2Ext & arduboy, int16_t ambulanceX, uint8_t ambulanceY, int16_t carX, uint8_t carLane) {

  Rect ambulanceRect = { ambulanceX + RACE_AMBULANCE_WIDTH - 1, ambulanceY + 21, 2, 10 };
  Rect carRect = { carX, 27 + (carLane * 14), 2, 10 };

  if (arduboy.collide(ambulanceRect, carRect)) return true;

  return false;

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void RaceState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
  auto & gameStats = machine.getContext().gameStats;


  // Render background ..

  for (uint8_t i = 0; i < 4; i++) {

    int16_t x = this->xScenery + (i*64);

    if (!this->showHospital || (x != this->xHospital)) {
    
      SpritesB::drawExternalMask(x, 0, Images::Building_BG, Images::Building_BG_Mask, 0, 0);

    }

    if (this->showHospital) {

      SpritesB::drawExternalMask(this->xHospital, 0, Images::Hospital, Images::Hospital_Mask, 0, 0);

    }

  }

  for (uint8_t i = 0; i < 10; i++) {

    SpritesB::drawExternalMask(this->xGrass + (i*16), 20, Images::Race_Grass, Images::Race_Grass_Mask, 0, 0);

  }


  // Render score ..

  BaseState::renderScore(machine, this->healthShowCountdown, (gameStats.health) / 5);


  // Render misses ..

  bool deathFlash = true;
  if (this->deathCountdown > 0) deathFlash = ((this->deathCountdown / 30) % 2) == 1;
  BaseState::renderMisses(machine, deathFlash);


  // Render road lines ..

  for (uint8_t i = 0; i < 5; i++) {

    uint8_t z = i * 32;
    SpritesB::drawOverwrite(this->xLine1 + z, 40, Images::Race_Line, 0);
    SpritesB::drawOverwrite(this->xLine2 + z, 53, Images::Race_Line, 0);

  }


  // Render ambulance and cars from top to bottom ..

  for (uint8_t iLane = 0; iLane < 3; iLane++) {


    // Draw coin ..

    if (this->coin.getEnabled() && iLane == this->coin.getLane()) {

      SpritesB::drawExternalMask(this->coin.getX(), 30 + (this->coin.getLane() * 13) + (this->coin.getLane() == 0 ? 1 : 0), Images::Race_Prize, Images::Race_Prize_Mask, this->coin_index, this->coin_index);

    }


    // Draw other cars ..

    for (auto &car : this->otherCars) {

      if (car.getEnabled() && car.getLane() == iLane) {

        SpritesB::drawExternalMask(car.getX(), 16 + (car.getLane() * 13), Images::Race_OtherCar, Images::Race_OtherCar_Mask, car.getType(), car.getType());
        
      }

    }



    // Draw Ambulance ..

    if (iLane == this->ambulance.getLane()) {
 

      // Draw puff above (and behind) the ambulance if it has crashed into something above ..

      if (this->ambulance.getPuffIndex(Direction::Up) > 0) {

        uint8_t puffIndex_Mask = this->ambulance.getPuffIndex(Direction::Up) - 1;
        uint8_t puffIndex = (puffIndex_Mask * 2);

        BaseState::renderPuff(this->ambulance.getX() + (RACE_AMBULANCE_WIDTH / 2) - 16, this->ambulance.getY(), puffIndex, puffIndex_Mask);

      }

      BaseState::renderAmbulance(machine, this->ambulance.getX(), this->ambulance.getY(), false);


      // Draw puffs if the ambulance has crashed into something ..

      const int8_t puffConsts[] = { static_cast<int8_t>(Direction::Right), RACE_AMBULANCE_WIDTH - 8, 16, 
                                    static_cast<int8_t>(Direction::Left), -8, 16, 
                                    static_cast<int8_t>(Direction::Down), (RACE_AMBULANCE_WIDTH / 2) - 8, 12 };

      for (uint8_t i = 0; i< 3; i++) {

        Direction direction = static_cast<Direction>(puffConsts[i * 3]);
        
        uint8_t puffIndex = this->ambulance.getPuffIndex(direction);
        int8_t x = puffConsts[(i * 3) + 1];
        int8_t y = puffConsts[(i * 3) + 2];
        
        if (puffIndex > 0) {

          uint8_t puffIndex_Mask = puffIndex - 1;
          uint8_t puffIndex = (puffIndex_Mask * 2);

          BaseState::renderPuff(this->ambulance.getX() + x, this->ambulance.getY() + y, puffIndex, puffIndex_Mask);

        }

      }

    }

  }


  // Render Game Over or Pause?

  BaseState::renderGameOverOrPause(machine);

	arduboy.displayWithBackground(gameStats.timeOfDay == TimeOfDay::Day ? TimeOfDay::Mixed : TimeOfDay::Night);
 
}

