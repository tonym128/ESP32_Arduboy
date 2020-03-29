/*
Buttons example
June 11, 2015
Copyright (C) 2015 David Martinez
All rights reserved.
This code is the most basic barebones code for showing how to use buttons in
Arduboy.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
*/

#define GAME_FRAME_RATE 60

//#include "ArduboyRem.h"
#include <ESP8266WiFi.h>
#include "Arduboy2.h"
#include <ArduboyTones.h>
#include "pics/playercar.h"
#include "pics/playercar_bottom.h"
#include "pics/skybox.h"
#include "pics/enemycar_z1.h"
#include "pics/enemycar_z2.h"
#include "pics/enemycar_z3.h"
#include "pics/enemycar_z4.h"
#include "pics/playercar_tire2.h"
#include "pics/puff_1.h"
#include "pics/puff_2.h"
#include "pics/puff_3.h"
#include "pics/palm_1.h"
#include "pics/palm_2.h"
#include "pics/palm_3.h"
#include "pics/title.h"
#include "pics/instructions.h"
#include "pics/big_1.h"
#include "pics/big_2.h"
#include "pics/big_3.h"
#include "pics/big_go.h"
#include "pics/font_drv.h"

// Make an instance of arduboy used for many functions
Arduboy2 arduboy;
ArduboyTones sound(arduboy.audio.enabled);


void drawOutrunTrack();

// Variables for your game go here.
byte fps = 0;
byte fpscount = 0;
uint16_t lastMilli = 0;

byte hflip;
byte gear;
byte gameTimer;
byte speed;
byte score;
uint16_t speedAcc;
int16_t playerX;
int16_t playerY;
int16_t currentRoadCurve;
int16_t desiredRoadCurve;
int16_t nextRoadCurveChange;
int16_t backgroundXOffset;
int8_t trackCenteredX;
int8_t puffX;
int8_t puffY;
int8_t puffFrame;

int16_t enemyX;
uint16_t enemyYAcc;
byte enemySpeed;

// Maximum number of simultaneously tree/road-side object
#define MAX_TREE 8
// Y position of palm trees
uint16_t palmYAcc[MAX_TREE];
// State of the object: 0=none, 1=left palm tree, 2=right palm tree, etc.
byte palmState[MAX_TREE];
// Counter to determine when the next palm tree should appear
uint16_t nextPalm;

byte trackPixWidth[44];
int16_t trackStartX[44];

// Perpective lookup table: for any y value (0..255), remap it to a perpective y value for our road (20..63)
const byte PROGMEM yLookup[256] = { 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 20, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 23, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 29, 29, 29, 29, 29, 29, 29, 29, 30, 30, 30, 30, 30, 30, 30, 30, 31, 31, 31, 31, 31, 31, 32, 32, 32, 32, 32, 32, 32, 33, 33, 33, 33, 33, 34, 34, 34, 34, 34, 35, 35, 35, 35, 35, 36, 36, 36, 36, 36, 37, 37, 37, 37, 38, 38, 38, 38, 39, 39, 39, 40, 40, 40, 40, 41, 41, 41, 42, 42, 42, 43, 43, 43, 44, 44, 45, 45, 45, 46, 46, 47, 47, 48, 48, 49, 49, 50, 50, 51, 51, 52, 52, 53, 53, 54, 55, 55, 56, 57, 58, 58, 59, 60, 61, 62, 62, 63, };

const PROGMEM byte* puffBitmaps[] = { puff_1, puff_2, puff_3 };

enum class EState : byte {
	Title,
	Instructions,
	Game,
	GameOver,
} currentState = EState::Title;

uint16_t stateFrame;
static uint8_t flicker = 0;
static uint32_t TTOT;

void paintScreen()
{
  static uint8_t alternate = 0xAA;
  //if (flicker & 1) alternate = ~alternate;
  for (uint32_t i = 0; i < HEIGHT*WIDTH/8; i++) {
    arduboy.sBuffer[i] = alternate;
    alternate = ~alternate;
  }
}


void drawFastHLine(int16_t x, uint8_t y, int16_t w, uint8_t color)
{
  // Do y bounds checks
  if (y >= HEIGHT)
    return;
  int16_t xEnd = x + w; // last x point + 1
  // Check if the entire line is not on the display
  if (xEnd <= 0 || x >= WIDTH || w <= 0)
    return;
  // clip left edge
  if (x < 0)
    x = 0;
  // clip right edge
  if (xEnd > WIDTH)
    xEnd = WIDTH;
  // calculate final clipped width (even if unchanged)
  uint8_t clippedW = xEnd - x;
  // buffer pointer plus row offset + x offset
  // (y >> 3) * WIDTH, optimized to: ((y * 16) & 0xFF80) because gcc O3 is not smart enough to do it.
  uint8_t *pBuf = arduboy.sBuffer + ((y * 16) & 0xFF80) + uint8_t(x);
  // pixel mask
  uint8_t mask = 1 << (y & 7);
  //uint8_t mask = pgm_read_byte(yMask + (y & 7));
  switch (color)
  {
  case WHITE:
    do
    {
      *pBuf++ |= mask;
    } while (--clippedW);
    break;
  case BLACK:
    mask = ~mask;
    do
    {
      *pBuf++ &= mask;
    } while (--clippedW);
    break;
  }
}


