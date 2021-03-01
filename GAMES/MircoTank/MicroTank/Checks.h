#ifndef Checks_h
#define Checks_h

void checkItemsCatch() {
  uint8_t index = iM.isItemAt(pT.x, pT.y, pT.getWidth(), 7);

  if (index == 0xff)
    return;

  uint8_t itemType = iM.list[index].type;

  sound.tones(soundGotItem);

  if (itemType == ITEM_HEART) {

    if (pT.lifes < 3) {
      pT.lifes += 1;

    } else {
      // all hearts are full so change to repair item
      itemType = ITEM_REPAIR;
    }
  }

  if (itemType == ITEM_REPAIR) {

    pT.health += 50 * 100;

    uint16_t maxHealth = (100 + pT.lvlChain * 10) * 100;

    if (pT.health > maxHealth)
      pT.health = maxHealth;

  } else if (itemType == ITEM_COIN) {

    pT.money += 1;

  } else if (itemType == ITEM_SLOWMO) {

    slowmoTimeout += 128;

    // let the screen flash for a frame to show slowmo started
    arduboy.clear();

  } else if (itemType == ITEM_SHIELD) {

    pT.shieldActive = true;

  } else if (itemType >= ITEM_SPECIAL1) {

    uint8_t specialStatus = itemType - ITEM_SPECIAL1 + 1;

    if (pT.special < specialStatus) {
      pT.special = specialStatus;

    } else if (pT.special == specialStatus) {
      pT.special++;

      if (pT.special > 3)
        pT.special = 3;
    }
  }

  // now the item can be deleted
  iM.list[index].active = false;
}

void checkItemsSpawn() {
  for (uint8_t i = 0; i < MAX_ENEMYS; i++) {

    // check only active enemys
    if (eM.list[i].active == false)
      continue;

    // add item because it is the last state
    if (eM.list[i].state == STATE_SPAWN) {
      //Serial.println("spawn item");
      iM.addRandom(eM.list[i].x, eM.list[i].y);

      // to just spawn item once
      eM.list[i].active = false;
    }
  }
}

void checkPlayerHit() {
  uint8_t projIndex = pM.isProjectileAt(pT.x, pT.y, pT.getWidth(), 7, 1);

  if (projIndex == 0xff)
    return;

  // return because it is the players bullet
  if (pM.list[projIndex].player == true)
    return;

  if (pT.spawnTimeout > 0)
    return;

  uint8_t projectileType = pM.list[projIndex].type;
  uint8_t projectileDmg;

  if (projectileType == WEAPON_STDGUN) {
    projectileDmg = 8;

  } else if (projectileType == WEAPON_SMALL_ROCKET) {
    projectileDmg = 15;
    /*
      } else if (projectileType == WEAPON_LASER) {
      projectileDmg = 20;
    */
  } else if (projectileType == WEAPON_MG) {
    projectileDmg = 4;

  } else if (projectileType == WEAPON_FLAME) {

    // better flame resistance for the flame tank
    if (pT.weaponType == WEAPON_FLAME) {
      projectileDmg = 1;
    } else {
      projectileDmg = 5;
    }
  }

  pT.damage(projectileDmg);

  // and impact animation
  aM.add(pM.list[projIndex].x, pM.list[projIndex].y, ANIMATION_IMPACT_R, 0);

  // deleted the projectile if the enemy did not die
  pM.list[projIndex].active = false;

}

void checkEnemyChrush() {

  uint8_t enemyIndex = eM.isEnemyAt(pT.x, pT.y, pT.getWidth(), 7);

  // no enemy found
  if (enemyIndex == 0xff)
    return;

  // is already dead
  if (eM.list[enemyIndex].state == STATE_DEAD)
    return;

  // trees can have 0 health
  if (eM.list[enemyIndex].health == 0)
    return;

  uint8_t t = eM.list[enemyIndex].type;
  uint8_t dmg = 0;

  if (t < ENEMY_FLAME) {
    dmg = 1;

  } else if (t == ENEMY_FLAME) {
    dmg = 15;

  } else if (t == ENEMY_MINE) {
    dmg = 80;

  } else {
    dmg = 5;
  }

  // with a shield everything can be crushed!!!
  if (pT.shieldActive)
    dmg = 0;

  // the player takes damage by crushing
  pT.damage(dmg);

  // destroy this enemy
  eM.list[enemyIndex].die();
}

