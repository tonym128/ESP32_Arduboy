#include "Runes.h"

#include "../utils/Arduboy2Ext.h"
#include "../images/Images.h"
#include "../utils/Constants.h"

Runes::Runes() {}

Rune & Runes::getRune(const uint8_t index) {

  return this->runes[index];

}

void Runes::checkRuneCombination(Player &player, MatchedRuneCombination &matchedRuneCombination) {

  uint8_t helmetAttack = 0;
  uint8_t sortedRunes[5];
  uint8_t runeCount[6] = { 0, 0, 0, 0, 0, 0 };
  uint8_t helmetAttacks[3] = { 10, 5, 0 };


  // Is the player wering a helmet?

  if (player.getHelmetType() != HelmetType::None) {

    helmetAttack = helmetAttacks[static_cast<uint8_t>(player.getHelmetType())];

  }


  // Sort Runes ..

  matchedRuneCombination.reset();

  for (uint8_t x = 0; x< 5; x++) {
    sortedRunes[x] = this->runes[x].getValue();
  }
  sort(sortedRunes, 5);


  // Check for venom mist first ..

  if (player.getRuneCombination(static_cast<uint8_t>(RuneCombinationType_Purchased::VenomMist)) > 0 && sortedRunes[0] != sortedRunes[1] && sortedRunes[1] != sortedRunes[2] && sortedRunes[2] != sortedRunes[3] && sortedRunes[3] != sortedRunes[4] ){

    matchedRuneCombination.id = static_cast<uint8_t>(RuneCombinationType_Purchased::VenomMist);
    memcpy_P(&matchedRuneCombination.dmg1, &RuneCombinations[(matchedRuneCombination.id * Constants::RuneCombinations_RecordLength) + Constants::RuneCombinations_DMG_1], 4);
    matchedRuneCombination.hp = pgm_read_byte(&RuneCombinations[(matchedRuneCombination.id * Constants::RuneCombinations_RecordLength) + Constants::RuneCombinations_HP]) + helmetAttack;
    matchedRuneCombination.dmg2Type = static_cast<uint8_t>(DamageType::VenomMist);
    matchedRuneCombination.dmg3Type = static_cast<uint8_t>(DamageType::VenomMist);

  }



  // Does the combination match a bought rune?

  for (uint8_t runeIdx = 0; runeIdx < Constants::RuneCombinations_Count; runeIdx++) {

    if (player.getRuneCombination(runeIdx) > 0) {

      uint8_t temp_runes[5];
      memcpy_P(&temp_runes, &RuneCombinations[runeIdx * Constants::RuneCombinations_RecordLength], 5);
      sort(temp_runes, 5);

      if (temp_runes[2] != 7 && temp_runes[0] == sortedRunes[0] && temp_runes[1] == sortedRunes[1] && temp_runes[2] == sortedRunes[2] && temp_runes[3] == sortedRunes[3] && temp_runes[4] == sortedRunes[4]) {

        matchedRuneCombination.id = runeIdx;
        memcpy_P(&matchedRuneCombination.dmg1, &RuneCombinations[(runeIdx * Constants::RuneCombinations_RecordLength) + Constants::RuneCombinations_DMG_1], 4);
        matchedRuneCombination.hp = pgm_read_byte(&RuneCombinations[(runeIdx * Constants::RuneCombinations_RecordLength) + Constants::RuneCombinations_HP]);

        if (runeIdx == static_cast<uint8_t>(RuneCombinationType_Purchased::FireBlaze)) {
          matchedRuneCombination.dmg2Type = static_cast<uint8_t>(DamageType::FireBlaze);
        }
        
        matchedRuneCombination.dmg1 = matchedRuneCombination.dmg1 + helmetAttack;
        return;
      }

    }

  }


  // Get match counts ..

  for (uint8_t x = 0; x < 5; x++) {
    runeCount[sortedRunes[x]]++;
  }

  sort(runeCount, 6);


  // Check for runes with ? marks ..

  for (uint8_t runeIdx = 0; runeIdx < Constants::RuneCombinations_Count; runeIdx++) {

    if (player.getRuneCombination(runeIdx) > 0) {

      uint8_t temp_runes[5];
      memcpy_P(&temp_runes, &RuneCombinations[runeIdx * Constants::RuneCombinations_RecordLength], 5);

      if (temp_runes[2] == 7 && runeCount[5] >= 2 && runeCount[4] >= 2) {

        if ((temp_runes[0] == sortedRunes[1] || temp_runes[0] == sortedRunes[3]) &&
            (temp_runes[3] == sortedRunes[1] || temp_runes[3] == sortedRunes[3])) {

          matchedRuneCombination.id = runeIdx;
          memcpy_P(&matchedRuneCombination.dmg1, &RuneCombinations[(runeIdx * Constants::RuneCombinations_RecordLength) + Constants::RuneCombinations_DMG_1], 4);

          if (runeIdx == static_cast<uint8_t>(RuneCombinationType_Purchased::VenomMist)) {
            matchedRuneCombination.dmg2Type = static_cast<uint8_t>(DamageType::VenomMist);
            matchedRuneCombination.dmg3Type = static_cast<uint8_t>(DamageType::VenomMist);
          }

          matchedRuneCombination.dmg1 = matchedRuneCombination.dmg1 + helmetAttack;
          return;

        }

      }

    }

  }


  // Otherwise look for combinations ..

  if (runeCount[5] == 5) {
    matchedRuneCombination.id = static_cast<uint8_t>(RuneCombinationType_Standard::FiveOfAKind);
    matchedRuneCombination.dmg1 = Constants::RuneCombination_FiveOfAKind_Score + helmetAttack;
    return;
  }

  if (runeCount[5] == 4) {
    matchedRuneCombination.id = static_cast<uint8_t>(RuneCombinationType_Standard::FourOfAKind);
    matchedRuneCombination.dmg1 = Constants::RuneCombination_FourOfAKind_Score + helmetAttack;
    return;
  }

  if (runeCount[5] == 3 && runeCount[4] == 2) {
    matchedRuneCombination.id = static_cast<uint8_t>(RuneCombinationType_Standard::FullHouse);
    matchedRuneCombination.dmg1 = Constants::RuneCombination_FullHouse_Score + helmetAttack;
    return;
  }

  if (runeCount[5] == 3 && runeCount[4] != 2) {
    matchedRuneCombination.id = static_cast<uint8_t>(RuneCombinationType_Standard::ThreeOfAKind);
    matchedRuneCombination.dmg1 = Constants::RuneCombination_ThreeOfAKind_Score + helmetAttack;
    return;
  }

  if (runeCount[5] == 2 && runeCount[4] == 2) {
    matchedRuneCombination.id = static_cast<uint8_t>(RuneCombinationType_Standard::TwoPairs);
    matchedRuneCombination.dmg1 = Constants::RuneCombination_TwoPair_Score + helmetAttack;
    return;
  }

}