void drawMaskBitmap(int8_t x, int8_t y, const uint8_t *bitmap, uint8_t hflip)
{
  // Read size
  uint8_t w = pgm_read_byte(bitmap++);
  uint8_t h = pgm_read_byte(bitmap++);
  // Read hotspot
  int8_t hx = pgm_read_byte(bitmap++);
  int8_t hy = pgm_read_byte(bitmap++);

  y -= hy;
  if (hflip == 0) {
    x -= hx;
  } else {
    x -= w - hx;
  }

  // no need to draw at all if we're offscreen
  int8_t endX = x + w;
  if ((endX < 0 && x < 0)/* || x >= WIDTH*/ || int8_t(y + h) < 0 || y >= HEIGHT)
    return;

  uint8_t yOffset = 1 << (y & 7);
 // uint8_t yOffset = pgm_read_byte(yMask + (y & 7));
  const uint8_t* sourceAddress = bitmap;

  int8_t sRow = y >> 3;
  uint8_t uRow = sRow; // Same as 'sRow' but unsigned: will be used after clipping
  uint8_t rows = (h + 7) >> 3;
  // Top clipping: skip 'n' rows if outside the screen
  if (sRow < 0) {
    sourceAddress += (-sRow * w) << 1;
    rows += sRow;
    uRow = 0;
  }
  // Bottom clipping: stop as soon as we reach bottom of screen
  if (uRow + rows > (HEIGHT >> 3)) {
    rows = (HEIGHT >> 3) - uRow;
  }
  // Optimization: Perform 'x' clipping outside of loop
  uint8_t startCol = 0;
  uint8_t endCol = w;
  if (x < 0) {
    startCol = -x;
  }
  if (x + w > WIDTH) {
    endCol = WIDTH - x;
  }

  if (hflip == 0) {
    sourceAddress += (startCol << 1);
  } else {
    sourceAddress += ((w - endCol) << 1);
  }
  uint8_t sourceAddressSkip = (w - (endCol - startCol)) << 1;

  if (hflip == 0) {
    uint16_t destAddress = (uRow * WIDTH) + x + startCol;
    uint8_t destAddressSkip = (WIDTH - (endCol - startCol));

    for (uint8_t bRow = uRow; bRow < uint8_t(uRow + rows); bRow++, sourceAddress += sourceAddressSkip, destAddress += destAddressSkip) {
      for (uint8_t iCol = startCol; iCol < endCol; iCol++, destAddress ++) {
        // Draw mask first with AND and Draw color with OR:
        // The GCC compiler is doing a poor job because of its constraint to keep 'r1' always 0 (two useless clr __zero_reg__ here)
        // and also because it can't accept to keep the last 'mul' result in R1:R0, so there's a useless 'movw'. Total: 3 wasted clock cycles in this loop
        uint16_t sourceMask = ~(pgm_read_byte(sourceAddress) * yOffset);
        sourceAddress++;
        uint16_t sourceByte = pgm_read_byte(sourceAddress) * yOffset;
        sourceAddress++;
        arduboy.sBuffer[destAddress] = (arduboy.sBuffer[destAddress] & uint8_t(sourceMask)) | uint8_t(sourceByte);
        arduboy.sBuffer[destAddress + WIDTH] = (arduboy.sBuffer[destAddress + WIDTH] & uint8_t(sourceMask >> 8)) | uint8_t(sourceByte >> 8);
      }
    }
  }
  else {
    uint16_t destAddress = (uRow * WIDTH) + x + endCol - 1;
    uint8_t destAddressSkip = (WIDTH + (endCol - startCol));

    for (uint8_t bRow = uRow; bRow < uint8_t(uRow + rows); bRow++, sourceAddress += sourceAddressSkip, destAddress += destAddressSkip) {
      for (uint8_t iCol = startCol; iCol < endCol; iCol++, destAddress--) {
        uint16_t sourceMask = ~(pgm_read_byte(sourceAddress) * yOffset);
        sourceAddress++;
        uint16_t sourceByte = pgm_read_byte(sourceAddress) * yOffset;
        sourceAddress++;
        arduboy.sBuffer[destAddress] = (arduboy.sBuffer[destAddress] & uint8_t(sourceMask)) | uint8_t(sourceByte);
        arduboy.sBuffer[destAddress + WIDTH] = (arduboy.sBuffer[destAddress + WIDTH] & uint8_t(sourceMask >> 8)) | uint8_t(sourceByte >> 8);
      }
    }
  }
}


