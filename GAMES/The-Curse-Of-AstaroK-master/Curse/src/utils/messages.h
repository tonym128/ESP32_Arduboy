#pragma once

//from Curse_PlayGameState_Render.ino
const uint8_t Inventory_Title[]  = " Inventory";
const uint8_t Inventory_Quantity[]  = " x ";
const uint8_t Statistics_Title[]  = "Statistics";
const uint8_t Statistics_Health[]  = "Health :\nSkill :\nDefence :\nGold :\nDungeons :";
const uint8_t Runes_Title[]  = "    Runes";
const uint8_t Enemy_Title[]  = " Enemy Sts";
const uint8_t Enemy_Stats[]  = "Name:\nHealth :\nDefence :\n";
const uint8_t Save_Game_Option[]  = "Save Game\nNew Game\n";
const uint8_t Restore_Game_Option[]  = "Restore Game";
const uint8_t Purchased_A[]  = "a ";
const uint8_t Cast_Nothing[]  = "nothing!";
const uint8_t You_Inflicted[]  = "You inflicted ";
const uint8_t Points_Damage[]  = " pts of damage.";
const uint8_t Plus_Extra[]  = "Plus an extra ";
const uint8_t You_Regained[]  = "You regained ";
const uint8_t Health_Points[]  = " health points.";
const uint8_t Aztarok_Stole[]  = "  AstaroK stole ";
const uint8_t Hp_Points[]  = " hp points.";
const uint8_t Attacks[]  = " attacks, ";
const uint8_t Damage_Points[]  = " damage pts.";
const uint8_t Gelly_Theft[]  = "The Gelatinous Cube stole a\n   ";
const uint8_t Rune[]  = " rune";
const uint8_t From_You[]  = " from you!";
const uint8_t You_Win[]  = " You killed AstaroK and freed\nthe villagers from the curse!";
const uint8_t You_Scored[]  = "You scored ";
const uint8_t Gold_Coins[]  = " gold coins.";
const uint8_t Skill_Points[]  = " skill points.";
const uint8_t You_Lost[]  = "You died! Lose half of your\n      gold and skill points!";
const uint8_t GP[]  = "GP";
const uint8_t SP[]  = "SP";
const uint8_t You_Cast[]  = "You cast ";
const uint8_t None[]  = "None !";

//from Enemy.cpp
const uint8_t Astarok_Name[]  = "ASTAROK";

//from Constants.h
const uint8_t Tombstone_Inscription_00[]  = "   Here lies Vampirics.\n  Born 1843 - Dec 1882";
const uint8_t Tombstone_Inscription_01[]  = " Rest In Peace Filmote.\n  Born 1841 - Dec 1889";
const uint8_t Tombstone_Inscription_02[]  = "   Press Play On Tape\n     Developed 2019";

const uint8_t Dialogue_00[]  = "Welcome traveller to the town";
const uint8_t Dialogue_01[]  = "of Griford. The evil AstaroK";
const uint8_t Dialogue_02[]  = "has turned us all into pumpkin";
const uint8_t Dialogue_03[]  = "people. Please help us defeat";
const uint8_t Dialogue_04[]  = "this crazy wizard!";
const uint8_t Dialogue_05[]  = "Be sure to equip yourself";
const uint8_t Dialogue_06[]  = "properly. You can press \"$% to";
const uint8_t Dialogue_07[]  = "access your inventory and to";
const uint8_t Dialogue_08[]  = "view information about you or";
const uint8_t Dialogue_09[]  = "your enemies.";
const uint8_t Dialogue_10[]  = "In combat, cast runes to score";
const uint8_t Dialogue_11[]  = "'poker' hands. Pressing \"#% will";
const uint8_t Dialogue_12[]  = "cast the runes - jump and";
const uint8_t Dialogue_13[]  = "touch runes to select them";
const uint8_t Dialogue_14[]  = "for recasting. Normally you";
const uint8_t Dialogue_15[]  = "can cast 3 times, but some";
const uint8_t Dialogue_16[]  = "items might give you extra.";

//from PLayers.h
const uint8_t PlayerItems_Caption00[]  = "Crystal Helmet";
const uint8_t PlayerItems_Caption01[]  = "High Helmet";
const uint8_t PlayerItems_Caption02[]  = "Round Helmet";
const uint8_t PlayerItems_Caption03[]  = "Fire Potion";
const uint8_t PlayerItems_Caption04[]  = "Health Potion";
const uint8_t PlayerItems_Caption05[]  = "Speed Potion";

const uint8_t RuneCombination_Purchased_Caption00[]  = "Fire Blaze";
const uint8_t RuneCombination_Purchased_Caption01[]  = "Healing Wind";
const uint8_t RuneCombination_Purchased_Caption02[]  = "Rising Star";
const uint8_t RuneCombination_Purchased_Caption03[]  = "Venom Mist";

const uint8_t Rune_Effect_Caption00[]  = "5 Fire DMG";
const uint8_t Rune_Effect_Caption01[]  = "Heal 20 HP";
const uint8_t Rune_Effect_Caption02[]  = "35 Damage";
const uint8_t Rune_Effect_Caption03[]  = "2 P. DMG";

const uint8_t RuneCombination_Standard_51[]  = "a Five of a Kind!";
const uint8_t RuneCombination_Standard_52[]  = "a Four of a Kind!";
const uint8_t RuneCombination_Standard_53[]  = "a Three of a Kind!";
const uint8_t RuneCombination_Standard_54[]  = "Two Pairs!";
const uint8_t RuneCombination_Standard_55[]  = "a Full House!";

//from Curse_PlayGameState_Update.ino
//Note: non of these messages may be the 1st message to prevent them from having a Idx of 0)
const uint8_t Pickup_HP[]  = "You picked up some HP!"
const uint8_t Thank_You[]  = "    Thank you for saving\n      us from the Curse!"
const uint8_t Overwrite_Confirm[]  = " Overwrite existing game?\n          \"#% Yes    \"$% No"
const uint8_t New_Game_Confirm[]  = "  Start a new game?\n     \"#% Yes    \"$% No"
const uint8_t Game_Restored[]  = "  Game state restored!"
const uint8_t Same_Helmet[]  = "    You already have\n one of those helmets!"
const uint8_t Max_Potions[]  = "  You already have as\nmany of these potions\n  as you can carry !"
const uint8_t Buy_Item[]  = "   Buy this item?\n   \"#% Yes    \"$% No"
const uint8_t No_GP[]  = "You do not have\n   enough gold!"
const uint8_t Same_Rune[]  = "   You are carrying\n one of those runes!"
const uint8_t Buy_Rune[]  = "  Buy this rune?\n  \"#% Yes    \"$% No"
const uint8_t No_SP[]  = " You do not have\n    enough SP!"
const uint8_t Monster_Attack[]  = "        A monster attacks you!"
const uint8_t Throw_Again[]  = "Throw the runes again!"
const uint8_t Game_Saved[]  = "  Game state saved!"
