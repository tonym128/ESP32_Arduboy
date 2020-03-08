#pragma once

// Imported 87 messages from "messages.h"

constexpr uint8_t Inventory_Title_Idx = 0;
constexpr uint8_t Inventory_Quantity_Idx = 1;
constexpr uint8_t Statistics_Title_Idx = 2;
constexpr uint8_t Statistics_Health_Idx = 3;
constexpr uint8_t Runes_Title_Idx = 4;
constexpr uint8_t Enemy_Title_Idx = 5;
constexpr uint8_t Enemy_Stats_Idx = 6;
constexpr uint8_t Save_Game_Option_Idx = 7;
constexpr uint8_t Restore_Game_Option_Idx = 8;
constexpr uint8_t Purchased_A_Idx = 9;
constexpr uint8_t Cast_Nothing_Idx = 10;
constexpr uint8_t You_Inflicted_Idx = 11;
constexpr uint8_t Points_Damage_Idx = 12;
constexpr uint8_t Plus_Extra_Idx = 13;
constexpr uint8_t You_Regained_Idx = 14;
constexpr uint8_t Health_Points_Idx = 15;
constexpr uint8_t Aztarok_Stole_Idx = 16;
constexpr uint8_t Hp_Points_Idx = 17;
constexpr uint8_t Attacks_Idx = 18;
constexpr uint8_t Damage_Points_Idx = 19;
constexpr uint8_t Gelly_Theft_Idx = 20;
constexpr uint8_t Rune_Idx = 21;
constexpr uint8_t From_You_Idx = 22;
constexpr uint8_t You_Win_Idx = 23;
constexpr uint8_t You_Scored_Idx = 24;
constexpr uint8_t Gold_Coins_Idx = 25;
constexpr uint8_t Skill_Points_Idx = 26;
constexpr uint8_t You_Lost_Idx = 27;
constexpr uint8_t GP_Idx = 28;
constexpr uint8_t SP_Idx = 29;
constexpr uint8_t You_Cast_Idx = 30;
constexpr uint8_t None_Idx = 31;
constexpr uint8_t Astarok_Name_Idx = 32;
constexpr uint8_t Tombstone_Inscription_00_Idx = 33;
constexpr uint8_t Tombstone_Inscription_01_Idx = 34;
constexpr uint8_t Tombstone_Inscription_02_Idx = 35;
constexpr uint8_t Dialogue_00_Idx = 36;
constexpr uint8_t Dialogue_01_Idx = 37;
constexpr uint8_t Dialogue_02_Idx = 38;
constexpr uint8_t Dialogue_03_Idx = 39;
constexpr uint8_t Dialogue_04_Idx = 40;
constexpr uint8_t Dialogue_05_Idx = 41;
constexpr uint8_t Dialogue_06_Idx = 42;
constexpr uint8_t Dialogue_07_Idx = 43;
constexpr uint8_t Dialogue_08_Idx = 44;
constexpr uint8_t Dialogue_09_Idx = 45;
constexpr uint8_t Dialogue_10_Idx = 46;
constexpr uint8_t Dialogue_11_Idx = 47;
constexpr uint8_t Dialogue_12_Idx = 48;
constexpr uint8_t Dialogue_13_Idx = 49;
constexpr uint8_t Dialogue_14_Idx = 50;
constexpr uint8_t Dialogue_15_Idx = 51;
constexpr uint8_t Dialogue_16_Idx = 52;
constexpr uint8_t PlayerItems_Caption00_Idx = 53;
constexpr uint8_t PlayerItems_Caption01_Idx = 54;
constexpr uint8_t PlayerItems_Caption02_Idx = 55;
constexpr uint8_t PlayerItems_Caption03_Idx = 56;
constexpr uint8_t PlayerItems_Caption04_Idx = 57;
constexpr uint8_t PlayerItems_Caption05_Idx = 58;
constexpr uint8_t RuneCombination_Purchased_Caption00_Idx = 59;
constexpr uint8_t RuneCombination_Purchased_Caption01_Idx = 60;
constexpr uint8_t RuneCombination_Purchased_Caption02_Idx = 61;
constexpr uint8_t RuneCombination_Purchased_Caption03_Idx = 62;
constexpr uint8_t Rune_Effect_Caption00_Idx = 63;
constexpr uint8_t Rune_Effect_Caption01_Idx = 64;
constexpr uint8_t Rune_Effect_Caption02_Idx = 65;
constexpr uint8_t Rune_Effect_Caption03_Idx = 66;
constexpr uint8_t RuneCombination_Standard_51_Idx = 67;
constexpr uint8_t RuneCombination_Standard_52_Idx = 68;
constexpr uint8_t RuneCombination_Standard_53_Idx = 69;
constexpr uint8_t RuneCombination_Standard_54_Idx = 70;
constexpr uint8_t RuneCombination_Standard_55_Idx = 71;
constexpr uint8_t Pickup_HP_Idx = 72;
constexpr uint8_t Thank_You_Idx = 73;
constexpr uint8_t Overwrite_Confirm_Idx = 74;
constexpr uint8_t New_Game_Confirm_Idx = 75;
constexpr uint8_t Game_Restored_Idx = 76;
constexpr uint8_t Same_Helmet_Idx = 77;
constexpr uint8_t Max_Potions_Idx = 78;
constexpr uint8_t Buy_Item_Idx = 79;
constexpr uint8_t No_GP_Idx = 80;
constexpr uint8_t Same_Rune_Idx = 81;
constexpr uint8_t Buy_Rune_Idx = 82;
constexpr uint8_t No_SP_Idx = 83;
constexpr uint8_t Monster_Attack_Idx = 84;
constexpr uint8_t Throw_Again_Idx = 85;
constexpr uint8_t Game_Saved_Idx = 86;