void inline drawByte(uint8_t x, uint8_t y, uint8_t color){
    arduboy.sBuffer[(y*WIDTH) + x] = color;
}


void printBytePadded(uint8_t x, uint8_t y, byte num)
{
  int ten = num / 10;
  int unit = num % 10;
  drawChar(x, y, '0' + ten);
  drawChar(x + 6, y, '0' + unit);
}


void drawChar(uint8_t x, uint8_t y, unsigned char c)
{
  if ((x >= WIDTH) || (y >= HEIGHT))
    return;
  y >>= 3;
  for (uint8_t i = 0; i < 6; i++)
    drawByte(x + i, y, pgm_read_byte(font_drv + ((c - 32) * 6) + i));
}



void prnt(String str){
  for (uint8_t i = 0; i < str.length(); i++)
    drawChar(arduboy.getCursorX() + i*6, arduboy.getCursorY(), str[i]);
}



// This function runs once in your game.
// use it for anything that needs to be set only once in your game.
void setup() {
	//initiate arduboy instance
  WiFi.mode(WIFI_OFF); //disable wifi to save some battery power
	arduboy.boot(); // raw hardware
	arduboy.blank(); // blank the display
	arduboy.flashlight(); // light the RGB LED and screen if UP button is being held.
	arduboy.systemButtons(); // check for and handle buttons held during start up for system control  
	arduboy.audio.begin();
	arduboy.audio.on();

	arduboy.setFrameRate(1000000 / GAME_FRAME_RATE);
  TTOT = ESP.getCpuFreqMHz()*1000000/GAME_FRAME_RATE;
 
	// At 67 fps, each frame takes 14925 microseconds
	// At 16 Mhz, we have about 238805 clock cycles per frame, i.e.: 16 clock cycle per microsecond

	// We draw 44 lines of road: 
	// drawFastHline can be called up to 220 times per frame to draw to road, so any clock cycle in that function cost 14 us (!!)
	// During the paint, the loop could have up to 128 times per line, so cost 352 us per clock cycle in the draw loop (!!)
	// For example: it takes 8 clock/pixel, so about 2816 us for the draw part

	// First, optimise drawChar (because extremely slow!)
	// First optim: removing size: prints take 3438 micros
	// Second optim: removing transparent: prints take 3290 micros
	// 3rd optim: removing color support: prints take 3110 micros
	// 4th optim: force full byte alignment: prints take 670 micros

	// Removed clear screen, should not be needed if we repaint the screen anyway. It is very fast at 188 us.
	// Copy to oled buffer takes 1816us.. slow! We have a lot of 'free cycles' during this operation because it needs to wait for the
	// SPI display acknowledgement after each byte
	// drawBitmap is very slow when not aligned on a full byte
	// Currently: 7532 idle, 1816 drawscreen, 4746 render stuff, 678 print debug
	// Currently, before drawHLine optimization: 6330 render stuff

	// Nov 13 2016, current state, with full optimize O3 in ArduboyCoreRem.h:
	// free time = 1100 us
	// copy oled = 1360 us
	// render game = 11700 us
	// print text = 844 us
	// total: 15004 us
	// Sketch uses 9,880 bytes (34%) of program storage space. Maximum is 28,672 bytes.
	// Global variables use 1, 360 bytes(53 % ) of dynamic memory, leaving 1, 200 bytes for local variables.Maximum is 2, 560 bytes.
	// With optimize Os, we save 222 bytes of rom, 8 bytes of ram, but lose almost 2000 us in rendering speed!

	// DONE: drawFastHLine: use uint8 internally (after clipping) for faster loop, free time = 1330 us, saved about 230 us
	// DONE: drawFastHLine optimized more:free time = 1440 us
	// DONE: after yMask[] in MaskBitmap, render: 10160, free time = 2544 us (!!) Suspicious, gain is way too high.. Maybe because a register was freed?
	// current render game = 10160 us
	// with sourceMask = ~(pgm_read_byte(sourceAddress++) * yOffset);  9980 us
	// with pgm_read_byte_and_increment: 9064 us! Mega performance boost using lpm Z+
	// DONE: After intense optimization to DrawMaskBitmap: New speed: 9060 us for render, currently 32 clocks per byte

	// TODO: idea: align screen buffer on a 256 bytes-address, so we can use only high-byte for quicker compare
	// TODO: support hflip for drawMaskedBitmap
	// TODO: integrate maskedbitmap size in the data itself
	// TODO: add support for sprite hotspot
	// TODO: ask LouisP to always trim bitmap perfectly flush (i.e.: no waste around)
	// TODO: ..which implies support non-multiple of 8 in Processing and auto-pad with transparent
	// TODO: optimize DrawOpaque (and drawMask?) when perfectly y-aligned on 8 pixels
	// TODO: draw scrolling city background
	// TODO: print score info at top of screen
	// TODO: sound and music, is it possible to integrate them into the screen copy feature?
	// TODO: Use Optimize O3 only in specific section: see performance vs rom space compromise
	// TODO: Road curve support
	// TODO: Road hill support?
	// TODO: Support inverted text (black, on white background)?
}

