#include <Arduino.h>
#include "save.h"
#include "globals.h"
#include "prompt.h"
#include "game.h"
#include "records.h"
#include "world.h"

void gameGotoLevel(uint8_t level)
{
	
	// If the level destination is zero, increment the current level
	if(level == 0) level = currentLevel + 1;
	
	// Wrap the last level around to the first
	if(level == 255) level = 0;
	
	// If there are no more levels, finish the game
	if(level >= numLevels) {
		doGameComplete();
		return;
	}
	
	// Add the remainder of rolling score to score variable
	if(rollingScore > 0) score += rollingScore;
	
	// Hide the rolling score
	rollingScore = -60;
	
	// Add the remainder of rolling health to health variable
	if(rollingHealth > 0) p1.health += rollingHealth;
	rollingHealth = 0;
	
	// Set the current level
	currentLevel = level;
	
	// Autosave progress
	saveGame(GAME_SAVE_FILE);
	
	// Draw the level name
	arduboy.clear();
	arduboy.setCursor(0, 64-8);
	arduboy.print(eps((char *)(levelNames + level*16)));
	arduboy.display();
	delay(1000);
	
	// Load the next level
	worldLoadLevel();
}

void gameSetup()
{
	// Initialize game variables
	levelsCompleted = 0;
	autoFireTime = 0;
	rollingScore = -60;
	quitGame = false;
	initialize_explorer(p1);
	currentLevel = 0;
}

void gameUpdate()
{
	int16_t i;

	// Update the spawners
	for(i=0; i<numSpawners; ++i) update_spawner(spawners[i]);
	
	// Update the bad guys, if it's time
	if(BadGuy_UpdateDelay <= 0)
	{
		// Update the badguys
		for(i=0; i<numBadguys; ++i) update_badguy(badguys[i]);
		
		// Set delay until next update
		BadGuy_UpdateDelay = 3;
	}
	
	// Update timer
	BadGuy_UpdateDelay--;
	
	// Update the bullets
	for(i=0; i<numBullets; ++i) update_bullet(bullets[i]);
	
	// Update the player
	update_explorer(p1);
	
	// Add score from the rolling score
	if(rollingScore > 0) score++;
	
	// Update timer that persists after rolling score is empty
	if(rollingScore > -60) rollingScore--;
	
	// Update the game timer
	if(gameTimeTick <= 0) {
		gameTime ++;
		gameTimeTick = 46;
	}
	gameTimeTick--;
}

void flashScreen()
{
	// Display a white rectangle
	arduboy.fillRect(0, 0, scrw, scrh, 1);
	arduboy.display();
	
	// Set the screen timer
	whiteScreenTime = 2;
}

void addScore(int16_t points)
{
	// If at end of score rolling animation, set the score
	if(rollingScore < 0)
	{
		rollingScore = points;
	}
	// Add to the rolling score
	else
	{
		rollingScore += points;
	}
}

void gameDraw()
{
	int16_t i;
    const uint8_t* drawSpr = (uint8_t*) sprites + SPRITE_WALL;
	
	// Draw flash effect
	if(whiteScreenTime > 0) {
		whiteScreenTime --;
		return;
	}
	
	arduboy.clear();
	
	// Draw the player
	draw_explorer(p1);
	
	// Draw all walls
	for(i=0; i<numWalls; ++i) {
		draw_wall(walls[i]);
	}
    
	// Draw level border
	for(i=-1; i<MAP_WIDTH; i++) {
		arduboy.drawBitmap(i*8+scrollx, -8+scrolly, drawSpr, 8, 8, 1);
		arduboy.drawBitmap(i*8+scrollx, gameh+scrolly, drawSpr, 8, 8, 1);
	}
	for(i=-1; i<=MAP_HEIGHT; i++) {
		arduboy.drawBitmap(-8+scrollx, i*8+scrolly, drawSpr, 8, 8, 1);
		arduboy.drawBitmap(gamew+scrollx, i*8+scrolly, drawSpr, 8, 8, 1);
	}
    
	// Draw game objects
	for(i=0; i<numSpawners; ++i)  draw_spawner(spawners[i]);
	for(i=0; i<numTreasures; ++i) draw_treasure(treasures[i]);
	for(i=0; i<numKeys; ++i)      draw_key(keys[i]);
	for(i=0; i<numExits; ++i)     draw_exit(exits[i]);
	for(i=0; i<numBadguys; ++i)   draw_badguy(badguys[i]);
	for(i=0; i<numBullets; ++i)   draw_bullet(bullets[i]);
	
	// Draw score
	if(rollingScore > -60) {
		arduboy.setCursor(0, 0/*scrh-8*/);
		arduboy.print(score);
		arduboy.print("00 pts");
	}
	
	arduboy.display();
}

void displayGameEnd()
{
	// Display playthough statistics
	arduboy.setCursor(6, 8*3);
	arduboy.print(F("You cleared "));
	arduboy.print(levelsCompleted);
	arduboy.print(F(" rooms"));
	arduboy.setCursor(6*6, 8*4);
	arduboy.print(F("In "));
	printTime(gameTime);
	arduboy.setCursor(2*8, 8*5);
	arduboy.print(F("With a score of"));
	arduboy.setCursor(6*5, 8*6);
	arduboy.print(score);
	arduboy.print(F("00 points"));
	arduboy.display();
	
	// Wait for input before quitting game
	tautInput();
	while(true) {
		updateInput();
		if(A_PRESSED || B_PRESSED) break;
	}
	
	// Quit game, delete save and handle high score input
	quitGame = true;
	deleteContinueSave();
	saveHighScore();
}

void doGameComplete()
{
	// Draw screen title
	arduboy.clear();
	arduboy.setCursor(3*6, 8*1);
	arduboy.print(F("Game Complete!"));

	// Handle the rest of game end
	displayGameEnd();
}

void doGameOver()
{
	// Save game to prevent user from retrying
	saveGame(GAME_SAVE_FILE);
	
	// Draw screen title
	arduboy.clear();
	arduboy.setCursor(5*8-4, 8*1);
	arduboy.print(F("Game Over"));

	// Handle the rest of game end
	displayGameEnd();
}

void gameLoop()
{
	// Repeat forever until it is time to quit
	while(!quitGame)
	{
		// Frame rate management
        if (!(arduboy.nextFrame())) continue;
		
		// Update input
		updateInput();
		
		// Update game
		gameUpdate();
		
		// Quit game if it's time
		if(quitGame) break;
		
		// Draw game
		gameDraw();
	}
}

void displayGame()
{
	// Setup game
	gameSetup();
	
	// Clear variables
	score = 0;
	gameTime = 0;
	gameTimeTick = 0;
	
	// Go to first level
	if(GameMode == GAME_MODE_RANDOM)
		gameGotoLevel(random(0,numLevels-1));
	else
		gameGotoLevel(255);
	
	// Enter game loop
	gameLoop();
}
