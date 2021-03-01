#ifndef Player_h
#define Player_h

#define PLAYER_WEAPONS 5
#define LEVELS 5

struct playerTank {
  // because the rocket tank is faster than the others
  float x;
  float y;

  uint8_t money;
  uint16_t health;
  uint8_t lifes;
  uint8_t power;
  uint8_t special;
  uint8_t spawnTimeout;

  // all weapon levels
  uint8_t lvlChain = 0;
  uint8_t lvlSpeed = 0;
  uint8_t lvlWeapon[PLAYER_WEAPONS] = {0, 0, 0, 0, 0};

  const uint8_t priceArray[LEVELS] = {2, 4, 6, 10, 16};

  // current selected weapon, just int for rolling
  int8_t weaponType = WEAPON_STDGUN;

  // only for the tank shop
  uint8_t nextLevel = 1;
  uint8_t updatePrice = priceArray[0];
  uint8_t menuCursor = 0;

  int8_t animation = 0;

  bool recoilActive = false;
  bool shieldActive = false;

  void init() {
    // set position
    x = 10;
    y = 30;

    // parameters
    lifes = 2;
    health = 100 * 100;
    special = 0;
    power = 0;
    money = 0;
    spawnTimeout = 30;

    // globals
    specialAttackTimeout = 0;

    // reset updates
    lvlChain = 0;
    lvlSpeed = 0;
    for (uint8_t i = 0; i < PLAYER_WEAPONS; i++) {
      lvlWeapon[i] = 0;
    }

    // add some goodys for the game start
    iM.add(100, 30, ITEM_SHIELD);
    iM.add(100, 50, ITEM_SPECIAL1);

    // recalculate the price
    calcUpdatePrice();
  }

  void update() {

    updatePower();

    drawPlayer();
  }

  void incPower(uint8_t v) {
    if (int16_t(power) + v >= 255) {
      power = 255;
    } else {
      power += v;
    }
  }

  void updatePower() {
    // laser works different
    if (weaponType == WEAPON_LASER)
      return;

    // increase the power faster with higher weapon levels
    incPower(10 + lvlWeapon[weaponType] * 2);

  }

  void damage(uint8_t dmg) {

    if (dmg == 0)
      return;

    // check if the item shield absorbs the dmg
    if (shieldActive) {
      shieldActive = false;
      return;
    }

    // check if the loaded laser absorbs it
    if (weaponType == WEAPON_LASER && arduboy.pressed(LEFT_BUTTON) && power < 100) {
      power = (power + 20) * 2;
      return;

      // TODO: make some sound or so
    }

    sound.tones(soundPlayerHit);

#ifdef DEGUG_DMG_PLAYER
    Serial.print("got DMG:");
    Serial.print(dmg, DEC);
#endif     

    uint16_t tmpDmg = dmg * 100;

    // reduce the damage because of next level chains, level 1 = -10%, lvl 2 = -20%...
    // extra damage reduction if default gun is active
    int8_t reducePercent = (lvlChain + uint8_t(weaponType == WEAPON_STDGUN) * 3) * 8;

    // with map progress the reduction will be less and less
    reducePercent -= traveledDist / 128;

#ifdef DEGUG_DMG_PLAYER
    Serial.print(" -%:");
    Serial.print(reducePercent, DEC);
#endif     

    tmpDmg -= dmg * reducePercent;

#ifdef DEGUG_DMG_PLAYER
    Serial.print(" is:");
    Serial.println(float(tmpDmg)/100, 2);
#endif   

    if (health > tmpDmg) {
      health -= tmpDmg;

    } else {
      //Serial.println("player Died");

      aM.add(x + 4, y, ANIMATION_BOOM, 0);

      if (lifes > 1) {
        lifes--;

        health = (100 + lvlChain * 10) * 100;

        // first do not draw it, than flickering
        spawnTimeout = GAME_FRAMES * 2;

      } else {
        //Serial.println("Game Over");

        // this sets the game over message in the shop
        isGameOver = true;
        nextButtonInput = millis() + 1000;

        // goto shop
        isPlayMode = false;

        lifes = 2;
        health = 100 * 100;
      }
    }
  }

