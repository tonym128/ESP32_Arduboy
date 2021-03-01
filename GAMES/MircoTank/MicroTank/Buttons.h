#ifndef Buttons_h
#define Buttons_h

void changeMenuCursor(int8_t newIndex) {

  if (millis() < nextButtonInput)
    return;

  pT.menuCursor = (newIndex + 7) % 7;

  //Serial.print("menuCursor: ");
  //Serial.println(pT.menuCursor, DEC);

  pT.calcUpdatePrice();

  nextButtonInput = millis() + 200;
}

void menuButtons() {

  if (arduboy.pressed(UP_BUTTON))
    changeMenuCursor(pT.menuCursor - 1);

  if (arduboy.pressed(DOWN_BUTTON))
    changeMenuCursor(pT.menuCursor + 1);

  if (arduboy.pressed(LEFT_BUTTON)) {
    if (millis() > nextButtonInput) {
      arduboy.audio.toggle();

      audioMessageTimeout = GAME_FRAMES * 2;
      nextButtonInput = millis() + 400;
    }
  }

  if (arduboy.pressed(RIGHT_BUTTON)) {
    if (millis() > nextButtonInput) {
      if (pT.buyUpgrade())
        nextButtonInput = millis() + 400;
    }
  }

  if (arduboy.pressed(A_BUTTON))  {
    if (pT.menuCursor < PLAYER_WEAPONS)
      pT.weaponType = pT.menuCursor;
  }
}

void playButtons() {

  float moveSpeed = 1.0 + 0.1 * float(pT.lvlSpeed);

  // increase the move speed for the rocket tank if the weapon is loaded
  if (pT.weaponType == WEAPON_ROCKET && pT.power > 200) {
    // the rocket tank is a bit faster
    moveSpeed += 0.2;

    // show extra speed with little animation
    if (arduboy.pressed(RIGHT_BUTTON) ) {
      bool o = (gameFrames % 2 == 0);

      // draw little fire animation
      drawSmallFire(pT.x + 1 - uint8_t(o), pT.y + 1, false);
    }
  }

  if (pT.weaponType == WEAPON_FLAME)
    // the flame tank is a bit slower
    moveSpeed -= 0.3;

  // check move
  if (arduboy.pressed(DOWN_BUTTON))
    pT.y += moveSpeed;

  if (arduboy.pressed(UP_BUTTON))
    pT.y -= moveSpeed;

  if (arduboy.pressed(RIGHT_BUTTON))
    pT.x += moveSpeed;

  if (arduboy.pressed(LEFT_BUTTON))
    pT.x -= moveSpeed;

  // keep player inside map
  if (pT.x < 0)
    pT.x = 0;

  if (pT.x > 121)
    pT.x = 121;

  if (pT.y < 12)
    pT.y = 12;

  if (pT.y > 56)
    pT.y = 56;

  // check special attack
  if (arduboy.justPressed(B_BUTTON) && arduboy.pressed(A_BUTTON) && pT.special != 0) {

    specialAttackTimeout = pT.special * 10;

    // extra rockets for the rocket tank
    if (pT.weaponType == WEAPON_ROCKET) {
      specialAttackTimeout += 10;
    }

    // delete special
    pT.special = 0;
  }

  // laser is a bit different than the others
  if (pT.weaponType == WEAPON_LASER) {

    if (arduboy.pressed(A_BUTTON)) {
      // load the laser
      uint8_t load = 5 + pT.lvlWeapon[WEAPON_LASER];
      if (pT.power >= load)
        pT.power -= load;

    } else {
      // the laser was not loaded
      if (pT.power == 255)
        return;

      int8_t pState = (255 - pT.power) / 2;

      sound.tones(soundLaserWeapon);

      pM.add(pT.x + pT.getCurrentWeaponWidth(), pT.y + 2, WEAPON_LASER, pState, true);

      // reset the power
      pT.power = 255;
    }

    return;
  }

  // check shooting
  if (arduboy.pressed(A_BUTTON)) {
    int8_t pState = 0;

    if (pT.weaponType == WEAPON_STDGUN) {

      if (pT.power < 160)
        return;

      sound.tones(soundStdWeapon);

      pT.power -= 160;
      pT.animation = 1;

    } else if (pT.weaponType == WEAPON_ROCKET) {
      // can only shoot with full power
      if (pT.power < 255)
        return;

      sound.tones(soundStdWeapon);

      pT.power = 0;
      // the animation will be done with the power bar

    } else if (pT.weaponType == WEAPON_MG) {

      // this decreases the shooting speed
      if (pT.animation & 3)
        return;

      // the highter the power bar, the more accurate is the shot
      pState = pT.power / 8;

      // add a random value to make it less predictable
      pState += rand() % 8;

      // calculate the power cost to simulate heating one part is fixed, the other depending on the power
      const uint8_t bulletCosts = (400 - pT.power) / 8 + 20;

      if (pT.power < bulletCosts)
        return;

      pT.power -= bulletCosts;

      sound.tones(soundMGWeapon);

      // add a bullet to the byte
      pT.animation = pT.animation | 1;

    } else if (pT.weaponType == WEAPON_FLAME) {

      if (pT.power < 20)
        return;

      sound.tones(soundFlameWeapon);

      pT.power -= 20;

    }

    // add projectile
    pM.add(pT.x + pT.getCurrentWeaponWidth(), pT.y + 2, pT.weaponType, pState, true);

  }
}

void changeGameMode() {
  if (millis() < nextButtonInput)
    return;

  if (isGameOver) {
    isGameOver = false;

    // reset stats
    enemyKills = 0;
    traveledDist = 0;
    mM.mapOffset = 0;

    // set all the list items inactive
    pM.init();
    eM.init();
    iM.init();

    // set all player values to  0
    pT.init();

  } else {
    isPlayMode = !isPlayMode;
  }

  nextButtonInput = millis() + 200;
}

void updateGameButtons() {

  // button b not pressed helps to activate the special
  if (arduboy.justPressed(B_BUTTON) && !arduboy.pressed(A_BUTTON))
    changeGameMode();

  if (isPlayMode) {
    playButtons();
  } else {
    menuButtons();
  }
}

#endif
