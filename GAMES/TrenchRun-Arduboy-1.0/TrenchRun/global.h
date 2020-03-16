#ifndef GLOBAL_H
#define GLOBAL_H

#include <Arduboy2.h>
//#include <ArduboyPlaytune.h>
//Constants

#define FPS 60

#define DISTANCE_FROM_EXAUST_PORT 7350
#define DISTANCE_ENABLE_TIE2 5510
#define DISTANCE_ENABLE_SPEED 3670
#define DISTANCE_ENABLE_SPEED 3670
#define DISTANCE_ENABLE_MAX_SPEED 1835

#define DEFAULT_CROSSHAIR_X 58
#define DEFAULT_CROSSHAIR_Y 24
#define CROSSHAIR_LIMIT_LEFT 8
#define CROSSHAIR_LIMIT_TOP 0
#define CROSSHAIR_LIMIT_RIGHT 110
#define CROSSHAIR_LIMIT_BOTTOM 46
#define MAXIMUM_TIES 2

#define TIE_MAX_COUNT 2

#define SHIP_SHAKE_COOLDOWN 30

//States
#define STATE_INTRO_TEXT 0
#define STATE_INTRO_CRAWL 1
#define STATE_MENU 2
#define STATE_WARMUP 3
#define STATE_GAME 4
#define STATE_PAUSE 5
#define STATE_DEATH 6
#define STATE_WIN 7
#define STATE_LOSE 8

#define TIE_FRAMES 7
#define TIE_FRAME_SPEED_SLOW 30
#define TIE_FRAME_SPEED_FAST 15
#define TIE_FRAME_SPEED_FASTEST 10

#define BLINK_FRAMES 6
#define SHOOT_COOLDOWN 20;
#define EXPLOSION_COOLDOWN 30;
#define EXPLOSION_BLINK_FRAMES 60;
#define LAST_SHOT_FRAMES 90;

Arduboy2 ab;
Sprites sprites;
//BeepPin1 beep;
//ArduboyPlaytune tunes(ab.audio.enabled);

char tBuffer[22];

byte gameState = STATE_INTRO_TEXT;

byte logoFrameCount = 0;
int crawlFrameCount = 64;

byte shipShakeCounter = 0;

byte defeatedTies = 0;

byte lastShotCooldown = LAST_SHOT_FRAMES;

byte tieFrameSpeed = TIE_FRAME_SPEED_SLOW;

bool shooting = false;

byte blinkCounter = 0;
byte explosionBlinkCounter = 0;
byte shootCooldownCounter = 0;

int8_t xDisplacement = 0;
int8_t yDisplacement = 0;

byte crosshairX = DEFAULT_CROSSHAIR_X;
byte crosshairY = DEFAULT_CROSSHAIR_Y;
unsigned int currentDistanceFromExaustPort = DISTANCE_FROM_EXAUST_PORT;

byte lives = 3;
byte hull = 3;

bool tieEnabled[3] = { true, false, false };
bool tieAlive[3] = { false, false, false };
byte tieRespawnTime[3] = {0 , 0, 0};
byte tieX[3] = {0, 0, 0};
byte tieY[3] = {0, 0, 0};
byte tieFrame[3] = {0, 0, 0};
byte tieExplosionCooldown[3] = {0, 0, 0};

void resetLevel() {
    tieAlive[0] = false;
    tieAlive[1] = false;
    tieRespawnTime[0] = random(2, 4);
    tieRespawnTime[1] = random(2, 4);
    tieEnabled[1] = false;
    tieEnabled[2] = false;
    tieFrameSpeed = TIE_FRAME_SPEED_SLOW;
    hull = 3;
    shipShakeCounter = 0;
    crosshairX = DEFAULT_CROSSHAIR_X;
    crosshairY = DEFAULT_CROSSHAIR_Y;
    currentDistanceFromExaustPort = DISTANCE_FROM_EXAUST_PORT;
    defeatedTies = 0;
    lastShotCooldown = LAST_SHOT_FRAMES;
    blinkCounter = 0;
}

void resetGame() {
  lives = 3;
  logoFrameCount = 0;
  crawlFrameCount = 64;
}

#endif