inline void AddWithClamp(int16_t& ioBaseNumber, int16_t inAddValue) {
	if (inAddValue > 0 && ioBaseNumber > 32767 - inAddValue) {
		ioBaseNumber = 32767;
		return;
	}
	if (inAddValue < 0 && ioBaseNumber < -32768 - inAddValue) {
		ioBaseNumber = -32768;
		return;
	}
	ioBaseNumber += inAddValue;
}

void UpdatePalm() {
	if (gameTimer > 99) return;

	if (nextPalm < speed) {
		// Spawn a new tree by checking the first available slot
		for (byte i = 0; i < MAX_TREE; i++) {
			if (palmState[i] == 0) {
				palmState[i] = random(2) + 1;
				palmYAcc[i] = 0;
				nextPalm = 100 + random(1400);
				break;
			}
		}
	}
	else {
		nextPalm -= speed;
	}

	for (byte i = 0; i < MAX_TREE; i++) {
		if (palmState[i] == 0) continue;

		// Make tree move according to player current speed
		palmYAcc[i] += speed;
		// Check if tree would go off the bottom of the screen: if so, free its slot
		uint16_t tempy = (palmYAcc[i] >> 3);
		if (tempy > 255) {
			palmState[i] = 0;
			continue;
		}

		// Calculate perspective y using lookup table
		byte palmy = pgm_read_byte(yLookup + byte(tempy));
		// Determine which sprite scale, depending on hard-coded y values:
		const byte* whichSprite2 = palmy > 43 ? palm_1 : palmy > 32 ? palm_2 : palm_3;

		// Draw test palm tree on side of the road, left or right:
		int16_t palmx = trackStartX[palmy - 20];
		if (palmState[i] == 1) {
			palmx -= 8;
			if (palmx >= 0) {
				drawMaskBitmap(palmx, palmy, whichSprite2, false);
			}
		}
		else {
			palmx += 8 + (trackPixWidth[palmy - 20] << 1);
			if (palmx <= 136) {
				drawMaskBitmap(palmx, palmy, whichSprite2, true);
			}
		}
	}
}

// 'x' position is based on a referencial of track line 41 (41+20 = 61, which is player 'y' position)
int8_t GetScreenXPos(int8_t inX, int8_t inY) {
	return (inX * trackPixWidth[inY - 20] / trackPixWidth[41]) + trackPixWidth[inY - 20] + trackStartX[inY - 20];
}

void UpdateEnemy() {
	// Make enemy move according to player current speed and its own speed
	// divide by a factor to make enemy slightly 'glue' to player speed, or else it is too frantic
	enemyYAcc += (speed - enemySpeed) >> 2;
	// Check if enemy would go off the bottom of the screen: if so, spawn a new one
	uint16_t tempy = (enemyYAcc >> 3);
	if (tempy > 255) {
		// Don't spawn enemies during the start countdown
		if (gameTimer > 99) {
			return;
		}
		
		enemySpeed = random(40) + 130;
		// If player is faster than enemy, spawn a new enemy ahead
		if (speed > enemySpeed) {
			enemyYAcc = 0;
			enemyX = random(65535) - 32768;
			score++;
		}
		else { // If player is slower, then spawn enemy behind
			enemyYAcc = 255 << 3;
			// Enemy spawning behind should never crash into the player: they avoid left or right
			enemyX = playerX + 10240 + random(50175);
		}
		return;
	}

	byte ey = pgm_read_byte(yLookup + byte(tempy));
	int8_t ex = GetScreenXPos(enemyX >> 8, ey);
	// Draw enemy car
	const byte* whichSprite = ey > 48 ? enemycar_z1 : ey > 37 ? enemycar_z2 : ey > 29 ? enemycar_z3 : enemycar_z4;
	byte hflip = 0;
	if (currentRoadCurve < -2000 || ex > 64) {
		hflip = 1;
	}
	drawMaskBitmap(ex, ey, whichSprite, hflip);

	// Detect collision with player:
	int8_t dx = (playerX - enemyX) >> 8;
	int8_t dy = (playerY >> 8) - ey;
	// Collision! Bring player speed equal to the enemy
	if (dx > -40 && dx < 40 && dy > -8 && dy < 8) {
		speed = enemySpeed - 20;
		playerX += dx << 6;
		enemyX -= dx << 6;
	}
}

void SetState(EState inState) {
	currentState = inState;
	stateFrame = 0;

	// Initialization when going to 'Game' state:
	if (currentState == EState::Game) {
		gameTimer = 104; // 102 = 3!,  101 = 2!, 100 = 1!,  99 = GO! ...
		gear = 0;
		hflip = 0;
		speed = 0;
		score = 0;
		speedAcc = 0;
		playerX = 0 << 8;
		playerY = 61 << 8;
		currentRoadCurve = 0 << 8;
		desiredRoadCurve = 0 << 8;
		nextRoadCurveChange = 0;
		backgroundXOffset = 0 << 8;
		trackCenteredX = 0;
		puffX = 0;
		puffY = 0;
		puffFrame = -1;

		enemyX = 0;
		enemyYAcc = 9999;
	}
}

