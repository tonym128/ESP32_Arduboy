#include "PlayGameState.h"

#include "../utils/Arduboy2Ext.h"
#include "../utils/Physics.h"
#include "../images/Images.h"
#include "../sounds/Sounds.h"
#include <ArduboyTones.h>

extern ArduboyTones sound;

const uint8_t PROGMEM init_turtles[25] = { 
  11, 29, static_cast<uint8_t>(Direction::Left), static_cast<uint8_t>(TurtleMode::LookingDown), 0,
  32, 29, static_cast<uint8_t>(Direction::Left), static_cast<uint8_t>(TurtleMode::LookingDown), 20,
  53, 29, static_cast<uint8_t>(Direction::Left), static_cast<uint8_t>(TurtleMode::LookingDown), 40,
  74, 29, static_cast<uint8_t>(Direction::Right), static_cast<uint8_t>(TurtleMode::LookingDown), 60,
  95, 29, static_cast<uint8_t>(Direction::Right), static_cast<uint8_t>(TurtleMode::LookingDown), 80,
};


// ----------------------------------------------------------------------------
//  Initialise state ..
//
void PlayGameState::activate(StateMachine & machine) {

  auto & gameStats = machine.getContext().gameStats;
  auto & arduboy = machine.getContext().arduboy;
  //auto & sound = machine.getContext().sound;  

  gameStats.gameOver = false;

  this->playing = false;
  

  // Initialise turtles ..

  for (uint8_t x = 0; x < 5; x++) {
  
    this->turtles[x].setX(pgm_read_byte(&init_turtles[(x * 5)]));
    this->turtles[x].setY(pgm_read_byte(&init_turtles[(x * 5) + 1]));
    this->turtles[x].setDirection(static_cast<Direction>(pgm_read_byte(&init_turtles[(x * 5) + 2])));
    this->turtles[x].setMode(static_cast<TurtleMode>(pgm_read_byte(&init_turtles[(x * 5) + 3])));
    this->turtles[x].setBobUpDown(pgm_read_byte(&init_turtles[(x * 5) + 4]));

  }

  this->stickHeadUp.min = 160;
  this->stickHeadUp.max = 200;
  this->stickHeadUp.counter = random(this->stickHeadUp.min, this->stickHeadUp.max);


  // Initialise fishes ..

  if (gameStats.mode == GameMode::Easy) {

    this->fishLaunch.min = fish_launch_easy_init_min;
    this->fishLaunch.max = fish_launch_easy_init_max;
    this->fishSpeed.min = fish_speed_easy_init_min;
    this->fishSpeed.max = fish_speed_easy_init_max;

  }
  else {

    this->fishLaunch.min = fish_launch_hard_init_min;
    this->fishLaunch.max = fish_launch_hard_init_max;
    this->fishSpeed.min = fish_speed_hard_init_min;
    this->fishSpeed.max = fish_speed_hard_init_max;

  }

  for (uint8_t x = 0; x < 5; x++) {

    auto &fish = this->fishes[x];
    fish.setFishIndex(x);
    fish.setEnabled(false);

  }

  this->fishLaunch.counter = random(this->fishLaunch.min, this->fishLaunch.max);


  // Initialise player ..

  this->player.initLife();
  this->player.setHoldingPackage(true);
  this->prevTurtleIndex = -1;


  // Initialise recipient ..

  this->recipient.visible.min = RECIPIENT_VISIBLE_LIMIT_MIN;
  this->recipient.visible.max = RECIPIENT_VISIBLE_LIMIT_MAX;
  this->recipient.reappear.min = RECIPIENT_REAPPEAR_LIMIT_MIN;
  this->recipient.reappear.max = RECIPIENT_REAPPEAR_LIMIT_MAX;
  this->recipient.x = RECIPIENT_X_DEFAULT;
  this->recipient.visible.counter = random(this->recipient.visible.min, this->recipient.visible.max);
  
  this->flashArrow = 216;

  BaseState::initWater();
  BaseState::setPaused(false);

  //sound.setOutputEnabled(arduboy.audio.enabled);

}
  

