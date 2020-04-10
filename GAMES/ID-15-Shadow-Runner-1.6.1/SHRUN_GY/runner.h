#ifndef __RUNNER__
#define __RUNNER__

#define RUNNERSTATE_PLAYING 0
#define RUNNERSTATE_DEAD 1

#define RUNNER_RUNNING 0
#define RUNNER_JUMPING 8
#define RUNNER_DUCKING 9

void runner_draw();
void runner_jump();
void runner_duck();

#endif __RUNNER__
