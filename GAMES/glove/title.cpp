#include <Arduino.h>
#include "globals.h"
#include "data.h"
#include "save.h"

#define WHITE_PIXEL_WAIT_TIME 20000
#define REDRAW_TITLE_WAIT_TIME 1000

void drawTitleAndSubtitle()
{
    // Get the length of the strings
    size_t titleLen = strlen_P(gameTitle);
    size_t subTitleLen = strlen_P(gameSubTitle);
	
	// Print16_t game title, large and centered
	arduboy.setTextSize(2);
	arduboy.setCursor(64 - titleLen*6, 12);
	arduboy.print(eps((char *)gameTitle));
	
	// Display game subtitle, small and centered
	arduboy.setTextSize(1);
	if(GameMode == GAME_MODE_RANDOM) {
		arduboy.setCursor(64 - 11*3, 30);
		arduboy.print(eps((char *)gameRandom));
	} else {
		arduboy.setCursor(64 - subTitleLen*3, 30);
		arduboy.print(eps((char *)gameSubTitle));
	}
}

void displayTitle()
{
	uint8_t upPressCount = 0;
	uint16_t randomCount;
	
	// Ticks until white pixels draw
	int16_t whitePixelDelay = WHITE_PIXEL_WAIT_TIME;
	
	// Ticks until glove logo redraw
	int16_t redrawTitleDelay = REDRAW_TITLE_WAIT_TIME;
    
	arduboy.clear();
	
	drawTitleAndSubtitle();

	int16_t roomsCleared = getRoomClearPercentage();
	
	// Display room clear count if the player beat at least level 1
	if(!GameMode && roomsCleared > 0)
	{
		arduboy.setCursor(6, 8*6 + 4);
		arduboy.print(F("ROOMS CLEARED: "));
		arduboy.print(roomsCleared);
		arduboy.print("%");
	}
	// Display copyright on an otherwise fresh file
	else
	{
		arduboy.setCursor(20, scrh-8);
		arduboy.print(F("(c) 2016 fuopy"));
	}
	
	arduboy.display();
	
	// Wait 1000 ticks
	delay(1000);
	
	// Start blanking out the screen
	while(true) {
		randomCount++;
		updateInput();
		
		if(UP_PRESSED){
			upPressCount++;
			if(upPressCount == 100) {
				clearAllRooms();
			}
		}
		
		// Draw black pixels
		arduboy.drawPixel(random(0,128), random(0,64), 0);
		arduboy.drawPixel(random(0,128), random(0,64), 0);
		// Draw white pixels
		if(whitePixelDelay <= 0) {
			arduboy.drawPixel(random(0,128), random(0,64), 1);
			redrawTitleDelay--;
			whitePixelDelay = 10;
			whitePixelDelay--;
		} else {
			whitePixelDelay--;
		}
		
		// Redraw title
		if(redrawTitleDelay <= 0) {
			drawTitleAndSubtitle();
			redrawTitleDelay = REDRAW_TITLE_WAIT_TIME;
		}
		
		arduboy.display();
		if(A_PRESSED) {
			randomSeed(random(100)+randomCount);
			break;
		}
	}
}
