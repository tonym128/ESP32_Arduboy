/*
 *      Player.cpp
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class for Player objects.
 */

#include "Drawing.h"
#include "Player.h"
#include "globals.h"

//const int[] = {0x0c, 0x0d, 0x25, 0x4d, 0x5f, 0x75, 0x55, 0x59};

// Helicopter Sprite
//PROGMEM const unsigned char helicopter[] = {0x0c, 0x0d, 0x25, 0x4d, 0x5f, 0x75, 0x55, 0x59};
//PROGMEM const unsigned char heliRight[] = {0x0c, 0x0c, 0x24, 0x4c, 0x5f, 0x75, 0x55, 0x59};
//PROGMEM const unsigned char heliLeft[]  = {0x0c, 0x0d, 0x25, 0x4d, 0x5f, 0x74, 0x54, 0x58};
PROGMEM const unsigned char heliRight[] = {0x0c, 0x0d, 0x25, 0x4d, 0x5f, 0x76, 0x56, 0x5a};
PROGMEM const unsigned char heliLeft[]  = {0x0c, 0x0e, 0x26, 0x4e, 0x5f, 0x75, 0x55, 0x59};


void Player::Init() { 

	// Set up the heli sprite and texture
//    heliImage.LoadImage("/3ds/Helii3DS/img/heli.png");
//	heliSprite.SetImage(&heliImage);
	heliSprite.SetX(32);
	heliSprite.DefineCollisionRectangle(0, 0, 8, 6);

    // Obviated by SetImage now
    //sf2d_texture * tex = heliImage.GetTexture();
    heliSprite.SetWidth(PLAYER_SIZE);
    heliSprite.SetHeight(PLAYER_SIZE);

	// Set up the fire sprite and texture
//	fireImageSheet.LoadImage("/3ds/Helii3DS/img/fire.png");
//	fireSprite.SetImage(&fireImageSheet, 64, 64);
//	fireSprite.SetX(120);

    theSmokeTrail.Init();

	// Set default physical state
	spawn();
}

// Destructor
Player::~Player() { }

// Initializer
void Player::spawn() {

	// Reset the smoke trail
	theSmokeTrail.resetAll();

	// Starting position for a Player
	y_coord = (HEIGHT / 2) - (PLAYER_SIZE / 2);

	// Reset Player velocity to 0
	y_veloc = -1;

    // Reset color
    heliSprite.SetTint(0xFF, 0xFF, 0xFF);
}

int Player::GetY() {
	return y_coord;
}

// Returns true if the helicopter is touching a given rectangle
bool Player::CollidesWith(const Rectangle rect, float x, float y) {
	return heliSprite.CollidesWith(rect, x, y);
}

// Control the player
void Player::thrust()
{
	y_veloc -= THRUST;		// Remember, the y- direction is up.
}

void Player::die()
{
    heliSprite.SetTint(0xFF, 0x00, 0x00);
}

// Reposition the player for a new frame
void Player::Move()
{
	y_veloc += GRAVITATION;		// Gravity influences velocity

	// Keep the velocity within bounds
	if (y_veloc > MAX_VELOCITY)
		y_veloc = MAX_VELOCITY;
	else if (y_veloc < -MAX_VELOCITY)
		y_veloc = -MAX_VELOCITY;

	y_coord += y_veloc;		// Velocity influences position

	theSmokeTrail.update();	// Update the smoke trail
}

// Draw the player
void Player::Draw()
{
	// Draw the smoke trail
	//theSmokeTrail.Draw();
    
    

	// Place the sprite where the player is
	heliSprite.SetY(y_coord);

	// Rotate the helicopter in accordance with its movement
	//heliSprite.SetRotation(y_veloc);

  // Draw the heli sprite
  uint8_t color = (g_currentState == DEAD_STATE) && ((g_ticks % 2) == 0) ? BLACK : WHITE;
	//heliSprite.Draw();
  arduboy.drawBitmap(
    heliSprite.GetX(),
    heliSprite.GetY(),
    (g_ticks % 4 < 2) ? heliLeft : heliRight,
    8,
    8,
    WHITE
  );
//
//	// Draw the fire, if DEAD_STATE.
//	if (g_currentState == DEAD_STATE) {
//		fireSprite.SetY(y_coord);
//		if (g_ticks % 5 == 0)
//			fireSprite.NextFrame();
//		fireSprite.Draw();
//	}
    /*
    // FIXME: Just draw a plain rect
    rend.drawFillRect(
        100,
        y_coord,
        100 + 16,
        y_coord + 16,
        255,
        0,
        0,
        rend.topl
    ); */
}
