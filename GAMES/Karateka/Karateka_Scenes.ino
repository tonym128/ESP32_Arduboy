#include "src/utils/Arduboy2Ext.h"
#include "src/utils/PlayerStances.h"
#include "src/utils/EnemyStances.h"
#include "src/images/images.h"
#include "Enums.h"

void drawPrincessBackground_1() {

  uint8_t x = 72;

  for (int y = 50; y < 64; y = y + 2) {
    arduboy.drawFastHLine(x, y, WIDTH - x);    
    x = x + (y < 56 ? -20 : 20);
  }

  arduboy.drawCompressedMirror(16, 7, arch_interior_rh_mask, BLACK, false);
  arduboy.drawCompressedMirror(16, 7, arch_interior_rh, WHITE, false);
  arduboy.drawCompressedMirror(89, 19, princess_seat_mask, BLACK, false);
  arduboy.drawCompressedMirror(89, 19, princess_seat, WHITE, false);

}

void drawPrincessBackground_2() {
  
  arduboy.drawCompressedMirror(0, 3, arch_interior_lh_mask, BLACK, false);
  arduboy.drawCompressedMirror(0, 3, arch_interior_lh, WHITE, false);

}


void showScene() {

  if (arduboy.justPressed(A_BUTTON)) {
    
    if (gameStateDetails.getCurrState() == GAME_STATE_THE_END) {
      gameStateDetails.sequence = 0;
    }     

    enemyStack.clear();
    gameStateDetails.setCurrState(GAME_STATE_FOLLOW_SEQUENCE);

  }
  else {
    
    switch (gameStateDetails.getCurrState()) {

      case GAME_STATE_TITLE_SCENE:
        arduboy.drawCompressedMirror(0, 0, title, WHITE, false);
        break;

#ifdef JORDAN_MECHNER
      case GAME_STATE_JM_SCENE:
        arduboy.drawCompressedMirror(0, 0, jordanMechner, WHITE, false);
        if (gameStateDetails.delayInterval > 8) {
          arduboy.fillRect(0, 40, WIDTH, 24, BLACK);
        }
        break;
#endif      

      case GAME_STATE_CASTLE_SCENE:
        arduboy.drawCompressedMirror(0, 0, castle, WHITE, false);
        break;
        
      case GAME_STATE_THE_END:
        arduboy.drawCompressedMirror(34, 26, theend, WHITE, false);
        break;
        
      case GAME_STATE_PRINCESS:

        drawPrincessBackground_1();
        drawPrincessBackground_2();
        arduboy.drawCompressedMirror(93, 19, princess_sitting_mask, BLACK, false);
        arduboy.drawCompressedMirror(93, 19, princess_sitting, WHITE, false);
        
        break;

      case GAME_STATE_ENEMY_APPROACH:

        draw_background();
    
        //  Update player and enemy positions and stances ..
    
        if (arduboy.everyXFrames(ANIMATION_NUMBER_OF_FRAMES)) {
          
          if (!enemyStack.isEmpty()) {
            enemy.stance = enemyStack.pop();
          }
          else {
            player.xPosDelta = 0;
            gameStateDetails.setCurrState(GAME_STATE_FOLLOW_SEQUENCE);
          }
          
        }
    
        
        // Move scenery if needed ..
    
        if (player.xPosDelta != 0) {
    
          mainSceneX = mainSceneX + player.xPosDelta;
    
          if (mainSceneX < -MAIN_SCENE_IMG_WIDTH) { mainSceneX = 0; }
          if (mainSceneX > 0) { mainSceneX = mainSceneX - MAIN_SCENE_IMG_WIDTH; }
    
        }
    
        if (enemy.xPos < 128) renderEnemyStance(arduboy, enemy.xPos, enemy.yPos, enemy.stance);

      default:
        break;

    }
    
    if (arduboy.everyXFrames(ANIMATION_NUMBER_OF_FRAMES)) {

      if (gameStateDetails.delayInterval > 0) {

        gameStateDetails.delayInterval--;

        if (gameStateDetails.delayInterval == 0) { 

          // if (gameStateDetails.getCurrState() == GAME_STATE_THE_END) {  // Not relevant for 'The End' scene.
          //   gameStateSeq = 0;
          // }            
          gameStateDetails.setCurrState(GAME_STATE_FOLLOW_SEQUENCE); 

        }

      }

    }
  
  }

}



void finalScene() {
  
  if (arduboy.justPressed(A_BUTTON)) {
    
    enemyStack.clear();
    gameStateDetails.setCurrState(GAME_STATE_FOLLOW_SEQUENCE);

  }
  else {
    

    // Draw background ..

    drawPrincessBackground_1();
    
    if (enemyStack.isEmpty() && playerStack.isEmpty()) {

      finalSceneMode++;

      switch (finalSceneMode) {

        case FINAL_SCENE_INIT:
        
          for (int i = 0; i < 3; i++) {
            
            playerStack.push(STANCE_RUNNING_LF_END, STANCE_RUNNING_2, STANCE_RUNNING_7);
            playerStack.push(STANCE_RUNNING_4);

            playerStack.push(STANCE_RUNNING_RF_END, STANCE_RUNNING_8, STANCE_RUNNING_5);
            playerStack.push(STANCE_RUNNING_4);

          }
          
          player.xPosDelta = 5;
          player.xPos = -48;
          
          enemyStack.push(STANCE_PRINCESS_STANDING);
          for (int i = 0; i < 12; i++) {
            enemyStack.push(STANCE_PRINCESS_SITTING);
          }
          enemy.xPos = 93;
          enemy.yPos = 14;

          break;

        case FINAL_SCENE_PAUSE:
          playerStack.push(STANCE_STANDING_UPRIGHT);
          playerStack.push(STANCE_RUNNING_STRAIGHTEN_UP);
          break;

        case FINAL_SCENE_PAUSE_2:
          player.xPosDelta = 0;
          enemy.xPosDelta = 0;
          for (int i = 0; i < 4; i++) {
            playerStack.push(STANCE_STANDING_UPRIGHT);
          }
          break;

        case FINAL_SCENE_KISSING:
          enemy.xPos = 90;
          enemy.yPos = 15;
          enemyStack.push(STANCE_PRINCESS_KISSING, STANCE_PRINCESS_KISSING, STANCE_PRINCESS_KISSING);
          break;

        case FINAL_SCENE_BREAK_UP:
          enemy.xPos = 93;
          enemy.yPos = 15;
          for (int i = 0; i < 18; i++) {
            enemyStack.push(STANCE_PRINCESS_STANDING);
          }
          break;
                    
      }

    }


    // Render screen ..
    
    if (arduboy.everyXFrames(ANIMATION_NUMBER_OF_FRAMES)) {
      
      if (!enemyStack.isEmpty()) {
        enemy.stance = enemyStack.pop();
        enemy.xPos = enemy.xPos + enemy.xPosDelta;
      }
           
      if (!playerStack.isEmpty()) {
        player.stance = playerStack.pop();
        player.xPos = player.xPos + player.xPosDelta;
        }

    }

    renderPlayerStance(arduboy, player.xPos, player.yPos, player.stance);
    renderEnemyStance(arduboy, enemy.xPos, enemy.yPos, enemy.stance);

    drawPrincessBackground_2();
    
    if (finalSceneMode == FINAL_SCENE_BREAK_UP && enemyStack.isEmpty()) {

      gameStateDetails.setCurrState(GAME_STATE_FOLLOW_SEQUENCE);

    }
    
  }

}