#include "src/Utils/Arduboy2Ext.h"

#include <ESP8266WiFi.h>
#include <ArduboyTones.h>
#include "src/Images/Images.h"
#include "src/Entities/Entities.h"
#include "src/Utils/Utils.h"
#include "src/Utils/Constants.h"
#include "src/Utils/EEPROM_Utils.h"
#include "src/Utils/FadeEffects.h"
#include <FixedPoints.h>
#include <FixedPointsCommon.h>
#include "src/Fonts/Font4x6.h"
#include "src/Utils/HighScoreEditor.h"
#include "src/Sounds/Themes.h"

Arduboy2Ext arduboy;
ArduboyTones sound(arduboy.audio.enabled);
Sprites sprite;

Player player;
Customer customer;

Font4x6 font4x6 = Font4x6(0);

FadeOutEffect fadeOutEffect;
FadeInEffect fadeInEffect;
HighScore highScore;


// Global Variables now --------------------------------------------------------

GameState state = GameState::VSBoot;
GameState nextState = GameState::VSBoot;

uint8_t levelNumber = 0;
uint8_t thrusterFrame = 0;
int16_t backdropx = 0;
uint8_t alternate = 0;
uint8_t fareCount = 0;
uint8_t dollarsCount = 0;
uint8_t flashingCounter = 0;
uint8_t bootCounter = 50;
uint8_t gotoCounter = 0;
uint8_t ouchCounter = 0;
uint8_t gateToRender = 1; // For introduction.
bool starsInitialised = false;

Level level;


//------------------------------------------------------------------------------
//  Setup 
//------------------------------------------------------------------------------

void setup() {
  WiFi.mode(WIFI_OFF);
  arduboy.boot();
  arduboy.display();
  arduboy.flashlight();
  arduboy.systemButtons();
  arduboy.audio.begin();
  arduboy.clear();
  arduboy.setFrameRate(60);

  EEPROM_Utils::initEEPROM(false);
    
}

void loop() {
  
  if (!(arduboy.nextFrame())) return;

  arduboy.pollButtons();

  switch (state) {

    case GameState::VSBoot:
      vsBoot();
      break;

    case GameState::SplashScreen_Init:
      gotoCounter = 0;
      ouchCounter = 0;
      levelNumber = 0;
      gateToRender = 1;
      initLevel(level, player, customer, levelNumber);
      launchCustomer(level, customer, gateToRender, GO_TO_GATE);
      player.setFrame(0);
      state = GameState::SplashScreen;
      fadeInEffect.reset(0, HEIGHT, 1);
      
      // break;  -- Fall through intentional.

    case GameState::SplashScreen:
      titleScreen(level, player, customer);
      break;

    case GameState::PlayGame_InitGame:
      levelNumber = 1;
      initGame(player);
      state = GameState::PlayGame_LevelIntroduction_Init;
      // break;  -- Fall through intentional.

    case GameState::PlayGame_LevelIntroduction_Init:
      level.reset(levelNumber);
      sound.tones(LevelTransition_Theme);
      state = GameState::PlayGame_LevelIntroduction;
      // break;  -- Fall through intentional.

    case GameState::PlayGame_LevelIntroduction:
      drawLevelStart(font4x6, level);
      nextState = GameState::PlayGame_FlashingCar;
      break;

    case GameState::PlayGame_InitLevel:
      thrusterFrame = 0;
      initLevel(level, player, customer, levelNumber);
      launchCustomer(level, customer, RANDOM_START_POSITION, RANDOM_END_POSITION);
      state = nextState;
      // break;  -- Fall through intentional.
        
    case GameState::PlayGame_FlashingCar:
    case GameState::PlayGame:
      #ifdef DEBUG
      Serial.print("PX: ");
      Serial.print(player.getX().getInteger());
      Serial.print(", PY: ");
      Serial.print(player.getY().getInteger());
      Serial.print(" - LX: ");
      Serial.print(level.xOffset.getInteger());
      Serial.print(", LY: ");
      Serial.print(level.yOffset.getInteger());
      Serial.println(", Fuel: ");
      #endif

      inGame(font4x6, level, player, customer);
      break;

    case GameState::MaxLevelCompleted:
      drawMaxLevelCompleted(font4x6);
      break;

    case GameState::GameOver_Init:
      sound.tones(GameOver_Theme);
      state = GameState::GameOver;
      // break;  -- Fall through intentional.

    case GameState::GameOver:
      gameoverScreen();
      levelNumber = 0;
      break;

    case GameState::SaveScore:
      highScore.reset();
      highScore.setSlotNumber(EEPROM_Utils::saveScore(player.getScore()));
      state = GameState::HighScore;
      fadeInEffect.reset(0, HEIGHT, 1);
      // break; Fall-through intentional.

    case GameState::HighScore:
      renderHighScore(highScore);
      break;

  }

  arduboy.display(true);

  switch (state) {

    case GameState::SplashScreen:
    case GameState::PlayGame:
      moveCab(level, player, customer);
      break;

    default: break;

  }

}
