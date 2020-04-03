#pragma once

#include "Arduboy2Ext.h"
#include "../images/images.h"
#include "../../Enums.h"

const int8_t PROGMEM enemy_head[] = {
13, -40, 1, 1,        // STANCE_DEFAULT
13, -40, 1, 1,        // STANCE_DEFAULT_LEAN_BACK
24, -40, 1, 1,        // STANCE_KICK_READY
13, -40, 1, 1,        // STANCE_DEFAULT_LEAN_FORWARD
22, -40, 1, 1,        // STANCE_KICK_STANDING_TRANSITION
25, -38, 1, 1,        // STANCE_KICK_MED_END
24, -40, 1, 1,        // STANCE_KICK_LOW_END
30, -36, 1, 1,        // STANCE_KICK_HIGH_END
11, -40, 1, 1,        // STANCE_PUNCH_MED_RH_END
12, -40, 1, 1,        // STANCE_PUNCH_MED_LH_END
12, -40, 1, 1,        // STANCE_PUNCH_HIGH_RH_END
11, -40, 1, 1,        // STANCE_PUNCH_HIGH_LH_END
11, -40, 1, 1,        // STANCE_PUNCH_LOW_RH_END
11, -40, 1, 1,        // STANCE_PUNCH_LOW_LH_END
14, -40, 1, 1,        // STANCE_PUNCH_READY
25, -40, 1, 1,        // STANCE_STANDING_UPRIGHT
13, -40, 1, 1,        // STANCE_SIDLING_1
13, -40, 1, 1,        // STANCE_SIDLING_2
13, -40, 1, 1,        // STANCE_SIDLING_3
13, -40, 1, 1,        // STANCE_SIDLING_4
13, -40, 1, 1,        // STANCE_SIDLING_5
13, -40, 1, 1,        // STANCE_SIDLING_6
13, -40, 1, 1,        // STANCE_SIDLING_7
15, -40, 1, 1,        // STANCE_RUNNING_1
12, -40, 1, 1,        // STANCE_RUNNING_2
12, -40, 1, 1,        // STANCE_RUNNING_3
12, -40, 1, 1,        // STANCE_RUNNING_4
12, -40, 1, 1,        // STANCE_RUNNING_5
12, -40, 1, 1,        // STANCE_RUNNING_6
12, -40, 1, 1,        // STANCE_RUNNING_7
12, -40, 1, 1,        // STANCE_RUNNING_8
21, -40, 1, 1,        // STANCE_RUNNING_STRAIGHTEN_UP
15, -40, 1, 0,        // STANCE_STANDING_UPRIGHT_REV
11, -40, 1, 0,        // STANCE_RUNNING_1_REV
15, -40, 1, 0,        // STANCE_RUNNING_2_REV
15, -40, 1, 0,        // STANCE_RUNNING_3_REV
15, -40, 1, 0,        // STANCE_RUNNING_4_REV
15, -40, 1, 0,        // STANCE_RUNNING_5_REV
15, -40, 1, 0,        // STANCE_RUNNING_6_REV
15, -40, 1, 0,        // STANCE_RUNNING_7_REV
15, -40, 1, 0,        // STANCE_RUNNING_8_REV
15, -40, 1, 0,        // STANCE_RUNNING_STRAIGHTEN_UP_REV
20, -38, 1, 0,        // STANCE_BOW_1
20, -36, 1, 0,        // STANCE_BOW_2
21, -35, 1, 1,        // STANCE_DEATH_1
25, -25, 1, 1,        // STANCE_DEATH_2
25, -22, 1, 1,        // STANCE_DEATH_3   
31, -18, 1, 1,        // STANCE_DEATH_4  
/*
0, 0, 127, 1,         // STANCE_DEATH_5  
0, 0, 127, 1,         // STANCE_DEATH_6  
0, 0, 127, 0,         // STANCE_ENTRANCE_1
0, 0, 127, 0,         // STANCE_ENTRANCE_2
0, 0, 127, 0,         // STANCE_ENTRANCE_3
0, 0, 127, 0,         // STANCE_ENTRANCE_4
0, 0, 127, 0,         // STANCE_ENTRANCE_5
0, 0, 127, 0,         // STANCE_ENTRANCE_6
0, 0, 127, 1,         // STANCE_EAGLE_1  
0, 0, 127, 1,         // STANCE_EAGLE_2  
0, 0, 127, 1,         // STANCE_EAGLE_3  
0, 0, 127, 1,         // STANCE_EAGLE_4  
0, 0, 127, 1,         // STANCE_EAGLE_1_REV  
0, 0, 127, 1,         // STANCE_EAGLE_2_REV  
0, 0, 127, 1,         // STANCE_EAGLE_3_REV  
0, 0, 127, 1,         // STANCE_EAGLE_4_REV  
0, 0, 127, 1,         // STANCE_PRINCESS_SITTING  
0, 0, 127, 1,         // STANCE_PRINCESS_STANDING  
0, 0, 127, 1,         // STANCE_PRINCESS_KISSING  
*/
};

