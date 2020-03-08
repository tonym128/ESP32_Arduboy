#include "Player.h"

#include "../utils/Constants.h"
#include "../utils/Utils.h"
#include "../utils/Enums.h"
#include "../images/Images.h"
#include "../ardBitmap/ArdBitmap.h"

Player::Player() { }


int16_t Player::getX() {

  return this->x;

}

uint8_t Player::getHP() {

  return this->hp;

}

uint8_t Player::getGP() {

  return this->gp;

}

uint8_t Player::getSP() {

  return this->sp;

}

uint8_t Player::getDEF() {

  return this->def;

}

uint8_t Player::getDEFOverall() {

  if (this->helmetType != HelmetType::None) {
    return this->def + pgm_read_byte(&Player_Items[(static_cast<uint8_t>(this->helmetType) * 4) + 2]);
  }

  return this->def;

}

bool Player::isJumping() {

  return this->jumping;

}

uint8_t Player::getRuneCombination(const uint8_t index) {
  return this->runeCombination[index];
}

PlayerDirection Player::getPlayerDirection() {

  return this->playerDirection;

}

HelmetType Player::getHelmetType() {

  return this->helmetType;

}

uint8_t Player::getHelmetTypeCount(const HelmetType HelmetType) {

  return this->playerItems[static_cast<uint8_t>(HelmetType)];

}

uint8_t Player::getPotionTypeCount(const PotionType potionType) {

  return this->playerItems[static_cast<uint8_t>(HelmetType::Count) + static_cast<uint8_t>(potionType)];

}

uint8_t Player::getItemCount(const uint8_t index) {

  return this->playerItems[index];

}

uint8_t Player::getDistinctItemCount() {

  uint8_t count = 0;

  for (uint8_t x = 0; x < Constants::PlayerItems_Count; x++) {

    if (this->playerItems[x] > 0) count++;

  }

  return count;

}

uint8_t Player::getDistinctPotionCount() {

  uint8_t count = 0;

  for (uint8_t x = static_cast<uint8_t>(HelmetType::Count); x < Constants::PlayerItems_Count; x++) {

    if (this->playerItems[x] > 0) count++;

  }

  return count;

}


uint8_t Player::getDistinctRuneCombinationCount() {

  uint8_t count = 0;

  for (uint8_t x = 0; x < Constants::RuneCombinations_Count; x++) {

    if (this->runeCombination[x] > 0) count++;

  }

  return count;

}


uint8_t Player::getPotionTypeCount() {

  uint8_t count = 0;

  for (uint8_t x = static_cast<uint8_t>(HelmetType::Count); x < Constants::PlayerItems_Count; x++) {

    count = count + this->playerItems[x];

  }

  return count;

}


uint8_t Player::getPlayerItemIndex_FromInvIndex(const uint8_t invIndex) {

  uint8_t count = 0;

  for (uint8_t x = 0; x < Constants::PlayerItems_Count; x++) {

    if (this->playerItems[x] > 0) {

      if (count == invIndex) return x;
      count++;

    }

  }

  return 255;
  
}

void Player::setX(const int16_t value) {
  
  this->x = value;
  
}

void Player::setY(const uint8_t value) {
  
  this->y = value;
  
}

void Player::setHP(const uint8_t value) {
  
  this->hp = (value > 100 ? 100 : value);
 
}

void Player::setGP(const uint16_t value) {
  
  this->gp = value;
  
}

void Player::setSP(const uint16_t value) {
  
  this->sp = value;
  
}

void Player::setDef(const uint16_t value) {
  
  this->def = value;
  
}

void Player::setJumping(const bool value) {
  
  this->jumping = value;

  if (value) this->jumpIndex = 0;
  
}

void Player::setRuneCombination(const uint8_t index, const uint8_t value) {
  this->runeCombination[index] = value;
}

void Player::setPlayerDirection(PlayerDirection value) {
  
  if (this->playerDirection != value) {
    this->playerDirection = value;
    this->frame = 0;
  }
  
}

void Player::setHelmetType(const HelmetType HelmetType) {
  
  this->helmetType = HelmetType;
  
}

void Player::setHelmetTypeCount(const HelmetType HelmetType, const uint8_t value) {
  
  this->playerItems[static_cast<uint8_t>(HelmetType)] = value;
  
}

void Player::setPotionTypeCount(const PotionType potionType, const uint8_t value) {
  
  this->playerItems[static_cast<uint8_t>(HelmetType::Count) + static_cast<uint8_t>(potionType)] = value;
  
}

void Player::setItemCount(const uint8_t index, const uint8_t value) {

  if (index >= static_cast<uint8_t>(HelmetType::Crystal) && index <= static_cast<uint8_t>(HelmetType::Count)) {

    if (this->helmetType == static_cast<HelmetType>(index)) {
      this->helmetType = HelmetType::None;
    }

  }

  this->playerItems[index] = value;

}

void Player::setRecast() {
  
// Double cast  this->recast = 16;
  this->recast = 12;
  
}

void Player::incX() {  
  this->x++;  
}

void Player::decX() {  
  this->x--;  
}

void Player::incRuneCombination(const RuneCombinationType_Purchased runeCombination) {
  this->runeCombination[static_cast<uint8_t>(runeCombination)]++;
}

void Player::decRuneCombination(const RuneCombinationType_Purchased runeCombination) {
  this->runeCombination[static_cast<uint8_t>(runeCombination)]--;
}

void Player::incHelmetType(const HelmetType HelmetType) {
  uint8_t index = static_cast<uint8_t>(HelmetType);
  this->playerItems[index]++;
}

void Player::decHelmetType(const HelmetType HelmetType) {
  this->playerItems[static_cast<uint8_t>(HelmetType)]--;
}