// ----------------------------------------------------------------------------
//  Handle state updates .. 
//
void PlayGameState::update(StateMachine & machine) { 

	auto & arduboy = machine.getContext().arduboy;
  auto & gameStats = machine.getContext().gameStats;
  //auto & sound = machine.getContext().sound;  
	auto pressed = arduboy.pressedButtons();
	auto justPressed = arduboy.justPressedButtons();

  if (!BaseState::getPaused()) {

    if (this->fishLaunch.counter  > 0)    this->fishLaunch.counter--;
    if (this->stickHeadUp.counter > 0)    this->stickHeadUp.counter--;


    // Launch a fish?

    if (this->fishLaunch.counter  == 0) {

      this->fishLaunch.counter = random(this->fishLaunch.min, this->fishLaunch.max);
      uint8_t fishIndex = getDisabledFish(machine);

      if (fishIndex != FISH_NONE) {

        auto &fish = this->fishes[fishIndex];
        fish.setEnabled(true);
        fish.setDelay(random(this->fishSpeed.min, this->fishSpeed.max));

      }

    }


    // Stick turtles head up?

    if (this->stickHeadUp.counter == 0) {

      uint8_t turtleIndex = this->player.getTurtleIndex();
      uint8_t turtleIndexPrev = this->player.getTurtleIndexPrev();

      if (turtleIndexPrev != TURTLE_NONE && turtleIndex == TURTLE_NONE && 
        (turtleIndexPrev !=5 || (turtleIndexPrev == 5 && this->player.getDirection() == Direction::Left))) {

        auto &fish = this->fishes[turtleIndexPrev];

        if (!fish.getEnabled()) {   

          auto &turtle = this->turtles[turtleIndexPrev];
          this->stickHeadUp.counter = random(this->stickHeadUp.min, this->stickHeadUp.max);
          turtle.setMode(TurtleMode::LookingUp);

        }

      }

    }


    // Update player position ..

    if (this->playing && !gameStats.gameOver) {

      if (this->flashArrow > 0)            this->flashArrow--;

      if (arduboy.everyXFrames(2)) {
          
        if ((pressed & LEFT_BUTTON) && this->player.canMoveLeft()) { 
          this->player.setDirection(Direction::Left);
        }

        if ((pressed & RIGHT_BUTTON) && this->player.canMoveRight()) { 
          this->player.setDirection(Direction::Right);
        }
      
        this->player.move(this->turtles[0].getMode() == TurtleMode::Diving, 
                          this->turtles[1].getMode() == TurtleMode::Diving, 
                          this->turtles[2].getMode() == TurtleMode::Diving, 
                          this->turtles[3].getMode() == TurtleMode::Diving, 
                          this->turtles[4].getMode() == TurtleMode::Diving);


        // Play note when on turtle's back ..

        uint8_t turtleIndex = this->player.getTurtleIndex();


        // The @UXE fix for sound ..

        if (this->player.isLeftCliffPosition() || (this->player.isPackagePosition() && !this->player.isHoldingPackage())) {
          this->prevTurtleIndex = -1;
        }

        if (turtleIndex != TURTLE_NONE && turtleIndex != this->prevTurtleIndex) {
          this->prevTurtleIndex = turtleIndex;
          sound.tones(Sounds::Turtle);
        }                  
        
        if (this->player.isPackagePosition() && this->player.isHoldingPackage()) {

          if (this->recipient.x < RECIPIENT_X_DEFAULT + 5) {
            
            gameStats.score++;

            if (gameStats.mode == GameMode::Easy) {

              if (gameStats.score % fish_launch_easy_mod_min == 0)     this->fishLaunch.min--; 
              if (gameStats.score % fish_launch_easy_mod_max == 0)     this->fishLaunch.max--; 

              if (gameStats.score % fish_speed_easy_mod_min == 0)      this->fishSpeed.min--; 
              if (gameStats.score % fish_speed_easy_mod_max == 0)      this->fishSpeed.max--; 

            }
            else {

              if (gameStats.score % fish_launch_hard_mod_min == 0)     this->fishLaunch.min--; 
              if (gameStats.score % fish_launch_hard_mod_max == 0)     this->fishLaunch.max--; 

              if (gameStats.score % fish_speed_hard_mod_min == 0)      this->fishSpeed.min--; 
              if (gameStats.score % fish_speed_hard_mod_max == 0)      this->fishSpeed.max--; 

            }

            this->player.setHoldingPackage(false);
            this->newPackage = true;
            sound.tones(Sounds::PackageDelivered);

          }
          else {
            sound.tones(Sounds::PackageNotDelivered);
          }

        }

        if (this->player.isLeftCliffPosition() && this->newPackage) {
          this->player.setHoldingPackage(true);
          this->newPackage = false;
          sound.tones(Sounds::PackagePickedUp);
        }


        // Update recipient position and counters ..

        if (this->recipient.visible.counter > 0) {

          this->recipient.visible.counter--;
          this->recipient.x = RECIPIENT_X_DEFAULT;

          if (this->recipient.visible.counter == 0) {

            this->recipient.reappear.counter = 0;
            this->recipient.reappear.upperLimit = random(this->recipient.reappear.min, this->recipient.reappear.max);

          }

        }
        else {

          if (this->recipient.reappear.counter < recipient.reappear.upperLimit) {

            this->recipient.reappear.counter++;
            this->recipient.x = RECIPIENT_X_DEFAULT + (this->recipient.reappear.counter < recipient.reappear.upperLimit / 2 ? this->recipient.reappear.counter : this->recipient.reappear.upperLimit - this->recipient.reappear.counter);

            if (this->recipient.reappear.counter == this->recipient.reappear.upperLimit) {

              this->recipient.visible.counter = random(this->recipient.visible.min, this->recipient.visible.max);

            }

          }

        }   

      }

    }

  }


  // Update fish image indexes ..

  if (arduboy.everyXFrames(16)) {

    for (auto &fish : this->fishes) {

      fish.updateImageIndex();

    }

  }

  BaseState::updateWater(machine);


  // Update fish locations ..

  if (!this->getPaused()) {

    for (uint8_t x = 0; x < TURTLES_COUNT; x++) {
    
      auto &fish = this->fishes[x];
      auto &turtle = this->turtles[x];

      fish.updatePositionIndex();

      if (fish.isEdible()) {
        turtle.setMode(TurtleMode::Diving);
      }

      turtle.updateMode();

    }

  }


  // Has the player died ?

  if (this->player.isDying() && !sound.playing()) {

    sound.tones(Sounds::Drowning);

  }

  if (this->player.isDead() && this->playing) {

    this->playing = false;
    gameStats.numberOfLivesLeft--;

    if (gameStats.numberOfLivesLeft == 0) {
      gameStats.gameOver = true;
    }

  }



  // Handle other buttons ..

  if (!this->playing && !gameStats.gameOver) {

    if ((justPressed & A_BUTTON) || (justPressed & B_BUTTON)) {

      sound.tones(Sounds::PackageNotDelivered);
      this->playing = true;
      this->player.initLife();
      this->prevTurtleIndex = -1;

    }

  }
  else {  
    BaseState::handleCommonButtons(machine);
  }

}


