#include "player.h"
#include "door.h"
#include "menu.h"
#include "pickup.h"

// globals ///////////////////////////////////////////////////////////////////

Player coolGirl;

int rollingScore = 0;
int16_t pickupsAvailable[] = {PICKUP_TYPE_COIN, PICKUP_TYPE_COIN, PICKUP_TYPE_COIN, PICKUP_TYPE_INACTIVE, PICKUP_TYPE_COIN, PICKUP_TYPE_HEART, PICKUP_TYPE_INACTIVE, PICKUP_TYPE_COIN, PICKUP_TYPE_INACTIVE, PICKUP_TYPE_COIN};
int16_t pickupsCounter;

// method implementations  ///////////////////////////////////////////////////

// initializePlayer
// sets the default values for player
void initializePlayer(Player& obj)
{
  scorePlayer = 0;
  rollingScore = 0;
  obj.direction = PLAYER_FACING_SOUTH;
  obj.health = PLAYER_START_HEALTH;
  obj.flashTime = 0;
  obj.shotDelay = 10;
  obj.coolDownVisible = true;
  obj.overHeated = false;
  obj.coolDownCounter = 0;
}

// updatePlayer
// updates the player according to game rules
void updatePlayer(Player& obj)
{
  if (obj.coolDownCounter > WEAPON_OVERHEAT) obj.overHeated = true;
  // Input velocity
  int16_t vx = 0;
  int16_t vy = 0;

  int16_t id;
  int16_t tileXMax;
  int16_t tileYMax;
  int16_t inputDirection = obj.direction;

  ///////////
  // input //
  ///////////

  bool left = arduboy.pressed(LEFT_BUTTON);
  bool right = arduboy.pressed(RIGHT_BUTTON);
  bool up = arduboy.pressed(UP_BUTTON);
  bool down = arduboy.pressed(DOWN_BUTTON);

  bool rungun = arduboy.pressed(A_BUTTON);
  bool standgun = arduboy.pressed(B_BUTTON);
  bool strafegun = rungun;

  obj.walking = up || down || left || right;
  //obj.walking = (standgun && !rungun) ? false : obj.walking;

  ////////////
  // timers //
  ////////////

  // Diagonal anti-jerk timer
  if (obj.diagonalTime > 0)
    obj.diagonalTime--;
  if ((up && left) || (down && left) || (up && right) || (down && right))
    obj.diagonalTime = 4;

  // Bullet timer
  if (obj.shotDelay > 0) obj.shotDelay--;

  ////////////////////////
  // horizontal physics //
  ////////////////////////

  // input
  if (left)
    vx = -1;
  else if (right)
    vx = 1;

  // update position
  //if(strafegun || !standgun)
  obj.x += vx;

  // collide with zombies
  zombieCollide(obj.x, obj.y, true, vx, PLAYER_WIDTH, PLAYER_HEIGHT);

  // collide with walls
  mapCollide(obj.x, obj.y, true, vx, PLAYER_WIDTH, PLAYER_HEIGHT);

  //////////////////////
  // vertical physics //
  //////////////////////

  // input
  if (up)
    vy = -1;
  else if (down)
    vy = 1;

  // update position
  //if(strafegun || !standgun)
  obj.y += vy;

  // collide with zombies
  zombieCollide(obj.x, obj.y, false, vy, PLAYER_WIDTH, PLAYER_HEIGHT);

  // collide with walls
  mapCollide(obj.x, obj.y, false, vy, PLAYER_WIDTH, PLAYER_HEIGHT);

  // collide with survivors
  survivorCollide(obj.x, obj.y);

  // collide with door
  if (checkDoorCollision()) gameState = STATE_GAME_PREPARE_LEVEL;

  // collide with pickup
  pickupCollision(obj.x, obj.y);

  ///////////////
  // direction //
  ///////////////

  // Update camera direction according to the way the player is moving

  if (!strafegun)
  {
    if (left)
    {
      inputDirection = PLAYER_FACING_WEST;
      if (up) inputDirection = PLAYER_FACING_NORTHWEST;
      else if (down) inputDirection = PLAYER_FACING_SOUTHWEST;
    }
    else if (right)
    {
      inputDirection = PLAYER_FACING_EAST;
      if (up) inputDirection = PLAYER_FACING_NORTHEAST;
      else if (down) inputDirection = PLAYER_FACING_SOUTHEAST;
    }
    else if (up)
    {
      inputDirection = PLAYER_FACING_NORTH;
    }
    else if (down)
    {
      inputDirection = PLAYER_FACING_SOUTH;
    }
  }

  obj.direction = inputDirection;

  // the camera will only be updated if moving nondiagonally or look mode
  if ((standgun && !rungun) || (obj.direction % 2) == 0)
  {
    obj.camDirection = inputDirection;
  }

  ////////////
  // timers //
  ////////////
  if (arduboy.everyXFrames(4))
  {
    if (obj.coolDownCounter > 1)obj.coolDownCounter--;
    if ((arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) && !obj.overHeated) obj.coolDownCounter += 2;
    if (obj.overHeated) coolGirl.coolDownVisible = !coolGirl.coolDownVisible;
  }

  if ((standgun || rungun) && !obj.overHeated)
  {
    if (obj.shotDelay == 0)
    {
      addBullet(obj.x + 10, obj.y + 12, obj.direction, 0, 0);
      obj.shotDelay = 10;
    }
  }

  if ((obj.overHeated == true) && (obj.coolDownCounter < 2))
  {
    obj.overHeated = false;
    obj.coolDownVisible = true;
  }

  // Update animation
  if (arduboy.everyXFrames(6) && obj.walking) obj.frame++;
  if (obj.frame > 3 ) obj.frame = 0;

  // update score
  if (rollingScore > 0)
  {
    rollingScore -= 5;
    scorePlayer += 5;
  }

  // update flashing
  if (obj.flashTime > 0)
    obj.flashTime--;

  ////////////
  // camera //
  ////////////

  // update camera
  short mapGoalX = coolGirl.x - WIDTH / 2 + PLAYER_WIDTH / 2;
  short mapGoalY = coolGirl.y - HEIGHT / 2 + PLAYER_HEIGHT / 2 - 4; // hud offset

  // offset the goal by the direction
  mapGoalX += BulletXVelocities[coolGirl.camDirection] * 4;
  mapGoalY += BulletXVelocities[(coolGirl.camDirection + 6) % 8] * 4;

  // move the camera toward the desired location
  mapPositionX = burp(mapPositionX, mapGoalX, 3);
  mapPositionY = burp(mapPositionY, mapGoalY, 3);

  // Clamp on screen boundaries
  mapPositionX = (mapPositionX < 0) ? 0 : mapPositionX;
  mapPositionX = (mapPositionX > LEVEL_WIDTH - WIDTH) ? LEVEL_WIDTH - WIDTH : mapPositionX;
  mapPositionY = (mapPositionY < 0) ? 0 : mapPositionY;
  mapPositionY = (mapPositionY > LEVEL_HEIGHT - HEIGHT) ? LEVEL_HEIGHT - HEIGHT : mapPositionY;
}