void TitleState()
{
	//!drawMaskBitmap(0, 0, title);
  drawMaskBitmap(0, 0, title, 0);
	if (stateFrame > 120) {
		arduboy.setCursor(104, 0);
		//arduboy.print("Sfx");
    prnt("Sfx");
		drawChar(122, 0, arduboy.audio.enabled() ? '@' : ' ');
	}

	if (stateFrame > 300) {
		if (stateFrame < 500 || (stateFrame & 64) == 0) {
			arduboy.setCursor(4, 56);
			//arduboy.print(" > Push A to start < ");
      prnt(" > Push A to start < ");
		}

		// Auto-switch to game after a delay, but instantly gameover by setting timer to 0
		if (stateFrame > 900) {
			SetState(EState::Game);
			speed = 220;
			gear = 1;
			gameTimer = 0;
		}
	}

	if (arduboy.justPressed(A_BUTTON))
	{
		SetState(EState::Instructions);
	}

	if (arduboy.justPressed(B_BUTTON)) {
		if (arduboy.audio.enabled()) {
			arduboy.audio.off();
		}
		else {
			arduboy.audio.on();
		}
	}
}

void InstructionsState()
{
	//!drawMaskBitmap(0, 0, instructions);
  drawMaskBitmap(0, 0, instructions,0);

	if (arduboy.justPressed(A_BUTTON) || stateFrame > 900)
	{
		SetState(EState::Game);
	}
}

void DrawCountdown() {
	const byte* whichSprite = nullptr;
	switch (gameTimer) {
	case 102:
		whichSprite = big_3;
		break;
	case 101:
		whichSprite = big_2;
		break;
	case 100:
		whichSprite = big_1;
		break;
	case 99:
	case 98:
		whichSprite = big_go;
		break;
	}
	if (whichSprite) {
		//!drawMaskBitmap(64, 23, whichSprite);
    drawMaskBitmap(64, 23, whichSprite,0);
	}
}

void DrawHud() {
	drawByte(0, 0, 0xFF);
	drawByte(1, 0, 0x81);
	// Convert speed (0..220) to a speedmeter range of 0..33
	byte speedometer = (speed * 39) >> 8;
	if (speedometer > 0 && flicker & 2) {
		speedometer--;
	}
	for (byte i = 0; i < speedometer; i++) {
		drawByte(i + 2, 0, 0xBD);
	}
	for (byte i = speedometer; i < 33; i++) {
		drawByte(i + 2, 0, 0x81);
	}
	drawByte(35, 0, 0x81);
	for (byte i = 36; i <= 56; i++) {
		drawByte(i, 0, 0xFF);
	}
	for (byte i = 69; i <= 115; i++) {
		drawByte(i, 0, 0xFF);
	}

	//for (byte i = 0; i < 128; i++) {
	//	arduboy.drawByte(i, 0, 0xFF);
	//}

	if (gameTimer <= 99) {
		printBytePadded(57, 0, gameTimer);
	}

	byte flickerGear = flicker & 4;
	// Flicker the currently selected gear
	if ((gear == 1) || flickerGear) {
		drawChar(81, 0, 'L');
	}
	if ((gear == 0) || flickerGear) {
		drawChar(93, 0, 'H');
	}

	if (gear == 0) {
		// Blink the arrow when running fast in low gear (indicating it is time to switch in high gear)
		if (speed > 120 && flickerGear) {
			drawChar(87, 0, '>');
		}
	}
	else {
		// Blink the arrow when running too slow in high gear (accelerate would be better in low gear)
		if (speed < 120 && flickerGear) {
			drawChar(87, 0, '<');
		}
	}
	printBytePadded(116, 0, score);

	if (gameTimer == 0) {
		arduboy.setCursor(31, 32);
		//arduboy.print(" Game over ");
    prnt(" Game over ");

		// If going into the 'game over' part of this game state:
		if (currentState == EState::Game) {
			currentState = EState::GameOver;
			stateFrame = 0;
		}
		else if (stateFrame > 300) {
			SetState(EState::Title);
		}
	}
}