const int8_t PROGMEM enemy_body[] = {
2, -31, 0, 1,         // STANCE_DEFAULT
2, -31, 0, 1,         // STANCE_DEFAULT_LEAN_BACK
11, -31, 1, 1,        // STANCE_KICK_READY
2, -31, 0, 1,         // STANCE_DEFAULT_LEAN_FORWARD
8, -32, 4, 1,         // STANCE_KICK_STANDING_TRANSITION
8, -31, 2, 1,         // STANCE_KICK_MED_END
9, -32, 3, 1,         // STANCE_KICK_LOW_END
19, -42, 5, 1,        // STANCE_KICK_HIGH_END
-8, -33, 6, 1,        // STANCE_PUNCH_MED_RH_END
-5, -33, 10, 1,       // STANCE_PUNCH_MED_LH_END
-8, -35, 7, 1,        // STANCE_PUNCH_HIGH_RH_END
-6, -37, 11, 1,       // STANCE_PUNCH_HIGH_LH_END
-4, -33, 8, 1,        // STANCE_PUNCH_LOW_RH_END
-5, -33, 9, 1,        // STANCE_PUNCH_LOW_LH_END
11, -32, 12, 1,       // STANCE_PUNCH_READY
25, -31, 13, 1,       // STANCE_STANDING_UPRIGHT
2, -31, 0, 1,         // STANCE_SIDLING_1
2, -31, 0, 1,         // STANCE_SIDLING_2
2, -31, 0, 1,         // STANCE_SIDLING_3
2, -31, 0, 1,         // STANCE_SIDLING_4
2, -31, 0, 1,         // STANCE_SIDLING_5
2, -31, 0, 1,         // STANCE_SIDLING_6
2, -31, 0, 1,         // STANCE_SIDLING_7
15, -32, 14, 1,       // STANCE_RUNNING_1
10, -32, 15, 1,       // STANCE_RUNNING_2
10, -32, 15, 1,       // STANCE_RUNNING_3
11, -32, 16, 1,       // STANCE_RUNNING_4
6, -32, 17, 1,        // STANCE_RUNNING_5
4, -32, 18, 1,        // STANCE_RUNNING_6
8, -32, 20, 1,        // STANCE_RUNNING_7
4, -32, 18, 1,        // STANCE_RUNNING_8
20, -32, 19, 1,       // STANCE_RUNNING_STRAIGHTEN_UP
13, -31, 13, 0,       // STANCE_STANDING_UPRIGHT_REV
5, -32, 14, 0,        // STANCE_RUNNING_1_REV
8, -32, 15, 0,        // STANCE_RUNNING_2_REV
8, -32, 15, 0,        // STANCE_RUNNING_3_REV
12, -32, 16, 0,       // STANCE_RUNNING_4_REV    
12, -32, 17, 0,       // STANCE_RUNNING_5_REV 
6, -32, 18, 0,        // STANCE_RUNNING_6_REV 
12, -32, 20, 0,       // STANCE_RUNNING_7_REV
6, -32, 18, 0,        // STANCE_RUNNING_8_REV 
10, -32, 19, 0,       // STANCE_RUNNING_STRAIGHTEN_UP_REV
13, -30, 21, 0,       // STANCE_BOW_1 
13, -28, 22, 0,       // STANCE_BOW_2
4, -27, 23, 1,        // STANCE_DEATH_1
9, -19, 24, 1,        // STANCE_DEATH_2
/*
0, 0, 127, 1,         // STANCE_DEATH_3   
0, 0, 127, 1,         // STANCE_DEATH_4   
0, 0, 127, 1,         // STANCE_DEATH_5   
0, 0, 127, 1,         // STANCE_DEATH_6   
0, 0, 127, 0,         // STANCE_ENTRANCE_1
0, 0, 127, 0,         // STANCE_ENTRANCE_2
0, 0, 127, 0,         // STANCE_ENTRANCE_3
0, 0, 127, 0,         // STANCE_ENTRANCE_4
0, 0, 127, 0,         // STANCE_ENTRANCE_5
0, 0, 127, 0,         // STANCE_ENTRANCE_6
0, 0, 127, 1,         // STANCE_EAGLE_1   
0, 0, 127, 1,         // STANCE_EAGLE_2   
0, 0, 127, 1,         // STANCE_EAGLE_3   
0, 0, 127, 1,         // STANCE_EAGLE_4   
0, 0, 127, 1,         // STANCE_EAGLE_1_REV   
0, 0, 127, 1,         // STANCE_EAGLE_2_REV   
0, 0, 127, 0,         // STANCE_EAGLE_3_REV  
0, 0, 127, 0,         // STANCE_EAGLE_4_REV  
0, 0, 127, 1,         // STANCE_PRINCESS_SITTING  
0, 0, 127, 1,         // STANCE_PRINCESS_STANDING  
0, 0, 127, 1,         // STANCE_PRINCESS_KISSING  
*/
};

