#pragma once

#include "../utils/Arduboy2Ext.h"
#include "Rune.h"
#include "Player.h"


struct MatchedRuneCombination {

  uint8_t id;             // 0 - 50 rule combos, RuneCombinations enum
  uint8_t dmg1;           // Damage to enemy
  uint8_t dmg2;           // Damage to enemy
  uint8_t dmg3;           // Damage to enemy
  uint8_t dmg2Type;       // Damage type = DamageType::FireBlaze && DamageType::VenomMist
  uint8_t dmg3Type;       // 
  uint8_t prevDmg1_Fire;       // Damage to enemy
  uint8_t prevDmg2_Fire;       // Damage to enemy
  uint8_t prevDmg3_Fire;       // Damage to enemy
  uint8_t prevDmg1_Venom;       // Damage to enemy
  uint8_t prevDmg2_Venom;       // Damage to enemy
  uint8_t prevDmg3_Venom;       // Damage to enemy
  uint8_t hp;    // Health to player

  void reset() {
    this->id = Constants::RuneNoValue;
    this->dmg1 = 0;
    this->dmg2 = 0;
    this->dmg3= 0;
    this->hp = 0;
  }

  uint8_t getPrevDmg1(EnemyType enemyType) {

    switch (enemyType) {

      case EnemyType::Skeleton:
        return this->prevDmg1_Fire;

      case EnemyType::LargeWerewolf:
        return this->prevDmg1_Fire + (this->prevDmg1_Venom * 2);

      case EnemyType::GelatinousCube:
        return this->prevDmg1_Venom + (this->prevDmg1_Fire * 2);

      default:
        return this->prevDmg1_Fire + this->prevDmg1_Venom;

    }

  }

  void roll() {

    if (dmg2Type == DamageType::FireBlaze) {
      prevDmg2_Fire = prevDmg2_Fire + dmg2;
      prevDmg3_Fire = prevDmg3_Fire + dmg3;
    }
    else {
      prevDmg2_Venom = prevDmg2_Venom + dmg2;
      prevDmg3_Venom = prevDmg3_Venom + dmg3;
    }

    prevDmg1_Fire = prevDmg2_Fire;
    prevDmg2_Fire = prevDmg3_Fire;
    prevDmg3_Fire = 0;

    prevDmg1_Venom = prevDmg2_Venom;
    prevDmg2_Venom = prevDmg3_Venom;
    prevDmg3_Venom = 0;

    dmg2Type = 0;
    dmg3Type = 0;

  }

void print() {

  // Serial.print("MRC ");
  // Serial.print(dmg1);
  // Serial.print(":");
  // Serial.print(dmg2);
  // Serial.print(":");
  // Serial.print(dmg3);
  // Serial.print(", type ");
  // Serial.print(dmg2Type);
  // Serial.print(":");
  // Serial.print(dmg3Type);

  // Serial.print("   Prev Fire ");
  // Serial.print(prevDmg1_Fire);
  // Serial.print(":");
  // Serial.print(prevDmg2_Fire);
  // Serial.print(":");
  // Serial.print(prevDmg3_Fire);
  // Serial.print("   Prev Venom ");
  // Serial.print(prevDmg1_Venom);
  // Serial.print(":");
  // Serial.print(prevDmg2_Venom);
  // Serial.print(":");
  // Serial.println(prevDmg3_Venom);
}

};


class Runes { 
    
  public:

    Runes();

    Rune & getRune(const uint8_t index);
    void checkRuneCombination(Player &player, MatchedRuneCombination &matchedRuneCombination);

    void setReroll(const bool value);
    void update(Arduboy2Ext &arduboy);
    void startSpinning();
    bool anyRotating();
    bool anyRotatingOrRerolling();


  private:

    void sort(uint8_t array[], uint8_t size);

  private:

    Rune runes[5];

    
    // Used to return the combination (if found) ..

    MatchedRuneCombination matchedRuneCombination;

};