void GameState()
{
	// the next couple of lines will deal with checking if the D-pad buttons
	// are pressed and move our text accordingly.
	// We check to make sure that x and y stay within a range that keeps the
	// text on the screen.

	// if the right button is pressed move 1 pixel to the right every frame
	constexpr uint16_t baseSpeed = 400;
	constexpr uint16_t topSpeed = 900;
	constexpr byte speedMultiplier = 5;
	uint16_t moveSpeed = speed >= byte((topSpeed - baseSpeed) / speedMultiplier) ? topSpeed : (speed * speedMultiplier) + baseSpeed;

	int16_t playerXMovement = 0;
	bool isMovingSide = false;
	byte buttons = arduboy.buttonsState();
	// Prevent any button pressed when game over
	if (gameTimer == 0) {
		buttons = 0;
	}

	if ((buttons & RIGHT_BUTTON) && playerX <= int16_t(32767 - moveSpeed)) {
		hflip = 0;
		isMovingSide = true;
		//x++;
		playerXMovement = moveSpeed;
	}

	// if the left button is pressed move 1 pixel to the left every frame
	if ((buttons & LEFT_BUTTON) && playerX >= (-32768 + moveSpeed)) {
		hflip = 1;
		isMovingSide = true;
		playerXMovement = -moveSpeed;
	}

	// Moving up/down is enabled only when race starts
	if (gameTimer <= 99) {
		// if the up button is pressed, move up
		if (((buttons & UP_BUTTON)) && playerY > 13568) {
			playerY -= 128;
		}

		// if the down button is pressed, move down
		if (((buttons & DOWN_BUTTON)) && playerY < 16128) {
			playerY += 128;
		}
	}

	if ((buttons & B_BUTTON) && arduboy.justPressed(B_BUTTON)) {
		gear = !gear;
	}

	// Hold A to accelerate, release to auto-brake
	if ((buttons & A_BUTTON)) {
		if (gear == 0) {
			if (speed < 140) { // Top speed in low gear:
				speed++;
			}
		}
		else {
			if (speed < 120) { // If running a low speed but in high gear, make it ineffective
				if ((flicker & 3) == 0) {
					speed++;
				}
			}
			else if (speed < 220) { // Top speed in high gear:
				speed++;
			}
		}
	}
	else if (speed > 0) {
		if (speed >= 2) {
			speed -= 2;
		}
		else {
			speed = 0;
		}
	}

	int8_t roadCurve8 = currentRoadCurve >> 8;

	// During initial countdown, don't really move the track: We consider the car to be in neutral and just revving
	if (gameTimer <= 99) {
		// Engine compression brake if running high speed in low gear:
		if (gear == 0 && speed > 140) {
			speed -= 2;
		}

		int8_t x = playerX >> 8;
		int8_t y = playerY >> 8;

		// Brake if running off-road (i.e.: left-side or right-side of the road
		if (speed > 50 && (x <= -127 || x >= 126)) {
			speed = 50;
		}

		// Make road curve 'push' player to outside of the track, according to its speed:
		// with >> 3, it is a bit easy, with >> 2 too hard
		playerXMovement -= (roadCurve8 * speed) >> 3;
		//playerXMovement -= (currentRoadCurve * (int16_t)speed) >> 10;
		AddWithClamp(playerX, playerXMovement);
		// And also make it move the skybox background
		backgroundXOffset -= (roadCurve8 * speed) >> 3;

		// Determine next road curve
		nextRoadCurveChange -= speed;
		if (nextRoadCurveChange <= 0) {
			nextRoadCurveChange = random(32767);
			desiredRoadCurve = random(25600) - 12800;
		}

		byte speedMult = 3;
		int16_t curveSpeedChange = (speed * speedMult) >> 1;
		// Comment this block to disable curves:
		if (currentRoadCurve + curveSpeedChange < desiredRoadCurve) {
			currentRoadCurve += curveSpeedChange;
		}
		else if (currentRoadCurve - curveSpeedChange > desiredRoadCurve) {
			currentRoadCurve -= curveSpeedChange;
		}
	}

	// With nothing, idle   11888
	// with clear:			11700 cost 188 us
	// with drawBitmap:		 7128 cost 4760 us
	// drawtrack:			 8356 cost 3532 us
	// clear+draw =			 6950 cost 4938 us
	// clear+drawtrack =	 8188 cost 3700 us
	// clear+bitmap+track =  3068 cost 8820 us

	// we clear our screen to black: useless if we repaint everything!
	// No need to clear, since 'paintScreen' does it for free.
	// arduboy.clear();
	drawOutrunTrack();
	//arduboy.drawTurboBitmap(x - 24, y - 24, pgm_read_ptr(mypics + (flicker & 1)), 85, 64);
	//arduboy.drawGrayBitmap(x - 24, y - 24, outrun, 82, 40);

	//for (byte i = 0; i < 6; ++i) {
	//	arduboy.drawMaskBitmap(i, i + 8, outrun);
	//}
	//arduboy.drawMaskBitmap(-127, -127, outrun, hflip);
	drawMaskBitmap((backgroundXOffset >> 8), 8, skybox, false);
	drawMaskBitmap((backgroundXOffset >> 8) + 128, 8, skybox, false);

	UpdatePalm();
	UpdateEnemy();

	// Draw player car
	int8_t py = playerY >> 8;
	int8_t px = GetScreenXPos(playerX >> 8, py);
	drawMaskBitmap(px, py, playercar_bottom, hflip);

	// Make tire animate according to current accumulator
	if (gameTimer <= 99) {
		int8_t tireOverlay = (speedAcc & 256) ? -17 : 13;
		//!drawMaskBitmap(px + tireOverlay, py, playercar_tire2);
   drawMaskBitmap(px + tireOverlay, py, playercar_tire2,0);
	}

	int8_t carBobOffset = (speedAcc & 512) ? -6 : -5;
	drawMaskBitmap(px, py + carBobOffset, playercar, hflip);

	// If puff is off and player is moving left/right and speed is high enough and we are in a steep curve, then emit a puff
	if (puffFrame == -1 && isMovingSide && speed > 100 && (roadCurve8 > 15 || roadCurve8 < -15)) {
		puffFrame = 0;
		puffX = px - 16;
		puffY = py - 1;
	}
	// If puff is currently active: make it animate then die
	if (puffFrame != -1) {
		drawMaskBitmap(puffX, puffY, (const byte*)pgm_read_ptr(puffBitmaps + (puffFrame >> 4)), 0);
		drawMaskBitmap(puffX + 34, puffY, (const byte*)pgm_read_ptr(puffBitmaps + (puffFrame >> 4)), 1);

		puffFrame += 8;
		if (puffFrame >= (3 << 4)) {
			puffFrame = -1;
		}
	}

	//arduboy.drawFastHLine(10, 5, 100, flicker & 1);
	//arduboy.drawFastHLine(10, 6, 100, 1);

	// we set our cursor x pixels to the right and y down from the top
	// arduboy.setCursor(x, y);

	//// pixel 0 is always full black
	//for (byte i = 1; i < 32; ++i) {
	//	if (flicker % (63 / i) == 0) {
	//		arduboy.drawFastHLine(80, i, 38, 1);
	//	}
	//	else {
	//		arduboy.drawFastHLine(80, 63 - i, 38, 1);
	//	}
	//}
	//// pixel 63 is always full white
	//arduboy.drawFastHLine(127, 63, 38, 1);

	// then we print to screen what is stored in our title variable we declared earlier
	//arduboy.print((__FlashStringHelper*)title);

	DrawHud();
	DrawCountdown();
	//arduboy.setCursor(0, 0);
	//arduboy.print(x);
	//arduboy.setCursor(30, 0);
	//arduboy.print(y);
	//arduboy.setCursor(60, 0);
	//arduboy.print(speed);
	//arduboy.setCursor(90, 0);
	//arduboy.print(fps);

	//uint16_t endMicros = micros();
	//arduboy.setCursor(90, 8);
	//arduboy.print(afterWaitMicros - startMicros);
	//arduboy.setCursor(90, 16);
	//arduboy.print(afterDisplay - afterWaitMicros);
	//arduboy.setCursor(90, 24);
	//arduboy.print(endMicros - afterDisplay);
	//arduboy.setCursor(90, 32);
	//// Special case for this profile: use a 'static' to print the time of the previous frame because we want to also include this print itself
	//static uint16_t previousFrameAfterPrintDelta = 0;
	//arduboy.print(previousFrameAfterPrintDelta);
	//previousFrameAfterPrintDelta = micros() - endMicros;

	// Only play sound during actual play, not while gameover:
	if (currentState == EState::Game) {
		if ((flicker & 1) == 0) {
			uint16_t freq = 16 + (speed >> 1);
			if (puffFrame != -1 && (flicker & 2) == 0) {
				freq = 800;
			}
			sound.tone(freq, 50);
			//else {
			//	sound.tone(16 + (enemySpeed >> 1), 30);
			//}
		}
	}
}

