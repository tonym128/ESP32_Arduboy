#include <avr/pgmspace.h>
#include <Gamby.h>
#include "runner.h"
#include "runner_bitmap.h"

extern GambyGraphicsMode gamby;
extern int runnerX;
extern int runnerY;

char runnerframe = 0;
bool jumping = false;
bool ducking = false;
byte leap[] = {19,13,8,6,8,13,19};

void runner_draw()
{
  runnerframe++;
  if (jumping)
  {
    runnerY=leap[runnerframe-RUNNER_JUMPING];
    if (runnerframe > 14)
    {
      runnerY = 28;
      jumping = false;
      runnerframe = RUNNER_RUNNING;
    }
    gamby.drawSprite(runnerX, runnerY, runner_bitmap,RUNNER_JUMPING,runnermask,RUNNER_JUMPING);
  }
  else if(ducking)
  {
    runnerY = 36;
    if (runnerframe > 14)
    {
      runnerY = 28;
      ducking = false;
      runnerframe = RUNNER_RUNNING;
    }
    gamby.drawSprite(runnerX, runnerY, runner_bitmap,RUNNER_DUCKING,runnermask,RUNNER_DUCKING);
  }
  else{
    if (runnerframe > 7)runnerframe = RUNNER_RUNNING;
    gamby.drawSprite(runnerX, runnerY, runner_bitmap,runnerframe,runnermask,runnerframe);
  }
  
}

void runner_jump()
{
  if (!ducking && !jumping)
  {
    jumping = true;
    runnerframe = RUNNER_JUMPING;
  }
}

void runner_duck()
{
  if (!ducking && !jumping)
  {
    ducking = true;
    runnerframe = RUNNER_DUCKING;
  }
}