void Player::incPotionType(const PotionType potionType) {
  this->playerItems[static_cast<uint8_t>(HelmetType::Count) + static_cast<uint8_t>(potionType)]++;
}

void Player::decPotionType(const PotionType potionType) {
  this->playerItems[static_cast<uint8_t>(HelmetType::Count) + static_cast<uint8_t>(potionType)]--;
}


void Player::updateFrame(Arduboy2Ext &arduboy, uint8_t everyFrame, uint8_t frameMax) {

  if (arduboy.everyXFrames(everyFrame)) {
    frame++;
    if (frame == frameMax) frame = 0;
  }     

}


void Player::render(Arduboy2Ext &arduboy, ArdBitmap<128, 64> &ardBitmap, bool inside) {

  uint8_t yPos = (this->jumping ? JumpHeight[this->jumpIndex] : this->y);

  {
    uint8_t mirror = (this->playerDirection == PlayerDirection::IdleRight || this->playerDirection == PlayerDirection::WalkingRight) ? MIRROR_HORIZONTAL : MIRROR_NONE;

    if (this->recast == 0){//} || (this->recast % 8 < 4)) {   Double cast 

      uint8_t frameOffset = 0;

      switch (this->playerDirection) {

        case PlayerDirection::IdleLeft:
        case PlayerDirection::IdleRight:
          this->updateFrame(arduboy, 6, 3);
          break;

        case PlayerDirection::WalkingLeft:
        case PlayerDirection::WalkingRight:
          frameOffset = 3;
          this->updateFrame(arduboy, 2, 4);
          break;
          
      }

      ardBitmap.drawCompressed_WithMask(this->x, yPos, reinterpret_cast<const uint8_t *>(pgm_read_ptr(&Images::Player_Images[frameOffset + this->frame])), reinterpret_cast<const uint8_t *>(pgm_read_ptr(&Images::Player_Masks[frameOffset + this->frame])), mirror);

    }
    else {

      ardBitmap.drawCompressed_WithMask(this->x + (mirror == MIRROR_NONE ? -3 : 1), yPos, Images::Recast, Images::Recast_Mask, mirror);

    }

  }


  // Render Helmet if one is being worn ..

  if (this->helmetType != HelmetType::None) {

    uint8_t yOffset = 5;
    uint8_t mirror = MIRROR_NONE;

    switch (this->playerDirection) {

      case PlayerDirection::IdleLeft:
        yOffset = 5 - (this->frame > 0 ? 1 : 0);
        break;

      case PlayerDirection::WalkingLeft:
        break;

      case PlayerDirection::IdleRight:
        yOffset = 5 - (this->frame > 0 ? 1 : 0);
        mirror = MIRROR_HORIZONTAL;
        break;

      case PlayerDirection::WalkingRight:
        yOffset = 5;
        mirror = MIRROR_HORIZONTAL;
        break;
        
    }

    ardBitmap.drawCompressed_WithMask(this->x + 2, yPos - yOffset, reinterpret_cast<const uint8_t *>(pgm_read_ptr(&Images::Helmets[static_cast<uint8_t>(this->helmetType)])), reinterpret_cast<const uint8_t *>(pgm_read_ptr(&Images::Helmets_Mask[static_cast<uint8_t>(this->helmetType)])), mirror);
 
  }


  // Render jumping puft ..

  if (this->jumping) {
    if (inside) {
      SpritesB::drawSelfMasked(this->x + 2, 46, Images::Jump_Effect,0);
    }
    else {
      SpritesB::drawErase(this->x + 2, 46, Images::Jump_Effect, 0);
    }
  }


  // Render HP Gauge ..

  if (inside) {

    SpritesB::drawExternalMask(0, 0, Images::hpGauge, Images::hpGauge_Mask, 0, 0);

    if ((this->hp < Constants::HP_FlashThreshold && arduboy.getFrameCountHalf(32)) || this->hp >= Constants::HP_FlashThreshold) {

      uint8_t hp = (this->hp > 100 ? 100 : this->hp);

      for (uint8_t x = 0; x < hp / 3; x = x + 2) {
        arduboy.drawFastVLine(16 + x, 3, 2);
      }

    }

  }

}

void Player::incDef(const uint8_t def) {
  this->def = (static_cast<uint8_t>(this->def + def) < this->def ? 255 : this->def + def);
}

void Player::incGP(const uint8_t gp) {
  this->gp = (static_cast<uint8_t>(this->gp + gp) < this->gp ? 255 : this->gp + gp);
}

void Player::incHP(const uint8_t hp) {
  this->hp = (static_cast<uint8_t>(this->hp + hp) > 100 ? 100 : this->hp + hp);
}

void Player::incSP(const uint8_t sp) {
  this->sp = (static_cast<uint8_t>(this->sp + sp) < this->sp ? 255 : this->sp + sp);
}

void Player::decHP(const uint8_t hp) {
  if (this->hp > hp) {
    this->hp = this->hp - hp;
  }
  else {
    this->hp = 0;
  }
}


void Player::update() {

  if (this->jumping) {
    this->jumpIndex++;
    if (this->jumpIndex == JumpHeight_Count) this->jumping = false;
  }

  if (this->recast > 0) this->recast--;

}

void Player::inflictDMG(uint8_t dmg) {

  uint8_t hpAdj = (dmg > this->getDEFOverall() ? dmg - this->getDEFOverall() : 0);
  
  if (this->hp > hpAdj) {
    this->hp = this->hp - hpAdj;
  }
  else {
    this->hp = 0;
  }

}

void Player::kill() {

  this->hp = 100;
  this->sp = this->sp / 2;
  this->gp = this->gp / 2;
  this->x = Constants::Player_CentrePos;		
  
}

