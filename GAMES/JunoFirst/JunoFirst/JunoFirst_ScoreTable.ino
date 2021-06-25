#include "src/utils/Arduboy2Ext.h"


// --------------------------------------------------------------------------------------
//  Render a score table ..
//
void ScoreTable() {

  alternate++;
  if (alternate > 108) alternate = 0;
  bool first = alternate % 36 < 18;


  // Render score table ..

  Sprites::drawOverwrite(20, 1, scoreTable, 0);

  Sprites::drawOverwrite(10, 21, (first ? alien_close_1 : alien_close_2), 0);
  Sprites::drawOverwrite(29, 20, ellipsis, 0);
  Sprites::drawOverwrite(44, 23, numbers, 1);  
  Sprites::drawOverwrite(49, 23, numbers, 5);  
  Sprites::drawOverwrite(54, 23, numbers, 0);  

  Sprites::drawOverwrite(10, 31, (first ? alien_close_2 : alien_close_1), 1);
  Sprites::drawOverwrite(29, 30, ellipsis, 0);
  Sprites::drawOverwrite(44, 33, numbers, 3);  
  Sprites::drawOverwrite(49, 33, numbers, 0);  
  Sprites::drawOverwrite(54, 33, numbers, 0);  
 
  Sprites::drawOverwrite(10, 41, alien_close_1, 04);
  Sprites::drawOverwrite(29, 40, ellipsis, 0);
  Sprites::drawOverwrite(44, 43, numbers, 4);  
  Sprites::drawOverwrite(49, 43, numbers, 5);  
  Sprites::drawOverwrite(54, 43, numbers, 0); 

  Sprites::drawOverwrite(70, 21, (first ? alien_close_1 : alien_close_2), 2);
  Sprites::drawOverwrite(89, 20, ellipsis, 0);
  Sprites::drawOverwrite(104, 23, numbers, 6);  
  Sprites::drawOverwrite(109, 23, numbers, 0);  
  Sprites::drawOverwrite(114, 23, numbers, 0); 

  Sprites::drawOverwrite(70, 31, (first ? alien_close_2 : alien_close_1), 3);
  Sprites::drawOverwrite(89, 30, ellipsis, 0);
  Sprites::drawOverwrite(104, 33, numbers, 2);  
  Sprites::drawOverwrite(109, 33, numbers, 5);  
  Sprites::drawOverwrite(114, 33, numbers, 0);  
 
  Sprites::drawOverwrite(70, 41, alien_close_1, 5);
  Sprites::drawOverwrite(89, 40, ellipsis, 0);
  Sprites::drawOverwrite(107, 43, x2_score_table, 0);  


  // Display 'Press A' button after a short period of time ..

  if (introDelay < INTRO_DELAY) {
    introDelay++;
  }
  else {    
    Sprites::drawOverwrite(51, 55, pressA, 0);
  }


  // If 'A' bUtton is pressed move to game play ..

  if (arduboy.justPressed(A_BUTTON)) { 

//    player.reset(); 
//    level.reset(enemies, bullets, &playerBullet);
    gameState = GameState::Wave_Init; 
    
  }

}