#pragma once

#define _DEBUG
#define _DEBUG_SERIAL
#define _DEBUG_ONSCREEN
#define _DEBUG_HITS
#define _DEBUG_HIDE_BAM
#define _DEBUG_STRIKING_RANGE

#define JORDAN_MECHNER
#define _SOUNDS_ARDUBOYTONES
#define _SOUNDS_ATMLIB
#define _USE_DIFFERENT_BAMS
#define USE_ENHANCED_BAM3

#define GAME_STATE_TITLE_SCENE                    0
#define GAME_STATE_JM_SCENE                       1
#define GAME_STATE_CASTLE_SCENE                   2
#define GAME_STATE_EMPEROR_INIT                   3
#define GAME_STATE_EMPEROR                        4
#define GAME_STATE_PLAY_INIT                      5
#define GAME_STATE_FOLLOW_SEQUENCE                6
#define GAME_STATE_PLAY                           7
#define GAME_STATE_ENEMY_APPROACH_INIT            8
#define GAME_STATE_ENEMY_APPROACH                 9
#define GAME_STATE_EAGLE_APPROACH_INIT            10
#define GAME_STATE_EAGLE_APPROACH                 11
#define GAME_STATE_THE_END                        12
#define GAME_STATE_GO_THROUGH_GATE                13
#define GAME_STATE_ENTRANCE                       14
#define GAME_STATE_ENTRANCE_INIT                  15
#define GAME_STATE_PRINCESS_INIT                  16
#define GAME_STATE_PRINCESS                       17
#define GAME_STATE_FINAL_SCENE                    18

#define GAME_STATE_SEQ_SIZE                       6

struct Entity {
  uint8_t stance;
  int16_t xPos;
  int8_t xPosDelta;
  int16_t xPosOverall;
  uint8_t yPos;
  int16_t yPosDelta;
  uint8_t health;
  uint8_t regain;
  uint8_t regainLimit;
  uint8_t activity;        // For enemies, defines how active they are (used in random actions). 
                           // For player, defines the number of kicks or punches they have taken from the kick / punch ready position.
  bool rightPunch;
  bool rightFoot;
  bool dead;
};

struct GameStateDetails {

  public:

    uint8_t getCurrState() {
      return _currState;
    }

    void setCurrState(uint8_t value) {
      prevState = _currState;
      _currState = value;
    }

    bool hasDelay;
    uint8_t delayInterval;
    uint8_t intArch;
    uint8_t extArch;
    uint8_t enemyType;
    bool showCrevice;
    int16_t archXPos;
    uint8_t prevState;
    uint8_t activity;
    uint16_t sequence;    

  private:
    uint8_t _currState;

};

const uint8_t PROGMEM gameSequence[] = {
  
  //                                      Delay   IntArch     Launch      Show        Enemy               
  //                                              1 = LH      Enemies     Crevice     Activity            
  //                                              2 = RH      1=Person                100 Slow / 0 Fast   
  //                                              ExtArch     2=Eagle                                     
  //                                              3 = LH                  
  //                                              4 = RH                  
  
  GAME_STATE_TITLE_SCENE,                 0,      0,          0,          0,          0,
  #ifdef JORDAN_MECHNER
  GAME_STATE_JM_SCENE,                    15,     0,          0,          0,          0,
  #endif
  GAME_STATE_CASTLE_SCENE,                15,     0,          0,          0,          0,
  GAME_STATE_ENTRANCE_INIT,               0,      0,          0,          1,          0,
  GAME_STATE_PLAY_INIT,                   25,     0,          0,          1,          0,
  GAME_STATE_ENEMY_APPROACH_INIT,         0,      0,          0,          0,          0,
  GAME_STATE_PLAY_INIT,                   0,      0,          1,          0,        130,
  GAME_STATE_ENEMY_APPROACH_INIT,         0,      0,          0,          0,          0,
  GAME_STATE_PLAY_INIT,                   0,      0,          1,          0,        125,
  GAME_STATE_GO_THROUGH_GATE,             0,      4,          1,          0,          0,
  GAME_STATE_PLAY_INIT,                   25,     3,          0,          0,        120,
  GAME_STATE_ENEMY_APPROACH_INIT,         0,      0,          0,          0,          0,
  GAME_STATE_PLAY_INIT,                   0,      0,          1,          0,        115,
  GAME_STATE_ENEMY_APPROACH_INIT,         0,      0,          0,          0,          0,
  GAME_STATE_PLAY_INIT,                   0,      0,          1,          0,        110,
  GAME_STATE_ENEMY_APPROACH_INIT,         0,      0,          0,          0,          0,
  GAME_STATE_PLAY_INIT,                   0,      0,          1,          0,        105,
  GAME_STATE_GO_THROUGH_GATE,             0,      2,          0,          0,          0,
  GAME_STATE_PRINCESS,                    15,     0,          0,          1,          0,
  GAME_STATE_EMPEROR_INIT,                0,      0,          0,          0,          0,
  GAME_STATE_PLAY_INIT,                   0,      0,          2,          0,          0,
  GAME_STATE_PLAY_INIT,                   0,      0,          2,          0,          0,
  GAME_STATE_PLAY_INIT,                   0,      0,          1,          0,        100,
  GAME_STATE_PLAY_INIT,                   0,      0,          2,          0,          0,
  GAME_STATE_PLAY_INIT,                   0,      0,          1,          0,         95,
  GAME_STATE_PLAY_INIT,                   0,      0,          2,          0,          0,
  GAME_STATE_PLAY_INIT,                   0,      0,          2,          0,          0,
  GAME_STATE_PLAY_INIT,                   0,      0,          1,          0,         80,
  GAME_STATE_PLAY_INIT,                   0,      0,          2,          0,          0,
  GAME_STATE_GO_THROUGH_GATE,             0,      2,          0,          0,         75,
  GAME_STATE_FINAL_SCENE,                 0,      0,          0,          0,          0,
  GAME_STATE_THE_END,                     0,      0,          0,          0,          0,
  
};

