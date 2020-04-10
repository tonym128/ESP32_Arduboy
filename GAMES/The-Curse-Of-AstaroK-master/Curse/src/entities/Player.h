#pragma once

#include "../utils/Arduboy2Ext.h"
#include "../utils/Constants.h"
#include "../utils/Enums.h"
#include "../ardBitmap/ArdBitmap.h"

const uint8_t PROGMEM Player_Items[] = {  // Type, Index, Def, GP
  static_cast<uint8_t>(PlayerItemType::Helmet), static_cast<uint8_t>(HelmetType::Crystal), 10, 65,
  static_cast<uint8_t>(PlayerItemType::Helmet), static_cast<uint8_t>(HelmetType::High), 5, 40,
  static_cast<uint8_t>(PlayerItemType::Helmet), static_cast<uint8_t>(HelmetType::Round), 5, 35,
  static_cast<uint8_t>(PlayerItemType::Potion), static_cast<uint8_t>(PotionType::Fire), 2, 20,
  static_cast<uint8_t>(PlayerItemType::Potion), static_cast<uint8_t>(PotionType::Health), 1, 15,
  static_cast<uint8_t>(PlayerItemType::Potion), static_cast<uint8_t>(PotionType::Speed), 1, 20,
  static_cast<uint8_t>(PlayerItemType::Exit), 0, 1, 30,
};

const uint8_t RuneCombination_Purchased_Captions_Length[] = { 102, 110, 106, 102 };
const uint8_t RuneCombination_Standard_Captions_Length[] = { 112, 112, 118, 92, 101 };

constexpr uint8_t JumpHeight_Count = 20;
constexpr uint8_t JumpHeight[JumpHeight_Count] = { 35, 33, 31, 29, 27, 26, 26, 25, 25, 25, 25, 25, 25, 26, 26, 27, 29, 31, 33, 35 };

class Player { 

  public:

    Player();

    int16_t getX();
    uint8_t getHP();
    uint8_t getGP();
    uint8_t getSP();
    uint8_t getDEF();
    uint8_t getDEFOverall();
    bool isJumping();
    uint8_t getRuneCombination(const uint8_t index);

    PlayerDirection getPlayerDirection();
    HelmetType getHelmetType();
    uint8_t getHelmetTypeCount(const HelmetType HelmetType);
    uint8_t getPotionTypeCount(const PotionType potionType);
    uint8_t getPotionTypeCount();
    uint8_t getItemCount(const uint8_t index);
    uint8_t getDistinctItemCount();
    uint8_t getDistinctPotionCount();
    uint8_t getPlayerItemIndex_FromInvIndex(const uint8_t invIndex);
    uint8_t getDistinctRuneCombinationCount();

    void setX(const int16_t value);
    void setY(const uint8_t value);
    void setHP(const uint8_t value);
    void setGP(const uint16_t value);
    void setSP(const uint16_t value);
    void setDef(const uint16_t value);
    void setJumping(const bool value);
    void setRuneCombination(const uint8_t index, const uint8_t value);
    void setItemCount(const uint8_t index, const uint8_t value);
    void setRecast();

    void setPlayerDirection(PlayerDirection value);
    void setHelmetType(const HelmetType HelmetType);
    void setHelmetTypeCount(const HelmetType HelmetType, const uint8_t value);
    void setPotionTypeCount(const PotionType potionType, const uint8_t value);
   
    void render(Arduboy2Ext &arduboy, ArdBitmap<128, 64> &ardBitmap, bool inside);
    void incX();
    void decX();
    void incRuneCombination(const RuneCombinationType_Purchased runeCombination);
    void decRuneCombination(const RuneCombinationType_Purchased runeCombination);
    void incHelmetType(const HelmetType HelmetType);
    void decHelmetType(const HelmetType HelmetType);
    void incPotionType(const PotionType potionType);
    void decPotionType(const PotionType potionType);
    void incDef(const uint8_t def);
    void incGP(const uint8_t gp);
    void incHP(const uint8_t hp);
    void incSP(const uint8_t sp);
    void decHP(const uint8_t hp);
    void inflictDMG(const uint8_t dmg);
    void update();
    void kill();

  private:

    void updateFrame(Arduboy2Ext &arduboy, uint8_t everyFrame, uint8_t frameMax);

  private:

    uint8_t hp;
    uint8_t gp;
    uint8_t sp;
    uint8_t def;
    uint8_t y;
    uint8_t frame;
    uint8_t recast;

    bool jumping;
    uint8_t jumpIndex;

    uint8_t runeCombination[Constants::RuneCombinations_Count];
    PlayerDirection playerDirection = PlayerDirection::IdleRight;

    HelmetType helmetType; // Being worn.
    uint8_t playerItems[Constants::PlayerItems_Count]; 
    int16_t x;

};
