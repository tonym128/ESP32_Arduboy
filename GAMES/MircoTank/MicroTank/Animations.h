#ifndef Animations_h
#define Animations_h

enum {
  ANIMATION_IMPACT_L,
  ANIMATION_IMPACT_R,
  ANIMATION_BOOM
};

struct animation {
  uint8_t x;
  uint8_t y;
  uint8_t type;
  int8_t state;
  bool active;

  void draw() {
    if (type == ANIMATION_IMPACT_L || type == ANIMATION_IMPACT_R) {

      uint8_t xPos;
      if (type == ANIMATION_IMPACT_L) {
        xPos = x - state * 2;
      } else {
        xPos = x + state * 2;
      }

      drawOffsetBitmap(xPos, y - 2, animationImpact, 16, 4, 3, state * 4, (type == ANIMATION_IMPACT_L), BLACK);

    } else if (type == ANIMATION_BOOM) {

      drawOffsetBitmap(x, y - 2, animationBoom, 32, 4, 10, state * 4, true, BLACK);
      drawOffsetBitmap(x - 3, y - 2, animationBoom, 32, 4, 10, state * 4, false, BLACK);
    }
  }

  void update() {
    if (slowmoTimeout % 2)
      return;

    // move animation with the map
    if (gameFrames % MOVE_MAP_INTERVAL == 0 && !isStartAnimation) 
      x -= 1;

    if (type == ANIMATION_IMPACT_L || type == ANIMATION_IMPACT_R) {

      if (gameFrames % 3 != 0)
        return;

      state++;
      if (state == 4)
        active = false;

    } else if (type == ANIMATION_BOOM) {

      if (gameFrames % 5 != 0)
        return;

      state++;
      if (state == 8)
        active = false;
    }
  }
};

struct animationManager {
  static const uint8_t maximum = 16;
  animation list[maximum];

  void add(uint8_t x, uint8_t y, uint8_t type, int8_t state) {

    bool foundSlot = false;
    for (uint8_t i = 0; i < maximum; i++) {

      if (list[i].active == true)
        continue;

      list[i].active = true;
      foundSlot = true;

      if (type == ANIMATION_BOOM)
        sound.tones(soundStdWeapon);

      // set values
      list[i].x = x;
      list[i].y = y;
      list[i].type = type;
      list[i].state = state;
      break;
    }

#ifdef DEBUG_ADD_FUNCTIONS
    if (!foundSlot)
      Serial.println("Warning, no slot for animation!");
#endif
  }

  void update() {

    for (uint8_t i = 0; i < maximum; i++) {

      // only active items
      if (list[i].active == false)
        continue;

      list[i].draw();

      list[i].update();
    }
  }
};

#endif
