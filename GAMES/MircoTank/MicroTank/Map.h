#ifndef Map_h
#define Map_h

#ifdef USE_FULL_MAP
#define MAP_LEN 57
#else 
#define MAP_LEN 6
#endif 

struct mapManager {
  bool nextMapImageFlip;
  const unsigned char *nextMapImage;

  uint16_t mapOffset = 0;
  bool mapToggler = false;

  /*
    void info() {
    Serial.print("getEnemyNum():");
    Serial.println(eM.getEnemyNum(), DEC);
    }
  */

  void setImage(uint8_t mapCnt) {
    // no filp is default value
    nextMapImageFlip = false;

    if (mapCnt == 0) {
      nextMapImage = NULL;

    } else if (mapCnt == 1) {
      nextMapImage = mapArmyCampTower;

    } else if (mapCnt == 2) {
      nextMapImage = mapArmyCampFence1;
      nextMapImageFlip = true;

    } else if (mapCnt == 3) {
      nextMapImage = mapArmyCampDoor;

    } else if (mapCnt == 4) {
      nextMapImage = mapArmyCampFence2;

    } else if (mapCnt == 5) {
      nextMapImage = mapArmyCampEnd;
      nextMapImageFlip = true;
      
    } else if (mapCnt == 6) {
      nextMapImage = NULL;
          
#ifndef USE_FULL_MAP
    }
#else 

    } else if (mapCnt == 7) {
      nextMapImage = mapHouseGarage;

    } else if (mapCnt == 8) {
      nextMapImage = mapHouseWithTree;

    } else if (mapCnt == 9) {
      nextMapImage = mapHouseWithDoor;

    } else if (mapCnt == 10) {
      nextMapImage = mapArmyCampEnd;

    } else if (mapCnt == 11) {
      nextMapImage = mapArmyCampFence2;
      nextMapImageFlip = true;

    } else if (mapCnt == 12) {
      nextMapImage = mapArmyCampDoor;
      nextMapImageFlip = true;

    } else if (mapCnt == 13) {
      nextMapImage = mapArmyCampHole;

    } else if (mapCnt == 14) {
      nextMapImage = mapArmyCampTower;
      nextMapImageFlip = true;

    } else if (mapCnt == 15) {
      nextMapImage = mapHouseSmall;

    } else if (mapCnt == 16) {
      nextMapImage = NULL;

    } else if (mapCnt == 17) {
      nextMapImage = mapCityLake;

    } else if (mapCnt == 18) {
      nextMapImage = mapCityThreeHouses;

    } else if (mapCnt == 19) {
      nextMapImage = mapCityChurch;

    } else if (mapCnt == 20) {
      nextMapImage = mapCityThreeHouses;
      nextMapImageFlip = true;

    } else if (mapCnt == 21) {
      nextMapImage = mapCityHospital;
      nextMapImageFlip = true;

    } else if (mapCnt == 22) {
      nextMapImage = mapCityTower;

    } else if (mapCnt == 23) {
      nextMapImage = mapCityTwoHouses;

    } else if (mapCnt == 24) {
      nextMapImage = mapCityChurch;
      nextMapImageFlip = true;

    } else if (mapCnt == 25) {
      nextMapImage = mapCityTwoHouses;
      nextMapImageFlip = true;

    } else if (mapCnt == 26) {
      nextMapImage = mapCityLake;
      nextMapImageFlip = true;

    } else if (mapCnt == 27) {
      nextMapImage = NULL;

    } else if (mapCnt == 28) {
      nextMapImage = mapHouseSmall;
      nextMapImageFlip = true;

    } else if (mapCnt == 29) {
      nextMapImage = mapArmyCampEnd;

    } else if (mapCnt == 30) {
      nextMapImage = mapArmyCampFence2;
      nextMapImageFlip = true;

    } else if (mapCnt == 31) {
      nextMapImage = mapArmyCampTower;
      nextMapImageFlip = true;

    } else if (mapCnt == 32) {
      nextMapImage = mapHouseFlag;

    } else if (mapCnt == 33) {
      nextMapImage = mapHouseBig;
      nextMapImageFlip = true;

    } else if (mapCnt == 34) {
      nextMapImage = mapHouseWithDoor;
      nextMapImageFlip = true;

    } else if (mapCnt == 35) {
      nextMapImage = NULL;

    } else if (mapCnt == 36) {
      nextMapImage = mapArmyCampTower;

    } else if (mapCnt == 37) {
      nextMapImage = mapArmyCampHole;
      nextMapImageFlip = true;

    } else if (mapCnt == 38) {
      nextMapImage = mapArmyCampTower;
      nextMapImageFlip = true;

    } else if (mapCnt == 39) {
      nextMapImage = mapHouseWithTree;

    } else if (mapCnt == 40) {
      nextMapImage = mapArmyCampEnd;

    } else if (mapCnt == 41) {
      nextMapImage = mapArmyCampFence1;
      nextMapImageFlip = true;

    } else if (mapCnt == 42) {
      nextMapImage = mapArmyCampFence2;

    } else if (mapCnt == 43) {
      nextMapImage = mapArmyCampTower;
      nextMapImageFlip = true;

    } else if (mapCnt == 44) {
      nextMapImage = mapHouseFlag;
      nextMapImageFlip = true;

    } else if (mapCnt == 45) {
      nextMapImage = NULL;

    } else if (mapCnt == 46) {
      nextMapImage = mapHouseGarage;
      nextMapImageFlip = true;
      
    } else if (mapCnt == 47) {
      nextMapImage = mapHouseBig;
      
    } else if (mapCnt == 48) {
      nextMapImage = mapHouseSmall;

    } else if (mapCnt == 49) {
      nextMapImage = NULL;

    } else if (mapCnt == 50) {
      nextMapImage = mapCityLake;
      nextMapImageFlip = true;
      
    } else if (mapCnt == 51) {
      nextMapImage = mapCityChurch;
      nextMapImageFlip = true;

    } else if (mapCnt == 52) {
      nextMapImage = NULL;

    } else if (mapCnt == 53) {
      nextMapImage = mapCityTower;
      nextMapImageFlip = true;

    } else if (mapCnt == 54) {
      nextMapImage = mapCityHospital;
      nextMapImageFlip = true;

    } else if (mapCnt == 55) {
      nextMapImage = mapCityThreeHouses;

    } else if (mapCnt == 56) {
      nextMapImage = mapCityTwoHouses;
      nextMapImageFlip = true;
      
    } else if (mapCnt == 57) {
      nextMapImage = NULL;
    }

#endif 
  }

  void drawMap() {
    uint8_t mapToDraw = mapOffset / 16;
    uint8_t xMap = 0;
    uint8_t imageOffset = mapOffset % 16;

    uint8_t mapsToPrint;
    if (imageOffset == 0) {
      mapsToPrint = 8;
    } else {
      mapsToPrint = 9;
    }

    /*
      Serial.println();
      Serial.print("for mapOffset:");
      Serial.print(mapOffset, DEC);
      Serial.print(" imageOffset:");
      Serial.print(imageOffset, DEC);
      Serial.print(" mapToDraw:");
      Serial.println(mapToDraw, DEC);
    */

    for (uint8_t i = 0; i < mapsToPrint; i++) {

      // calculate current mapToDraw and set next Map image with flip info
      setImage((mapToDraw + i) % MAP_LEN);

      // draw if it is not empty
      if (nextMapImage != NULL) {
        drawOffsetBitmap(xMap - imageOffset, 0, nextMapImage, 16, 16, 16, 0, nextMapImageFlip, BLACK);
      }

      // set position right to the current image
      xMap += 16;
    }
  }


  void update() {
    drawMap();

    if (slowmoTimeout % 2)
      return;

    // move map to the left
    if (gameFrames % MOVE_MAP_INTERVAL == 0) {
      mapOffset = (mapOffset + 1) % (MAP_LEN * 16);

      traveledDist++;
    }
  }
};

#endif
