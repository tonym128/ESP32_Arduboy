#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Stack.h"
#include "src/utils/PlayerStances.h"
#include "src/utils/EnemyStances.h"
#include "src/images/images.h"
#include "Enums.h"

#ifdef SOUNDS_ARDUBOYTONES
#include <ArduboyTones.h>
#include "src/sounds/Sounds_ArduboyTones.h"
#endif

#ifdef SOUNDS_ATMLIB
#include <ATMlib.h>
#include "src/sounds/Sounds_ATMLib.h"
#endif

Arduboy2Ext arduboy;

#ifdef SOUNDS_ARDUBOYTONES
ArduboyTones sound(arduboy.audio.on);
#endif

#ifdef SOUNDS_ATMLIB
ATMsynth ATM;
#endif

Stack <uint8_t, 30> playerStack;
Stack <uint8_t, 30> enemyStack;

int8_t mainSceneX = 0;
bool displayHealth = false;
bool outdoors = true;

uint8_t enemyHit = 0;
uint8_t playerHit = 0;

uint8_t eagleMode = EAGLE_MODE_NONE;
bool eagleWingUp = false;

uint8_t emperorMode = EMPEROR_MODE_INIT;
uint8_t finalSceneMode = FINAL_SCENE_INACTIVE;

#ifdef USE_DIFFERENT_BAMS
const uint8_t *bam_images[] =               { bam1, bam2, bam3 };
const uint8_t *bam_masks[] =                { bam1_mask, bam2_mask, bam3_mask };
#endif

bool outside = true;
GameStateDetails gameStateDetails;

#ifdef DEBUG_HITS
int16_t _distBetween = 0;
int16_t _distTest = 0;
uint8_t _targetStance = 0;
uint8_t _action = 0;
uint16_t _pos = 0;
#endif

Entity player = { STANCE_DEFAULT, 10, 0, 0, 55, 0, HEALTH_STARTING_POINTS, 0, HEALTH_STARTING_POINTS, 0, true, true, false };
Entity enemy = { STANCE_DEFAULT, 153, 0, 0, 55, 0, HEALTH_STARTING_POINTS, 0, HEALTH_STARTING_POINTS, 0, true, true, false };

bool enemyImmediateAction = false;
bool enemyImmediateRetreat = false;



// ---------------------------------------------------------------------------------------------------------------
//  Setup
// ---------------------------------------------------------------------------------------------------------------

void setup() {

  arduboy.boot();
  arduboy.safeMode(); 
  arduboy.setFrameRate(23);
  arduboy.initRandomSeed();
  
  #ifdef SOUNDS_ATMLIB
  arduboy.audio.on;
  #endif

  gameStateDetails.setCurrState(GAME_STATE_FOLLOW_SEQUENCE);

}


// ---------------------------------------------------------------------------------------------------------------
//  Main Loop ..
// ---------------------------------------------------------------------------------------------------------------

