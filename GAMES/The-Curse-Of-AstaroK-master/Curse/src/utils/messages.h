#pragma once

//from Curse_PlayGameState_Render.ino
const uint8_t Inventory_Title[] PROGMEM = " Inventory";
const uint8_t Inventory_Quantity[] PROGMEM = " x ";
const uint8_t Statistics_Title[] PROGMEM = "Statistics";
const uint8_t Statistics_Health[] PROGMEM = "Health :\nSkill :\nDefence :\nGold :\nDungeons :";
const uint8_t Runes_Title[] PROGMEM = "    Runes";
const uint8_t Enemy_Title[] PROGMEM = " Enemy Sts";
const uint8_t Enemy_Stats[] PROGMEM = "Name:\nHealth :\nDefence :\n";
const uint8_t Save_Game_Option[] PROGMEM = "Save Game\nNew Game\n";
const uint8_t Restore_Game_Option[] PROGMEM = "Restore Game";
const uint8_t Purchased_A[] PROGMEM = "a ";
const uint8_t Cast_Nothing[] PROGMEM = "nothing!";
const uint8_t You_Inflicted[] PROGMEM = "You inflicted ";
const uint8_t Points_Damage[] PROGMEM = " pts of damage.";
const uint8_t Plus_Extra[] PROGMEM = "Plus an extra ";
const uint8_t You_Regained[] PROGMEM = "You regained ";
const uint8_t Health_Points[] PROGMEM = " health points.";
const uint8_t Aztarok_Stole[] PROGMEM = "  AstaroK stole ";
const uint8_t Hp_Points[] PROGMEM = " hp points.";
const uint8_t Attacks[] PROGMEM = " attacks, ";
const uint8_t Damage_Points[] PROGMEM = " damage pts.";
const uint8_t Gelly_Theft[] PROGMEM = "The Gelatinous Cube stole a\n   ";
const uint8_t Rune[] PROGMEM = " rune";
const uint8_t From_You[] PROGMEM = " from you!";
const uint8_t You_Win[] PROGMEM = " You killed AstaroK and freed\nthe villagers from the curse!";
const uint8_t You_Scored[] PROGMEM = "You scored ";
const uint8_t Gold_Coins[] PROGMEM = " gold coins.";
const uint8_t Skill_Points[] PROGMEM = " skill points.";
const uint8_t You_Lost[] PROGMEM = "You died! Lose half of your\n      gold and skill points!";
const uint8_t GP[] PROGMEM = "GP";
const uint8_t SP[] PROGMEM = "SP";
const uint8_t You_Cast[] PROGMEM = "You cast ";
const uint8_t None[] PROGMEM = "None !";

//from Enemy.cpp
const uint8_t Astarok_Name[] PROGMEM = "ASTAROK";

//from Constants.h
const uint8_t Tombstone_Inscription_00[] PROGMEM = "   Here lies Vampirics.\n  Born 1843 - Dec 1882";
const uint8_t Tombstone_Inscription_01[] PROGMEM = " Rest In Peace Filmote.\n  Born 1841 - Dec 1889";
const uint8_t Tombstone_Inscription_02[] PROGMEM = "   Press Play On Tape\n     Developed 2019";

const uint8_t Dialogue_00[] PROGMEM = "Welcome traveller to the town";
const uint8_t Dialogue_01[] PROGMEM = "of Griford. The evil AstaroK";
const uint8_t Dialogue_02[] PROGMEM = "has turned us all into pumpkin";
const uint8_t Dialogue_03[] PROGMEM = "people. Please help us defeat";
const uint8_t Dialogue_04[] PROGMEM = "this crazy wizard!";
const uint8_t Dialogue_05[] PROGMEM = "Be sure to equip yourself";
const uint8_t Dialogue_06[] PROGMEM = "properly. You can press \"$% to";
const uint8_t Dialogue_07[] PROGMEM = "access your inventory and to";
const uint8_t Dialogue_08[] PROGMEM = "view information about you or";
const uint8_t Dialogue_09[] PROGMEM = "your enemies.";
const uint8_t Dialogue_10[] PROGMEM = "In combat, cast runes to score";
const uint8_t Dialogue_11[] PROGMEM = "'poker' hands. Pressing \"#% will";
const uint8_t Dialogue_12[] PROGMEM = "cast the runes - jump and";
const uint8_t Dialogue_13[] PROGMEM = "touch runes to select them";
const uint8_t Dialogue_14[] PROGMEM = "for recasting. Normally you";
const uint8_t Dialogue_15[] PROGMEM = "can cast 3 times, but some";
const uint8_t Dialogue_16[] PROGMEM = "items might give you extra.";