  void drawPlayer() {
    // always increment timout!
    if (spawnTimeout > 0)
      spawnTimeout--;

    // do not draw the player
    if (spawnTimeout > GAME_FRAMES)
      return;

    // draw the player flickering
    if (spawnTimeout > 0 && gameFrames % 2)
      return;

    drawWeapon(x, y, weaponType, lvlWeapon[weaponType], BLACK);

    drawWeaponAnimation();

    drawChain(x, y, lvlChain, BLACK);
  }

  void drawWeaponAnimation() {
    if (weaponType == WEAPON_STDGUN) {

      if (animation == 0)
        return;

      const uint8_t lev = lvlWeapon[weaponType];
      const uint8_t weaponOffset = getOffset(lev);
      const uint8_t weaponWidth = getWeaponWidth(lev);
      const uint8_t yPos = y - 2 + (lev < 2);
      const uint8_t xPos = x + getCurrentWeaponWidth() + animation;

      // draw the current rocket with offset
      drawOffsetBitmap(xPos, yPos, animationStdGun, 24, 3, 4, (animation - 1) * 3, false, BLACK);
      drawOffsetBitmap(xPos, yPos + 3, animationStdGun, 24, 3, 4, (animation - 1) * 3, 2, BLACK);

      if (gameFrames % 2 != 0)
        return;

      if (animation < 8) {
        animation++;
      } else {
        animation = 0;
      }

    } else if (weaponType == WEAPON_ROCKET) {
      // no animation because it has reloaded
      if (power == 255)
        return;

      const uint8_t lev = lvlWeapon[weaponType];
      const uint8_t yReload = (255 - power) / 64;

      const uint8_t weaponOffset = getOffset(lev);
      const uint8_t weaponWidth = getWeaponWidth(lev);

      // clear space for animation
      arduboy.fillRect(x, y, weaponWidth, 4, WHITE);

      //Serial.println("reloadY:" + String(reloadY));

      // draw the current rocket with offset
      drawOffsetBitmap(x, y + yReload, tankGunRocket, 51, weaponWidth, 4 - yReload, weaponOffset, false, BLACK);

    } else if (weaponType == WEAPON_MG) {
      // TODO: animation for flying bullets
      if (animation & 0b00000001)
        arduboy.drawPixel(x + 4, y, BLACK);

      if (animation & 0b00000010)
        arduboy.drawPixel(x + 3, y - 1, BLACK);

      if (animation & 0b00000100)
        arduboy.drawPixel(x + 1, y - 2, BLACK);

      if (animation & 0b00001000)
        arduboy.drawPixel(x - 1, y - 1, BLACK);

      if (animation & 0b00010000)
        arduboy.drawPixel(x - 2, y, BLACK);

      if (animation & 0b00100000)
        arduboy.drawPixel(x - 3, y + 1, BLACK);

      if (animation & 0b01000000)
        arduboy.drawPixel(x - 4, y + 3, BLACK);

      if (animation & 0b10000000)
        arduboy.drawPixel(x - 5, y + 6, BLACK);

      // shift to the left to simulate a bullet falling to the ground
      animation = animation << 1;

    } else if (weaponType == WEAPON_LASER) {

      // no animation because it is idle
      if (power == 255)
        return;

      const uint8_t r = (255 - power) / 50 + gameFrames % 2;
      const uint8_t xPos = x + getCurrentWeaponWidth() + 1 + r;

      arduboy.drawCircle(xPos, y + 1, r, BLACK);

    } else if (weaponType == WEAPON_MG) {

      animation = -1;
    }

  }

  uint8_t getWidth() {
    // NOTE
    return getChainWidth(lvlChain);
  }

  void drawShield(uint8_t xPos, uint8_t yPos) {
    // those pixel should look like a shield
    arduboy.drawPixel(xPos, yPos + 1, BLACK);
    arduboy.fillRect(xPos + 1, yPos + 1, 1, 2, BLACK);
    arduboy.fillRect(xPos + 2, yPos, 1, 4, BLACK);
    arduboy.drawPixel(xPos + 3, yPos + 3, BLACK);
  }