void loop() {

  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();

  switch (gameStateDetails.getCurrState()) {

    case GAME_STATE_TITLE_SCENE:

      player = { STANCE_DEFAULT, 10, 0, 0, 55, 0, HEALTH_STARTING_POINTS, 0, HEALTH_STARTING_POINTS, 0, true, true, false };
      enemy = { STANCE_DEFAULT, 153, 0, 0, 55, 0, HEALTH_STARTING_POINTS, 0, HEALTH_STARTING_POINTS, 0, true, true, false };
      arduboy.clear();
      showScene();
      break;

    case GAME_STATE_FOLLOW_SEQUENCE:
      {
        gameStateDetails.setCurrState(pgm_read_byte(&gameSequence[ (gameStateDetails.sequence * GAME_STATE_SEQ_SIZE) ])); 
        gameStateDetails.delayInterval = pgm_read_byte(&gameSequence[ (gameStateDetails.sequence * GAME_STATE_SEQ_SIZE) + 1]); 
      
        uint8_t arches = pgm_read_byte(&gameSequence[ (gameStateDetails.sequence * GAME_STATE_SEQ_SIZE) + 2]);
        gameStateDetails.intArch = (arches <= 2 ? arches : 0); 
        gameStateDetails.extArch = (arches >= 3 ? arches - 2 : 0); 

        if (arches == 0) {

          gameStateDetails.archXPos = 0;
          gameStateDetails.enemyType = pgm_read_byte(&gameSequence[ (gameStateDetails.sequence * GAME_STATE_SEQ_SIZE) + 3]); 
          
        }
        else {

          gameStateDetails.enemyType = 0;
          if (arches % 2 == 1) {
            gameStateDetails.archXPos = pgm_read_byte(&gameSequence[ (gameStateDetails.sequence * GAME_STATE_SEQ_SIZE) + 3]);
          }
          else {
            gameStateDetails.archXPos = (uint16_t)WIDTH - (uint16_t)pgm_read_byte(&gameSequence[ (gameStateDetails.sequence * GAME_STATE_SEQ_SIZE) + 3]);
          }
          
        }

        gameStateDetails.showCrevice = (pgm_read_byte(&gameSequence[ (gameStateDetails.sequence * GAME_STATE_SEQ_SIZE) + 4]) == 1); 
        gameStateDetails.hasDelay = (gameStateDetails.delayInterval > 0);
        gameStateDetails.activity = pgm_read_byte(&gameSequence[ (gameStateDetails.sequence * GAME_STATE_SEQ_SIZE) + 5]);
        gameStateDetails.sequence++;

      }
      break;
      
    case GAME_STATE_JM_SCENE:
    case GAME_STATE_CASTLE_SCENE:
    case GAME_STATE_THE_END:
    case GAME_STATE_ENEMY_APPROACH:
    case GAME_STATE_PRINCESS:
      arduboy.clear();
      showScene();
      break;

    case GAME_STATE_EMPEROR_INIT:
      playerStack.clear();
      enemyStack.clear();
      emperorMode = EMPEROR_MODE_INIT;

      enemy = { STANCE_DEFAULT, 153, 0, 0, 55, 0, HEALTH_STARTING_POINTS, 0, HEALTH_STARTING_POINTS, 0, true, true, false };
      gameStateDetails.setCurrState(GAME_STATE_EMPEROR);
      outside = false;
      break;

    case GAME_STATE_EMPEROR:
      arduboy.clear();
      emperor_loop();
      break;
 
    case GAME_STATE_ENTRANCE_INIT:
      playerStack.clear();
      enemyStack.clear();
      gameStateDetails.setCurrState(GAME_STATE_ENTRANCE);
      player.stance = STANCE_ENTRANCE_1;
      enemy.xPos = 140;
      break;

    case GAME_STATE_ENTRANCE:

      arduboy.clear();
      gameStateDetails.setCurrState(GAME_STATE_FOLLOW_SEQUENCE);
      player.stance = STANCE_ENTRANCE_1;

      playerStack.push(STANCE_STANDING_UPRIGHT, STANCE_ENTRANCE_6, STANCE_ENTRANCE_5);
      playerStack.push(STANCE_ENTRANCE_4, STANCE_ENTRANCE_3, STANCE_ENTRANCE_2);
      playerStack.push(STANCE_ENTRANCE_1);

      break;

    case GAME_STATE_PLAY_INIT:
      
      gameStateDetails.setCurrState(GAME_STATE_PLAY);
      
      player.xPos = 10;
      player.xPosOverall = 0;
      
      if (gameStateDetails.enemyType == ENEMY_TYPE_PERSON) {
        if (player.health - 10 < HEALTH_STARTING_POINTS) { player.health = player.health + 10; }
        if (player.health - 10 < HEALTH_STARTING_POINTS) { player.health = player.health + 10; }
      }

      player.regainLimit = (enemy.health < HEALTH_STARTING_POINTS - (2 * HEALTH_UNIT) ? player.health + (2 * HEALTH_UNIT) : HEALTH_STARTING_POINTS);
      enemy = { STANCE_DEFAULT, (gameStateDetails.enemyType == ENEMY_TYPE_EAGLE ? 170 : 140), 0, 0, 55, 0, HEALTH_STARTING_POINTS, 0, 0, true, true, false };
      enemy.health = ((HEALTH_STARTING_POINTS * 2) - player.health  > HEALTH_MAX_POINTS ? HEALTH_MAX_POINTS : (HEALTH_STARTING_POINTS * 2) - player.health);
      enemy.regainLimit = enemy.health;
      eagleMode = EAGLE_MODE_FLY_INIT;
      break;

    case GAME_STATE_PLAY:
      arduboy.clear();
      play_loop();
      break;

    case GAME_STATE_ENEMY_APPROACH_INIT:
      playerStack.clear();
      enemyStack.clear();
      enemy_approach_init();
      break;

    case GAME_STATE_GO_THROUGH_GATE:
      playerStack.clear();
      enemyStack.clear();
      player.xPosOverall = -30;
      gameStateDetails.setCurrState(GAME_STATE_PLAY);
      break;
      
    case GAME_STATE_FINAL_SCENE:
      arduboy.clear();
      finalScene();
      break;

  }

  arduboy.display();
  
}

  
// ---------------------------------------------------------------------------------------------------------------
//  Draw the background ..
// ---------------------------------------------------------------------------------------------------------------