const int8_t PROGMEM enemy_legs[] = {
0, -18, 0, 1,         // STANCE_DEFAULT
-1, -18, 1, 1,        // STANCE_DEFAULT_LEAN_BACK
9, -22, 2, 1,         // STANCE_KICK_READY
6, -18, 1, 0,         // STANCE_DEFAULT_LEAN_FORWARD
8, -19, 7, 1,         // STANCE_KICK_STANDING_TRANSITION
-5, -25, 3, 1,        // STANCE_KICK_MED_END
-9, -24, 6, 1,        // STANCE_KICK_LOW_END
-6, -35, 8, 1,        // STANCE_KICK_HIGH_END
-2, -18, 9, 1,        // STANCE_PUNCH_MED_RH_END
-1, -18, 9, 1,        // STANCE_PUNCH_MED_LH_END
-1, -18, 9, 1,        // STANCE_PUNCH_HIGH_RH_END
-1, -18, 9, 1,        // STANCE_PUNCH_HIGH_LH_END
-1, -18, 9, 1,        // STANCE_PUNCH_LOW_RH_END
-1, -18, 9, 1,        // STANCE_PUNCH_LOW_LH_END
3, -19, 10, 1,        // STANCE_PUNCH_READY
24, -15, 11, 1,       // STANCE_STANDING_UPRIGHT
9, -18, 5, 1,         // STANCE_SIDLING_1
9, -18, 12, 1,        // STANCE_SIDLING_2
7, -18, 5, 0,         // STANCE_SIDLING_3
-2, -18, 22, 1,       // STANCE_SIDLING_4
-2, -18, 23, 1,       // STANCE_SIDLING_5
7, -18, 24, 1,        // STANCE_SIDLING_6
-2, -18, 22, 1,       // STANCE_SIDLING_7
14, -8, 13, 1,        // STANCE_RUNNING_1
6, -20, 14, 1,        // STANCE_RUNNING_2
5, -20, 15, 1,        // STANCE_RUNNING_3   
9, -20, 16, 1,        // STANCE_RUNNING_4 
9, -20, 17, 1,        // STANCE_RUNNING_5
6, -20, 18, 1,        // STANCE_RUNNING_6
9, -19, 21, 1,        // STANCE_RUNNING_7
5, -20, 20, 1,        // STANCE_RUNNING_8
15, -20, 19, 1,       // STANCE_RUNNING_STRAIGHTEN_UP
13, -15, 11, 0,       // STANCE_STANDING_UPRIGHT_REV
-1, -8, 13, 0,        // STANCE_RUNNING_1_REV
-1, -20, 14, 0,       // STANCE_RUNNING_2_REV
-4, -20, 15, 0,       // STANCE_RUNNING_3_REV
-0, -20, 16, 0,       // STANCE_RUNNING_4_REV
8, -20, 17, 0,        // STANCE_RUNNING_5_REV 
-5, -20, 18, 0,       // STANCE_RUNNING_6_REV    
8, -19, 21, 0,        // STANCE_RUNNING_7_REV
0, -20, 20, 0,        // STANCE_RUNNING_8_REV 
9, -20, 19, 0,        // STANCE_RUNNING_STRAIGHTEN_UP_REV
12, -19, 25, 0,       // STANCE_BOW_1
12, -19, 26, 0,       // STANCE_BOW_2
3, -13, 27, 1,        // STANCE_DEATH_1    
2, -11, 28, 1,        // STANCE_DEATH_2
3, -17, 29, 1,        // STANCE_DEATH_3  
-1, -13, 30, 1,       // STANCE_DEATH_4  
-3, -10, 31, 1,       // STANCE_DEATH_5  
-2, -11, 32, 1,       // STANCE_DEATH_6  
0, 0, 127, 0,         // STANCE_ENTRANCE_1
0, 0, 127, 0,         // STANCE_ENTRANCE_2
0, 0, 127, 0,         // STANCE_ENTRANCE_3
0, 0, 127, 0,         // STANCE_ENTRANCE_4
0, 0, 127, 0,         // STANCE_ENTRANCE_5
0, 0, 127, 0,         // STANCE_ENTRANCE_6
-2, -2, 39, 0,        // STANCE_EAGLE_1  
-2, -11, 40, 0,       // STANCE_EAGLE_2  
-2, -11, 41, 0,       // STANCE_EAGLE_3  
-2, -11, 42, 0,       // STANCE_EAGLE_4  
-2, -2, 39, 1,        // STANCE_EAGLE_1_REV  
-2, -11, 40, 1,       // STANCE_EAGLE_2_REV  
0, 0, 41, 0,          // STANCE_EAGLE_3_REV
0, 0, 42, 0,          // STANCE_EAGLE_4_REV
0, 0, 43, 0,          // STANCE_PRINCESS_SITTING  
0, 0, 44, 0,          // STANCE_PRINCESS_STANDING  
0, 0, 45, 0,          // STANCE_PRINCESS_KISSING  

};    

