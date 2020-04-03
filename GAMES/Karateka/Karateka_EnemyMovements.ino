#include "src/utils/Arduboy2Ext.h"
#include "src/utils/PlayerStances.h"
#include "src/utils/EnemyStances.h"
#include "src/images/images.h"
#include "Enums.h"

// ---------------------------------------------------------------------------------------------------------------
//  Enemy movements ..

void enemyMovements() {

  int16_t distanceBetween = absT(enemy.xPos - player.xPos);

  if (enemyStack.isEmpty()) {

    enemy.xPosDelta = 0;

    switch (enemy.stance) {

      case STANCE_DEFAULT:

        switch (player.stance) {

          case STANCE_STANDING_UPRIGHT:
          
            if (distanceBetween < 17) {
              
              returnFromAction(ACTION_HIGH_KICK, ACTION_RETURN_TO_DEFAULT);
              enemyStack.push(STANCE_KICK_HIGH_END, STANCE_KICK_STANDING_TRANSITION, STANCE_DEFAULT_LEAN_BACK);

#ifdef DEBUG_HITS 
_action = 1; 
#endif            

            }
            else if (distanceBetween > 14) { // Otherwise creep forward ..
              
              enemyStack.push(STANCE_DEFAULT, STANCE_SIDLING_3, STANCE_SIDLING_1);
              enemy.xPosDelta = -MAIN_SCENE_X_SIDLING_1_DELTA;

#ifdef DEBUG_HITS 
_action = 2;              
#endif

            }

            break;

          case STANCE_RUNNING_1:
          case STANCE_RUNNING_2:
          case STANCE_RUNNING_3:
          case STANCE_RUNNING_4:
          case STANCE_RUNNING_5:
          case STANCE_RUNNING_6:
          case STANCE_RUNNING_7:
          case STANCE_RUNNING_8:
          case STANCE_RUNNING_STRAIGHTEN_UP:
          
            if (distanceBetween < 79) {
        
              returnFromAction(ACTION_HIGH_KICK, ACTION_RETURN_TO_DEFAULT);
              enemyStack.push(STANCE_KICK_HIGH_END, STANCE_KICK_STANDING_TRANSITION, STANCE_DEFAULT_LEAN_BACK);

#ifdef DEBUG_HITS 
_action = 3;              
#endif

            }
            else if (distanceBetween > 14) { // Otherwise creep forward ..
              
              enemyStack.push(STANCE_DEFAULT, STANCE_SIDLING_3, STANCE_SIDLING_1);
              enemy.xPosDelta = -MAIN_SCENE_X_SIDLING_1_DELTA;

#ifdef DEBUG_HITS 
_action = 4;
#endif

            }

            break;

          default:
            {

              uint8_t action = 0;
              uint8_t returnAction = 0;
              bool performActionRegardless = false;

              if (distanceBetween < 50) {

                if (enemyImmediateAction) {

                  action = ACTION_MED_KICK;

                }
                else {
                  action = random(ACTION_MIN_KICK, ACTION_MAX_PUNCH + gameStateDetails.activity);
                }

                returnAction = random(ACTION_RETURN_TO_DEFAULT, ACTION_RETURN_TO_ACTION_READY + 1);
                performActionRegardless = random(CHANCE_PERFORM_ACTION, CHANCE_PERFORM_ACTION_REGARDLESS_MAX) == CHANCE_PERFORM_ACTION;
                

                // If an action was selected then perform it if it will strike the player or we are doing it regardless ..

                if (action <= ACTION_MAX && (inStrikingRange(action, enemy.xPos, ENEMY_TYPE_PERSON, player.stance, player.xPos) > 0 || performActionRegardless)) {

                  returnFromAction(action, returnAction);

                  switch (action) {

                    case ACTION_HIGH_KICK:
                      enemyStack.push(STANCE_KICK_HIGH_END, STANCE_KICK_STANDING_TRANSITION, STANCE_DEFAULT_LEAN_BACK);
                      break;

                    case ACTION_MED_KICK:
                      enemyStack.push(STANCE_KICK_MED_END, STANCE_KICK_STANDING_TRANSITION, STANCE_DEFAULT_LEAN_BACK);
                      break;

                    case ACTION_LOW_KICK:
                      enemyStack.push(STANCE_KICK_LOW_END, STANCE_KICK_STANDING_TRANSITION, STANCE_DEFAULT_LEAN_BACK);
                      break;

                    case ACTION_HIGH_PUNCH:
                      enemyStack.push(STANCE_PUNCH_HIGH_RH_END, STANCE_PUNCH_READY, STANCE_DEFAULT_LEAN_BACK);
                      break;

                    case ACTION_MED_PUNCH:
                      enemyStack.push(STANCE_PUNCH_MED_RH_END, STANCE_PUNCH_READY, STANCE_DEFAULT_LEAN_BACK);
                      break;

                    case ACTION_LOW_PUNCH:
                      enemyStack.push(STANCE_PUNCH_LOW_RH_END, STANCE_PUNCH_READY, STANCE_DEFAULT_LEAN_BACK);
                      break;
                      
                  }

#ifdef DEBUG_HITS 
_action = 5;                  
#endif

                }
                else {
                      
                    
                  // Is the enemy too close to the player?  If so, should he move back ..

                  bool moveEnemy = enemyImmediateRetreat || (random(CHANCE_PERFORM_ACTION, CHANCE_MOVE_ENEMY_MAX + 1) == CHANCE_PERFORM_ACTION);

                  if (moveEnemy && distanceBetween < DISTANCE_TOO_CLOSE) {

                    enemyStack.push(STANCE_DEFAULT, STANCE_SIDLING_7, STANCE_DEFAULT_LEAN_FORWARD);
                    enemyStack.push(STANCE_SIDLING_6, STANCE_SIDLING_5, STANCE_SIDLING_4);
                    enemy.xPosDelta = +MAIN_SCENE_X_SIDLING_1_DELTA;

#ifdef DEBUG_HITS 
_action = 6;
#endif

                  }


                  // Are the player and enemy too far apart?  If so, should he move forward ..
                  
                  else if (moveEnemy && distanceBetween > DISTANCE_TOO_FAR) {
                    
                    enemyStack.push(STANCE_DEFAULT, STANCE_SIDLING_3, STANCE_SIDLING_1);
                    enemy.xPosDelta = -MAIN_SCENE_X_SIDLING_1_DELTA;

#ifdef DEBUG_HITS 
_action = 7;
#endif

                  }

                }

              }
              else {

                enemyStack.push(STANCE_DEFAULT, STANCE_SIDLING_3, STANCE_SIDLING_1);
                enemy.xPosDelta = -MAIN_SCENE_X_SIDLING_1_DELTA;

#ifdef DEBUG_HITS 
_action = 8;
#endif

              }

            }

            break;
                        
        }

        break;
    
      case STANCE_KICK_READY:
        {
          
          uint8_t action = (enemyImmediateAction ? ACTION_MED_KICK : random(ACTION_MIN_KICK, ACTION_MAX_KICK + 1));
          uint8_t returnAction = random(ACTION_RETURN_TO_DEFAULT, ACTION_RETURN_TO_ACTION_READY + 1);
        
          if (inStrikingRange(action, enemy.xPos, ENEMY_TYPE_PERSON, player.stance, player.xPos) > 0) {

            returnFromAction(action, returnAction);
            
            switch (action) {

              case ACTION_HIGH_KICK:
                enemyStack.push(STANCE_KICK_READY, STANCE_KICK_HIGH_END);
                break;

              case ACTION_MED_KICK:
                enemyStack.push(STANCE_KICK_READY, STANCE_KICK_MED_END);
                break;

              case ACTION_LOW_KICK:
                enemyStack.push(STANCE_KICK_READY, STANCE_KICK_LOW_END);
                break;
                
            }

#ifdef DEBUG_HITS 
_action = 9;
#endif

          }
          else {


            // Should we return to the default standing position ?

            if (random(ACTION_RETURN_TO_DEFAULT, ACTION_RETURN_TO_DEFAULT + 1) == ACTION_RETURN_TO_DEFAULT) {
              enemyStack.push(STANCE_DEFAULT, STANCE_DEFAULT_LEAN_FORWARD);
            }

#ifdef DEBUG_HITS 
_action = 10;           
#endif

          }

        }

        break;

      case STANCE_PUNCH_READY:
        {
          
          uint8_t action = (enemyImmediateAction ? ACTION_MED_PUNCH : random(ACTION_MIN_PUNCH, ACTION_MAX_PUNCH + 1));
          uint8_t returnAction = random(ACTION_RETURN_TO_DEFAULT, ACTION_RETURN_TO_ACTION_READY + 1);
        
          if (inStrikingRange(action, enemy.xPos, ENEMY_TYPE_PERSON, player.stance, player.xPos) > 0) {

            returnFromAction(action, returnAction);
            
            switch (action) {

              case ACTION_HIGH_PUNCH:
                enemyStack.push(STANCE_PUNCH_READY, (enemy.rightPunch ? STANCE_PUNCH_HIGH_RH_END : STANCE_PUNCH_HIGH_LH_END));
                break;

              case ACTION_MED_PUNCH:
                enemyStack.push(STANCE_PUNCH_READY, (enemy.rightPunch ? STANCE_PUNCH_MED_RH_END : STANCE_PUNCH_MED_LH_END));
                break;

              case ACTION_LOW_PUNCH:
                enemyStack.push(STANCE_PUNCH_READY, (enemy.rightPunch ? STANCE_PUNCH_LOW_RH_END : STANCE_PUNCH_LOW_LH_END));
                break;
                
            }
            
            enemy.rightPunch = !enemy.rightPunch;

#ifdef DEBUG_HITS 
_action = 11;
#endif

          }
          else {

            
            // Should we return to the default standing position ?

            if (random(ACTION_RETURN_TO_DEFAULT, ACTION_RETURN_TO_DEFAULT + 1) == ACTION_RETURN_TO_DEFAULT) {
              enemyStack.push(STANCE_DEFAULT, STANCE_DEFAULT_LEAN_FORWARD);

#ifdef DEBUG_HITS 
_action = 12;
#endif

            }
            
          }

        }

        break;

    }

  }

  enemyImmediateAction = false;
  enemyImmediateRetreat = false;

}