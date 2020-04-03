#pragma once

#include "Arduboy2Ext.h"
#include "../images/images.h"
#include "../../Enums.h"

const int8_t PROGMEM player_head[] = {
14, -39, 0, 0,        // STANCE_DEFAULT
14, -39, 0, 0,        // STANCE_DEFAULT_LEAN_BACK
3, -39, 0, 0,         // STANCE_KICK_READY
14, -39, 0, 0,        // STANCE_DEFAULT_LEAN_FORWARD
5, -39, 0, 0,         // STANCE_KICK_STANDING_TRANSITION
2, -37, 0, 0,         // STANCE_KICK_MED_END
3, -39, 0, 0,         // STANCE_KICK_LOW_END
-3, -35, 0, 0,        // STANCE_KICK_HIGH_END
16, -39, 0, 0,        // STANCE_PUNCH_MED_RH_END
15, -39, 0, 0,        // STANCE_PUNCH_MED_LH_END
15, -39, 0, 0,        // STANCE_PUNCH_HIGH_RH_END
16, -39, 0, 0,        // STANCE_PUNCH_HIGH_LH_END
16, -39, 0, 0,        // STANCE_PUNCH_LOW_RH_END
16, -39, 0, 0,        // STANCE_PUNCH_LOW_LH_END
12, -39, 0, 0,        // STANCE_PUNCH_READY
2, -39, 0, 0,         // STANCE_STANDING_UPRIGHT
14, -39, 0, 0,        // STANCE_SIDLING_1
14, -39, 0, 0,        // STANCE_SIDLING_2
14, -39, 0, 0,        // STANCE_SIDLING_3
14, -39, 0, 0,        // STANCE_SIDLING_4
14, -39, 0, 0,        // STANCE_SIDLING_5
14, -39, 0, 0,        // STANCE_SIDLING_6
14, -39, 0, 0,        // STANCE_SIDLING_7
11, -39, 0, 0,        // STANCE_RUNNING_1
15, -39, 0, 0,        // STANCE_RUNNING_2
15, -39, 0, 0,        // STANCE_RUNNING_3
15, -39, 0, 0,        // STANCE_RUNNING_4
15, -39, 0, 0,        // STANCE_RUNNING_5 
15, -39, 0, 0,        // STANCE_RUNNING_6
15, -39, 0, 0,        // STANCE_RUNNING_7
15, -39, 0, 0,        // STANCE_RUNNING_8
5, -39, 0, 0,         // STANCE_RUNNING_STRAIGHTEN_UP
0, 0, 127, 0,         // STANCE_STANDING_UPRIGHT_REV               
0, 0, 127, 0,         // STANCE_RUNNING_1_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_2_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_3_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_4_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_5_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_6_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_7_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_8_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_STRAIGHTEN_UP_REV          
7, -37, 0, 0,         // STANCE_BOW_1
7, -35, 0, 0,         // STANCE_BOW_2   
6, -34, 0, 0,         // STANCE_DEATH_1
2, -24, 0, 0,         // STANCE_DEATH_2
2, -21, 0, 0,         // STANCE_DEATH_3  
-4, -17, 0, 0,        // STANCE_DEATH_4  
0, 0, 127, 0,         // STANCE_DEATH_5  
0, 0, 127, 0,         // STANCE_DEATH_6  
-1, -21, 0, 0,        // STANCE_ENTRANCE_1
7, -21, 0, 0,         // STANCE_ENTRANCE_2
7, -22, 0, 0,         // STANCE_ENTRANCE_3
0, 0, 127, 0,         // STANCE_ENTRANCE_4
0, 0, 127, 0,         // STANCE_ENTRANCE_5   
7, -36, 0, 0,         // STANCE_ENTRANCE_6
/*
0, 0, 127, 0,         // STANCE_EAGLE_1
0, 0, 127, 0,         // STANCE_EAGLE_2
0, 0, 127, 0,         // STANCE_EAGLE_3
0, 0, 127, 0,         // STANCE_EAGLE_4
0, 0, 127, 0,         // STANCE_EAGLE_1_REV
0, 0, 127, 0,         // STANCE_EAGLE_2_REV
0, 0, 127, 0,         // STANCE_EAGLE_3_REV
0, 0, 127, 0,         // STANCE_EAGLE_4_REV
*/
};
    
