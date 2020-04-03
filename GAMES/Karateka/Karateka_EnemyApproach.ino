#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Stack.h"
#include "src/utils/PlayerStances.h"
#include "src/utils/EnemyStances.h"
#include "src/images/images.h"
#include "Enums.h"

void enemy_approach_init() {

  for (int i = 0; i < 2; i++) {

    enemyStack.push(STANCE_RUNNING_RF_END, STANCE_RUNNING_8, STANCE_RUNNING_5);
    enemyStack.push(STANCE_RUNNING_4, STANCE_RUNNING_LF_END, STANCE_RUNNING_2);
    enemyStack.push(STANCE_RUNNING_7, STANCE_RUNNING_4);
    
  }

  player.xPosDelta = MAIN_SCENE_X_SIDLING_2_DELTA;
  gameStateDetails.setCurrState(GAME_STATE_ENEMY_APPROACH);

  enemy.xPos = 48;

}
