#include <Arduboy2.h>

#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Enums.h"
#include "src/utils/Name.h"
#include "src/images/Images.h"
#include "src/fonts/Font4x6.h"
#include "src/utils/EEPROM_Utils.h"

#ifndef DEBUG
//ARDUBOY_NO_USB
#endif

#ifdef SOUNDS
#include <ArduboyTones.h>
#endif

Arduboy2Ext arduboy;  
Font4x6 font4x6 = Font4x6();
GameMode gameMode;
TitleMode titleMode = TitleMode::PressA;

Name player0;
Name player1;
Name player2;

Name * players[3] = { &player0, &player1, &player2 };

#ifdef SOUNDS
  ArduboyTones sound(arduboy.audio.enabled);
#endif
                                                    // The 'bones_ref' array contains an element for each bone in a standard
                                                    // 28 bone set.  Arrays that represent players hands, bones available or 
                                                    // bones played all contain indexes that point to this array.

                            //    0     1     2     3     4     5     6
uint8_t bones_ref[BONES_COUNT] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
                            //    7     8     9    10    11    12  
                               0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 
                            //   13    14    15    16    17
                               0x22, 0x23, 0x24, 0x25, 0x26, 
                            //   18    19    20    21
                               0x33, 0x34, 0x35, 0x36, 
                            //   22    23    24
                               0x44, 0x45, 0x46, 
                            //   25    26
                               0x55, 0x56,
                            //   27
                               0x66};
                               
uint8_t bones_available[BONES_COUNT] = {0};            // Array of bones that the players can draw from. Each array value 
uint8_t bones_available_idx = 0;                       // points to an entry in the 'bones_ref' array which contains the 
                                                       // actual bone details (left and right pip values). The next available
                                                       // bone is indicated by the 'bones_available_idx' variable.

uint8_t bones_played[BONES_COUNT] = {0};               // Array of bones played. Again, each array value points to an entry
uint8_t bones_played_idx = 0;                          // in the 'bones_ref' array.

uint8_t players_hand[3][BONES_COUNT] = {0};            // Multi-dimensional array that holds the two player's hand.  Bones 
uint8_t players_hand_idx[3] = {0};                     // will fill the lowest positions in the array - when a bone is played
                                                       // from a hand, the bones are shuffled to the 'left' of the array.
                                                       // The 'players_hand_idx[]' indicates the number of bones in each hand.  

uint8_t players_score[3] = {0};                        // Players score.

                                                       // When making a move, the player must select a bone from their hand
                                                       // followed by a position on the board.  Alternatively, they can select
                                                       // a bone from the graveyard to add to their hand (although in reality
                                                       // they will always get the next bone from the 'bones_available' array.

uint8_t players_hand_visible_idx = 0;                  // Only six player's bones are visible at one time and the player can
                                                       // scroll through these.  The 'players_hand_visible_idx' defines which
                                                       // should be displayed in the left most position on the display.
uint8_t players_hand_highlight_idx = 0;                // Used to store the currently selected bone.
uint8_t graveyard_highlight_idx = NOTHING;             // Used to store the currently selected graveyard bone.
Position board_highlighted_idx = Position::Nothing;    // Used to store the currently seelcted board location.  

uint8_t boneCounts_Overall[7] = {0};                   // Used to count how many of each bone is visible on the board.
uint8_t boneCounts_Inhand[7] = {0};                    // Used to count how many of each bone is visible in the players hand.

uint8_t bone_n = NOTHING;                              // Stores the index reference (to bones_ref) for each board position.
uint8_t bone_s = NOTHING;
uint8_t bone_e = NOTHING;
uint8_t bone_w = NOTHING;
uint8_t bone_c = NOTHING;

uint8_t bone_n_pips_inner = NOTHING;                    // Stores the inner and outer PIP count for each board position.
uint8_t bone_s_pips_inner = NOTHING;
uint8_t bone_e_pips_inner = NOTHING;
uint8_t bone_w_pips_inner = NOTHING;
uint8_t bone_n_pips_outer = NOTHING;
uint8_t bone_s_pips_outer = NOTHING;
uint8_t bone_e_pips_outer = NOTHING;
uint8_t bone_w_pips_outer = NOTHING;
uint8_t bone_c_pips_inner = NOTHING;
uint8_t bone_c_pips_outer = NOTHING;