//from PLayers.h
const uint8_t PlayerItems_Caption00[] PROGMEM = "Crystal Helmet";
const uint8_t PlayerItems_Caption01[] PROGMEM = "High Helmet";
const uint8_t PlayerItems_Caption02[] PROGMEM = "Round Helmet";
const uint8_t PlayerItems_Caption03[] PROGMEM = "Fire Potion";
const uint8_t PlayerItems_Caption04[] PROGMEM = "Health Potion";
const uint8_t PlayerItems_Caption05[] PROGMEM = "Speed Potion";

const uint8_t RuneCombination_Purchased_Caption00[] PROGMEM = "Fire Blaze";
const uint8_t RuneCombination_Purchased_Caption01[] PROGMEM = "Healing Wind";
const uint8_t RuneCombination_Purchased_Caption02[] PROGMEM = "Rising Star";
const uint8_t RuneCombination_Purchased_Caption03[] PROGMEM = "Venom Mist";

const uint8_t Rune_Effect_Caption00[] PROGMEM = "5 Fire DMG";
const uint8_t Rune_Effect_Caption01[] PROGMEM = "Heal 20 HP";
const uint8_t Rune_Effect_Caption02[] PROGMEM = "35 Damage";
const uint8_t Rune_Effect_Caption03[] PROGMEM = "2 P. DMG";

const uint8_t RuneCombination_Standard_51[] PROGMEM = "a Five of a Kind!";
const uint8_t RuneCombination_Standard_52[] PROGMEM = "a Four of a Kind!";
const uint8_t RuneCombination_Standard_53[] PROGMEM = "a Three of a Kind!";
const uint8_t RuneCombination_Standard_54[] PROGMEM = "Two Pairs!";
const uint8_t RuneCombination_Standard_55[] PROGMEM = "a Full House!";

//from Curse_PlayGameState_Update.ino
//Note: non of these messages may be the 1st message to prevent them from having a Idx of 0)
const uint8_t Pickup_HP[] PROGMEM = "You picked up some HP!"
const uint8_t Thank_You[] PROGMEM = "    Thank you for saving\n      us from the Curse!"
const uint8_t Overwrite_Confirm[] PROGMEM = " Overwrite existing game?\n          \"#% Yes    \"$% No"
const uint8_t New_Game_Confirm[] PROGMEM = "  Start a new game?\n     \"#% Yes    \"$% No"
const uint8_t Game_Restored[] PROGMEM = "  Game state restored!"
const uint8_t Same_Helmet[] PROGMEM = "    You already have\n one of those helmets!"
const uint8_t Max_Potions[] PROGMEM = "  You already have as\nmany of these potions\n  as you can carry !"
const uint8_t Buy_Item[] PROGMEM = "   Buy this item?\n   \"#% Yes    \"$% No"
const uint8_t No_GP[] PROGMEM = "You do not have\n   enough gold!"
const uint8_t Same_Rune[] PROGMEM = "   You are carrying\n one of those runes!"
const uint8_t Buy_Rune[] PROGMEM = "  Buy this rune?\n  \"#% Yes    \"$% No"
const uint8_t No_SP[] PROGMEM = " You do not have\n    enough SP!"
const uint8_t Monster_Attack[] PROGMEM = "        A monster attacks you!"
const uint8_t Throw_Again[] PROGMEM = "Throw the runes again!"
const uint8_t Game_Saved[] PROGMEM = "  Game state saved!"