void renderEnemyStance(Arduboy2Ext &arduboy, int8_t x, int8_t y, uint8_t stance) {
  
  RenderDetails head = { 0, 0, 127, false };
  RenderDetails body = { 0, 0, 127, false };
  RenderDetails leg = { 0, 0, 127, false };

  if (stance <= STANCE_DEATH_4) {

    head.xOffset = pgm_read_byte(&enemy_head[ (stance * 4) ]);
    head.yOffset = pgm_read_byte(&enemy_head[ (stance * 4) + 1]);
    head.index = pgm_read_byte(&enemy_head[ (stance * 4)  + 2]);
    head.rev = (pgm_read_byte(&enemy_head[ (stance * 4) ] + 3) == 1);

  }

  if (stance <= STANCE_DEATH_2) {

    body.xOffset = pgm_read_byte(&enemy_body[ (stance * 4) ]);
    body.yOffset = pgm_read_byte(&enemy_body[ (stance * 4) + 1]);
    body.index = pgm_read_byte(&enemy_body[ (stance * 4)  + 2]);
    body.rev = (pgm_read_byte(&enemy_body[ (stance * 4) ] + 3) == 1);

  }

  leg.xOffset = pgm_read_byte(&enemy_legs[ (stance * 4) ]);
  leg.yOffset = pgm_read_byte(&enemy_legs[ (stance * 4) + 1]);
  leg.index = pgm_read_byte(&enemy_legs[ (stance * 4)  + 2]);
  leg.rev = (pgm_read_byte(&enemy_legs[ (stance * 4) ] + 3) == 1);  

  if (leg.index != 127) arduboy.drawCompressedMirror(x + leg.xOffset, y + leg.yOffset, masks_leg[leg.index], BLACK, leg.rev);
  if (leg.index != 127) arduboy.drawCompressedMirror(x + leg.xOffset, y + leg.yOffset, imgs_leg[leg.index], WHITE, leg.rev);
  if (body.index != 127) arduboy.drawCompressedMirror(x + body.xOffset, y + body.yOffset, imgs_body[body.index], WHITE, body.rev);
  if (head.index != 127) arduboy.drawCompressedMirror(x + head.xOffset, y + head.yOffset, imgs_head[head.index], WHITE, head.rev);

}
