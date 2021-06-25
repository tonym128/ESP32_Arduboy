/*----------------------
    PowerUps
  --------------------- */

struct PowerUps
{
  float x = 128;
  float y = 32;
  uint8_t type = 0;
  bool active = false;
  float xspd = 0;
  float yspd = 0;
  uint8_t spCount = 0;
} powerup;


void initPowerUp() {
  powerup.spCount = 0;
  powerup.active = false;
}


void createPowerUp(uint8_t type)
{
  powerup.xspd = 1;
  powerup.yspd = 1.5;

  powerup.x = 10 + random(110);
  powerup.y = 10 + random(34);

  //
  if (pX - 8 < powerup.x && powerup.x < pX + 16) {
    return;
  }

  powerup.type = type;
  powerup.active = true;
}

void movePowerUp()
{
  powerup.spCount++;

  if (powerup.spCount == 50) {
    powerup.active = false;
  }

  if (powerup.spCount == 200) {
    powerup.spCount = 0;

    uint8_t r = random(100);
    uint8_t item[] = {5, 20, 30, 45, 50};
    for (int i = 0;i < 5;i++) {
      if (r <= item[i]) {
        createPowerUp(i);
        break;
      }
    }
  }

  collisionPowerUp();
}


void collisionPowerUp()
{
  if (powerup.active) {
    if (powerup.x < pX + 8 &&
        powerup.x + 8 > pX &&
        powerup.y < pY + 8 &&
        8 + powerup.y > pY)
    {
      if (powerup.type == 0) // heart
      {
        sound.tone(NOTE_C4, 80);
        lives++;
        powerup.active = false;
        powerup.x = 128;
      }
      if (powerup.type == 1) // spring=+1
      {
        setAddSpring();
        sound.tone(NOTE_C4, 80);
        powerup.active = false;
      }
      if (powerup.type == 2) // spring=-1
      {
        // setAllOneSpring();
        // setOddSpring();
        setDelSpring();
        sound.tone(NOTE_C2, 80);
        powerup.active = false;
      }

      if (powerup.type == 3) // coin add item
      {
        resetSpCount();
        updateCurrentCoinMax(3);
        sound.tone(NOTE_C3, 80);
        powerup.active = false;
      }

      if (powerup.type == 4) // flag item
      {
        setAddMaxSpring();
        sound.tone(NOTE_C4, 80);
        powerup.active = false;
      }
    }
  }
}
/*------------------
    Draw Power Up
  ----------------- */
void drawPowerUp()
{
  if (powerup.active) {
    if (powerup.type == 0) {
      arduboy.drawBitmap(powerup.x, powerup.y, bHeart, 8, 8, 1);
    }
    if (powerup.type == 1) {
      arduboy.drawSlowXYBitmap(powerup.x, powerup.y, bItemPlus, 8, 8, 1);
    }
    if (powerup.type == 2) {
      arduboy.drawSlowXYBitmap(powerup.x, powerup.y, bItemMinus, 8, 8, 1);
    }
    if (powerup.type == 3) {
      arduboy.drawSlowXYBitmap(powerup.x, powerup.y, bItemAddCoins, 8, 8, 1);
    }
    if (powerup.type == 4) {
      arduboy.drawSlowXYBitmap(powerup.x, powerup.y, bItemFlag, 8, 8, 1);
    }
  }
}