void draw_background() {

  const uint8_t *backdrop_img = (outdoors ? backdrop: backdrop2);

  arduboy.drawFastHLine(0, 47, WIDTH, WHITE);
  
  for (int i = 49; i < 58; i++) {
    drawHorizontalDottedLine(i % 2, WIDTH, i);
  }

  if (gameStateDetails.showCrevice) {
    arduboy.drawCompressedMirror(-player.xPosOverall, 0, crevice_mask, BLACK, false);
    arduboy.drawCompressedMirror(-player.xPosOverall, 0, crevice, WHITE, false);
  }
  
  if (outside) {
    arduboy.drawCompressedMirror(mainSceneX, -2, backdrop_img, WHITE, false);
    arduboy.drawCompressedMirror(mainSceneX + MAIN_SCENE_IMG_WIDTH, -2, backdrop_img, WHITE, false);
    arduboy.drawCompressedMirror(mainSceneX + (2 * MAIN_SCENE_IMG_WIDTH), -2, backdrop_img, WHITE, false);
    arduboy.drawCompressedMirror(mainSceneX + (3 * MAIN_SCENE_IMG_WIDTH), -2, backdrop_img, WHITE, false);
  }
  
  
  // Draw player triangles ..

  if (arduboy.everyXFrames(ANIMATION_FLASHING_TRIANGLES)) displayHealth = !displayHealth;

  if (player.health > 20 || displayHealth) {
    for (uint8_t i = 0; i < (player.health / 10); i++) {
      arduboy.drawCompressedMirror((i * 4), 59, arrow, WHITE, false);
    }
  }


  // Draw enemy triangles ..

  if (gameStateDetails.enemyType == ENEMY_TYPE_PERSON) {

    if (enemy.health > 20 || displayHealth) {
        
      for (uint8_t i = (enemy.health / 10); i > 0; i--) {
        arduboy.drawCompressedMirror(129 - (i * 4), 59, arrow, WHITE, true);
      }

    }

  }

}


// ---------------------------------------------------------------------------------------------------------------
//  Play Karateka !
// ---------------------------------------------------------------------------------------------------------------

