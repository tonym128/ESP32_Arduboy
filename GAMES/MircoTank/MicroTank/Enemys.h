#ifndef Enemys_h
#define Enemys_h

#define MAX_ENEMYS      12
#define ENEMY_TYPES     4

#define STATE_DEAD     250
#define STATE_SPAWN    255

enum {
  ENEMY_ROCKET,
  ENEMY_STD,
  ENEMY_MG,
  ENEMY_SHIELD,
  ENEMY_FLAME,  
  ENEMY_MINE,
  ENEMY_CAR,
  ENEMY_TREE
};

struct enemy {
  uint8_t x;
  uint8_t y;
  uint8_t type;
  uint8_t state;
  uint16_t health;
  bool active = false;

  void update() {
    if (slowmoTimeout % 2)
      return;

    // move enemy with the map
    if (gameFrames % MOVE_MAP_INTERVAL == 0)
      x -= 1;

    // trees do nothing
    if (type == ENEMY_TREE)
      return;

    // cars move if they are alive with diffent speeds
    if (type == ENEMY_CAR) {
      if (gameFrames % (2 + state) == 0 && health > 0)
        x -= 1;
      return;
    }

    uint8_t framesModulo10 = gameFrames % 10;

    if (type == ENEMY_MINE) {
      if (gameFrames % 5 == 0)
        state = (state + 1) % 8;

      return;
    }

    // now we animate humans


    // set next state for die animation
    if (health == 0) {
      // update ememys only 3 times per second
      if (framesModulo10 != 0)
        return;

      //Serial.println(String(millis()) + " type:" + String(type) + " state:" + String(state));

      state++;

      // end of die animation
      if (state > STATE_DEAD + 3)
        state = STATE_SPAWN;

      return;
    }

    if (type == ENEMY_MG) {
      state++;

      if (state < 60 && state % 3 == 0) {
        uint8_t heatSim = (100 - state) / 4  + rand() % 16;

        //Serial.println("state:" + String(state) + " heat:" + String(heatSim));
        pM.add(x, y + 4, WEAPON_MG, heatSim, false);
      }

      // keep the states inside the allowed area
      if (state == 120 - 1)
        state = 0;

      return;
    }

    if (type == ENEMY_FLAME) {

      // do this slowly because it is also walking
      if (framesModulo10 == 0) {
        state++;

        // walk a bit
        if (state < 10 && state % 3 == 0)
          x--;
      }

      // fire
      if (state >= 10)
        pM.add(x - 4, y + 5, WEAPON_FLAME, 0, false);

      if (state == 20 - 1)
        state = 0;

      return;
    }

    // update these ememys only 3 times per second
    if (framesModulo10 != 0)
      return;

    // next frame
    state = (state + 1) % 4;

    // move at state 3
    if (state == 3)
      x -= 1;

    // shoot at state
    if (state == 2) {

      //add(uint8_t x, uint8_t y, uint8_t type, int8_t state, bool player) {

      if (type == ENEMY_STD) {
        // draw mg
        pM.add(x + 1, y + 4, WEAPON_STDGUN, 0, false);

      } else if (type == ENEMY_ROCKET) {
        // calculate vertical distance to player tank
        int8_t yDist = pT.y - y;

        pM.add(x + 1, y + 5, WEAPON_SMALL_ROCKET, yDist, false);
      }
    }
  }

  bool damage(uint8_t dmg) {

#ifdef DEGUG_DMG_ENEMYS    
    Serial.print("Enemy got DMG:");
    Serial.print(dmg, DEC);
#endif 

    if (dmg == 0)
      return false;

    uint16_t tmpDmg = dmg * 100;

    // reduce the damage because of map progress
    uint16_t reducePercent = traveledDist / 128;

#ifdef DEGUG_DMG_ENEMYS   
    Serial.print(" -%:");
    Serial.print(reducePercent, DEC);
#endif 

    tmpDmg -= dmg * reducePercent;

#ifdef DEGUG_DMG_ENEMYS   
    Serial.print(" is:");
    Serial.println(float(tmpDmg)/100, 2);
#endif 

    if (health > tmpDmg) {
      health -= tmpDmg;

    } else {
      //Serial.println("Enemy Died");
      die();

      // trees and cars also count, they are important!
      enemyKills++;

      return true;
    }

    return false;
  }