const uint8_t keywords[] PROGMEM = {
'e','l','m','e','t' + 0x80, // 129
'n','v','e','n','t','o','r','y' + 0x80, // 130
' ','o','f',' ','a',' ','K','i','n','d','!' + 0x80, // 131
'"','#','%',' ','Y','e','s' + 0x80, // 132
'"','$','%',' ','N','o' + 0x80, // 133
'1','8','8' + 0x80, // 134
'1','8','4' + 0x80, // 135
'H','e','a','l','t','h' + 0x80, // 136
'D','e','f','e','n','c','e' + 0x80, // 137
'G','a','m','e' + 0x80, // 138
'Y','o','u' + 0x80, // 139
'p','t','s' + 0x80, // 140
'd','a','m','a','g','e' + 0x80, // 141
'e','x','t','r','a' + 0x80, // 142
'p','o','i','n','t','s' + 0x80, // 143
'A','s','t','a','r','o','K' + 0x80, // 144
's','t','o','l','e' + 0x80, // 145
'a','t','t','a','c','k','s' + 0x80, // 146
'T','h','e' + 0x80, // 147
'r','u','n','e' + 0x80, // 148
'f','r','o','m' + 0x80, // 149
'y','o','u','!' + 0x80, // 150
'a','n','d' + 0x80, // 151
't','h','e' + 0x80, // 152
'g','o','l','d' + 0x80, // 153
's','k','i','l','l' + 0x80, // 154
'y','o','u','r' + 0x80, // 155
'c','a','s','t' + 0x80, // 156
'B','o','r','n' + 0x80, // 157
'D','e','c' + 0x80, // 158
't','h','i','s' + 0x80, // 159
'c','a','n' + 0x80, // 160
'y','o','u' + 0x80, // 161
'r','u','n','e','s' + 0x80, // 162
'f','o','r' + 0x80, // 163
's','o','m','e' + 0x80, // 164
'F','i','r','e' + 0x80, // 165
'P','o','t','i','o','n' + 0x80, // 166
'D','M','G' + 0x80, // 167
'K','i','n','d','!' + 0x80, // 168
'g','a','m','e' + 0x80, // 169
'Y','e','s' + 0x80, // 170
's','t','a','t','e' + 0x80, // 171
'a','l','r','e','a','d','y' + 0x80, // 172
'h','a','v','e' + 0x80, // 173
'o','n','e' + 0x80, // 174
't','h','o','s','e' + 0x80, // 175
'B','u','y' + 0x80, // 176
'n','o','t' + 0x80, // 177
'e','n','o','u','g','h' + 0x80, // 178
'k','i','l','l' + 0x80, // 179
'r','e','s','s' + 0x80, // 180
' ',' ',' ',' ',' ' + 0x80, // 181
' ',' ',' ' + 0x80, // 182
' ',' ' + 0x80, // 183
'e','d',' ' + 0x80, // 184
'i','n','g' + 0x80, // 185
't','o',' ' + 0x80, // 186
' ','t','o' + 0x80, // 187
' ','o','f',' ' + 0x80, // 188
'i','s','t' + 0x80, // 189
'l','l' + 0x80, // 190
'i','v','e' + 0x80, // 191
'i','t','e' + 0x80, // 192
'e','s','t' + 0x80, // 193
'o','r','e' + 0x80, // 194
'i','o','n' + 0x80, // 195
'a','v','e' + 0x80, // 196
'i','e','s' + 0x80, // 197
'i','c','s' + 0x80, // 198
'u','m','p' + 0x80, // 199
'e','a' + 0x80, // 200
'o','u' + 0x80, // 201
'i','n' + 0x80, // 202
'u','s' + 0x80, // 203
'r','e' + 0x80, // 204
's','e' + 0x80, // 205
'e','r' + 0x80, // 206
'a','r' + 0x80, // 207
' ','d','o',' ' + 0x80, // 208
'T','h' + 0x80, // 209
'a',' ' + 0x80, // 210
};


