#ifndef Items_h
#define Items_h

enum {
  ITEM_HEART,
  ITEM_COIN,
  ITEM_SHIELD,   
  ITEM_REPAIR,
  ITEM_SLOWMO,
  ITEM_SPECIAL1,
  ITEM_SPECIAL2,
  ITEM_SPECIAL3,
};

struct item {
  uint8_t x = 0;
  uint8_t y = 0;
  uint8_t status = 0;
  uint8_t type = 0;
  bool active = false;

  // TODO: this is not neccassary, put it in the status byte
  bool flip = false;

  void draw() {
    if (type == ITEM_HEART) {
      drawOffsetBitmap(x, y, itemHeart, 28, 7, 6, (status % 4) * 7, flip, BLACK);

    } else if (type == ITEM_COIN) {
      drawOffsetBitmap(x, y, itemCoin, 28, 7, 7, (status % 4) * 7, flip, BLACK);

    } else if (type == ITEM_REPAIR) {
      drawOffsetBitmap(x, y, itemRepair, 48, 8, 4, (status % 6) * 8, flip, BLACK);
      drawOffsetBitmap(x, y + 4, itemRepair, 48, 8, 4, (status % 6) * 8, int(!flip) + 2, BLACK);

    } else if (type == ITEM_SLOWMO) {
      drawOffsetBitmap(x, y, itemSlowmo, 42, 7, 7, (status % 6) * 7, false, BLACK);

    } else if (type == ITEM_SHIELD) {
      drawOffsetBitmap(x, y, itemShield, 16, 4, 7, (status % 4) * 4, false, BLACK);
      drawOffsetBitmap(x + 3, y, itemShield, 16, 4, 7, (status % 4) * 4, true, BLACK);
      
    } else if (type >= ITEM_SPECIAL1) {
      drawOffsetBitmap(x, y, specialFrame, 32, 8, 6, (gameFrames % 4) * 8, false, BLACK);

      // draw type of special item
      drawSpecialStatus(x, y, type - ITEM_SPECIAL1 + 1, BLACK);
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

  void update() {

    // move item with the map
    if (gameFrames % MOVE_MAP_INTERVAL == 0)
      x -= 1;
    
    // this sets after how many frames the bitmap will update
    if (gameFrames % 10 != 0)
      return;

    // this lets the images rotate, 4 * 5 * 6 = 120 is "größter gemeinsamer Nenner"
    status = (status + 1) % 120;

    // this flips the image for rotations
    if (type == ITEM_HEART || type == ITEM_COIN) {
      if (status % 4  == 0)
        flip = !flip;

    } else if (type == ITEM_REPAIR) {
      if (status % 6  == 0)
        flip = !flip;     
    } 
  }

  bool offScreen() {
    if (x > 127 || y > 63) {
      return true;
    } else {
      return false;
    }
  }

};


struct itemsManager {
  static const uint8_t maximum = 8;  
  item list[maximum];

  void init() {
    for (uint8_t i = 0; i < maximum; i++) {
      list[i].active = false;
    }
  }

  uint8_t isItemAt(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {

    for (uint8_t i = 0; i < maximum; i++) {

      // check only active items
      if (list[i].active == false)
        continue;

      // if active[0].isPosition(..) return index 1, 0 is no item
      if (list[i].touchesPosition(x, y, w, h)) {

        /*
        Serial.print("touch item:");
        Serial.print(i, DEC);
        Serial.print(" playerTank x:");
        Serial.print(x, DEC);
        Serial.print(" y:");
        Serial.println(y, DEC);
        */

        return i;

      }
    }

    return 0xff;
  }

  void addRandom(uint8_t x, uint8_t y) {

    // get Number from 0 - 99
    uint8_t type, r = rand() % 100;

    // get nothing
    if (r > 60)
      return; 

    if (r < 2) {
      type = ITEM_HEART; // 2%
      
    } else if (r < 5) {
      type = ITEM_REPAIR; // 3%
      
    } else if (r < 9) {
      type = ITEM_SPECIAL1; // 4%

    } else if (r < 11) {
      type = ITEM_SPECIAL2; // 2%

    } else if (r < 12) {
      type = ITEM_SPECIAL3; // 1%
      
    } else if (r < 17) {
      type = ITEM_SLOWMO; // 5%

    } else if (r < 20) {
      type = ITEM_SHIELD; // 5%
      
    } else  {
      type = ITEM_COIN;
    }

    add(x, y, type);
  }

  void add(uint8_t x, uint8_t y, uint8_t type) {

    bool foundSlot = false;
    for (uint8_t i = 0; i < maximum; i++) {

      if (list[i].active == false) {
        list[i].active = true;
        foundSlot = true;
        
        // set values
        list[i].x = x;
        list[i].y = y;
        list[i].type = type;
        list[i].status = 0;
        break;
      }
    }
    
#ifdef DEBUG_ADD_FUNCTIONS
    if (!foundSlot)
      Serial.println("Warning, no slot for item!");
#endif      
  }

  void update() {

    for (uint8_t i = 0; i < maximum; i++) {

      // update only active items
      if (list[i].active == false)
        continue;

      list[i].update();

      if (list[i].offScreen())
        list[i].active = false;

      list[i].draw();
    }
  }
};

#endif