//#define UP_BUTTON_MASK                            128
//#define DOWN_BUTTON_MASK                          16
//#define LEFT_BUTTON_MASK                          32
//#define RIGHT_BUTTON_MASK                         64
//#define A_BUTTON_MASK                             8
//#define B_BUTTON_MASK                             4

#define UP_BUTTON_MASK                            8
#define DOWN_BUTTON_MASK                          4
#define LEFT_BUTTON_MASK                          32
#define RIGHT_BUTTON_MASK                         16
#define A_BUTTON_MASK                             2
#define B_BUTTON_MASK                             1

#define ANIMATION_NUMBER_OF_FRAMES                5
#define ANIMATION_FLASHING_TRIANGLES              12

#define MAIN_SCENE_X_SIDLING_1_DELTA              1
#define MAIN_SCENE_X_SIDLING_2_DELTA              2
#define MAIN_SCENE_X_SIDLING_3_DELTA              3
#define MAIN_SCENE_X_RUNNING_DELTA                6
#define MAIN_SCENE_X_EAGLE_FLYING_DELTA           4
#define MAIN_SCENE_IMG_WIDTH                      48

#define STANCE_DEFAULT                            0
#define STANCE_DEFAULT_LEAN_BACK                  1
#define STANCE_KICK_READY                         2
#define STANCE_DEFAULT_LEAN_FORWARD               3
#define STANCE_KICK_STANDING_TRANSITION           4
#define STANCE_KICK_MED_END                       5
#define STANCE_KICK_LOW_END                       6
#define STANCE_KICK_HIGH_END                      7

#define STANCE_PUNCH_MED_RH_END                   8
#define STANCE_PUNCH_MED_LH_END                   9 
#define STANCE_PUNCH_HIGH_RH_END                  10
#define STANCE_PUNCH_HIGH_LH_END                  11
#define STANCE_PUNCH_LOW_RH_END                   12
#define STANCE_PUNCH_LOW_LH_END                   13
#define STANCE_PUNCH_READY                        14

#define STANCE_STANDING_UPRIGHT                   15

#define STANCE_SIDLING_1                          16
#define STANCE_SIDLING_2                          17
#define STANCE_SIDLING_3                          18
#define STANCE_SIDLING_4                          19
#define STANCE_SIDLING_5                          20
#define STANCE_SIDLING_6                          21
#define STANCE_SIDLING_7                          22

#define STANCE_RUNNING_1                          23
#define STANCE_RUNNING_2                          24
#define STANCE_RUNNING_3                          25
#define STANCE_RUNNING_4                          26
#define STANCE_RUNNING_5                          27
#define STANCE_RUNNING_6                          28
#define STANCE_RUNNING_7                          29
#define STANCE_RUNNING_8                          30
#define STANCE_RUNNING_STRAIGHTEN_UP              31
#define STANCE_RUNNING_LF_END                     STANCE_RUNNING_3
#define STANCE_RUNNING_RF_END                     STANCE_RUNNING_6

/* The following are not mapped for a player */
#define STANCE_STANDING_UPRIGHT_REV               32
#define STANCE_RUNNING_1_REV                      33
#define STANCE_RUNNING_2_REV                      34
#define STANCE_RUNNING_3_REV                      35
#define STANCE_RUNNING_4_REV                      36
#define STANCE_RUNNING_5_REV                      37
#define STANCE_RUNNING_6_REV                      38
#define STANCE_RUNNING_7_REV                      39
#define STANCE_RUNNING_8_REV                      40
#define STANCE_RUNNING_STRAIGHTEN_UP_REV          41
#define STANCE_RUNNING_LF_END_REV                 STANCE_RUNNING_3_REV
#define STANCE_RUNNING_RF_END_REV                 STANCE_RUNNING_6_REV
/* .. only for enemies */


