#include "Enemy.h"

#include "../images/Images.h"
#include "../utils/Constants.h"
#include "../ardBitmap/ArdBitmap.h"
#include "../fonts/Font3x6.h"

Enemy::Enemy() {}

EnemyType Enemy::getEnemyType() {
  return this->enemyType;
}

uint8_t Enemy::getHP() {
  return this->hp;
}

uint8_t Enemy::getDEF() {
  return this->def;
}

void Enemy::incEnemyType(Arduboy2Ext &arduboy) {
  setEnemyType(arduboy, ++this->enemyType);
}

void Enemy::decEnemyType(Arduboy2Ext &arduboy) {
  setEnemyType(arduboy, --this->enemyType);
}

void Enemy::setEnemyType(Arduboy2Ext &arduboy, EnemyType enemyType) {

  this->enemyType = enemyType;
  this->frame = 0;

  for (uint8_t x = 0; x < 3; x++) {
    this->names[x] = arduboy.randomLFSR(0, RandomNames_Count);
  }

}

void Enemy::setHP(uint8_t hp) {
  this->hp = hp;
  this->hpMax = hp;
}

void Enemy::setDEF(uint8_t def) {
  this->def = def;
}

void Enemy::inflictDMG(uint8_t dmg) {

  uint8_t hpAdj = (dmg > this->def ? dmg - this->def : 0);
  
  if (this->hp > hpAdj) {
    this->hp = this->hp - hpAdj;
  }
  else {
    this->hp = 0;
  }

}

const uint8_t PROGMEM largeWerewolf_Head_Offset[] = { 0, 1, 1 };
const uint8_t PROGMEM largeWerewolf_Torso_Offset[] = { 0, 0, 1 };
const uint8_t PROGMEM aztarok_Left_Offset[] = { 1, 0, 0, 1 };
const uint8_t PROGMEM aztarok_Middle_Offset[] = { 0, 1, 2, 1 };
const uint8_t PROGMEM aztarok_Right_Offset[] = { 1, 1, 0, 0 };

void Enemy::render(Arduboy2Ext &arduboy, ArdBitmap<128, 64> &ardBitmap, Font3x6 &font3x6) {

  switch (this->enemyType) {

    case EnemyType::Skeleton:
      ardBitmap.drawCompressed(86, 34, reinterpret_cast<const uint8_t *>(pgm_read_ptr(&Images::Skeleton[frame])), WHITE, MIRROR_NONE);
      this->updateFrame(arduboy, 3);
      break;

    case EnemyType::SmallWerewolf:
      ardBitmap.drawCompressed(86, 34, reinterpret_cast<const uint8_t *>(pgm_read_ptr(&Images::SmallWerewolf[frame])), WHITE, MIRROR_NONE);
      this->updateFrame(arduboy, 3);
      break;

    case EnemyType::GelatinousCube:
      ardBitmap.drawCompressed(75, 18,reinterpret_cast<const uint8_t *>(pgm_read_ptr(&Images::GelatinousCube[frame])), WHITE, MIRROR_NONE);
      this->updateFrame(arduboy, 4);
      break;

    case EnemyType::LargeWerewolf:
      SpritesB::drawSelfMasked(75, 26 + pgm_read_byte(&largeWerewolf_Torso_Offset[frame]), Images::LargeWerewolf_Torso, 0);
      SpritesB::drawExternalMask(76, 21 + pgm_read_byte(&largeWerewolf_Head_Offset[frame]), Images::LargeWerewolf_Head, Images::LargeWerewolf_Head_Mask, 0, 0);
      SpritesB::drawSelfMasked(84, 42, Images::LargeWerewolf_Bottom, 0);
      this->updateFrame(arduboy, 3);
      break;

    case EnemyType::Astarok:
      SpritesB::drawSelfMasked(77, 21 + pgm_read_byte(&aztarok_Middle_Offset[frame]), Images::AztaroK_Middle, 0);
      SpritesB::drawExternalMask(65, 30 + pgm_read_byte(&aztarok_Left_Offset[frame]), Images::AztaroK_Left, Images::AztaroK_Left_Mask, 0, 0);
      SpritesB::drawExternalMask(102, 30 + pgm_read_byte(&aztarok_Right_Offset[frame]), Images::AztaroK_Right, Images::AztaroK_Right_Mask, 0, 0);
      this->updateFrame(arduboy, 4);
      break;
      
  }


  // Draw enemy gauge ..

  ardBitmap.drawCompressed(73, 0, Images::Enemy_InfoPanel, WHITE, MIRROR_NONE);
  
  if ((this->hp < Constants::HP_FlashThreshold && arduboy.getFrameCountHalf(32)) || this->hp >= Constants::HP_FlashThreshold) {

    uint16_t hpInc = (33 * this->hp) / this->hpMax;

    for (uint8_t x = 0; x < hpInc; x = x + 2) {
      arduboy.drawFastVLine(75 + x, 10, 2);
    }
    
  }

  if (this->enemyType != EnemyType::Astarok) {
    font3x6.setCursor(80, 1);
  }
  else {
    font3x6.setCursor(78, 1);
  }

  this->renderName(font3x6);

}

void Enemy::updateFrame(Arduboy2Ext &arduboy, uint8_t frameMax) {

  if (arduboy.everyXFrames(8)) {
    frame++;
    if (frame == frameMax) frame = 0;
  }     

}


void Enemy::renderName(Font3x6 &font3x6) {

  if (this->enemyType != EnemyType::Astarok) {

    char name[7];
    name[6]=0;
    for (uint8_t x = 0; x < 3; x++) {
      memcpy_P(&name[x * 2], &RandomNames[this->names[x] * 2], 2);
    }

    font3x6.print(name);

  }
  else {
    font3x6.printMessage(Astarok_Name_Idx);
  }
  
}