// hurtPlayer
// make player take damage
void playerHealthOffset(Player& obj, int8_t amount)
{
  if (obj.flashTime == 0)
  {
    obj.health += amount;

    if (amount < 0)
    {
      obj.flashTime = PLAYER_FLASH_TIME;
      sound.tone(880, 20);
    }

    if (obj.health > PLAYER_MAXHEALTH)
    {
      obj.health = PLAYER_MAXHEALTH;
    }
    else if (obj.health == 0)
    {
      gameState = STATE_GAME_OVER;
    }
  }
}

// drawPlayer
// draws the player to the screen
void drawPlayer(Player& obj)
{
  if ((obj.flashTime % 8) < 4)
  {
    sprites.drawErase(obj.x - mapPositionX, obj.y - mapPositionY, playerAmyMask, obj.direction);
    sprites.drawSelfMasked(obj.x - mapPositionX, obj.y - mapPositionY, playerAmy, obj.frame + 4 * obj.direction);
  }
}

void drawLife(Player& obj)
{
  for (int16_t amountLife = 0; amountLife < obj.health; amountLife++)
  {
    sprites.drawPlusMask((amountLife / 2) * 10, 0, HUD_plus_mask, 0);
  }
  for (int16_t amountLife = 0; amountLife < obj.health / 2; amountLife++)
  {
    sprites.drawPlusMask(amountLife * 10, 0, HUD_plus_mask, 1);
  }
}

void drawCoolDown()
{
  if (coolGirl.coolDownVisible)
  {
    for (int16_t i = 0; i < coolGirl.coolDownCounter; i++)
    {
      sprites.drawPlusMask(i, 7, coolDownLine_plus_mask, 0);
    }
  }
}