const int8_t PROGMEM player_body[] = {
9, -31, 0, 0,         // STANCE_DEFAULT
9, -31, 0, 0,         // STANCE_DEFAULT_LEAN_BACK
0, -31, 1, 0,         // STANCE_KICK_READY
9, -31, 0, 0,         // STANCE_DEFAULT_LEAN_FORWARD
0, -32, 4, 0,         // STANCE_KICK_STANDING_TRANSITION
0, -31, 2, 0,         // STANCE_KICK_MED_END
0, -32, 3, 0,         // STANCE_KICK_LOW_END
-9, -42, 5, 0,        // STANCE_KICK_HIGH_END
7, -33, 6, 0,         // STANCE_PUNCH_MED_RH_END
9, -33, 10, 0,        // STANCE_PUNCH_MED_LH_END
9, -35, 7, 0,         // STANCE_PUNCH_HIGH_RH_END
7, -37, 11, 0,        // STANCE_PUNCH_HIGH_LH_END
7, -33, 8, 0,         // STANCE_PUNCH_LOW_RH_END
9, -33, 9, 0,         // STANCE_PUNCH_LOW_LH_END
10, -32, 12, 0,       // STANCE_PUNCH_READY
0, -31, 13, 0,        // STANCE_STANDING_UPRIGHT
9, -31, 0, 0,         // STANCE_SIDLING_1
9, -31, 0, 0,         // STANCE_SIDLING_2
9, -31, 0, 0,         // STANCE_SIDLING_3
9, -31, 0, 0,         // STANCE_SIDLING_4
9, -31, 0, 0,         // STANCE_SIDLING_5
9, -31, 0, 0,         // STANCE_SIDLING_6
9, -31, 0, 0,         // STANCE_SIDLING_7
5, -32, 14, 0,        // STANCE_RUNNING_1
8, -32, 15, 0,        // STANCE_RUNNING_2
8, -32, 15, 0,        // STANCE_RUNNING_3
12, -32, 16, 0,       // STANCE_RUNNING_4
12, -32, 17, 0,       // STANCE_RUNNING_5
6, -32, 18, 0,        // STANCE_RUNNING_6
12, -32, 20, 0,       // STANCE_RUNNING_7
6, -32, 18, 0,        // STANCE_RUNNING_8
0, -32, 19, 0,        // STANCE_RUNNING_STRAIGHTEN_UP 
0, 0, 127, 0,         // STANCE_STANDING_UPRIGHT_REV               
0, 0, 127, 0,         // STANCE_RUNNING_1_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_2_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_3_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_4_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_5_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_6_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_7_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_8_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_STRAIGHTEN_UP_REV       
0, -30, 21, 0,        // STANCE_BOW_1
0, -28, 22, 0,        // STANCE_BOW_2
2, -27, 23, 0,        // STANCE_DEATH_1
-2, -19, 24, 0,       // STANCE_DEATH_2
/*
0, 0, 127, 0,         // STANCE_DEATH_3  
0, 0, 127, 0,         // STANCE_DEATH_4  
0, 0, 127, 0,         // STANCE_DEATH_5  
0, 0, 127, 0,         // STANCE_DEATH_6  
0, 0, 127, 0,         // STANCE_ENTRANCE_1
0, 0, 127, 0,         // STANCE_ENTRANCE_2
0, 0, 127, 0,         // STANCE_ENTRANCE_3
0, 0, 127, 0,         // STANCE_ENTRANCE_4   
0, 0, 127, 0,         // STANCE_ENTRANCE_5   
0, 0, 127, 0,         // STANCE_ENTRANCE_6   
0, 0, 127, 0,         // STANCE_EAGLE_1
0, 0, 127, 0,         // STANCE_EAGLE_2
0, 0, 127, 0,         // STANCE_EAGLE_3
0, 0, 127, 0,         // STANCE_EAGLE_4
0, 0, 127, 0,         // STANCE_EAGLE_1_REV
0, 0, 127, 0,         // STANCE_EAGLE_2_REV
0, 0, 127, 0,         // STANCE_EAGLE_3_REV
0, 0, 127, 0,         // STANCE_EAGLE_4_REV
*/
};

