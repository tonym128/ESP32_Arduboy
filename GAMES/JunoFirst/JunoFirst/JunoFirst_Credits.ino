#include "src/utils/Arduboy2Ext.h"

#define CREDITS_RANGE_INC       10
#define CREDITS_RANGE_1_START   0
#define CREDITS_RANGE_1_END     CREDITS_RANGE_INC - 1
#define CREDITS_RANGE_2_START   (CREDITS_RANGE_INC)
#define CREDITS_RANGE_2_END     (CREDITS_RANGE_INC * 2) - 1
#define CREDITS_RANGE_3_START   (CREDITS_RANGE_INC * 2)
#define CREDITS_RANGE_3_END     (CREDITS_RANGE_INC * 3) - 1
#define CREDITS_RANGE_4_START   (CREDITS_RANGE_INC * 3)
#define CREDITS_RANGE_4_END     (CREDITS_RANGE_INC * 4) - 1
#define CREDITS_RANGE_5_START   (CREDITS_RANGE_INC * 4)
#define CREDITS_RANGE_5_END     (CREDITS_RANGE_INC * 5) - 1
#define CREDITS_RANGE_6_START   (CREDITS_RANGE_INC * 5)
#define CREDITS_RANGE_6_END     (CREDITS_RANGE_INC * 6) - 1
#define CREDITS_RANGE_7_START   (CREDITS_RANGE_INC * 6)
#define CREDITS_RANGE_7_END     (CREDITS_RANGE_INC * 7) - 1


// --------------------------------------------------------------------------------------
//  Show credits screen ..
//
void Credits() {

  uint8_t const *filmoteImage = nullptr;
  uint8_t vampiricsImage = 0;


  // Draw logo and names ..

  Sprites::drawOverwrite(37, 6, credits, 0);
  arduboy.drawFastHLine(37, 25, 54, WHITE);
  arduboy.drawFastHLine(37, 27, 54, WHITE);


  // Display 'Press A' button after a short period of time ..

  introDelay++;

  switch (introDelay) {

    case CREDITS_RANGE_1_START ... CREDITS_RANGE_1_END:
      filmoteImage = filmote1;
      vampiricsImage = 0;
      break;

    case CREDITS_RANGE_2_START ... CREDITS_RANGE_2_END:
      filmoteImage = filmote1;
      vampiricsImage = 1;
      break;

    case CREDITS_RANGE_3_START ... CREDITS_RANGE_3_END:
      filmoteImage = filmote1;
      vampiricsImage = 2;
      break;

    case CREDITS_RANGE_4_START ... CREDITS_RANGE_4_END:
      filmoteImage = filmote2;
      vampiricsImage = 3;
      break;

    case CREDITS_RANGE_5_START ... CREDITS_RANGE_5_END:
      filmoteImage = filmote2;
      vampiricsImage = 4;
      break;

    case CREDITS_RANGE_6_START ... CREDITS_RANGE_6_END:
      filmoteImage = filmote2;
      vampiricsImage = 5;
      break;

    case CREDITS_RANGE_7_START ... CREDITS_RANGE_7_END:
      filmoteImage = filmote1;
      vampiricsImage = 0;
      break;

  }
  
  if (introDelay > CREDITS_RANGE_6_END) {
    introDelay = 0;
  }

  Sprites::drawOverwrite(38, 38, filmoteImage, 0);
  Sprites::drawOverwrite(76, 38, vampiricsBase, 0);
  Sprites::drawSelfMasked(76, 38, vampiricsAnimation, vampiricsImage);
  Sprites::drawOverwrite(32, 50, filmoteText, 0);
  Sprites::drawOverwrite(68, 50, vampriricsText, 0);


  // If 'A' bUtton is pressed move to introduction ..

  if (arduboy.justPressed(A_BUTTON) || arduboy.justPressed(B_BUTTON)) { 

    gameState = GameState::Intro_Init; 
    
  }

}