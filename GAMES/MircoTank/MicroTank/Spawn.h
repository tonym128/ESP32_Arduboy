#ifndef Spawn_h
#define Spawn_h

#define SPAWN_SLOTS   11
#define BLOCK_FRAMES  16

struct spawnManager {
  uint8_t spawnSlot[SPAWN_SLOTS];
  int16_t difficulty = 1;

  bool isSlotFree(uint8_t i) {
    return spawnSlot[i] == 0;
  }

  uint8_t getSlot(uint16_t r) {
    for (uint8_t i = 0; i < SPAWN_SLOTS; i++) {
      uint8_t testSlot = r % SPAWN_SLOTS;

      if (!isSlotFree(testSlot))
        continue;

      spawnSlot[testSlot] = BLOCK_FRAMES;

      return testSlot;
    }
    
#ifdef DEBUG_ADD_FUNCTIONS
    Serial.println("no free slot for enemy!");
#endif 

    return 0xff;
  }

  void init() {
    for (uint8_t i = 0; i < SPAWN_SLOTS; i++) {
      spawnSlot[i] = 0;
    }
  }

  void decrementSlots() {
    for (uint8_t i = 0; i < SPAWN_SLOTS; i++) {
      if (!isSlotFree(i))
        spawnSlot[i] -= 1;
    }
  }

  void update() {

    if (gameFrames % MOVE_MAP_INTERVAL == 0)
      decrementSlots();

    /* draws slots
      Serial.print("spawnSlots: " );
      for (uint8_t i = 0; i < SPAWN_SLOTS; i++) {
      Serial.print(spawnSlot[i], DEC);
      Serial.print(" ");
      }
      Serial.println("done");
    */

    difficulty = 200;

    // get the number of enemys on the screen and make it harder if there are to less
    difficulty -= (5 - eM.count()) * 20;

    // make it faster with map progress
    difficulty -= traveledDist / 32;

    // keep value greater than zero
    difficulty = constrain(difficulty, 10, 200);

    // get random number
    uint16_t spawnSpeed = rand() % difficulty;

    if (spawnSpeed != millis() % difficulty)
      return;

    // get new random number for type of enemy and slot number
    uint8_t r = rand() % 100;

    // find free slot
    uint8_t slot = getSlot(r);

    //Serial.print("slot is:" );
    //Serial.println(slot, DEC);

    if (slot == 0xff)
      return;

    if (r < 20) {
      eM.add(128, 16 + slot * 4, ENEMY_TREE, r % 4); // 20%

    } else if (r < 40) {
      eM.add(128, 16 + slot * 4, ENEMY_CAR, r % 6); // 20%

    } else {
      // make sure framethrowers and mgs and so on appear not on start
      uint8_t moduloValue = 1 + traveledDist / 512;

      moduloValue = constrain(moduloValue, 2, 6);
   
      eM.add(128, 16 + slot * 4, r % moduloValue, 0); // 60%
    }
  }
};

#endif