uint8_t boardMode = BOARD_MODE_NO_BONES_PLAYED;        // Stores the current mode of the board which is used to determine what
                                                       // board positions should be rendered and selectable when playing a bone.
                                                       // For example, when the boardMode is equal to 'NO_BONES_PLAYED' only the
                                                       // center bone of the board is rendered and selectable.

uint8_t frame = 0;
Player playersTurn = Player::Human_1;                  // Used to retain the players turn at the end of each round.

GameState gameState = GameState::SplashScreen;    
uint8_t playerBeingEdited = 1;

/* ----------------------------------------------------------------------------
 *  Initialise the Arduboy and get ready ..
 */
void setup() {

	arduboy.boot();
	arduboy.flashlight();
	arduboy.systemButtons();
	arduboy.initRandomSeed();

  EEPROM_Utils::initEEPROM(false);

  player0.setChar(0, 'C');
  player0.setChar(1, 'o');
  player0.setChar(2, 'm');
  player0.setChar(3, 'p');
  player0.setChar(4, 'u');
  player0.setChar(5, 't');
  player0.setChar(6, 'e');
  player0.setChar(7, 'r');
  player0.setChar(8, 0);

#ifdef SOUNDS
  arduboy.audio.begin();
#endif

}
    

/* ----------------------------------------------------------------------------
 *  Play dominoes !
 */
void loop() {

  if (!(arduboy.nextFrame())) return;
  
  arduboy.pollButtons();

  switch (gameState) {

    case GameState::SplashScreen:
      drawSplash_Loop();
      break;

    case GameState::TitleScreen:
      drawTitle_Loop();
      break;

    case GameState::HighScore:
      drawHighScore_Loop(nullptr);
      break;

    case GameState::GetNames:
      if (gameMode == GameMode::SinglePlayer) {
        playersTurn = Player::Human_1; 
      }
      else {
        playersTurn = Player::Human_2; 
      }
      playerBeingEdited = 1;
      getNames_Loop();
      break;

    case GameState::PlayGame:
      playGame_Loop();
      break;

    case GameState::PlayComputer:
      computersHand_Loop();
      break;

    case GameState::SwapPlayers:
      swapPlayers(false);
      break;

    case GameState::GameOver:
      gameState = GameState::HighScore;
      titleMode = TitleMode::PressA;
      if (gameMode == GameMode::SinglePlayer){
        drawHighScore_Loop(players[getAltPlayer(gameMode, playersTurn)]);
      }
      else {
        drawHighScore_Loop(players[playersTurn]);
      }

      break;

    default:
      playersHand_Loop(playersTurn, getAltPlayer(gameMode, playersTurn));
      break;

  }
      
}


/* ----------------------------------------------------------------------------
 *  Outer game loop.
 *  
 *  Game play continues until one player reaches 100 points.  
 */
