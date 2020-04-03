#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Stack.h"
#include "src/utils/PlayerStances.h"
#include "src/utils/EnemyStances.h"
#include "src/images/images.h"
#include "Enums.h"


// ---------------------------------------------------------------------------------------------------------------
//  Player movements ..

void playerMovements() {
  
  uint8_t currPressed = arduboy.buttonsState();
  uint16_t distBetween = (gameStateDetails.enemyType != ENEMY_TYPE_NONE ? absT(enemy.xPos - player.xPos) : 9999);

  if (playerStack.isEmpty()) {
  
    player.xPosDelta = 0;

    if ((currPressed & A_BUTTON_MASK)) {

      switch (player.stance) {

        case STANCE_DEFAULT:

          // Move to the Kick Ready position ..

          if (!(currPressed & UP_BUTTON_MASK) && !(currPressed & RIGHT_BUTTON_MASK) && !(currPressed & RIGHT_BUTTON_MASK) && !(currPressed & DOWN_BUTTON_MASK)) {

            playerStack.push(STANCE_KICK_READY, STANCE_DEFAULT_LEAN_BACK);
            
          }
          else {

            // Perform an immediate kick from the default position ..

            if (currPressed & RIGHT_BUTTON_MASK)  {             // Medium kick ..
              playerStack.push(STANCE_KICK_MED_END, STANCE_KICK_STANDING_TRANSITION, STANCE_DEFAULT_LEAN_BACK);
            }

            else if (currPressed & UP_BUTTON_MASK)  {           // High kick ..
              playerStack.push(STANCE_KICK_HIGH_END, STANCE_KICK_STANDING_TRANSITION, STANCE_DEFAULT_LEAN_BACK);
            }

            else if (currPressed & DOWN_BUTTON_MASK)  {         // Low kick ..
              playerStack.push(STANCE_KICK_LOW_END, STANCE_KICK_STANDING_TRANSITION, STANCE_DEFAULT_LEAN_BACK);
            }

          }
          break;

        case STANCE_KICK_HIGH_END:
        case STANCE_KICK_MED_END:
        case STANCE_KICK_LOW_END:                               // As the A button is still being held, return to the Kick Ready position ..
          playerStack.push(STANCE_KICK_READY);
          break;

        case STANCE_KICK_READY:

          if ((currPressed & UP_BUTTON_MASK) || (currPressed & RIGHT_BUTTON_MASK) || (currPressed & DOWN_BUTTON_MASK)) {

            player.activity++;

            if (player.activity > 4) {
              playerStack.push(STANCE_DEFAULT, STANCE_DEFAULT_LEAN_FORWARD);
              player.activity = 0;
            }

            if (currPressed & RIGHT_BUTTON_MASK)  {               // Medium kick ..
              playerStack.push(STANCE_KICK_READY, STANCE_KICK_MED_END);
            }

            else if (currPressed & UP_BUTTON_MASK)  {             // High kick ..
              playerStack.push(STANCE_KICK_READY, STANCE_KICK_HIGH_END);
            }

            else if (currPressed & DOWN_BUTTON_MASK)  {           // Low kick ..
              playerStack.push(STANCE_KICK_READY, STANCE_KICK_LOW_END);
            }

          }

          break;

        case STANCE_PUNCH_READY:                                  // Really shouldn't happen but if B is pressed before A and both held ..
            
            if (currPressed & RIGHT_BUTTON_MASK)  {               // Medium punch ..
              playerStack.push(STANCE_PUNCH_READY, (player.rightPunch ? STANCE_PUNCH_MED_RH_END : STANCE_PUNCH_MED_LH_END));
            }

            else if (currPressed & UP_BUTTON_MASK)  {             // High punch ..
              playerStack.push(STANCE_PUNCH_READY, (player.rightPunch ? STANCE_PUNCH_HIGH_RH_END : STANCE_PUNCH_HIGH_LH_END));
            }

            else if (currPressed & DOWN_BUTTON_MASK)  {           // Low punch ..
              playerStack.push(STANCE_PUNCH_READY, (player.rightPunch ? STANCE_PUNCH_LOW_RH_END : STANCE_PUNCH_LOW_LH_END));
            }

            player.rightPunch = !player.rightPunch;
            break;

      }
              
    }
    else {


      // If the A Button is no longer being pressed ..

      switch (player.stance) {

        case STANCE_KICK_READY:
          player.activity = 0;
          playerStack.push(STANCE_DEFAULT, STANCE_DEFAULT_LEAN_FORWARD);
          break;

        case STANCE_KICK_HIGH_END:
        case STANCE_KICK_MED_END:
        case STANCE_KICK_LOW_END:
          player.activity = 0;
          playerStack.push(STANCE_DEFAULT, STANCE_DEFAULT_LEAN_FORWARD, STANCE_KICK_READY);
          break;
        
      }


      // Is the B button being pressed ?

      if ((currPressed & B_BUTTON_MASK)) {

        switch (player.stance) {

          case STANCE_STANDING_UPRIGHT:                         // If standing upright, move to the fighting position ..
            playerStack.push(STANCE_DEFAULT, STANCE_DEFAULT_LEAN_BACK);
            break;

          case STANCE_DEFAULT:                                  // Move to the Punch Ready position ..

            if (!(currPressed & UP_BUTTON_MASK) && !(currPressed & RIGHT_BUTTON_MASK) && !(currPressed & DOWN_BUTTON_MASK)) {

              playerStack.push(STANCE_PUNCH_READY, STANCE_DEFAULT_LEAN_BACK);
              
            }
            else {


              // Perform a immediate punch from the standing position ..

              if (currPressed & RIGHT_BUTTON_MASK)  {           // Medium punch ..
                playerStack.push(STANCE_PUNCH_MED_RH_END, STANCE_PUNCH_READY, STANCE_DEFAULT_LEAN_BACK);
              }

              else if (currPressed & UP_BUTTON_MASK)  {              // High punch ..
                playerStack.push(STANCE_PUNCH_HIGH_RH_END, STANCE_PUNCH_READY, STANCE_DEFAULT_LEAN_BACK);
              }

              else if (currPressed & UP_BUTTON_MASK)  {              // Low punch ..
                playerStack.push(STANCE_PUNCH_LOW_RH_END, STANCE_PUNCH_READY, STANCE_DEFAULT_LEAN_BACK);
              }

            }
            
            break;

          case STANCE_PUNCH_HIGH_LH_END:                        // As the B button is still being held, return to the Puch Ready position ..
          case STANCE_PUNCH_HIGH_RH_END:
          case STANCE_PUNCH_MED_LH_END:
          case STANCE_PUNCH_MED_RH_END:
          case STANCE_PUNCH_LOW_LH_END:
          case STANCE_PUNCH_LOW_RH_END:

            playerStack.push(STANCE_PUNCH_READY);
            break;

          case STANCE_PUNCH_READY:
            
            if (currPressed & RIGHT_BUTTON_MASK)  {             // Medium punch ..
              playerStack.push(STANCE_PUNCH_READY, (player.rightPunch ? STANCE_PUNCH_MED_RH_END : STANCE_PUNCH_MED_LH_END));
            }

            else if (currPressed & UP_BUTTON_MASK)  {           // High punch ..
              playerStack.push(STANCE_PUNCH_READY, (player.rightPunch ? STANCE_PUNCH_HIGH_RH_END : STANCE_PUNCH_HIGH_LH_END));
            }

            else if (currPressed & DOWN_BUTTON_MASK)  {         // Low punch ..
              playerStack.push(STANCE_PUNCH_READY, (player.rightPunch ? STANCE_PUNCH_LOW_RH_END : STANCE_PUNCH_LOW_LH_END));
            }

            player.rightPunch = !player.rightPunch;
            break;
          
        }

      }
      else {


        switch (player.stance) {    // If the B Button is no longer being pressed return to the default stance ..

          case STANCE_PUNCH_READY:

            playerStack.push(STANCE_DEFAULT, STANCE_DEFAULT_LEAN_FORWARD);
            break;

          case STANCE_PUNCH_HIGH_LH_END:
          case STANCE_PUNCH_HIGH_RH_END:
          case STANCE_PUNCH_MED_LH_END:
          case STANCE_PUNCH_MED_RH_END:
          case STANCE_PUNCH_LOW_LH_END:
          case STANCE_PUNCH_LOW_RH_END:

            playerStack.push(STANCE_DEFAULT, STANCE_DEFAULT_LEAN_FORWARD, STANCE_PUNCH_READY);
            break;

        }


        // Neither the A or B button has been pressed ..

        switch (player.stance) {

          case STANCE_DEFAULT:

            if (currPressed & UP_BUTTON_MASK)  { // Stand upright again ..
              playerStack.push(STANCE_STANDING_UPRIGHT, STANCE_DEFAULT_LEAN_BACK);
            }

            else if (currPressed & RIGHT_BUTTON_MASK)  { // Sidle forward ..
              
              if (distBetween > DISTANCE_BETWEEN_LRG || !gameStateDetails.enemyType == ENEMY_TYPE_PERSON) {

                player.xPosDelta = -MAIN_SCENE_X_SIDLING_2_DELTA; 
                playerStack.push(STANCE_DEFAULT, STANCE_DEFAULT_LEAN_BACK, STANCE_SIDLING_3);
                playerStack.push(STANCE_SIDLING_2, STANCE_SIDLING_1, STANCE_DEFAULT_LEAN_FORWARD);
  
              }
              else if (distBetween > DISTANCE_BETWEEN_MED) { 
                
                player.xPosDelta = -MAIN_SCENE_X_SIDLING_1_DELTA; 
                playerStack.push(STANCE_DEFAULT, STANCE_DEFAULT_LEAN_BACK, STANCE_SIDLING_3);
                playerStack.push(STANCE_SIDLING_2, STANCE_SIDLING_1, STANCE_DEFAULT_LEAN_FORWARD);
                
              }
              else if (distBetween > DISTANCE_BETWEEN_SML) { 
                
                player.xPosDelta = -MAIN_SCENE_X_SIDLING_1_DELTA; 
                playerStack.push(STANCE_DEFAULT, STANCE_SIDLING_2, STANCE_SIDLING_1);
                
              }
              else if (distBetween > DISTANCE_BETWEEN_TINY) { 

                player.xPosDelta = -MAIN_SCENE_X_SIDLING_1_DELTA; 
                playerStack.push(STANCE_DEFAULT, STANCE_SIDLING_1);
                
              }
                                            
            }

            else if (currPressed & LEFT_BUTTON_MASK && player.xPosOverall > 12)  { // Sidle backward ..
              
              player.xPosDelta = MAIN_SCENE_X_SIDLING_1_DELTA; 
              playerStack.push(STANCE_DEFAULT, STANCE_SIDLING_7, STANCE_DEFAULT_LEAN_FORWARD);
              playerStack.push(STANCE_SIDLING_6, STANCE_SIDLING_5, STANCE_SIDLING_4);
              
            }

            break;

          case STANCE_STANDING_UPRIGHT:

            if (currPressed & DOWN_BUTTON_MASK)  { // If standing upright, move to the fighting position ..
              playerStack.push(STANCE_DEFAULT, STANCE_DEFAULT_LEAN_BACK);
            }
            
            else if (currPressed & RIGHT_BUTTON_MASK && distBetween > 100)  { // Start running ..
              player.rightFoot = true;
              player.xPosDelta = -MAIN_SCENE_X_SIDLING_2_DELTA;            
              playerStack.push(STANCE_RUNNING_6, STANCE_RUNNING_8, STANCE_RUNNING_5);
//              playerStack.push(STANCE_RUNNING_4, STANCE_RUNNING_3, STANCE_RUNNING_2);
              playerStack.push(STANCE_RUNNING_1);
            }
            break;

          case STANCE_RUNNING_LF_END:
          case STANCE_RUNNING_RF_END:
            
            if (currPressed & RIGHT_BUTTON_MASK && distBetween > 100)  { // Continue running ..

              if (!player.rightFoot) {
                player.xPosDelta = -MAIN_SCENE_X_SIDLING_2_DELTA;            
                playerStack.push(STANCE_RUNNING_LF_END, STANCE_RUNNING_2, STANCE_RUNNING_7);
                playerStack.push(STANCE_RUNNING_4);
              }
              else {
                player.xPosDelta = -MAIN_SCENE_X_SIDLING_2_DELTA;            
                playerStack.push(STANCE_RUNNING_RF_END, STANCE_RUNNING_8, STANCE_RUNNING_5);
                playerStack.push(STANCE_RUNNING_4);
              }

              player.rightFoot = !player.rightFoot;

            }
            else {
              player.xPosDelta = -MAIN_SCENE_X_SIDLING_2_DELTA;            
              playerStack.push(STANCE_STANDING_UPRIGHT, STANCE_RUNNING_STRAIGHTEN_UP);
            }
            break;
                    
        }

      }
      
    }

  }
    
}