// ----------------------------------------------------------------------------
//  Get a random fish that is disabled .. 
//
uint8_t PlayGameState::getDisabledFish(StateMachine & machine) {

  auto & gameStats = machine.getContext().gameStats;
  
  uint8_t fishes[5];
  uint8_t fishCount = 0;

  for (uint8_t x = 0; x < TURTLES_COUNT; x++) {
  
    auto fish = this->fishes[x];

    if (!fish.getEnabled() && (gameStats.mode == GameMode::Hard || (gameStats.mode == GameMode::Easy && x != 2))) {
      fishes[fishCount] = x;
      fishCount++;
    }

  }

  if (fishCount > 0) {
    return fishes[random(0, fishCount)];
  }
  else {
    return FISH_NONE;
  }

}


// ----------------------------------------------------------------------------
//  Render the state .. 
//
void PlayGameState::render(StateMachine & machine) {

	auto & arduboy = machine.getContext().arduboy;
  auto & gameStats = machine.getContext().gameStats;

  BaseState::renderCommonScenery(machine);



  // Render Fishes ..

  for (uint8_t x = 0; x < TURTLES_COUNT; x++) {

    auto fish = this->fishes[x];

    if (fish.getEnabled()) {
      Sprites::drawExternalMask(fish.getDisplayX(), fish.getDisplayY(), Images::Fish, Images::Fish_Mask, fish.getImageIndex() + (x < 3 ? 2 : 0), fish.getImageIndex() + (x < 3 ? 2 : 0));
    }

  }


  // Render turtles ..

  for (auto &turtle : this->turtles) {
  
    if (turtle.getType() == TurtleType::Black) {
      Sprites::drawExternalMask(turtle.getDisplayX(), turtle.getDisplayY(), Images::Turtle_03, Images::Turtle_02_03_Mask, turtle.getImageIndex(), turtle.getImageIndex());
    }
    else {
      Sprites::drawExternalMask(turtle.getDisplayX(), turtle.getDisplayY(), Images::Turtle_01, Images::Turtle_01_Mask, turtle.getImageIndex(), turtle.getImageIndex());
    }

  }


  // Render package ..

  if (this->newPackage) {
    Sprites::drawExternalMask(1, 8, Images::Package, Images::Package_Mask, 0, 0);
  }


  // Render score ..

  BaseState::renderScore(machine, false, 0);


  // Render recipient ..

  Sprites::drawExternalMask(this->recipient.x + 3, 11, Images::Recipient, Images::Recipient_Mask, 0, 0);


  // Render player ..

  uint8_t turtleIndex = this->player.getTurtleIndex();
  Turtle &turtle = this->turtles[turtleIndex != TURTLE_NONE ? turtleIndex : 0];

  Sprites::drawExternalMask(this->player.getDisplayX(), this->player.getDisplayY(turtleIndex != TURTLE_NONE ? (turtle.getBobUp() ? 1 : 0) : 0), Images::Arduboy, Images::Arduboy_Mask, this->player.getImageIndex(), this->player.getImageIndex());

  if (this->player.showTombstone()) {
    Sprites::drawExternalMask(this->player.getDisplayX(), this->player.getTombstoneY(), Images::Arduboy, Images::Arduboy_Mask, static_cast<uint8_t>(Player_Positions::Tombstone), static_cast<uint8_t>(Player_Positions::Tombstone));
  }

  BaseState::renderWater();


  // Render flashing arrow ..

  if (this->flashArrow > 0 && (this->flashArrow / 24) % 2 == 0) {
    Sprites::drawExternalMask(115, 1, Images::DownArrow, Images::DownArrow_Mask, 0, 0);
  }


  if (!this->playing && !gameStats.gameOver) {

    Sprites::drawExternalMask(27, 20, Images::PlayerFrame, Images::PlayerFrame_Mask, 0, 0);
    Sprites::drawSelfMasked(79, 23, Images::Player_Number, gameStats.numberOfLivesLeft - 1);

  }
  else {

    BaseState::renderGameOverOrPause(machine);

  }
  arduboy.displayWithBackground();

}