void playGame_Loop() {

  while (gameState == GameState::PlayGame) {

    
    // Clear and reset the players hands, the available bones and the board ..
    
    memset(bones_available, 0, (sizeof(bones_available) / sizeof(bones_available[0])));
    memset(bones_played, 0, (sizeof(bones_played) / sizeof(bones_played[0])));

    boardMode = BOARD_MODE_NO_BONES_PLAYED;
    bones_available_idx = 0;
    bones_played_idx = 0; 
    players_hand_visible_idx = 0;
    
    memset(players_hand[0], 0, (sizeof(players_hand[0]) / sizeof(players_hand[0][0])));
    memset(players_hand_idx, 0, (sizeof(players_hand_idx) / sizeof(players_hand_idx[0])));
    
    players_hand_highlight_idx = 0;
    board_highlighted_idx = Position::Nothing;

    bone_n = NOTHING;
    bone_s = NOTHING;
    bone_e = NOTHING;
    bone_w = NOTHING;
    bone_c = NOTHING;

    bone_n_pips_inner = NOTHING;
    bone_s_pips_inner = NOTHING;
    bone_e_pips_inner = NOTHING;
    bone_w_pips_inner = NOTHING;
    bone_n_pips_outer = NOTHING;
    bone_s_pips_outer = NOTHING;
    bone_e_pips_outer = NOTHING;
    bone_w_pips_outer = NOTHING;
    bone_c_pips_inner = NOTHING;
    bone_c_pips_outer = NOTHING;
           

    // Shuffle and deal the bones ..

#ifdef DEBUG  
    randomSeed(3);          // Seed with a constant to enable debugging with a repeatable bone set. 
#else
    arduboy.initRandomSeed();
#endif

    shuffleBones();
    dealBones(gameMode);

#ifdef DEBUG_PLAYER_CANNOT_PLAY
    Serial.println("DEBUG_PLAYER_CANNOT_PLAY");
    bones_available_idx = 0;
    players_hand[static_cast<uint8_t>(Player::Computer)][0] = 26;  // 5:6
    players_hand[static_cast<uint8_t>(Player::Computer)][1] = 27;  // 6:6
    players_hand[static_cast<uint8_t>(Player::Human_1)][0] = 25;     // 5:5 
    players_hand[static_cast<uint8_t>(Player::Human_1)][1] = 0;      // 0:0
    players_hand_idx[static_cast<uint8_t>(Player::Computer)] = 2;
    players_hand_idx[static_cast<uint8_t>(Player::Human_1)] = 2;
    printHand();
#endif

#ifdef DEBUG_PLAYER_CAN_FINISH_GAME
    Serial.println("DEBUG_PLAYER_CAN_FINISH_GAME");
    bones_available_idx = 5;
    if (gameMode == GameMode::SinglePlayer) {
      players_hand[static_cast<uint8_t>(Player::Computer)][0] = 26;  // 5:6
      players_hand[static_cast<uint8_t>(Player::Computer)][1] = 27;  // 6:6
      players_hand_idx[static_cast<uint8_t>(Player::Computer)] = 2;
    }
    else {
      players_hand[static_cast<uint8_t>(Player::Human_2)][0] = 26;  // 5:6
      players_hand[static_cast<uint8_t>(Player::Human_2)][1] = 27;  // 6:6
      players_hand_idx[static_cast<uint8_t>(Player::Human_2)] = 2;
    }
    players_hand[static_cast<uint8_t>(Player::Human_1)][0] = 25;     // 5:5 
    players_hand_idx[static_cast<uint8_t>(Player::Human_1)] = 1;
    playBone(gameMode, gameMode == GameMode::SinglePlayer ? Player::Computer : Player::Human_2, 0, Position::Center);
    printHand();
#endif

#ifdef DEBUG_PLAYER_A_FEW_TILES_EACH
    Serial.println("DEBUG_PLAYER_A_FEW_TILES_EACH");
    bones_available_idx = 5;
    if (gameMode == GameMode::SinglePlayer) {
      players_hand[static_cast<uint8_t>(Player::Computer)][0] = 26;  // 5:6
      players_hand[static_cast<uint8_t>(Player::Computer)][1] = 27;  // 6:6
      players_hand[static_cast<uint8_t>(Player::Computer)][2] = 24;  // 4:6
      players_hand_idx[static_cast<uint8_t>(Player::Computer)] = 3;
    }
    else {
      players_hand[static_cast<uint8_t>(Player::Human_2)][0] = 26;  // 5:6
      players_hand[static_cast<uint8_t>(Player::Human_2)][1] = 27;  // 6:6
      players_hand[static_cast<uint8_t>(Player::Human_2)][2] = 24;  // 4:6
      players_hand_idx[static_cast<uint8_t>(Player::Human_2)] = 3;
    }
    players_hand[static_cast<uint8_t>(Player::Human_1)][0] = 25;     // 5:5 
    players_hand[static_cast<uint8_t>(Player::Human_1)][1] = 22;     // 4:$
    players_hand_idx[static_cast<uint8_t>(Player::Human_1)] = 2;
    playBone(gameMode, gameMode == GameMode::SinglePlayer ? Player::Computer : Player::Human_2, 0, Position::Center);
    printHand();
#endif
    
#ifdef DEBUG_COMPUTER_CAN_FINISH_GAME
    Serial.println("DEBUG_COMPUTER_CAN_FINISH_GAME");
    bones_available_idx = 5;
    if (gameMode == GameMode::SinglePlayer) {
      players_hand[static_cast<uint8_t>(Player::Computer)][0] = 25;  // 5:5
      players_hand_idx[static_cast<uint8_t>(Player::Computer)] = 1;
    }
    else {
      players_hand[static_cast<uint8_t>(Player::Human_2)][0] = 25;  // 5:5
      players_hand_idx[static_cast<uint8_t>(Player::Human_2)] = 1;
    }
    players_hand[static_cast<uint8_t>(Player::Human_1)][0] = 5;     // 5:0 
    players_hand[static_cast<uint8_t>(Player::Human_1)][1] = 27;     // 6:6 
    players_hand_idx[static_cast<uint8_t>(Player::Human_1)] = 2;
    printHand();
#endif
    
#ifdef DEBUG_NO_ONE_CAN_PLAY_1
    // first player plays 4:6 and then stalemate.
    Serial.println("DEBUG_NO_ONE_CAN_PLAY_1");
    bones_available_idx = 0;
    boardMode = BOARD_MODE_ONE_BONE_PLAYED;

    if (gameMode == GameMode::SinglePlayer) {
      players_hand[static_cast<uint8_t>(Player::Computer)][0] = 25;  // 5:5
      players_hand[static_cast<uint8_t>(Player::Computer)][1] = 20;  // 5:3
      players_hand_idx[static_cast<uint8_t>(Player::Computer)] = 2;
      players_score[static_cast<uint8_t>(Player::Computer)] = 30;
    }
    else {
      players_hand[static_cast<uint8_t>(Player::Human_2)][0] = 25;  // 5:5
      players_hand[static_cast<uint8_t>(Player::Human_2)][1] = 20;  // 5:3
      players_hand_idx[static_cast<uint8_t>(Player::Human_2)] = 2;
      players_score[static_cast<uint8_t>(Player::Human_2)] = 30;
    }
    players_hand[static_cast<uint8_t>(Player::Human_1)][0] = 9;     // 1:3 
    players_hand[static_cast<uint8_t>(Player::Human_1)][1] = 24;    // 4:6 
    players_hand_idx[static_cast<uint8_t>(Player::Human_1)] = 2;
    players_score[static_cast<uint8_t>(Player::Human_1)] = 50;
    bone_c = 4;
    bone_c_pips_inner = 0;
    bone_c_pips_outer = 4;
    printHand();
#endif
    
#ifdef DEBUG_NO_ONE_CAN_PLAY_2
    // first player has to pass, second player plays 4:6 and then stalemate.
    Serial.println("DEBUG_NO_ONE_CAN_PLAY_2");
    bones_available_idx = 0;
    boardMode = BOARD_MODE_ONE_BONE_PLAYED;

    if (gameMode == GameMode::SinglePlayer) {
      players_hand[static_cast<uint8_t>(Player::Computer)][0] = 9;  // 1:3
      players_hand[static_cast<uint8_t>(Player::Computer)][1] = 24;  // 4:6
      players_hand_idx[static_cast<uint8_t>(Player::Computer)] = 2;
      players_score[static_cast<uint8_t>(Player::Computer)] = 50;
    }
    else {
      players_hand[static_cast<uint8_t>(Player::Human_2)][0] = 9;  // 1:2
      players_hand[static_cast<uint8_t>(Player::Human_2)][1] = 24;  // 4:6
      players_hand_idx[static_cast<uint8_t>(Player::Human_2)] = 2;
      players_score[static_cast<uint8_t>(Player::Human_2)] = 50;
    }
    players_hand[static_cast<uint8_t>(Player::Human_1)][0] = 25;     // 5:5 
    players_hand[static_cast<uint8_t>(Player::Human_1)][1] = 20;    // 5:3
    players_hand_idx[static_cast<uint8_t>(Player::Human_1)] = 2;
    players_score[static_cast<uint8_t>(Player::Human_1)] = 30;
    bone_c = 4;
    bone_c_pips_inner = 0;
    bone_c_pips_outer = 4;
    printHand();
#endif
   
#ifdef DEBUG_CORRUPTED_GRAPHICS
    // first player has to pass, second player plays 4:6 and then stalemate.
    Serial.println("DEBUG_CORRUPTED_GRAPHICS");

    if (gameMode == GameMode::SinglePlayer) {
      players_hand[static_cast<uint8_t>(Player::Computer)][0] = 11;  // 5:1
    }
    else {
      players_hand[static_cast<uint8_t>(Player::Human_2)][0] = 11;  // 5:1
    }
    players_hand[static_cast<uint8_t>(Player::Human_1)][0] = 25;     // 5:5 
    printHand();
#endif
 
#ifdef DEBUG_ALLSIDES_CANNOT_PLAY
    // first player has to pass, second player plays 4:6 and then stalemate.
    // first player has to pass, second player plays 4:6 and then stalemate.
    Serial.println("DEBUG_ALLSIDES_CANNOT_PLAY");
    bones_available_idx = 0;
    boardMode = BOARD_MODE_X_AXIS_PLAYED;

    if (gameMode == GameMode::SinglePlayer) {
      players_hand[static_cast<uint8_t>(Player::Computer)][0] = 9;  // 1:3
      players_hand[static_cast<uint8_t>(Player::Computer)][1] = 24;  // 4:6
      players_hand_idx[static_cast<uint8_t>(Player::Computer)] = 2;
      players_score[static_cast<uint8_t>(Player::Computer)] = 50;
    }
    else {
      players_hand[static_cast<uint8_t>(Player::Human_2)][0] = 9;  // 1:2
      players_hand[static_cast<uint8_t>(Player::Human_2)][1] = 24;  // 4:6
      players_hand_idx[static_cast<uint8_t>(Player::Human_2)] = 2;
      players_score[static_cast<uint8_t>(Player::Human_2)] = 50;
    }
    players_hand[static_cast<uint8_t>(Player::Human_1)][0] = 4;     // 0:4
    players_hand[static_cast<uint8_t>(Player::Human_1)][1] = 1;    // 0:1
    players_hand_idx[static_cast<uint8_t>(Player::Human_1)] = 2;
    players_score[static_cast<uint8_t>(Player::Human_1)] = 30;
    bone_c = 22;
    bone_c_pips_inner = 4;
    bone_c_pips_outer = 4;
    bone_n = 21;
    bone_n_pips_inner = 3;
    bone_n_pips_outer = 6;
    bone_e = 23;
    bone_e_pips_inner = 4;
    bone_e_pips_outer = 5;
    bone_s = 16;
    bone_s_pips_inner = 5;
    bone_s_pips_outer = 2;
    bone_w = 2;
    bone_w_pips_inner = 0;
    bone_w_pips_outer = 2;
    printHand();
#endif

//                             //    0     1     2     3     4     5     6
// uint8_t bones_ref[BONES_COUNT] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 
//                             //    7     8     9    10    11    12  
//                                0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 
//                             //   13    14    15    16    17
//                                0x22, 0x23, 0x24, 0x25, 0x26, 
//                             //   18    19    20    21
//                                0x33, 0x34, 0x35, 0x36, 
//                             //   22    23    24
//                                0x44, 0x45, 0x46, 
//                             //   25    26
//                                0x55, 0x56,
//                             //   27
//                                0x66};

    // Render the screen ..
    
    arduboy.clear();
    renderGraveyard(playersTurn, NOTHING, players_hand_highlight_idx, true);
    gameState = (playersTurn == Player::Computer ? GameState::PlayComputer : GameState::PlayHuman_HandSel);

    if (gameMode == GameMode::SinglePlayer) {
      renderBoard(gameMode, playersTurn, Position::Nothing, false);
    }
    else {

      swapPlayers(true);

    }

  }

}

/* ----------------------------------------------------------------------------
 *  Swap players.
 */
void swapPlayers(bool startOfGame) {

  playersTurn = (playersTurn == Player::Human_1 ? Player::Human_2 : Player::Human_1);
  
  Player altPlayer = getAltPlayer(gameMode, playersTurn);

  renderPlayerScore(HandPosition::Top, altPlayer, false);
  renderPlayerScore(HandPosition::Bottom, playersTurn, false);
  renderPlayersHand(playersTurn, NOTHING, true);

  if (startOfGame) {
    drawMessageBox(F("You~can~start\nthe~new~game,"), 3);
    font4x6.setCursor(63,31);
  }
  else {
    drawMessageBox(F("Its~your~turn"), 2);
    font4x6.setCursor(63,23);
  }

  font4x6.setTextColor(BLACK);
  font4x6.print(players[static_cast<uint8_t>(playersTurn)]->getString());
  font4x6.print("!");
  font4x6.setTextColor(WHITE);
  arduboy.display();

  arduboy.pollButtons();
  DelayOrButtonPress(200);

  gameState = GameState::PlayHuman_HandSel;

}