  void die() {
    health = 0;
    state = STATE_DEAD;

    // add car spawn item imediatelly
    if (type == ENEMY_CAR) {
      iM.addRandom(x, y - 2);

      aM.add(x + 4, y - 1, ANIMATION_BOOM, 0);

    } else if (type == ENEMY_MINE) {

      aM.add(x - 2, y, ANIMATION_IMPACT_L, 0);
      aM.add(x + 2, y, ANIMATION_IMPACT_R, 0);

      aM.add(x, y - 4, ANIMATION_BOOM, 0);

    } else if (type == ENEMY_FLAME) {

      aM.add(x + 4, y, ANIMATION_BOOM, 0);
    }
  }

  bool offScreen() {
    // add + something because they spawn right to the screen
    if (x > 128 + 10 || y > 63) {
      return true;
    } else {
      return false;
    }
  }

  void drawCrushedThing(uint8_t offset) {
    drawOffsetBitmap(x, y + offset, crushedThing, 32, 8, 2, (state % 4) * 8, false, BLACK);
  }

  void drawHealthBar() {
    // draw healt bar
    uint8_t hLen = (health / 100) / 14 + 1;
    arduboy.fillRect((8 - hLen) + x, y, hLen, 1, BLACK);
  }

  void drawFeet(uint8_t s) {
    if (s < 2) {
      // standing still
      arduboy.drawPixel(x + 3, y + 7, BLACK);
      arduboy.drawPixel(x + 4, y + 7, BLACK);

    } else if (s == 2) {
      // step forward and shoot
      arduboy.drawPixel(x + 2, y + 7, BLACK);
      arduboy.drawPixel(x + 4, y + 7, BLACK);

    } else if (s == 3) {
      // step back
      arduboy.drawPixel(x + 3 , y + 7, BLACK);
      arduboy.drawPixel(x + 5, y + 7, BLACK);
    }
  }

  void draw() {
    // this item just waits to be removed
    if (state == STATE_SPAWN)
      return;

    if (type == ENEMY_TREE) {
      if (health == 0) {
        drawCrushedThing(6);
      } else {
        drawOffsetBitmap(x, y, enemyTrees, 24, 6, 8, state * 6, false, BLACK);
      }
      return;
    }

    if (type == ENEMY_CAR) {
      if (health == 0) {
        drawCrushedThing(5);
      } else {
        drawOffsetBitmap(x, y + 2, enemyCars, 48, 8, 5, state * 8, true, BLACK);
        drawHealthBar();
      }
      return;
    }

    if (type == ENEMY_MINE) {
      if (health == 0)
        return;

      arduboy.drawPixel(x + 1, y + 4, BLACK);
      arduboy.drawPixel(x + 3, y + 3, BLACK);
      arduboy.drawPixel(x + 5, y + 4, BLACK);

      // nightrider dot
      if (state < 5) {
        arduboy.drawPixel(x + 1 + state, y + 4, BLACK);
      } else {
        arduboy.drawPixel(x + 9 - state, y + 4, BLACK);
      }
      return;
    }

    if (type == ENEMY_MG) {
      if (health == 0) {
        // draw only the MG
        drawOffsetBitmap(x, y + 3, enemyMG, 24, 5, 5, 8, false, BLACK);
      } else {
        uint8_t m = 0;

        if (state < 70) {
          m = 0;
        } else if (state < 80) {
          m = 1;
        } else if (state < 90) {
          m = 2;
        } else {
          m = 1;
        }

        if (state < 60 && state % 3 == 0)
          drawSmallFire(x - 1, y + 4, false);

        drawOffsetBitmap(x, y + 3, enemyMG, 24, 8, 5, m * 8, false, BLACK);
        drawHealthBar();
        return;
      }
    }

    // draw human dead animation
    if (state >= STATE_DEAD && state < STATE_DEAD + 4) {
      uint8_t o;
      if (type == ENEMY_MG) {
        o = 6;
      } else {
        o = 2;
      }

      drawOffsetBitmap(x + o, y + 4, humanFall, 24, 6, 4, (state - STATE_DEAD) * 6, false, BLACK);
      return;
    }

    // draw body with eye
    arduboy.fillRect(x + 3, y + 3, 2, 4, BLACK);
    arduboy.drawPixel(x + 3, y + 4, WHITE);

    drawFeet(state % 4);

    drawHealthBar();

    // draw weapon
    if (type == ENEMY_STD) {
      // draw mg
      arduboy.drawPixel(x + 1 , y + 5, BLACK);
      arduboy.drawPixel(x + 2, y + 5, BLACK);

      // draw fire in front of weapon
      if (state == 2)
        drawSmallFire(x, y + 5, false);

    } else if (type == ENEMY_ROCKET) {
      // draw rocket launcher
      arduboy.drawPixel(x + 1 , y + 4, BLACK);
      arduboy.drawPixel(x + 2, y + 4, BLACK);
      arduboy.drawPixel(x + 5, y + 4, BLACK);

      // draw fire behind the rocket launcher
      if (state == 2)
        drawSmallFire(x + 6, y + 4, true);

    } else if (type == ENEMY_SHIELD) {
      // draw shield
      arduboy.fillRect(x + 1 , y + 3, 1, 4, BLACK);
      arduboy.drawPixel(x + 2, y + 5, BLACK);

      if (state == 0)
        arduboy.drawPixel(x + 5, y + 5, BLACK);

    } else if (type == ENEMY_FLAME) {
      // draw flame thrower
      if (state < 8) {
        arduboy.drawPixel(x + 1 , y + 6, BLACK);
      } else {
        arduboy.drawPixel(x + 1 , y + 5, BLACK);
      }
      arduboy.drawPixel(x + 2, y + 5, BLACK);

      // draw backpack
      arduboy.fillRect(x + 5 , y + 3, 3, 4, BLACK);
      arduboy.drawPixel(x + 5, y + 3, WHITE);
      arduboy.drawPixel(x + 5, y + 6, WHITE);
    }

  }