void swap(uint8_t & x, uint8_t & y) {
  auto t = x;
  x = y;
  y = t;
}

// A function to implement selection sort
// See: https://en.wikipedia.org/wiki/Selection_sort
void Runes::sort(uint8_t array[], uint8_t size) {

  for (uint8_t startIndex = 0; startIndex < size; ++startIndex)
    for (uint8_t index = (startIndex + 1); index < size; ++index)
      if (array[index] < array[startIndex])
        swap(array[index], array[startIndex]);

}


void Runes::update(Arduboy2Ext &arduboy) {

  for (uint8_t x = 0; x < 5; x++) {
    this->runes[x].update(arduboy);
  }

}

void Runes::startSpinning() {

  for (uint8_t x = 0; x < 5; x++) {
    this->runes[x].startSpinning();
  }

}

bool Runes::anyRotating() {

  for (uint8_t x = 0; x < 5; x++) {
    if (this->runes[x].isRotating()) return true;
  }

  return false;

}

bool Runes::anyRotatingOrRerolling() {

  for (uint8_t x = 0; x < 5; x++) {
    if (this->runes[x].isRotatingOrRerolling()) return true;
  }

  return false;

}

void Runes::setReroll(bool value) {

  for (uint8_t x = 0; x < 5; x++) {
    this->runes[x].setReroll(value);
  }
  
}