  void drawChain(uint8_t xPos, uint8_t yPos, uint8_t lev, uint8_t color) {
    // a vertical line...
    bool isUpRight = arduboy.pressed(UP_BUTTON) && !arduboy.pressed(LEFT_BUTTON) || arduboy.pressed(DOWN_BUTTON) && arduboy.pressed(LEFT_BUTTON);
    bool isUpLeft =  arduboy.pressed(DOWN_BUTTON) && !arduboy.pressed(LEFT_BUTTON) || arduboy.pressed(UP_BUTTON) && arduboy.pressed(LEFT_BUTTON);
    bool toggler = (gameFrames % 5 == 0);

    uint8_t offset = getOffset(lev);
    uint8_t chainWidth = getChainWidth(lev);

    if (isUpRight || isUpLeft) {
      if (toggler) {
        drawOffsetBitmap(xPos, yPos + 4, tankChainVert0, 51, chainWidth, 5, offset, isUpRight, color);

      } else {
        drawOffsetBitmap(xPos, yPos + 4, tankChainVert1, 51, chainWidth, 5, offset, isUpRight, color);

      }
    } else {
      drawOffsetBitmap(xPos, yPos + 4, tankChainStraight, 51, chainWidth, 5, offset, toggler, color);
    }

    if (shieldActive) {
      drawShield(xPos + chainWidth, yPos + 4 + uint8_t(isUpLeft));
    }
  }

  void drawWeapon(uint8_t xPos, uint8_t yPos, uint8_t type, uint8_t lev, uint8_t color) {


    if (animation > 0 && animation < 3) {
      xPos--;
    }

    uint8_t weaponOffset = getOffset(lev);
    uint8_t weaponWidth = getWeaponWidth(lev);

    const uint8_t *weaponImage;

    if (type == WEAPON_STDGUN) {
      weaponImage = tankGunStd;

    } else if (type == WEAPON_ROCKET) {
      weaponImage = tankGunRocket;

    } else if (type == WEAPON_LASER) {
      weaponImage = tankGunLaser;

    } else if (type == WEAPON_MG) {
      weaponImage = tankGunMG;

    } else {
      weaponImage = tankGunFlame;
    }

    drawOffsetBitmap(xPos, yPos, weaponImage, 51, weaponWidth, 4, weaponOffset, false, color);
  }

  uint8_t getChainWidth(uint8_t lvl) {
    if (lvl == 0) {
      return 6;
    }
    else if (lvl == 1) {
      return 8;
    }
    else if (lvl == 2) {
      return 10;
    }
    else if (lvl == 3) {
      return 10;
    }
    else if (lvl == 4) {
      return 12;
    }
  }

  uint8_t getCurrentWeaponWidth() {
    return getWeaponWidth(lvlWeapon[weaponType]);
  }

  uint8_t getWeaponWidth(uint8_t lvl) {
    if (lvl == 0) {
      return 6;
    }
    else if (lvl == 1) {
      return 8;
    }
    else if (lvl == 2) {
      return 10;
    }
    else if (lvl == 3) {
      return 11;
    }
    else if (lvl == 4) {
      return 13;
    }
  }

  uint8_t getCurrentOffset() {
    return getOffset(lvlWeapon[weaponType]);
  }

  uint8_t getOffset(uint8_t lvl) {
    if (lvl == 0) {
      return 0;
    }
    else if (lvl == 1) {
      return 7;
    }
    else if (lvl == 2) {
      return 16;
    }
    else if (lvl == 3) {
      return 27;
    }
    else if (lvl == 4) {
      return 38;
    }
  }