  bool touchesPosition(uint8_t p_x, uint8_t p_y, uint8_t p_w, uint8_t p_h) {
    // player to far left
    if (p_x + p_w < x + 3)
      return false;

    // player to high up
    if (p_y + p_h < y + 3)
      return false;

    // player to far right
    if (x + 7 < p_x + 3)
      return false;

    // player to far down
    if (y + 7 < p_y + 3)
      return false;

    return true;
  }

};

struct enemyManager {
  enemy list[MAX_ENEMYS];

  void init() {
    for (uint8_t i = 0; i < MAX_ENEMYS; i++) {
      list[i].active = false;
    }
  }

  void add(uint8_t x, uint8_t y, uint8_t type, uint8_t state) {

    bool foundSlot = false;
    for (uint8_t i = 0; i < MAX_ENEMYS; i++) {

      // look if this slot is already used
      if (list[i].active == true)
        continue;

      list[i].active = true;
      foundSlot = true;

      // set values
      list[i].x = x;
      list[i].y = y;
      list[i].type = type;
      list[i].state = state;
      list[i].health = 100 * 100;
      break;
    }

#ifdef DEBUG_ADD_FUNCTIONS
    if (!foundSlot)
      Serial.println("Warning, no slot for enemy!");
#endif      
  }

  uint8_t isEnemyAt(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {

    for (uint8_t i = 0; i < MAX_ENEMYS; i++) {

      // check only active enemys
      if (list[i].active == false)
        continue;

      // if active[0].isPosition(..) return index 1, 0 is no item
      if (list[i].touchesPosition(x, y, w, h))
        return i;

    }

    return 0xff;
  }

  uint8_t count() {
    uint8_t num = 0;

    for (uint8_t i = 0; i < MAX_ENEMYS; i++) {
      if (list[i].active && list[i].health > 0)
        num++;
    }

    return num;
  }

  void update() {

    for (uint8_t i = 0; i < MAX_ENEMYS; i++) {

      // update only active enemys
      if (list[i].active == false)
        continue;

      list[i].update();

      // if they went inactive while updating
      if (list[i].active == false)
        continue;

      // check if off screen
      if (list[i].offScreen())
        list[i].active = false;

      list[i].draw();
    }
  }
};

#endif
