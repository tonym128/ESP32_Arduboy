#include "world.h"
#include "physics.h"

void initialize_explorer(Explorer& obj)
{
	// Set default values
	obj.x = spawnx*8;
	obj.y = spawny*8;
	obj.direction = FACE_UP;
	obj.frame = false;
	obj.frameTime = 0;
	obj.active = true;
	obj.health = 1000;
	obj.nextHealthDecrease = 5;
}

void update_explorer(Explorer& obj)
{
	int8_t i, vx, vy;
	int16_t wx, wy, ww, wh;
	int16_t nvx, nvy;
	
	// Game over if button pressed while dead
	if(!obj.active) {
		if((B_PRESSED || A_PRESSED) && !quitGame) {
			doGameOver();
		}
		return;
	}
	
	// Add health from rolling health
	if(rollingHealth > 0) {
		rollingHealth -= 10;
		obj.health += 10;
		if(obj.health > 1000) {
			obj.health = 1000;
			rollingHealth = 0;
		}
	}
	
	// Decrease health
	if(obj.nextHealthDecrease <= 0) {
		obj.nextHealthDecrease = 5;
		damage_explorer(obj, 1);
	}
	obj.nextHealthDecrease--;
	
	// Update animation
	if(LEFT_DOWN || RIGHT_DOWN || UP_DOWN || DOWN_DOWN) {
		obj.frameTime ++;
		if(obj.frameTime > 8) {
			obj.frame = !obj.frame;
			obj.frameTime = 0;
		}
	} else {
	// If facing left or right, set sprite to standing
		if(obj.direction == FACE_LEFT || obj.direction == FACE_RIGHT)
			obj.frame = true;
	}
	
	// Get horizontal velocity
	if(RIGHT_DOWN) {
		vx = 1;
		obj.direction = FACE_RIGHT;
	} else if(LEFT_DOWN) {
		vx = -1;
		obj.direction = FACE_LEFT;
	} else {
		vx = 0;
	}
	
	// Update horizontal position
	obj.x += vx;
	obj.x = (obj.x + 8 > gamew) ? gamew - 8 : obj.x;
	obj.x = (obj.x < 0) ? 0 : obj.x;
	
	// WALL COLLISION
	for(i = 0; i<numWalls; ++i) {
		if(!walls[i].active) continue;
		wx = walls[i].x*8;
		wy = walls[i].y*8;
		ww = walls[i].w*8;
		wh = walls[i].h*8;
		collideSpriteWall(obj.x, obj.y, true, vx, wx, wy, ww, wh);
	}
	// SPAWNER COLLISION
	for(i = 0; i<numSpawners; ++i) {
		if(!spawners[i].active) continue;
		wx = spawners[i].x*8;
		wy = spawners[i].y*8;
		collideSpriteSprite(obj.x, obj.y, true, vx, wx, wy);
	}
	
	// BADGUY COLLISION
	for(i = 0; i<numBadguys; ++i) {
		if(!badguys[i].active) continue;
		wx = badguys[i].x;
		wy = badguys[i].y;
		if(collideSpriteSprite(obj.x, obj.y, true, vx, wx, wy))
		{
			damage_explorer(p1, 1);
		}
	}
	
	// Get vertical velocity
	if(DOWN_DOWN) {
		vy = 1;
		obj.direction = FACE_DOWN;
	} else if(UP_DOWN) {
		vy = -1;
		obj.direction = FACE_UP;
	} else {
		vy = 0;
	}
	
	// Update vertical position
	obj.y += vy;
	obj.y = (obj.y + 8 > gameh) ? gameh - 8 : obj.y;
	obj.y = (obj.y < 0) ? 0 : obj.y;
	
	// WALL COLLISION
	for(i = 0; i<numWalls; ++i) {
		if(!walls[i].active) continue;
		wx = walls[i].x*8;
		wy = walls[i].y*8;
		ww = walls[i].w*8;
		wh = walls[i].h*8;
		collideSpriteWall(obj.x, obj.y, false, vy, wx, wy, ww, wh);
	}
	
	// SPAWNER COLLISION
	for(i = 0; i<numSpawners; ++i) {
		if(!spawners[i].active) continue;
		wx = spawners[i].x*8;
		wy = spawners[i].y*8;
		collideSpriteSprite(obj.x, obj.y, false, vy, wx, wy);
	}
	
	// BADGUY COLLISION
	for(i = 0; i<numBadguys; ++i) {
		if(!badguys[i].active) continue;
		wx = badguys[i].x;
		wy = badguys[i].y;
		if(collideSpriteSprite(obj.x, obj.y, false, vy, wx, wy))
		{
			damage_explorer(p1, 1);
		}
	}
	
	// KEY COLLISION
	for(i = 0; i<numKeys; ++i) {
		if(!keys[i].active) continue;
		wx = keys[i].x * 8;
		wy = keys[i].y * 8;
		if(intersectSpriteSprite(obj.x, obj.y, wx, wy))
		{
			activate_key(keys[i]);
		}
	}
	
	// EXIT COLLISION
	for(i = 0; i<numExits; ++i) {
		if(!exits[i].active) continue;
		wx = exits[i].x * 8;
		wy = exits[i].y * 8;
		if(intersectSpriteSprite(obj.x, obj.y, wx, wy))
		{
			activate_exit(exits[i]);
		}
	}
	
	// TREASURE COLLISION
	for(i = 0; i<numTreasures; ++i) {
		if(!treasures[i].active) continue;
		wx = treasures[i].x * 8;
		wy = treasures[i].y * 8;
		if(intersectSpriteSprite(obj.x, obj.y, wx, wy))
		{
			activate_treasure(treasures[i]);
		}
	}
	
	// Player shooting
	if(A_PRESSED) {
		autoFireTime = 8;
		nvx = (obj.direction == FACE_LEFT || LEFT_DOWN) ? -2 : 0;
		nvx = (obj.direction == FACE_RIGHT || RIGHT_DOWN) ? 2 : nvx;
		nvy = (obj.direction == FACE_UP || UP_DOWN) ? -2 : 0;
		nvy = (obj.direction == FACE_DOWN || DOWN_DOWN) ? 2 : nvy;
		
		add_bullet(obj.x+3, obj.y+3, nvx+vx, nvy+vy);
	}
	
	// Update Camera
	scrollx = 64 - obj.x;
	scrolly = 32 - obj.y;
}

void draw_explorer(Explorer& obj)
{
	// Select sprite to draw
	const uint8_t* drawSpr = (uint8_t*) sprites + SPRITE_GUY + (obj.direction*2*8) + (obj.frame%2)*8;
    
	// Draw the sprite if alive
    if(obj.active)
        arduboy.drawBitmap(obj.x+scrollx, obj.y+scrolly, drawSpr, 8, 8, 1);
    
	// Display health
	arduboy.setCursor(0, scrh - 8);
    //arduboy.print(arduboy.cpuLoad());
	arduboy.print(obj.health);
}

void damage_explorer(Explorer& obj, int16_t dmg)
{
	// Remove health
	obj.health -= dmg;
	
	// Kill player if health drops below zero
	if(obj.health <= 0) {
		obj.active = false;
		obj.health = 0;
	}
}