void play_loop() {

  draw_background();
  if (gameStateDetails.hasDelay && gameStateDetails.delayInterval == 0 && playerStack.isEmpty())   { 

    gameStateDetails.setCurrState(GAME_STATE_FOLLOW_SEQUENCE); 

  }
  else {

    playerMovements();

  }

  switch (gameStateDetails.enemyType) {
  
    case ENEMY_TYPE_PERSON:
      enemyMovements();
      break;
  
    case ENEMY_TYPE_EAGLE:
      eagleMovements();
      break;

  }



  // Has the player or enemy been hit ?
 
  uint8_t player_BamX = 0;
  uint8_t player_BamY = 0;

  uint8_t enemy_BamX = 0;
  uint8_t enemy_BamY = 0;

  #ifdef USE_DIFFERENT_BAMS
  const int8_t player_BamXPos[] = {  32,  31,  33,            33,  33,  34,  34,  29,  29 };
  const int8_t enemy_BamXPos[] =  { -17, -17, -19,           -19, -19, -18, -18, -18, -18 };
  const int8_t both_BamYPos[] =   { -30, -17, -42,           -37, -37, -41, -41, -27, -27 };
  #endif
  //                           Kick   M,   L,   H     Punch   MR,  ML,  HR,  HL,  LR,  LL     
  #ifndef USE_DIFFERENT_BAMS
  const int8_t player_BamXPos[] = {  27,  25,  30,            30,  30,  30,  30,  23,  23 };
  const int8_t enemy_BamXPos[] =  { -20, -23, -19,           -22, -22, -18, -18, -24, -24 };
  const int8_t both_BamYPos[] =   { -33, -21, -42,           -39, -39, -45, -45, -31, -31 };
  #endif

  if (arduboy.everyXFrames(ANIMATION_NUMBER_OF_FRAMES)) {
    
    enemyHit = 0;
    playerHit = 0;

    if (gameStateDetails.hasDelay && gameStateDetails.delayInterval > 0)      { gameStateDetails.delayInterval--; }


    // Update the player and enemy stances from the stack ..

    if (!playerStack.isEmpty()) {
      player.stance = playerStack.pop();
    }
    else {
      player.xPosDelta = 0;
    }
        
    if (!enemyStack.isEmpty()) {
      enemy.stance = enemyStack.pop();
    }
    else {
      enemy.xPosDelta = 0;
    }


    // If we are fighting, check to see if a strike has been made ..

    if (gameStateDetails.enemyType != ENEMY_TYPE_NONE) {

      enemyImmediateAction = false;

      if (player.stance >= STANCE_KICK_MED_END && player.stance <= STANCE_PUNCH_LOW_LH_END) {

        enemyHit = inStrikingRange(getActionFromStance(player.stance), player.xPos, gameStateDetails.enemyType, enemy.stance, enemy.xPos);

      }

      if (enemy.stance >= STANCE_KICK_MED_END && enemy.stance <= STANCE_PUNCH_LOW_LH_END) {
    
        playerHit = inStrikingRange(getActionFromStance(enemy.stance), enemy.xPos, ENEMY_TYPE_PERSON, player.stance, player.xPos);

      }
      if (playerHit > 0 || enemyHit > 0) {


        #ifdef SOUNDS_ARDUBOYTONES
        if (arduboy.everyXFrames(ANIMATION_NUMBER_OF_FRAMES)) {
          sound.tones(ouch);
        }
        #endif 

        #ifdef SOUNDS_ATMLIB
        if (arduboy.everyXFrames(ANIMATION_NUMBER_OF_FRAMES)) {
          ATM.play(ouch);
        }
        #endif 

      }

    }

  }


  // Render the background, acrhes and the players ..

  if (player.xPosDelta != 0) {

    int16_t archXPos = gameStateDetails.archXPos;

    if ((gameStateDetails.extArch == 0 && gameStateDetails.intArch == 0) || 
        (gameStateDetails.extArch == ARCH_RIGHT_HAND && archXPos > 96) ||
        (gameStateDetails.intArch == ARCH_RIGHT_HAND && archXPos > 96) ||
        (gameStateDetails.extArch == ARCH_LEFT_HAND && archXPos < 30) ||
        (gameStateDetails.intArch == ARCH_LEFT_HAND && archXPos < 30)
    ) {

      mainSceneX = mainSceneX + player.xPosDelta;
      player.xPosOverall = player.xPosOverall - player.xPosDelta;
      enemy.xPos = enemy.xPos + player.xPosDelta;
      gameStateDetails.archXPos = archXPos + player.xPosDelta;

      if (mainSceneX < -MAIN_SCENE_IMG_WIDTH) { mainSceneX = 0; }
      if (mainSceneX > 0) { mainSceneX = mainSceneX - MAIN_SCENE_IMG_WIDTH; }

    }
    else {

      player.xPos = player.xPos - player.xPosDelta;
      player.xPosOverall = player.xPosOverall - player.xPosDelta;

    }

  }

  if (gameStateDetails.extArch == ARCH_RIGHT_HAND) {
    arduboy.drawCompressedMirror(gameStateDetails.archXPos, -2, arch_exterior_lh_mask, BLACK, false);
    arduboy.drawCompressedMirror(gameStateDetails.archXPos, -2, arch_exterior_lh, WHITE, false);
  }

  if (gameStateDetails.extArch == ARCH_LEFT_HAND) {
    arduboy.drawCompressedMirror(gameStateDetails.archXPos + 16, -2, arch_exterior_lh_mask, BLACK, true);
    arduboy.drawCompressedMirror(gameStateDetails.archXPos + 16, -2, arch_exterior_lh, WHITE, true);
  }

  if (gameStateDetails.intArch == ARCH_RIGHT_HAND) {
    arduboy.drawCompressedMirror(gameStateDetails.archXPos + 2, 7, arch_interior_rh_mask, BLACK, true);
    arduboy.drawCompressedMirror(gameStateDetails.archXPos + 2, 7, arch_interior_rh, WHITE, true);
  }

  if (enemy.xPosDelta != 0) { enemy.xPos = enemy.xPos + enemy.xPosDelta; }
  renderPlayerStance(arduboy, player.xPos, player.yPos, player.stance);
  if (gameStateDetails.enemyType != ENEMY_TYPE_NONE && enemy.xPos < 128) renderEnemyStance(arduboy, enemy.xPos, enemy.yPos, enemy.stance);
  if (gameStateDetails.prevState == GAME_STATE_GO_THROUGH_GATE) renderEnemyStance(arduboy, enemy.xPos, enemy.yPos, STANCE_DEATH_6); 
  
  if (gameStateDetails.extArch == ARCH_RIGHT_HAND) {
    arduboy.drawCompressedMirror(gameStateDetails.archXPos + 17, -2, arch_exterior_rh_mask, BLACK, false);
    arduboy.drawCompressedMirror(gameStateDetails.archXPos + 17, -2, arch_exterior_rh, WHITE, false);
  }
  
  if (gameStateDetails.extArch == ARCH_LEFT_HAND) {
    arduboy.drawCompressedMirror(gameStateDetails.archXPos - 6, -2, arch_exterior_rh_mask, BLACK, true);
    arduboy.drawCompressedMirror(gameStateDetails.archXPos - 6, -2, arch_exterior_rh, WHITE, true);
  }
  
  if (gameStateDetails.intArch == ARCH_RIGHT_HAND) {
    arduboy.drawCompressedMirror(gameStateDetails.archXPos + 16, 3, arch_interior_lh_mask, BLACK, true);
    arduboy.drawCompressedMirror(gameStateDetails.archXPos + 16, 3, arch_interior_lh, WHITE, true);
  }



  //  If the player or enemy has previously been hit, then update their health and render ..

  if (playerHit > 0 || enemyHit > 0) {

    if (playerHit > 0) {

      uint8_t index = (playerHit > 3 ? 2 : playerHit - 1);

      player_BamX = (enemy.xPos + enemy_BamXPos[enemy.stance - STANCE_KICK_MED_END]); 
      if (player_BamX > 128) { player_BamX = 0; }
      player_BamY = enemy.yPos + both_BamYPos[enemy.stance - STANCE_KICK_MED_END]; 

      #ifndef DEBUG_HIDE_BAM

        #ifdef USE_DIFFERENT_BAMS
        arduboy.drawCompressedMirror(player_BamX, player_BamY, bam_masks[index], BLACK, false);
        arduboy.drawCompressedMirror(player_BamX, player_BamY, bam_images[index], WHITE, false);
        #endif

        #ifndef USE_DIFFERENT_BAMS
        arduboy.drawCompressedMirror(player_BamX, player_BamY, bam3_mask, BLACK, false);
        arduboy.drawCompressedMirror(player_BamX, player_BamY, bam3, WHITE, false);
        #endif

      #endif

      player.health = (player.health - playerHit < 10 ? 0 : player.health - playerHit);
      player.regain = 0;

    }
    
    if (enemyHit > 0) {

      uint8_t index = (enemyHit > 3 ? 2 : enemyHit - 1);
      enemy_BamX = player.xPos + player_BamXPos[player.stance - STANCE_KICK_MED_END] ; enemy_BamY = player.yPos + both_BamYPos[player.stance - STANCE_KICK_MED_END]; 

      #ifdef USE_DIFFERENT_BAMS
        arduboy.drawCompressedMirror(enemy_BamX, enemy_BamY, bam_masks[index], BLACK, false);
        arduboy.drawCompressedMirror(enemy_BamX, enemy_BamY, bam_images[index], WHITE, false);
      #endif

      #ifndef USE_DIFFERENT_BAMS
        arduboy.drawCompressedMirror(enemy_BamX, enemy_BamY, bam3_mask, BLACK, false);
        arduboy.drawCompressedMirror(enemy_BamX, enemy_BamY, bam3, WHITE, false);
      #endif

      if (gameStateDetails.enemyType == ENEMY_TYPE_PERSON) {

        enemy.health = (enemy.health - enemyHit < 10 ? 0 : enemy.health - enemyHit);
        enemy.regain = 0;

        if (enemyStack.isEmpty()) {

            enemyImmediateAction = (random(0, 2) == 0);     // Should the enemy take an immediate action?
            enemyImmediateRetreat = (random(0, 3) == 0);    // Should the enemy retreat immediately?
          
        }
      
      }
      else {

        eagleMode = EAGLE_MODE_FLY_AWAY;

      }

    }

  }
  else {


    // Let the player and enemy regain some health if they haven't been hit in a while ..

    if (playerHit == 0) {

      player.regain++;

      if (player.regain >= HEALTH_REGAIN_LIMIT) {

        player.health = (player.health + HEALTH_REGAIN_POINTS < player.regainLimit ? player.health + HEALTH_REGAIN_POINTS : player.regainLimit);
        player.regain = 0;

      }
    
    }

    if (enemyHit == 0) {

      enemy.regain++;

      if (enemy.regain >= HEALTH_REGAIN_LIMIT) {

        enemy.health = (enemy.health + HEALTH_REGAIN_POINTS < enemy.regainLimit ? enemy.health + HEALTH_REGAIN_POINTS : enemy.regainLimit);
        enemy.regain = 0;
                
      }
    
    }

  }


  // Has the player died ?

  if (!player.dead && player.health == 0) {

    playerStack.clear();
    playerStack.push(STANCE_DEATH_6, STANCE_DEATH_5, STANCE_DEATH_4);
    playerStack.push(STANCE_DEATH_3, STANCE_DEATH_2, STANCE_DEATH_1);
    player.dead = true;

    if (eagleMode == EAGLE_MODE_NONE || eagleMode == EAGLE_MODE_FLY_INIT) {

      enemyStack.insert(STANCE_DEFAULT_LEAN_BACK);
      for (int i = 0; i < 20; i++) {
        enemyStack.insert(STANCE_STANDING_UPRIGHT);
      }

      enemy.xPosDelta = 0;
      
    }

    player.xPosDelta = 0;
    
  }


  // Has the enemy died ?

  if (!enemy.dead && enemy.health == 0) {
    
    enemyStack.clear();
    enemyStack.push(STANCE_DEATH_6, STANCE_DEATH_5, STANCE_DEATH_4);
    enemyStack.push(STANCE_DEATH_3, STANCE_DEATH_2, STANCE_DEATH_1);
    enemy.dead = true;

    playerStack.insert(STANCE_DEFAULT_LEAN_BACK);
    for (int i = 0; i < 15; i++) {
      playerStack.insert(STANCE_STANDING_UPRIGHT);
    }

    player.xPosDelta = 0;
    enemy.xPosDelta = 0;
    
  }


  if (gameStateDetails.enemyType != ENEMY_TYPE_NONE && playerStack.isEmpty() && enemyStack.isEmpty() && player.dead)    { gameStateDetails.setCurrState(GAME_STATE_THE_END); }
  if (gameStateDetails.enemyType == ENEMY_TYPE_PERSON && playerStack.isEmpty() && enemyStack.isEmpty() && enemy.dead)   { gameStateDetails.setCurrState(GAME_STATE_FOLLOW_SEQUENCE); }
  
  if (gameStateDetails.prevState == GAME_STATE_GO_THROUGH_GATE && player.xPos > 128 && playerStack.isEmpty()) {
    gameStateDetails.setCurrState(GAME_STATE_FOLLOW_SEQUENCE);
  }

  #ifdef DEBUG_ONSCREEN
    int16_t distBetween = absT(enemy.xPos - player.xPos);
    arduboy.fillRect(0, 0, 75, 10, BLACK);
    arduboy.setCursor(1, 1);
    arduboy.print(distBetween);
    arduboy.setCursor(25, 1);
    arduboy.print(enemyHit);
    arduboy.setCursor(35, 1);
    arduboy.print(playerHit);
    arduboy.setCursor(45, 1);
    arduboy.print(player.stance);
    arduboy.setCursor(60, 1);
    arduboy.print(enemy.stance);
  #endif

  #ifdef DEBUG_HITS
    int16_t distBetween = absT(enemy.xPos - player.xPos);
    arduboy.fillRect(0, 0, WIDTH, 10, BLACK);
    arduboy.setCursor(1, 1);
    arduboy.print(_distBetween);
    arduboy.setCursor(15, 1);
    arduboy.print(_distTest);
    arduboy.setCursor(30, 1);
    arduboy.print(enemyHit);
    arduboy.setCursor(40, 1);
    arduboy.print(playerHit);
    arduboy.setCursor(50, 1);
    arduboy.print(_targetStance);
    arduboy.setCursor(65, 1);
    arduboy.print(_pos);
    arduboy.setCursor(85, 1);
    arduboy.print(_action);
  #endif

  #ifdef DEBUG_STRIKING_RANGE
    int16_t distBetween = absT(enemy.xPos - player.xPos);
    arduboy.fillRect(0, 0, WIDTH, 10, BLACK);
    arduboy.setCursor(1, 1);
    arduboy.print(_distBetween);
    arduboy.setCursor(15, 1);
    arduboy.print(_distTest);
    arduboy.setCursor(30, 1);
    arduboy.print(enemyHit);
    arduboy.setCursor(40, 1);
    arduboy.print(playerHit);
    arduboy.setCursor(50, 1);
    arduboy.print(_targetStance);
    arduboy.setCursor(65, 1);
    arduboy.print(_pos);
    arduboy.setCursor(85, 1);
    arduboy.print(_action);
  #endif
  
}