void drawOutrunTrack() {
	uint16_t trackWidth = 1 << 7;
	//uint16_t trackIncrement = 128 + (y << 4);
	uint16_t trackIncrement = 128 + (47 << 4);
	int16_t trackCenter = 64 << 4;

	uint16_t roadLineAcc = 0;

	if (gameTimer <= 99) {
		speedAcc += speed;
	}

	roadLineAcc = 8192 - (speedAcc << 6);

	int16_t roadCurve = (currentRoadCurve >> 1);
	trackCenter += roadCurve >> 2;
	//byte yMult = 139;
	//int8_t roadCurveInc = ((currentRoadCurve >> 8) * yMult) >> 8;
	int8_t roadCurveInc = currentRoadCurve / 118;
	int8_t newCenteredX = (playerX >> 10);
	if (trackCenteredX < newCenteredX && trackCenteredX < 45) {
		trackCenteredX++;
	}
	else if (trackCenteredX > newCenteredX && trackCenteredX > -45) {
		trackCenteredX--;
	}
	//byte xMult = 120;
	//int8_t xViewShift = (trackCenteredX * xMult + 128) >> 8;

	for (byte i = 20; i < 64; ++i) {
		//arduboy.drawFastHLine(0, i, 127, 1);

		byte pixWidth = trackWidth >> 8;
		trackPixWidth[i - 20] = pixWidth;

		int16_t startx = (trackCenter - (int16_t)(trackWidth >> 4)) >> 4;
		trackStartX[i - 20] = startx;

		byte sideline = (pixWidth >> 4) + 1;
		byte road = pixWidth - ((sideline * 3 + 1) >> 1);
		// using this if, we skip drawing completely if the HLine would be color 2 (gray), since the screen is pre-filled with gray
		if (roadLineAcc & 16384) {
			drawFastHLine(0, i, startx, 1);
		}

		drawFastHLine(startx, i, sideline, roadLineAcc & 8192 ? 1 : 0);
		startx += sideline;
		//arduboy.drawFastHLine(startx, i, road, 2);
		startx += road;

		if (roadLineAcc & 16384) {
			drawFastHLine(startx, i, sideline, 1);
		}
		startx += sideline;

		//arduboy.drawFastHLine(startx, i, road, 2);
		startx += road;

		drawFastHLine(startx, i, sideline, roadLineAcc & 8192 ? 1 : 0);
		startx += sideline;

		if (roadLineAcc & 16384) {
			drawFastHLine(startx, i, 256, 1);
		}

		//roadLineAcc += (45184 - trackWidth) >> 3;
		roadLineAcc += uint16_t(60000) / byte(6 + (trackWidth >> 9));

		trackWidth += trackIncrement;
		trackCenter -= trackCenteredX + ((-roadCurveInc + (roadCurve >> 4)) >> 2);
		roadCurve -= roadCurveInc;
	}
	// perspective: 
	// at horizon (20), track width should be 4
	// at bottom (63), track width should be 220

	//uint16_t trackWidth = 4 << 7;
	//uint16_t trackIncrement = 128;
	//int16_t trackCenter = 64 << 4;
	//static uint16_t stripOffset = 6912;
	//uint16_t stripSize = (stripOffset + (24000 / y) * (y - 32)) & 8191;

	//byte alternateStrip = 0;
	//arduboy.setCursor(90, 40);
	//arduboy.print(stripOffset);

	//static uint16_t speed = 0;

	///*
	//a y=26, strip 1280 (938 par y)
	//a y=30, strip 5120 (896 par y)
	//a y=32, strip 6912 reference
	//a y=34, strip 256  (768 par y)
	//a y=36, strip 1280 (640 par y)
	//a y=40, strip 3584 (608 par y)
	//*/

	//if (arduboy.pressed(A_BUTTON)) {
	//	speed--;
	//}
	//if (arduboy.pressed(B_BUTTON)) {
	//	speed++;
	//}

	//stripOffset += speed;
	//stripOffset = stripOffset & 8191;

	//for (byte i = 20; i < 64; ++i) {
	//	uint16_t pixWidth = trackWidth >> 7;
	//	//arduboy.drawFastHLine((trackCenter >> 4) - (pixWidth >> 1), i, pixWidth, 1);
	//	uint16_t startx = (trackCenter >> 4) - (pixWidth >> 1);
	//	uint16_t sideline = (pixWidth >> 4) + 1;
	//	uint16_t road = (pixWidth - sideline * 3) >> 1;
	//	arduboy.drawFastHLine(startx, i, sideline, 1);
	//	startx += sideline;
	//	arduboy.drawFastHLine(startx, i, road, 0);
	//	startx += road;
	//	if (alternateStrip) {
	//		arduboy.drawFastHLine(startx, i, sideline, 1);
	//	}
	//	startx += sideline;
	//	arduboy.drawFastHLine(startx, i, road, 0);
	//	startx += road;
	//	arduboy.drawFastHLine(startx, i, sideline, 1);

	//	trackWidth += trackIncrement;
	//	trackIncrement += (y);

	//	trackCenter += (x - 64);

	//	stripSize += 2048;
	//	if (stripSize > trackWidth) {
	//		alternateStrip ^= 1;
	//		stripSize -= trackWidth;
	//	}
	//}
}

// our main game loop, this runs once every cycle/frame.
// this is where our game logic goes.
void loop() {
 static uint32_t lastMillis;
 static uint32_t startTime;
	
	arduboy.nextFrame();
  flicker++;

	arduboy.display();
  paintScreen();
	arduboy.pollButtons();

  while (((ESP.getCycleCount()) - startTime) < TTOT /*it's global, sets in setup()*/);
  startTime = ESP.getCycleCount();

	if (millis() - lastMillis > 1000) {
		lastMillis = millis();
		if (gameTimer > 0) gameTimer--;
	}
	stateFrame++;

	switch (currentState) {
	case EState::Title:
		TitleState();
		break;
	case EState::Instructions:
		InstructionsState();
		break;
	case EState::Game:
	case EState::GameOver:
		GameState();
		break;
	}
}