  void drawTankShop() {

    const uint8_t yText = 57;
    if (audioMessageTimeout != 0) {

      if (arduboy.audio.enabled()) {
        drawOffsetBitmap(69, yText, menuSound, 34, 34, 5, 0, false, BLACK);
        drawOffsetBitmap(108, yText, menuOn, 13, 13, 5, 0, false, BLACK);

      } else {
        drawOffsetBitmap(66, yText, menuSound, 34, 34, 5, 0, false, BLACK);
        drawOffsetBitmap(105, yText, menuOff, 20, 20, 5, 0, false, BLACK);
      }

    } else {
      if (isGameOver) {
        drawOffsetBitmap(66, yText, menuGameOver, 59, 59, 5, 0, false, BLACK);

      } else if (traveledDist == 0) {
        drawOffsetBitmap(79, yText, menuStart, 34, 34, 5, 0, false, BLACK);

      } else {
        drawOffsetBitmap(70, yText, menuContinue, 51, 51, 5, 0, false, BLACK);
      }
    }

    // frame around the left block
    drawRectWithFrame(15, 7, 44, 60);

    // draw 5 weapon level bars
    for (uint8_t i = 0; i < PLAYER_WEAPONS; i++) {
      drawBar(19, 10 + i * 8, lvlWeapon[i] * 4 + 2);
    }

    // draw speed level bar
    drawBar(19, 10 + 5 * 8, lvlSpeed * 4 + 2);

    // draw chain level bar
    drawBar(19, 10 + 6 * 8, lvlChain * 4 + 2);

    // draw all weapons to buy behind bars
    for (uint8_t i = 0; i < PLAYER_WEAPONS; i++) {
      uint8_t updateNum = lvlWeapon[i] + 1;

      if (updateNum == LEVELS)
        updateNum = LEVELS - 1;

      drawWeapon(41, 11 + i * 8, i, updateNum, WHITE);
    }

    uint8_t lvlS = lvlSpeed + 1;
    if (lvlS == LEVELS)
      lvlS = LEVELS - 1;

    // draw speed symbol
    uint8_t speedLen = 1 + lvlSpeed * 2;
    arduboy.fillRect(41, 14 + 5 * 8, speedLen, 1, WHITE);
    drawOffsetBitmap(41 + speedLen, 11 + 5 * 8, menuSpeedHeart, 8, 4, 4, 0, false, WHITE);

    // if max level is reached draw the max level and not a not existing next level
    uint8_t lvlC = lvlChain + 1;
    if (lvlC == LEVELS)
      lvlC = LEVELS - 1;

    // draw chain to buy behind bars
    drawOffsetBitmap(41, 11 + 6 * 8, tankChainStraight, 51, getChainWidth(lvlC), 5, getOffset(lvlC), false, WHITE);

    // clear a bit of the map if cursor is 0
    if (menuCursor == 0)
      arduboy.fillRect(0, 8, 15, 8, WHITE);

    // draw current Tank
    uint8_t rightShift = 5 - lvlChain - uint8_t(shieldActive);
    drawWeapon(rightShift, 8 + menuCursor * 8, weaponType, lvlWeapon[weaponType], BLACK);
    drawChain(rightShift, 8 + menuCursor * 8, lvlChain, BLACK);

    // draw bar update
    if (nextLevel < LEVELS)
      fillRectChess(18 + nextLevel * 4, 11 + menuCursor * 8, 4, 4, WHITE);

    // frame around the info block
    const uint8_t xInfo = 65;
    const uint8_t yInfo = 17;

    drawRectWithFrame(xInfo, yInfo, 60, 36);

    // draw my money
    drawOffsetBitmap(xInfo + 9, yInfo + 4, itemCoin, 28, 7, 7, 7, false, WHITE);
    uint8_t st = drawNumbers(xInfo + 22, yInfo + 5, money);

    // draw enemy kills
    drawOffsetBitmap(xInfo + 10, yInfo + 15, menuHumanArrow, 8, 5, 5, 0, false, WHITE);
    drawNumbers(xInfo + 22, yInfo + 15, enemyKills);

    // draw distance
    drawOffsetBitmap(xInfo + 10, yInfo + 25, menuHumanArrow, 8, 5, 5, 3, false, WHITE);
    drawNumbers(xInfo + 22, yInfo + 25, traveledDist);

    // draw the doublepoints after the infos
    for (uint8_t i = 0; i < 3; i++) {
      arduboy.drawPixel(xInfo + 18, yInfo + 7 + i * 10, WHITE);
      arduboy.drawPixel(xInfo + 18, yInfo + 9 + i * 10, WHITE);
    }

    if (nextLevel >= LEVELS)
      return;

    // draw minus
    arduboy.fillRect(xInfo + 25 + st * 5, yInfo + 8, 3, 1, WHITE);

    // draw what it costs
    drawNumbers(xInfo + 31 + st * 5, yInfo + 5, updatePrice);
  }

