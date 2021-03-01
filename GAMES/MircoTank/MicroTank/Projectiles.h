#ifndef Projectiles_h
#define Projectiles_h

#define MAX_PROJECTILES 64

struct projectile {
  uint8_t x;
  uint8_t y;
  uint8_t type;
  int8_t state;
  bool player;
  bool active;

  void draw() {
    if (type == WEAPON_STDGUN) {

      if (player) {
        arduboy.fillRect(x, y, 3 + pT.lvlWeapon[WEAPON_STDGUN], 1, BLACK);

      } else {
        arduboy.drawPixel(x, y, BLACK);
        arduboy.drawPixel(x + 1, y, BLACK);

      }

    } else if (type == WEAPON_ROCKET) {

      const bool o = (gameFrames % 4 == 0);
      const uint8_t wWidth = pT.getCurrentWeaponWidth();
      const uint8_t wOffset = pT.getCurrentOffset();

      drawOffsetBitmap(x, y - 2, tankGunRocket, 51, wWidth, 3, wOffset, false, BLACK);

      drawSmallFire(x - (2 + uint8_t(o)), y - 1, false);

    } else if (type == WEAPON_LASER) {
      arduboy.fillRect(x, y, 32, 1, BLACK);

    } else if (type == WEAPON_MG) {
      arduboy.drawPixel(x, y, BLACK);

      if (player) {
        arduboy.drawPixel(x + 1, y, BLACK);
      }

    } else if (type == WEAPON_FLAME) {

      if (state < 4 * 4) {
        arduboy.fillRect(x, y, 4, 1, BLACK);

      } else if (state < 4 * 7) {
        fillRectChess(x, y - 1, 4, 3, BLACK);

      } else if (state < 4 * (13 + pT.lvlWeapon[WEAPON_FLAME])) {
        fillRectChess(x, y - 2, 4, 5, BLACK);

      } else if (state < 4 * (15 + pT.lvlWeapon[WEAPON_FLAME])) {
        fillRectChess(x, y - 1, 4, 3, BLACK);
      }

    } else if (type == WEAPON_SMALL_ROCKET) {

      bool o = (gameFrames % 4 == 0);

      arduboy.fillRect(x, y, 4, 1, BLACK);

      if (player) {    
        drawSmallFire(x - (1 + uint8_t(o)), y, false);
        
      } else {
        drawSmallFire(x + (4 + uint8_t(o)), y, true);
      }
    }
  }

  void update() {
    int8_t vX, vY;

    if (slowmoTimeout % 2 && !player)
      return;

    // move item with the map
    if (gameFrames % MOVE_MAP_INTERVAL == 0)
      x -= 1;

    if (type == WEAPON_STDGUN) {
      vX = 5;
      vY = 0;

    } else if (type == WEAPON_ROCKET) {
      vX = 4 + pT.lvlWeapon[WEAPON_ROCKET];
      vY = 0;

    } else if (type == WEAPON_LASER) {
      vX = 10;
      vY = 0;

    } else if (type == WEAPON_MG) {
      vX = 4;
      vY = 0;

      if (state != 0) {
        if (gameFrames % state == 0) {
          if (state % 2) {
            vY = 1;
          } else {
            vY = -1;
          }
        }
      }

    } else if (type == WEAPON_FLAME) {
      vX = 4;
      vY = 0;

      state += 4;

      // the range increases with a higher level, also by enemys!!
      if (state > 4 * (15 + pT.lvlWeapon[WEAPON_FLAME])) {
        active = false;
      }

    } else if (type == WEAPON_SMALL_ROCKET) {
      vX = 2;
      vY = 0;

      if (state != 0) {
        // the smaller the value that is divided, the more vertical the ememys shoot
        if (gameFrames % abs(80 / state) == 0 )
          vY = state / abs(state);
      }
    }

    if (!player) {
      vX = vX * -1;
    }

    x += vX;
    y += vY;
  }

  bool touchesPosition(uint8_t p_x, uint8_t p_y, uint8_t p_w, uint8_t p_h, uint8_t tolerance) {
    // target to far left
    if (p_x + p_w < x + tolerance)
      return false;

    // target to high up
    if (p_y + p_h < y + tolerance)
      return false;

    // get the height and witdh depending on the type
    uint8_t h, w;
    if (type == WEAPON_LASER) {
      h = 1;
      w = 20;

    } else {
      h = 1;
      w = 3;
    }

    // target to far right
    if (x + w < p_x + tolerance)
      return false;

    // target to far down
    if (y + h < p_y + tolerance)
      return false;

    return true;
  }

  bool offScreen() {
    if (x < 0 || x > 127 || y < 0 || y > 63) {
      return true;
    } else {
      return false;
    }
  }
};

struct projectileManager {
  projectile list[MAX_PROJECTILES];

  void init() {
    for (uint8_t i = 0; i < MAX_PROJECTILES; i++) {
      list[i].active = false;
    }
  }

  void add(uint8_t x, uint8_t y, uint8_t type, int8_t state, bool player) {

    bool foundSlot = false;
    for (uint8_t i = 0; i < MAX_PROJECTILES; i++) {

      if (list[i].active == true)
        continue;

      list[i].active = true;
      foundSlot = true;

      //Serial.println("add p: " + String(i) + " of type: " + String(type));

      // set values
      list[i].x = x;
      list[i].y = y;
      list[i].type = type;
      list[i].state = state;
      list[i].player = player;
      break;

    }
    
#ifdef DEBUG_ADD_FUNCTIONS
    if (!foundSlot)
      Serial.println("Warning, no slot for projectile!");
#endif 
  }

  uint8_t isProjectileAt(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint8_t t) {

    for (uint8_t i = 0; i < MAX_PROJECTILES; i++) {

      // check only active items
      if (list[i].active == false)
        continue;

      // if active[0].isPosition(..) return index 1, 0 is no item
      if (list[i].touchesPosition(x, y, w, h, t)) {
        return i;
      }
    }

    return 0xff;
  }

  void update() {

    for (uint8_t i = 0; i < MAX_PROJECTILES; i++) {

      // only active items
      if (list[i].active == false)
        continue;

      if (list[i].offScreen())
        list[i].active = false;

      list[i].draw();

      list[i].update();
    }
  }
};

#endif