const int8_t PROGMEM player_legs[] = {
-1, -18, 0, 0,        // STANCE_DEFAULT
5, -18, 1, 0,         // STANCE_DEFAULT_LEAN_BACK
7, -22, 2, 0,         // STANCE_KICK_READY
-2, -18, 1, 1,        // STANCE_DEFAULT_LEAN_FORWARD
1, -19, 7, 0,         // STANCE_KICK_STANDING_TRANSITION
10, -25, 3, 0,        // STANCE_KICK_MED_END
9, -24, 6, 0,         // STANCE_KICK_LOW_END
10, -35, 8, 0,        // STANCE_KICK_HIGH_END
-1, -18, 9, 0,        // STANCE_PUNCH_MED_RH_END
-1, -18, 9, 0,        // STANCE_PUNCH_MED_LH_END
-1, -18, 9, 0,        // STANCE_PUNCH_HIGH_RH_END
-1, -18, 9, 0,        // STANCE_PUNCH_HIGH_LH_END
-1, -18, 9, 0,        // STANCE_PUNCH_LOW_RH_END
-1, -18, 9, 0,        // STANCE_PUNCH_LOW_LH_END
0, -19, 10, 0,        // STANCE_PUNCH_READY
0, -15, 11, 0,        // STANCE_STANDING_UPRIGHT
7, -18, 5, 0,         // STANCE_SIDLING_1
8, -18, 12, 0,        // STANCE_SIDLING_2
9, -18, 5, 1,         // STANCE_SIDLING_3
1, -18, 22, 0,        // STANCE_SIDLING_4
5, -18, 23, 0,        // STANCE_SIDLING_5
5, -18, 24, 0,        // STANCE_SIDLING_6
-3, -18, 22, 1,       // STANCE_SIDLING_7
-1, -8, 13, 0,        // STANCE_RUNNING_1
-1, -20, 14, 0,       // STANCE_RUNNING_2
-4, -20, 15, 0,       // STANCE_RUNNING_3    
0, -20, 16, 0,        // STANCE_RUNNING_4
8, -20, 17, 0,        // STANCE_RUNNING_5  
-5, -20, 18, 0,       // STANCE_RUNNING_6
8, -19, 21, 0,        // STANCE_RUNNING_7
0, -20, 20, 0,        // STANCE_RUNNING_8   
-1, -20, 19, 0,       // STANCE_RUNNING_STRAIGHTEN_UP
0, 0, 127, 0,         // STANCE_STANDING_UPRIGHT_REV               
0, 0, 127, 0,         // STANCE_RUNNING_1_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_2_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_3_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_4_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_5_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_6_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_7_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_8_REV                      
0, 0, 127, 0,         // STANCE_RUNNING_STRAIGHTEN_UP_REV       
-1, -19, 25, 0,       // STANCE_BOW_1
-1, -19, 26, 0,       // STANCE_BOW_2  
-1, -13, 27, 0,       // STANCE_DEATH_1
-1, -11, 28, 0,       // STANCE_DEATH_2
-1, -17, 29, 0,       // STANCE_DEATH_3  
-5, -13, 30, 0,       // STANCE_DEATH_4  
-8, -10, 31, 0,       // STANCE_DEATH_5  
-8, -11, 32, 0,       // STANCE_DEATH_6  
-12, -15, 33, 0,      // STANCE_ENTRANCE_1
-10, -16, 34, 0,      // STANCE_ENTRANCE_2
-10, -19, 35, 0,      // STANCE_ENTRANCE_3
-10, -24, 36, 0,      // STANCE_ENTRANCE_4
-10, -30, 37, 0,      // STANCE_ENTRANCE_5
-10, -30, 38, 0,      // STANCE_ENTRANCE_6
/*
0, 0, 127, 0,         // STANCE_EAGLE_1
0, 0, 127, 0,         // STANCE_EAGLE_2
0, 0, 127, 0,         // STANCE_EAGLE_3
0, 0, 127, 0,         // STANCE_EAGLE_4
0, 0, 127, 0,         // STANCE_EAGLE_1_REV
0, 0, 127, 0,         // STANCE_EAGLE_2_REV
0, 0, 127, 0,         // STANCE_EAGLE_3_REV
0, 0, 127, 0,         // STANCE_EAGLE_4_REV
*/
};
       

void renderPlayerStance(Arduboy2Ext &arduboy, int8_t x, int8_t y, uint8_t stance) {

  RenderDetails head = { 0, 0, 127, false };
  RenderDetails body = { 0, 0, 127, false };
  RenderDetails leg = { 0, 0, 127, false };
  
  if (stance <= STANCE_ENTRANCE_6) {

    head.xOffset = pgm_read_byte(&player_head[ (stance * 4) ]);
    head.yOffset = pgm_read_byte(&player_head[ (stance * 4) + 1]);
    head.index = pgm_read_byte(&player_head[ (stance * 4)  + 2]);
    head.rev = (pgm_read_byte(&player_head[ (stance * 4) ] + 3) == 1);

  }

  if (stance <= STANCE_DEATH_2) {

    body.xOffset = pgm_read_byte(&player_body[ (stance * 4) ]);
    body.yOffset = pgm_read_byte(&player_body[ (stance * 4) + 1]);
    body.index = pgm_read_byte(&player_body[ (stance * 4)  + 2]);
    body.rev = (pgm_read_byte(&player_body[ (stance * 4) ] + 3) == 1);

  }

  leg.xOffset = pgm_read_byte(&player_legs[ (stance * 4) ]);
  leg.yOffset = pgm_read_byte(&player_legs[ (stance * 4) + 1]);
  leg.index = pgm_read_byte(&player_legs[ (stance * 4)  + 2]);
  leg.rev = (pgm_read_byte(&player_legs[ (stance * 4) ] + 3) == 1);  
  
  if (leg.index != 127) arduboy.drawCompressedMirror(x + leg.xOffset, y + leg.yOffset, masks_leg[leg.index], BLACK, leg.rev);
  if (leg.index != 127) arduboy.drawCompressedMirror(x + leg.xOffset, y + leg.yOffset, imgs_leg[leg.index], WHITE, leg.rev);
  if (body.index != 127) arduboy.drawCompressedMirror(x + body.xOffset, y + body.yOffset, imgs_body[body.index], WHITE, body.rev);
  if (head.index != 127) arduboy.drawCompressedMirror(x + head.xOffset, y + head.yOffset, imgs_head[head.index], WHITE, head.rev);

}