  void drawRectWithFrame(int8_t x, int8_t y, int8_t h, int8_t w) {
    // draw the nice looling frame for the menus
    arduboy.drawRoundRect(x, y, h, w, 3, BLACK);
    arduboy.drawRoundRect(x + 1, y + 1, h - 2, w - 2, 2, WHITE);
    arduboy.fillRoundRect(x + 2, y + 2, h - 4, w - 4, 1, BLACK);
  }

  void calcUpdatePrice() {

    // get the level of the current menuCursor position
    if (menuCursor < PLAYER_WEAPONS) {
      nextLevel = lvlWeapon[menuCursor] + 1;

    } else if (menuCursor == PLAYER_WEAPONS) {
      nextLevel = lvlSpeed + 1;

    } else {
      nextLevel = lvlChain + 1;
    }

    if (nextLevel >= LEVELS)
      return;

    // every level costs the same
    updatePrice = priceArray[nextLevel - 1];
  }

  void drawCornerInfo() {
    // frame around the hole info corner
    drawRectWithFrame(65, -3, 66, 12);

    // draw power bar
    uint8_t p = map(power, 0, 255, 0, 18);
    drawBar(68, 0, p);

    // makes frame only move if a special is collected
    uint8_t specialCnt;
    if (special == 0) {
      specialCnt = 0;
    } else {
      specialCnt = (gameFrames % 4) * 8;
    }

    // special frame, needs to be inverted
    drawOffsetBitmap(90, 0, specialFrame, 32, 8, 6, specialCnt, false, WHITE);

    // draw the inside of the special frame
    drawSpecialStatus(90, 0, special, WHITE);

    // the heart, needs to be inverted
    drawOffsetBitmap(103 - lifes, 0, itemHeart, 28, 7, 6, 7, false, WHITE);

    if (lifes > 1)
      drawOffsetBitmap(109 - lifes, 0, menuSpeedHeart, 8, 4, 6, 4, false, WHITE);

    if (lifes > 2)
      drawOffsetBitmap(109, 0, menuSpeedHeart, 8, 4, 6, 4, false, WHITE);

    // draw the health
    drawNumbers(114, 0, health / 100);
  }

  bool buyUpgrade() {
    if (nextLevel >= LEVELS)
      return false;

    if (money < updatePrice)
      return false;

    // need bigger chains for this upgrade
    if (lvlChain + 1 < nextLevel)
      return false;

    money -= updatePrice;

    sound.tones(soundShopBuy);

    if (menuCursor < PLAYER_WEAPONS) {
      lvlWeapon[menuCursor]++;

    } else if (menuCursor == PLAYER_WEAPONS) {
      lvlSpeed++;

    } else {
      lvlChain++;

    }

    // calculate the price for the next update
    calcUpdatePrice();

    return true;
  }

  void drawBar(uint8_t x, uint8_t y, uint8_t v) {
    // draw frame and inside
    arduboy.drawRoundRect(x, y, 20, 6, 1, WHITE);
    arduboy.fillRect(x + 1, y + 1, 18, 4, BLACK);

    // draw points for better reading
    for (uint8_t i = 0; i < 4; i++) {
      arduboy.drawPixel(x + 2 + i * 4, y + 1, WHITE);
    }

    // draw bar
    arduboy.fillRect(x + 1, y + 1, v, 4, WHITE);
    if (v > 2)
      arduboy.fillRect(x + 2, y + 1, v - 2, 1, BLACK);
  }

};

#endif