const uint8_t messages[] PROGMEM = {
' ','I',130, 0x80,
' ','x',' ', 0x80,
'S','t','a','t',189,198, 0x80,
136,' ',':','\n','S',179,' ',':','\n',137,' ',':','\n','G','o','l','d',' ',':','\n','D','u','n','g','e','o','n','s',' ',':', 0x80,
182,' ','R','u','n','e','s', 0x80,
' ','E','n','e','m','y',' ','S','t','s', 0x80,
'N','a','m','e',':','\n',136,' ',':','\n',137,' ',':','\n', 0x80,
'S',196,' ',138,'\n','N','e','w',' ',138,'\n', 0x80,
'R',193,194,' ',138, 0x80,
210, 0x80,
177,'h',185,'!', 0x80,
139,' ',202,'f','l','i','c','t',184, 0x80,
' ',140,188,141,'.', 0x80,
'P','l',203,' ','a','n',' ',142,' ', 0x80,
139,' ',204,'g','a',202,184, 0x80,
' ','h',200,'l','t','h',' ',143,'.', 0x80,
183,144,' ',145,' ', 0x80,
' ','h','p',' ',143,'.', 0x80,
' ',146,',',' ', 0x80,
' ',141,' ',140,'.', 0x80,
147,' ','G','e','l','a','t',202,201,'s',' ','C','u','b','e',' ',145,' ','a','\n',182, 0x80,
' ',148, 0x80,
' ',149,' ',150, 0x80,
' ',139,' ',179,184,144,' ',151,' ','f',204,'e','d','\n',152,' ','v','i',190,'a','g',206,'s',' ',149,' ',152,' ','c','u','r',205,'!', 0x80,
139,' ','s','c','o','r',184, 0x80,
' ',153,' ','c','o',202,'s','.', 0x80,
' ',154,' ',143,'.', 0x80,
139,' ','d','i','e','d','!',' ','L','o',205,' ','h','a','l','f',188,155,'\n',181,' ',153,' ',151,' ',154,' ',143,'!', 0x80,
'G','P', 0x80,
'S','P', 0x80,
139,' ',156,' ', 0x80,
'N',174,' ','!', 0x80,
'A','S','T','A','R','O','K', 0x80,
182,'H','e',204,' ','l',197,' ','V','a','m','p','i','r',198,'.','\n',183,157,' ',135,'3',' ','-',' ',158,' ',134,'2', 0x80,
' ','R',193,' ','I','n',' ','P',200,'c','e',' ','F','i','l','m','o','t','e','.','\n',183,157,' ',135,'1',' ','-',' ',158,' ',134,'9', 0x80,
182,'P',180,' ','P','l','a','y',' ','O','n',' ','T','a','p','e','\n',181,'D','e','v','e','l','o','p',184,'2','0','1','9', 0x80,
'W','e','l','c','o','m','e',' ','t','r',196,190,206,' ',186,152,187,'w','n', 0x80,
'o','f',' ','G','r','i',163,'d','.',' ',147,' ','e','v','i','l',' ',144, 0x80,
'h','a','s',' ','t','u','r','n',184,203,' ','a',190,' ',202,186,'p',199,'k',202, 0x80,
'p','e','o','p','l','e','.',' ','P','l',200,205,' ','h','e','l','p',' ',203,' ','d','e','f',200,'t', 0x80,
159,' ','c','r','a','z','y',' ','w','i','z',207,'d','!', 0x80,
'B','e',' ','s','u',204,' ',186,'e','q','u','i','p',' ',155,205,'l','f', 0x80,
'p','r','o','p',206,'l','y','.',' ',139,' ',160,' ','p',180,' ','"','$','%',187, 0x80,
'a','c','c','e','s','s',' ',155,' ','i',130,' ',151,187, 0x80,
'v','i','e','w',' ',202,163,'m','a','t',195,' ','a','b',201,'t',' ',161,' ','o','r', 0x80,
155,' ','e','n','e','m',197,'.', 0x80,
'I','n',' ','c','o','m','b','a','t',',',' ',156,' ',148,'s',' ',186,'s','c',194, 0x80,
'\'','p','o','k',206,'\'',' ','h',151,'s','.',' ','P',180,185,' ','"','#','%',' ','w','i',190, 0x80,
156,' ',152,' ',148,'s',' ','-',' ','j',199,' ',151, 0x80,
't',201,'c','h',' ',148,'s',' ',186,205,'l','e','c','t',' ',152,'m', 0x80,
163,' ',204,156,185,'.',' ','N','o','r','m','a',190,'y',' ',161, 0x80,
160,' ',156,' ','3',' ','t','i','m','e','s',',',' ','b','u','t',' ',164, 0x80,
192,'m','s',' ','m','i','g','h','t',' ','g',191,' ',161,' ',142,'.', 0x80,
'C','r','y','s','t','a','l',' ','H',129, 0x80,
'H','i','g','h',' ','H',129, 0x80,
'R',201,'n','d',' ','H',129, 0x80,
165,' ',166, 0x80,
136,' ',166, 0x80,
'S','p','e',184,166, 0x80,
165,' ','B','l','a','z','e', 0x80,
'H',200,'l',185,' ','W',202,'d', 0x80,
'R','i','s',185,' ','S','t',207, 0x80,
'V','e','n','o','m',' ','M',189, 0x80,
'5',' ',165,' ',167, 0x80,
'H',200,'l',' ','2','0',' ','H','P', 0x80,
'3','5',' ','D','a','m','a','g','e', 0x80,
'2',' ','P','.',' ',167, 0x80,
210,'F',191,131, 0x80,
210,'F',201,'r',131, 0x80,
210,209,204,'e',131, 0x80,
'T','w','o',' ','P','a','i','r','s','!', 0x80,
210,'F','u',190,' ','H',201,205,'!', 0x80,
139,' ','p','i','c','k',184,'u','p',' ',164,' ','H','P','!', 0x80,
182,' ',209,'a','n','k',' ',161,' ',163,' ','s','a','v',185,'\n',181,' ',203,' ',149,' ',152,' ','C','u','r',205,'!', 0x80,
' ','O','v',206,'w','r',192,' ','e','x',189,185,' ',169,'?','\n',181,181,132,182,' ',133, 0x80,
183,'S','t',207,'t',' ',210,'n','e','w',' ',169,'?','\n',181,132,182,' ',133, 0x80,
183,138,' ',171,' ','r',193,194,'d','!', 0x80,
182,' ',139,' ',172,' ',173,'\n',' ',174,188,175,' ','h',129,'s','!', 0x80,
183,139,' ',172,' ',173,' ','a','s','\n','m','a','n','y',188,152,205,' ','p','o','t',195,'s','\n',183,'a','s',' ',161,' ',160,' ','c',207,'r','y',' ','!', 0x80,
182,176,' ',159,' ',192,'m','?','\n',182,132,182,' ',133, 0x80,
139,208,177,' ',173,'\n',182,178,' ',153,'!', 0x80,
182,139,' ','a',204,' ','c',207,'r','y',185,'\n',' ',174,188,175,' ',148,'s','!', 0x80,
183,176,' ',159,' ',148,'?','\n',183,132,182,' ',133, 0x80,
' ',139,208,177,' ',173,'\n',182,' ',178,' ','S','P','!', 0x80,
181,182,'A',' ','m','o','n','s','t',206,' ',146,' ',150, 0x80,
209,'r','o','w',' ',152,' ',148,'s',' ','a','g','a',202,'!', 0x80,
183,138,' ',171,' ','s',196,'d','!', 0x80,
};
