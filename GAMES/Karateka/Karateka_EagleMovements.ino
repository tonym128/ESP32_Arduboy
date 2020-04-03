#include "src/utils/Stack.h"
#include "src/utils/Arduboy2Ext.h"
#include "src/utils/PlayerStances.h"
#include "src/utils/EnemyStances.h"
#include "src/images/images.h"
#include "Enums.h"


// ---------------------------------------------------------------------------------------------------------------
//  Enemy movements ..

const uint8_t PROGMEM eagle_heights[] = { EAGLE_LEVEL_LOW, EAGLE_LEVEL_MED, EAGLE_LEVEL_HIGH };

void eagleMovements() {

  if (enemyStack.isEmpty()) {

    switch (eagleMode) {

      case EAGLE_MODE_FLY_INIT:

        enemy.yPos = pgm_read_byte(&eagle_heights[ random(0, 3) ]);
        enemyStack.push(STANCE_EAGLE_1);
        enemy.xPosDelta = -MAIN_SCENE_X_EAGLE_FLYING_DELTA;
        enemy.xPos = 150;
        eagleMode++;
        break;

      case EAGLE_MODE_FLY_TOWARDS:
        {
          int16_t distanceBetween = absT(enemy.xPos - player.xPos);
          uint8_t testDistance = 20;
          
          switch (player.stance) {

            case STANCE_KICK_READY:

              switch (enemy.yPos) {

                case EAGLE_LEVEL_LOW:
                case EAGLE_LEVEL_MED:
                  testDistance = 40;
                  break;

                default:
                  testDistance = 24;
                  break;

              }

              break;
          
            case STANCE_STANDING_UPRIGHT:
            case STANCE_STANDING_UPRIGHT_REV:
              testDistance = 30;
              break;

            default:
              testDistance = 40;
              break;

          }
          
          if (distanceBetween > testDistance) {

            if (eagleWingUp) {
              enemyStack.push(STANCE_EAGLE_1);
            } 
            else {
              enemyStack.push(STANCE_EAGLE_2);
            }

            eagleWingUp = !eagleWingUp;
            enemy.xPosDelta = -MAIN_SCENE_X_EAGLE_FLYING_DELTA;
            
          }
          else {

            eagleMode++;
            
          }
        
        }
        
        break;

      case EAGLE_MODE_ATTACK_1 ... EAGLE_MODE_ATTACK_3:  
        {
          int16_t deltaX = 0;
          
          switch (player.stance) {

            case STANCE_KICK_READY:

              switch (enemy.yPos) {

                case EAGLE_LEVEL_LOW:
                case EAGLE_LEVEL_MED:
                  deltaX = 20;
                  break;

                case EAGLE_LEVEL_HIGH:
                  deltaX = 5;
                  break;

              }
              break;
          
            case STANCE_STANDING_UPRIGHT:
            case STANCE_STANDING_UPRIGHT_REV:
              deltaX = 9;
              break;

            default:
              deltaX = 20;
              break;

          }

          switch (enemy.yPos) {

            case EAGLE_LEVEL_LOW:
            case EAGLE_LEVEL_MED:
              enemy.xPos = player.xPos + deltaX; 
              enemyStack.push(STANCE_EAGLE_3);
              break;

            default:
              enemy.xPos = player.xPos + deltaX; 
              enemyStack.push(STANCE_EAGLE_4);
              break;

          } 
        
          if (eagleMode == EAGLE_MODE_ATTACK_2) {
          
            switch (enemy.yPos) {

              case EAGLE_LEVEL_LOW:
                player.health = (player.health > EAGLE_LEVEL_LOW_HEALTH ? player.health - EAGLE_LEVEL_LOW_HEALTH : 0);
                break;

              case EAGLE_LEVEL_MED:
                player.health = (player.health > EAGLE_LEVEL_MED_HEALTH ? player.health - EAGLE_LEVEL_MED_HEALTH : 0);
                break;

              default:
                player.health = (player.health > EAGLE_LEVEL_HIGH_HEALTH ? player.health - EAGLE_LEVEL_HIGH_HEALTH : 0);
                break;

            }

          }
          
        }

        enemy.xPosDelta = 0;
        eagleMode++; 
        break;
 
      case EAGLE_MODE_FLY_AWAY:
      
        for (int i = 0; i < 5; i++) {
          
          enemyStack.push(STANCE_EAGLE_1_REV);
          enemyStack.push(STANCE_EAGLE_2_REV);

        }
        
        enemy.xPosDelta = MAIN_SCENE_X_EAGLE_FLYING_DELTA;
        eagleMode++; 
        break;     

      default:
      
        eagleMode = EAGLE_MODE_FLY_INIT;
        if (playerStack.isEmpty() && enemyStack.isEmpty()) {
          gameStateDetails.setCurrState(GAME_STATE_FOLLOW_SEQUENCE);
        }
        break;     
     
    }
  
  }
 
}