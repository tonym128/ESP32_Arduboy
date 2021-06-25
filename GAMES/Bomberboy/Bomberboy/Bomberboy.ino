#include "defines.h"
#include "Menu.h"
#include "Game.h"

//#ifdef ESP8266
//#define sprites arduboy_sprites
//#else
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);
Sprites sprites;
//#endif

#define MODE_MENU 0
#define MODE_GAME 1
#define MODE_FINAL_CUT_SCENE 2
#define MODE_GAME_INFO 3

static uint8_t s_mode;

//#ifdef ESP8266
//void Game_Bomberboy_setup()
//#else
void inogamesetup()
//#endif
{  
 // #ifdef ESP8266
 // arduboy_tones = new ArduboyTones(arduboy.audio.enabled);
 // #endif

  arduboy.begin();
  arduboy.setFrameRate(TARGET_FRAMERATE);
  s_mode = MODE_MENU;
  Bomberboy::Menu::Init();

  //Bomberboy::Game::Init();
  //Bomberboy::Game::StartLevel();
  
  //s_mode = MODE_FINAL_CUT_SCENE;
  //Bomberboy::FinalCutScene::Init();
}

//#ifdef ESP8266
//void Game_Bomberboy_loop()
//#else
void inogameloop()
//#endif
{
  if (!arduboy.nextFrame())
    return;

  if (s_mode == MODE_MENU)
  {
    if (Bomberboy::Menu::Control(arduboy, Arduboy2Core::buttonsState(), arduboy.frameCount))
    {
      s_mode = MODE_GAME;
      Bomberboy::Game::Init();
      Bomberboy::Game::StartLevel();
    } else if ( Bomberboy::Menu::m_present_time >= GAME_INFO_ACTIVATION_FRAMES )
    {
      s_mode = MODE_GAME_INFO;
      Bomberboy::GameInfoScene::Init();
    } else
      Bomberboy::Menu::Draw(arduboy);
  } else if (s_mode == MODE_GAME)
  {
    if (Bomberboy::Game::Control(Arduboy2Core::buttonsState(), arduboy.frameCount))
    {
      if (Bomberboy::Game::m_level > 50)
      {
        s_mode = MODE_FINAL_CUT_SCENE;
        Bomberboy::FinalCutScene::Init();
      } else
      {
        s_mode = MODE_MENU;
        Bomberboy::Menu::Init();
      }
    } else
      Bomberboy::Game::Draw(arduboy);
  } else if (s_mode == MODE_FINAL_CUT_SCENE)
  {
    if (Bomberboy::FinalCutScene::Control(Arduboy2Core::buttonsState(), arduboy.frameCount))
    {
      s_mode = MODE_MENU;
      Bomberboy::Menu::Init();
    } else
      Bomberboy::FinalCutScene::Draw(arduboy);
  } else if (s_mode == MODE_GAME_INFO)
  {
    if ( Bomberboy::GameInfoScene::Control(Arduboy2Core::buttonsState(), arduboy.frameCount) )
    {
      s_mode = MODE_MENU;
      Bomberboy::Menu::Init();
    } else
    {
      Bomberboy::GameInfoScene::Draw(arduboy);
    }
  }
  arduboy.display(true);
}
void gameLogicLoop(void *)
{
  for (;;) {
    inogameloop(); 
    // ArduinoOTA.handle();
  }
}

void setup() {
  inogamesetup();
  xTaskCreatePinnedToCore(gameLogicLoop, "g", 4096, nullptr, 0, nullptr, 0);
}

void loop() {
	delay(60000);
}