#define STANCE_BOW_1                              42
#define STANCE_BOW_2                              43

#define STANCE_DEATH_1                            44
#define STANCE_DEATH_2                            45
#define STANCE_DEATH_3                            46
#define STANCE_DEATH_4                            47
#define STANCE_DEATH_5                            48
#define STANCE_DEATH_6                            49

#define STANCE_ENTRANCE_1                         50
#define STANCE_ENTRANCE_2                         51
#define STANCE_ENTRANCE_3                         52
#define STANCE_ENTRANCE_4                         53
#define STANCE_ENTRANCE_5                         54
#define STANCE_ENTRANCE_6                         55

#define STANCE_EAGLE_1                            56
#define STANCE_EAGLE_2                            57
#define STANCE_EAGLE_3                            58
#define STANCE_EAGLE_4                            59

#define STANCE_EAGLE_1_REV                        60
#define STANCE_EAGLE_2_REV                        61
#define STANCE_EAGLE_3_REV                        62
#define STANCE_EAGLE_4_REV                        63

#define STANCE_PRINCESS_SITTING                   64
#define STANCE_PRINCESS_STANDING                  65
#define STANCE_PRINCESS_KISSING                   66

#define ACTION_MIN_KICK                           ACTION_MED_KICK
#define ACTION_MED_KICK                           0
#define ACTION_LOW_KICK                           1
#define ACTION_HIGH_KICK                          2
#define ACTION_MAX_KICK                           ACTION_HIGH_KICK
#define ACTION_MIN_PUNCH                          ACTION_MED_PUNCH
#define ACTION_MED_PUNCH                          3
#define ACTION_HIGH_PUNCH                         4
#define ACTION_LOW_PUNCH                          5
#define ACTION_MAX_PUNCH                          ACTION_LOW_PUNCH
#define ACTION_MAX                                ACTION_LOW_PUNCH
#define ACTION_RETURN_TO_DEFAULT                  6
#define ACTION_RETURN_TO_ACTION_READY             7

#define DISTANCE_TOO_CLOSE                        30
#define DISTANCE_TOO_FAR                          38
#define DISTANCE_BETWEEN_LRG                      80
#define DISTANCE_BETWEEN_MED                      55
#define DISTANCE_BETWEEN_SML                      35
#define DISTANCE_BETWEEN_TINY                     25

#define CHANCE_PERFORM_ACTION                     0
#define CHANCE_PERFORM_ACTION_REGARDLESS_MAX      4
#define CHANCE_MOVE_ENEMY_MAX                     10
 
#define EMPEROR_MODE_INIT                         0
#define EMPEROR_MODE_PAUSE_1                      1
#define EMPEROR_LIFT_ARM                          2
#define EMPEROR_MODE_PAUSE_2                      3
#define EMPEROR_MODE_BOW                          4
#define EMPEROR_MODE_PAUSE_3                      5
#define EMPEROR_MODE_FIGHTER_LEAVE                6

#define EAGLE_MODE_NONE                           0
#define EAGLE_MODE_FLY_INIT                       1
#define EAGLE_MODE_FLY_TOWARDS                    2
#define EAGLE_MODE_ATTACK_1                       3
#define EAGLE_MODE_ATTACK_2                       4
#define EAGLE_MODE_ATTACK_3                       5
#define EAGLE_MODE_FLY_AWAY                       6
#define EAGLE_MODE_DONE                           7

#define EAGLE_LEVEL_LOW                           40
#define EAGLE_LEVEL_MED                           27
#define EAGLE_LEVEL_HIGH                          19

#define EAGLE_LEVEL_LOW_HEALTH                    30
#define EAGLE_LEVEL_MED_HEALTH                    30
#define EAGLE_LEVEL_HIGH_HEALTH                   50

#define HEALTH_UNIT                               16
#define HEALTH_MAX_POINTS                         240
#define HEALTH_STARTING_POINTS                    160
#define HEALTH_REGAIN_POINTS                      3
#define HEALTH_REGAIN_LIMIT                       150

#define DAMAGE_1_POINT                            1
#define DAMAGE_2_POINT                            2
#define DAMAGE_3_POINT                            3
#define DAMAGE_MAX_POINTS                         0

#define ARCH_LEFT_HAND                            1
#define ARCH_RIGHT_HAND                           2

#define ENEMY_TYPE_NONE                           0
#define ENEMY_TYPE_PERSON                         1
#define ENEMY_TYPE_EAGLE                          2

#define FINAL_SCENE_INACTIVE                      0
#define FINAL_SCENE_INIT                          1
#define FINAL_SCENE_PAUSE                         2
#define FINAL_SCENE_PAUSE_2                       3
#define FINAL_SCENE_KISSING                       4
#define FINAL_SCENE_BREAK_UP                      5

struct RenderDetails {
  int8_t xOffset;
  int8_t yOffset;
  uint8_t index;
  bool rev;
};
