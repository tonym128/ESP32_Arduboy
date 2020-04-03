#include "src/utils/Arduboy2Ext.h"
#include "src/utils/PlayerStances.h"
#include "src/utils/EnemyStances.h"
#include "src/images/images.h"
#include "Enums.h"

void emperor_loop() {

  if (arduboy.justPressed(A_BUTTON)) {
    
    enemyStack.clear();
    gameStateDetails.setCurrState(GAME_STATE_FOLLOW_SEQUENCE);

  }
  else {
    

    // Draw background ..

    arduboy.drawFastHLine(0, 52, WIDTH);

    for (int i=54; i< 64; i+=2) {

      drawHorizontalDottedLine(i % 2, WIDTH, i);

    }
    
    if (enemyStack.isEmpty()) {

      switch (emperorMode) {

        case EMPEROR_MODE_INIT:
        
          enemyStack.push(STANCE_RUNNING_STRAIGHTEN_UP_REV);
          
          for (int i = 0; i < 2; i++) {
            
            enemyStack.push(STANCE_RUNNING_RF_END_REV, STANCE_RUNNING_8_REV, STANCE_RUNNING_5_REV);
            enemyStack.push(STANCE_RUNNING_4_REV, STANCE_RUNNING_LF_END_REV, STANCE_RUNNING_2_REV);
            enemyStack.push(STANCE_RUNNING_7_REV, STANCE_RUNNING_4_REV);
            
          }
          
          enemy.xPosDelta = 5;
          enemy.xPos = -48;
          
          break;

        case EMPEROR_MODE_PAUSE_1:
          for (int i = 0; i < 5; i++) {
            enemyStack.push(STANCE_STANDING_UPRIGHT_REV);
          }
      
          break;

        case EMPEROR_LIFT_ARM:
        
          for (int i =0; i < 9; i++) {
            enemyStack.push(STANCE_STANDING_UPRIGHT_REV);
          }
          
          break;
          
        case EMPEROR_MODE_PAUSE_2:
        case EMPEROR_MODE_PAUSE_3:
        
          for (int i = 0; i < 3; i++) {
            enemyStack.push(STANCE_STANDING_UPRIGHT_REV);
          }
      
          break;
        
        case EMPEROR_MODE_BOW:
        
          enemyStack.push(STANCE_BOW_1, STANCE_BOW_2);
          enemyStack.push(STANCE_BOW_2, STANCE_BOW_1);
          
          break;
          
        case EMPEROR_MODE_FIGHTER_LEAVE:
        
          for (int i = 0; i < 2; i++) {

            enemyStack.push(STANCE_RUNNING_RF_END, STANCE_RUNNING_8, STANCE_RUNNING_5);
            enemyStack.push(STANCE_RUNNING_4, STANCE_RUNNING_LF_END, STANCE_RUNNING_2);
            enemyStack.push(STANCE_RUNNING_7, STANCE_RUNNING_4);
            
          }

          enemy.xPosDelta = -5;
          break;

      }

    }


    // Render screen ..
    
    if (arduboy.everyXFrames(ANIMATION_NUMBER_OF_FRAMES)) {
      
      enemy.stance = enemyStack.pop();
      enemy.xPos = enemy.xPos + enemy.xPosDelta;

      if (enemyStack.isEmpty()) {

        enemy.xPosDelta = 0;
        emperorMode++;

        if (emperorMode > EMPEROR_MODE_FIGHTER_LEAVE) { gameStateDetails.setCurrState(GAME_STATE_FOLLOW_SEQUENCE); }

      }
      
    }

    arduboy.drawCompressedMirror(92, 45, emperor_mask, BLACK, false);
    arduboy.drawCompressedMirror(87, 4, emperor, WHITE, false);

    if (emperorMode >= EMPEROR_LIFT_ARM && emperorMode < EMPEROR_MODE_FIGHTER_LEAVE) {

      arduboy.drawCompressedMirror(70, 19, emperor_arm_out, WHITE, false);
      
    }
    else {

      arduboy.drawCompressedMirror(87, 21, emperor_arm_normal, WHITE, false);
      
    }

    arduboy.drawCompressedMirror(16, 7, arch_interior_rh_mask, BLACK, false);
    arduboy.drawCompressedMirror(16, 7, arch_interior_rh, WHITE, false);
    renderEnemyStance(arduboy, enemy.xPos, enemy.yPos, enemy.stance);
    arduboy.drawCompressedMirror(0, 3, arch_interior_lh_mask, BLACK, false);
    arduboy.drawCompressedMirror(0, 3, arch_interior_lh, WHITE, false);
    
  }

}