void checkEnemyHit() {

  for (uint8_t i = 0; i < MAX_ENEMYS; i++) {

    // check only active enemys
    if (eM.list[i].active == false)
      continue;

    uint8_t projIndex = pM.isProjectileAt(eM.list[i].x, eM.list[i].y, 8, 8, 1);

    // continue is no index was found
    if (projIndex == 0xff)
      continue;

    uint8_t enemyType = eM.list[i].type;

    // mines can not be destroyed
    if (enemyType == ENEMY_MINE)
      continue;

    // make sure projectiles go through destroyed things
    if (enemyType >= ENEMY_CAR && eM.list[i].health == 0)
      continue;

    // continue because of friendly fire
    if (pM.list[projIndex].player == false)
      continue;

    // continue because enemy is already dead
    if (eM.list[i].state >= STATE_DEAD)
      continue;

    uint8_t projectileType = pM.list[projIndex].type;
    uint8_t projectileDmg = 0;
    uint8_t weaponLevel = pT.lvlWeapon[pT.weaponType];

    if (projectileType == WEAPON_STDGUN) {
      projectileDmg = 30 + weaponLevel * 5;

    } else if (projectileType == WEAPON_ROCKET) {
      projectileDmg = 70 + weaponLevel * 8;

    } else if (projectileType == WEAPON_LASER) {
      projectileDmg = pM.list[projIndex].state / 3 + weaponLevel * 2;

    } else if (projectileType == WEAPON_MG) {
      projectileDmg = 10 + weaponLevel * 2;

    } else if (projectileType == WEAPON_FLAME) {
      projectileDmg = 5 + weaponLevel;

    } else if (projectileType == WEAPON_SMALL_ROCKET) {
      projectileDmg = 110;
    }

    // shielded enemys are hard to shoot
    if (enemyType == ENEMY_SHIELD)
      projectileDmg /= 4;

    // do the damage and decide if it dies
    bool isEnemyDead = eM.list[i].damage(projectileDmg);

    // this need to be done because the laser ray is too long
    uint8_t impactPosX = pM.list[projIndex].x;
    if (projectileType == WEAPON_LASER)
      impactPosX = eM.list[i].x;

    aM.add(impactPosX, pM.list[projIndex].y, ANIMATION_IMPACT_L, 0);

    bool destroyProjectile = true;

    // rocket can hit the enemy behind
    if (isEnemyDead && projectileType == WEAPON_ROCKET)
      destroyProjectile = false;
    
    // lasers go through the whole map
    if (projectileType == WEAPON_LASER)
      destroyProjectile = false;

    // now the projectilie can be deleted
    if (destroyProjectile) 
      pM.list[projIndex].active = false;
  }
}

void checkSpecialAttack() {

  if (specialAttackTimeout == 0)
    return;

  if (gameFrames % 7 != 0)
    return;

  specialAttackTimeout--;

  int8_t rocketDir = rand() % 16;

  // random plus or minus
  if (rocketDir % 2)
    rocketDir *= -1;

  const int8_t rocketPos = 16 + (specialAttackTimeout % 5) * 8 + rocketDir / 4;

  //Serial.println("fire specialAttack " + String(rocketDir));

  pM.add(0, rocketPos, WEAPON_SMALL_ROCKET, rocketDir, true);
}

void checkSlowmo() {
  if (slowmoTimeout == 0)
    return;

  slowmoTimeout--;

  // show status and end of the slowmo
  if (slowmoTimeout == 1) {
    arduboy.clear();

  } else {
    arduboy.fillRect(0, 63, slowmoTimeout, 1, BLACK);

  }
}

void playAnimation() {

  // button b not pressed helps to activate the special
  if (arduboy.justPressed(B_BUTTON) || arduboy.pressed(A_BUTTON)) {
    isStartAnimation = false;

    pT.init();

    nextButtonInput = millis() + 400;
  }

  mM.drawMap();
  aM.update();

  // draw some fancy lines
  float lineLen = 48;
  for (uint8_t i = 0; i < 13; i++) {
    arduboy.fillRect(i * 2 + 1, 16 + 48 - lineLen, 1, 50, BLACK);
    arduboy.fillRect(128 - (i * 2 + 1), 16 + 48 - lineLen, 1, 50, BLACK);

    lineLen *= 0.7;
  }

  // and some fancy explosions
  if (gameFrames % GAME_FRAMES == 0) {
    uint8_t r = rand();

    aM.add(16 + r % 96, 24 + r % 40, ANIMATION_BOOM, 0);
  }

  pT.drawPlayer();
  playButtons();

  drawOffsetBitmap(32, 18, menuMicroTank, 64, 64, 5, 0, false, BLACK);
}

void prepareThings() {
  // calculate some modulos

  // decrement timer for sound on/off message
  if (audioMessageTimeout > 0)
    audioMessageTimeout--;
}

void checkProjectiles() {
  checkPlayerHit();
  checkEnemyHit();
}

#endif
