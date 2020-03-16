/*
 SHADOW RUNNER: http://www.team-arg.org/shrun-manual.html
 
 Gamby version 1.0 : http://www.team-arg.org/more-about.html
 
 MADE by TEAM a.r.g. : http://www.team-arg.org/more-about.html
 
 2015 - JO3RI - Trodoss
 
 */

//determine the game
#define GAME_ID 15

#include <Gamby.h>
#include <avr/pgmspace.h>

#define PAUSE_BETWEEN_ACTIONS 80
#define PAUSE_BETWEEN_MENUS   180

//define menu states (on main menu)
#define STATE_MENU_MAIN    0
#define STATE_MENU_HELP    1
#define STATE_MENU_CREDITS 2
#define STATE_MENU_SOUND   3

#define GAME_INIT    4
#define GAME_PLAYING 5
#define GAME_PAUSED  6
#define GAME_OVER    7

#include "other_bitmap.h"
#include "runner.h"
#include "stage.h"
#include "candle.h"

GambyGraphicsMode gamby;

//determines the state of the game
unsigned char game_state = 0;
unsigned long next_action = 0;

// These are all getting a value in STATE_GAME_INIT
int life;
unsigned long score;
unsigned char level;
unsigned char countlevels;

//generic variable to store selection (on screens)
unsigned char selection = 0;
boolean soundyesno = true;

//these are used for the runner animation in the mainscreen
int runnerX = -95;
int runnerY = 0;
boolean show_runner = false;

void setup () {
  randomSeed(analogRead(0));
  gamby.drawPattern = PATTERN_BLACK;
  gamby.rect(0,0,95,63);
  gamby.drawSprite(2,16,TEAM_arg_logo);
  gamby.update();
  delay(3000L);
  game_state = STATE_MENU_MAIN;
  runnerX = -95;
  runnerY = 0;
}

void loop ()
{
  byte lastInputs;
  switch (game_state)
  {

    //******* MENU-specific events **************    

  case STATE_MENU_MAIN:
    menu_main();
    if (millis() >= next_action)
    {
      gamby.readInputs();
      lastInputs = gamby.inputs;             

      if (gamby.inputs & DPAD_UP)
      {
        if (selection > 0) selection--;
      }

      if (gamby.inputs & DPAD_DOWN)
      {
        if (selection < 3) selection++;
      }
      if (gamby.inputs & (BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4))
      {
        switch (selection)
        {
        case 0:
          gamby.clearScreen();
          gamby.update();
          game_state = STATE_MENU_HELP;
          menu_help();
          break;

        case 1:
          gamby.clearScreen();
          gamby.update();
          game_state = STATE_MENU_CREDITS;
          menu_credits();
          break;

        case 2:
          soundyesno = !soundyesno;
          break;

        case 3:
          game_state = GAME_INIT;
          break;
        }
      }

      next_action = millis() + PAUSE_BETWEEN_MENUS; 
    }
    break;

  case STATE_MENU_HELP:
  case STATE_MENU_CREDITS:
    if (millis() >= next_action)
    {
      gamby.readInputs();
      lastInputs = gamby.inputs;             

      if (gamby.inputs & (BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4))
      {
        gamby.clearScreen();
        gamby.update();
        game_state = STATE_MENU_MAIN;
      }
      next_action = millis() + PAUSE_BETWEEN_MENUS; 
    }
    break;  


    //******* Game-specific events **************

  case GAME_OVER:
    game_over();
    if (millis() >= next_action)
    {
      gamby.readInputs();
      lastInputs = gamby.inputs;
      //wait for a button to be pressed to continue
      if (lastInputs & (BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4))
      {
        game_state = STATE_MENU_MAIN;
      }

      next_action = millis() + PAUSE_BETWEEN_MENUS; 
    }
    gamby.update();
    break;

  case GAME_INIT:
    runnerX = 16;
    runnerY = 28;
    score = 0;
    level = 1;
    life = 96;
    countlevels = 0;
    game_state = GAME_PLAYING;
    break;

  case GAME_PLAYING:
    stage_update();
    if (millis() >= next_action)
    { 
      gamby.readInputs();     
      lastInputs = gamby.inputs;
      if (lastInputs & (BUTTON_2)) runner_jump();
      if (lastInputs & (BUTTON_4)) runner_duck();
      if (lastInputs & BUTTON_1) game_state = GAME_PAUSED;   

      next_action = millis() + PAUSE_BETWEEN_ACTIONS;
    }
    //check the runner's state
      if (life < 0)
      {
        game_state = GAME_OVER; 
        delay(1500L);
      }
    gamby.update();
    break;

  case GAME_PAUSED:
    game_pause();
    if (millis() >= next_action)
    {
      gamby.readInputs();
      lastInputs = gamby.inputs;
      if (lastInputs & (BUTTON_1 | BUTTON_2 | BUTTON_3 | BUTTON_4)) game_state = GAME_PLAYING;
      next_action = millis() + PAUSE_BETWEEN_MENUS; 
    }
    gamby.update();
    break;
  }
}


//****************** Menu-specific functions
void menu_main()
{  

  // show the splash art
  gamby.rect(0,0,95,63);
  gamby.drawSprite(1,0,shadowtittle_bitmap);
  gamby.drawSprite(33,26,menu);
  switch (soundyesno){
  case true:
    gamby.drawSprite(73,42,yes_bitmap);
    break;
  case false:
    gamby.drawSprite(73,42,no_bitmap);   
    break;
  }
  candle_draw(13,selection*8);
  for (byte i = 0; i < 4; i++)
  {
    if (selection != i) gamby.drawSprite(33,(i*8)+26,shade,shade);
  }
  if (show_runner)
  {
    gamby.drawPattern = PATTERN_WHITE;
    gamby.disc(runnerX+13,14,14);
    gamby.drawPattern = PATTERN_BLACK;
    runner_draw();
  }
  gamby.update();
  runnerY = 0;
  runnerX += 4;
  if (runnerX>95)
  {
    runnerX = -96;
    show_runner = !show_runner;
  }
}


void menu_credits()
{
  gamby.rect(0,0,95,63);
  gamby.drawSprite(1,0,shadowtittle_bitmap);
  gamby.drawSprite(0,23,info);
  gamby.update();  
}

void menu_help()
{
  gamby.drawSprite(19,-4,QRcode);
  gamby.update();  
}

void game_pause()
{
  gamby.rect(0,0,95,63);
  gamby.drawSprite(20,4,pause_bitmap);
  candle_draw(40,8);
}

void game_over()
{
  gamby.rect(0,0,95,63);
  gamby.drawSprite(18,16,gameover_bitmap);
  score_draw(14,40);
}
