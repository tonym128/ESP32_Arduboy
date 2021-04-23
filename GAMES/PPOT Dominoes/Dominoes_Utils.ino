#include "src/utils/Arduboy2Ext.h"

Player getFirstPlayer(GameMode gameMode) {

  if (gameMode == GameMode::SinglePlayer) {

    return Player::Computer;

  }
  else {

    return Player::Human_1;

  }

}


Player getSecondPlayer(GameMode gameMode) {

  if (gameMode == GameMode::SinglePlayer) {

    return Player::Human_1;

  }
  else {

    return Player::Human_2;

  }

}

Player getAltPlayer(GameMode gameMode, Player player) {

  if (gameMode == GameMode::SinglePlayer) {

    if (player == Player::Computer) {
      return Player::Human_1;
    }
    else {
      return Player::Computer;
    }

  }
  else {

    if (player == Player::Human_2) {
      return Player::Human_1;
    }
    else {
      return Player::Human_2;
    }

  }

}


/* ----------------------------------------------------------------------------
 *  Shuffle the bones ..
 *  
 */
void shuffleBones() {

  
  // Clear the bones_played array and populate the bones_available array with 
  // a sequential number between 0 and BONES_COUNT (26).

  for (uint8_t x = 0; x < BONES_COUNT; x++) {
      
    bones_available[x] = x;
    bones_played[x] = 0;
            
  }
    
  
  // Randomly switch bones in the array with each other ..
  
  for (uint8_t x = BONES_COUNT - 1; x > 0; x--) {
      
    uint8_t y = random(x + 1);

    uint8_t z = bones_available[y];
    bones_available[y] = bones_available[x];
    bones_available[x] = z;

  }
  
  bones_available_idx = BONES_COUNT - 1;
  bones_played_idx = 0;
   
}


/* ----------------------------------------------------------------------------
 *  Deal the bones to the players ..
 *  
 */
void dealBones(GameMode gameMode) {
  
  Player player1 = getFirstPlayer(gameMode);
  Player player2 = getSecondPlayer(gameMode);
  
  memset(players_hand[0], 0, (sizeof(players_hand[0]) / sizeof(players_hand[0][0])));
  memset(players_hand_idx, 0, (sizeof(players_hand_idx) / sizeof(players_hand_idx[0])));
  
  for (uint8_t x = static_cast<uint8_t>(player1); x <= static_cast<uint8_t>(player2); x++) {

      for (uint8_t y = 0; y < BONES_INITIAL_COUNT; y++) {
      
      players_hand[x][y] = bones_available[bones_available_idx];
      
      bones_available_idx--;
      players_hand_idx[x]++;
    
    }
            
  }
    
}


/* ----------------------------------------------------------------------------
 *  Get the pip count of the board ..
 *  
 */
uint8_t getPipCount() {

  if (bone_c == NOTHING && bone_n == NOTHING && bone_s == NOTHING && bone_e == NOTHING && bone_w == NOTHING) {
      
    return NOTHING;
            
  }

  if (bone_c != NOTHING && bone_n == NOTHING && bone_s == NOTHING && bone_e == NOTHING && bone_w == NOTHING) {
      
    return bone_c_pips_inner + bone_c_pips_outer;
            
  }

  if (bone_e != NOTHING && bone_w != NOTHING) {
    
    uint8_t wPips = getPipCount(bone_w_pips_outer, bone_w);
    uint8_t ePips = getPipCount(bone_e_pips_outer, bone_e);
    uint8_t nPips = 0;
    uint8_t sPips = 0;

    if (bone_n != NOTHING) nPips = getPipCount(bone_n_pips_outer, bone_n);
    if (bone_s != NOTHING) sPips = getPipCount(bone_s_pips_outer, bone_s);
    
    return wPips + ePips + nPips + sPips;
          
  }
  else {

    uint8_t wPips = 0;
    uint8_t ePips = 0;

    if (bone_w != NOTHING) wPips = getPipCount(bone_w_pips_outer, bone_w);
    if (bone_e != NOTHING) ePips = getPipCount(bone_e_pips_outer, bone_e);
   
    return (bone_c_pips_inner == bone_c_pips_outer ? bone_c_pips_inner + bone_c_pips_outer : bone_c_pips_outer) + wPips + ePips;
          
  }
  
}

uint8_t getPipCount(uint8_t outerPips, uint8_t boneId) {

  return outerPips + (leftPips_ByBoneId(boneId) == rightPips_ByBoneId(boneId) ? outerPips : 0);

}

bool isDouble(uint8_t boneId) {

  return (leftPips_ByBoneId(boneId) == rightPips_ByBoneId(boneId));
  
}

/* ----------------------------------------------------------------------------
 *  Get the left-hand pip count of the bone by value.
 *  
 *  Bone values are a literal representation of the bone represented as a two-digit
 *  number such as 13 where the 1 is the right hand pip value and 3 is the left 
 *  hand pip value.
 *  
 */
uint8_t leftPips_ByBoneValue(uint8_t boneValue) {

  return boneValue >> 4; 
      
}


/* ----------------------------------------------------------------------------
 *  Get the right-hand pip count of the bone by value.
 *  
 *  Bone values are a literal representation of the bone represented as a two-digit
 *  number such as 13 where the 1 is the right hand pip value and 3 is the left 
 *  hand pip value.
 *  
 */
uint8_t rightPips_ByBoneValue(uint8_t boneValue) {

  return boneValue & 0x0F; 
      
}


/* ----------------------------------------------------------------------------
 *  Get the left-hand pip count of the bone by id.
 *  
 *  The bone value (left and right hand pip values as a two-digit value) must 
 *  be looked up via the bones-ref array.
 *  
 */
uint8_t leftPips_ByBoneId(uint8_t boneId) {

  return bones_ref[boneId] >> 4; 
      
}


/* ----------------------------------------------------------------------------
 *  Get the right-hand pip count of the bone by id.
 *  
 *  The bone value (left and right hand pip values as a two-digit value) must 
 *  be looked up via the bones-ref array.
 *  
 */
uint8_t rightPips_ByBoneId(uint8_t boneId) {

  return bones_ref[boneId] & 0x0F; 
      
}


/* ----------------------------------------------------------------------------
 *  Calculate the board's pip value if the nominated bone is played.
 *  
 */
uint8_t playBone_NewPipValue(uint8_t existingBonePips, uint8_t newBoneId) {

  if (existingBonePips == rightPips_ByBoneId(newBoneId)) {
  
    return leftPips_ByBoneId(newBoneId);
    
  }
  else {
  
    return rightPips_ByBoneId(newBoneId);
    
  }
     
} 


/* ----------------------------------------------------------------------------
 *  Have all 3 bones on the X-axis of play been played?
 *  
 */
boolean hasXAxisBeenPlayed() {

  return (bone_w != NOTHING && bone_c != NOTHING && bone_e != NOTHING);
  
}


/* ----------------------------------------------------------------------------
 *  Has a double bone been played?
 *  
 */
boolean hasADoubleBeenPlayed(uint8_t currentMode) {
 
  // Has a double been played ?

  switch (currentMode) {
     
    case BOARD_MODE_NO_BONES_PLAYED:

      return (bone_c_pips_inner == bone_c_pips_outer);
    
    case BOARD_MODE_ONE_BONE_PLAYED:
    case BOARD_MODE_TWO_BONES_PLAYED:

      if (bone_c_pips_inner == bone_c_pips_outer) {
        return true;
      }

      if (bone_w_pips_inner == bone_w_pips_outer && bone_e == NOTHING) {
        bone_e = bone_c;
        bone_e_pips_inner = bone_c_pips_inner;
        bone_e_pips_outer = bone_c_pips_outer;
        bone_c = bone_w;
        bone_c_pips_inner = bone_w_pips_inner;
        bone_c_pips_outer = bone_w_pips_outer;
        bone_w = NOTHING;
        bone_w_pips_inner = NOTHING;
        bone_w_pips_outer = NOTHING;
        return true;
      }
      
      if (bone_e_pips_inner == bone_e_pips_outer && bone_w == NOTHING) {
        bone_w = bone_c;
        bone_w_pips_inner = bone_c_pips_outer;
        bone_w_pips_outer = bone_c_pips_inner;
        bone_c = bone_e;
        bone_c_pips_inner = bone_e_pips_inner;
        bone_c_pips_outer = bone_e_pips_outer;
        bone_e = NOTHING;
        bone_e_pips_inner = NOTHING;
        bone_e_pips_outer = NOTHING;
        return true;
      }
           
      if (bone_e_pips_inner != bone_e_pips_outer && bone_w == NOTHING) {
        bone_w = bone_c;
        bone_w_pips_outer = bone_c_pips_inner;
        bone_w_pips_inner = bone_c_pips_outer;
        bone_c = bone_e;
        bone_c_pips_inner = bone_e_pips_inner;
        bone_c_pips_outer = bone_e_pips_outer;
        bone_e = NOTHING;
        bone_e_pips_inner = NOTHING;
        bone_e_pips_outer = NOTHING;
        return false;
      }

      return false;

  }
  
  return true;
  
}


/* ----------------------------------------------------------------------------
 *  Will the nominated bone block play?
 *  
 */
boolean canBlock(uint8_t pips) {

  return (boneCounts_Overall[pips] + boneCounts_Inhand[pips] == 7);

}


/* ----------------------------------------------------------------------------
 *  Populate bone counts.
 *  
 *  When determining whether a bone could block play, a count of each pip value
 *  (from one to six) that is visible is determined by inspecting the players 
 *  hand and the played bones.   The bone counts per pip are recorded into two
 *  arrays, boneCounts_Overall and boneCounts_Inhand.
 *  
 */
void populateBoneCounts(Player player) {


  // Clear array before processing ..
  
  memset(boneCounts_Overall, 0, sizeof(boneCounts_Overall));
  memset(boneCounts_Inhand, 0, sizeof(boneCounts_Inhand));


  // Populate bone count from players hand ..

  for (uint8_t x = 0; x < players_hand_idx[static_cast<uint8_t>(player)]; x++) {
    
    boneCounts_Inhand[leftPips_ByBoneId(players_hand[static_cast<uint8_t>(player)][x])]++;
    
    if (leftPips_ByBoneId(players_hand[static_cast<uint8_t>(player)][x]) != rightPips_ByBoneId(players_hand[static_cast<uint8_t>(player)][x])) {
        boneCounts_Inhand[rightPips_ByBoneId(players_hand[static_cast<uint8_t>(player)][x])]++;
      }
    
  }


  // Populate visible bones in grave yard ..

  for (uint8_t x = 0; x < bones_played_idx; x++) {
    
    boneCounts_Overall[leftPips_ByBoneId(bones_played[x])]++;
    
    if (leftPips_ByBoneId(bones_played[x]) != rightPips_ByBoneId(bones_played[x])) {
      boneCounts_Overall[rightPips_ByBoneId(bones_played[x])]++;
    }
    
  }


  // Populate visible bones on board ..
  
  if (bone_c != NOTHING) {
    boneCounts_Overall[bone_c_pips_inner]++;
    if (bone_c_pips_inner != bone_c_pips_outer) { boneCounts_Overall[bone_c_pips_outer]++; }
  }

  if (bone_n != NOTHING) {
    boneCounts_Overall[bone_n_pips_inner]++;
    if (bone_n_pips_inner != bone_n_pips_outer) { boneCounts_Overall[bone_n_pips_outer]++; }
  }

  if (bone_s != NOTHING) {
    boneCounts_Overall[bone_s_pips_inner]++;
    if (bone_s_pips_inner != bone_s_pips_outer) { boneCounts_Overall[bone_s_pips_outer]++; }
  }

  if (bone_e != NOTHING) {
    boneCounts_Overall[bone_e_pips_inner]++;
    if (bone_e_pips_inner != bone_e_pips_outer) { boneCounts_Overall[bone_e_pips_outer]++; }
  }

  if (bone_w != NOTHING) {
    boneCounts_Overall[bone_w_pips_inner]++;
    if (bone_w_pips_inner != bone_w_pips_outer) { boneCounts_Overall[bone_w_pips_outer]++; }
  }

}


/* ----------------------------------------------------------------------------
 *  What is the value of the pips in a player's hand ? (rounded to nearest 5).
 *  
 */
uint8_t handValue(Player player) {

  uint8_t score = 0;
  
  for (uint8_t x = 0; x < players_hand_idx[static_cast<uint8_t>(player)]; x++) {
    score = score + leftPips_ByBoneId(players_hand[static_cast<uint8_t>(player)][x]);
    score = score + rightPips_ByBoneId(players_hand[static_cast<uint8_t>(player)][x]);
  }

  if (score % 5 <= 2) {

    return (score / 5) * 5;

  }
  else {

    return ((score / 5) + 1) * 5;

  }

}


/* ----------------------------------------------------------------------------
 *  Can the player play a bone?
 *  
 */
boolean canPlay(Player player) {

  for (uint8_t x = 0; x < players_hand_idx[player]; x++) {

    if (canPlayBone(players_hand[static_cast<uint8_t>(player)][x])) {

        return true;
        
    }

  }

  return false;
  
}


/* ----------------------------------------------------------------------------
 *  Can the bone be played?
 *  
 */
boolean canPlayBone(uint8_t boneId) {

  if (boardMode == BOARD_MODE_NO_BONES_PLAYED) {
    
      return true;

  }
  else {

    uint8_t leftPips = leftPips_ByBoneId(boneId);
    uint8_t rightPips = rightPips_ByBoneId(boneId);
    
    if (bone_c_pips_outer == bone_c_pips_inner) {

      if ((bone_c_pips_outer == leftPips || bone_c_pips_outer == rightPips) && (bone_e_pips_outer == NOTHING || bone_w_pips_outer == NOTHING || bone_n_pips_outer == NOTHING || bone_s_pips_outer == NOTHING)) {

        return true;

      }

    }
    else {

      if ((bone_c_pips_inner == leftPips || bone_c_pips_inner == rightPips) && bone_w_pips_outer == NOTHING) {

        return true;

      }

      if ((bone_c_pips_outer == leftPips || bone_c_pips_outer == rightPips) && bone_e_pips_outer == NOTHING) {

        return true;
      }

    }

    if (bone_e_pips_outer == NOTHING && (bone_c_pips_outer == leftPips || bone_c_pips_outer == rightPips))     { return true; }  
    if (bone_e_pips_outer != NOTHING && (bone_e_pips_outer == leftPips || bone_e_pips_outer == rightPips))     { return true; }
    if (bone_w_pips_outer == NOTHING && (bone_c_pips_inner == leftPips || bone_c_pips_inner == rightPips))     { return true; }
    if (bone_w_pips_outer != NOTHING && (bone_w_pips_outer == leftPips || bone_w_pips_outer == rightPips))     { return true; }

    if (boardMode == BOARD_MODE_X_AXIS_PLAYED) {

      if (bone_n_pips_outer == NOTHING && (bone_c_pips_outer == leftPips || bone_c_pips_outer == rightPips))    { return true; }
      if (bone_n_pips_outer != NOTHING && (bone_n_pips_outer == leftPips || bone_n_pips_outer == rightPips))    { return true; }
      if (bone_s_pips_outer == NOTHING && (bone_c_pips_outer == leftPips || bone_c_pips_outer == rightPips))    { return true; }
      if (bone_s_pips_outer != NOTHING && (bone_s_pips_outer == leftPips || bone_s_pips_outer == rightPips))    { return true; }

    }
      
  }
           
  return false;

}


/* ----------------------------------------------------------------------------
 *  Can either player make a move or draw a bone?
 *  
 */
boolean canEitherPlayerMove(GameMode gameMode) {
  
  Player player1 = getFirstPlayer(gameMode);
  Player player2 = getSecondPlayer(gameMode);
  

  // Check to see if a move exists for any player ..

  if (bones_available_idx == 0 && !canPlay(player1) && !canPlay(player2)) {

    uint8_t hand0 = handValue(player1);
    uint8_t hand1 = handValue(player2);

    char message[] = (" ~No~one~can\n~play~a~bone!");

    if (hand0 == hand1) {
      
      drawMessageBox(message, 2, true);
      return false;
    }

    if (hand0 > hand1) {
      
      players_score[static_cast<uint8_t>(player2)] = players_score[static_cast<uint8_t>(player2)] + (hand0 - hand1);
      drawMessageBox(message, 2, true, player2);
      return false;         

    }

    if (hand0 < hand1) {
      
      players_score[static_cast<uint8_t>(player1)] = players_score[static_cast<uint8_t>(player1)] + (hand1 - hand0);
      drawMessageBox(message, 2, true, player1);
      return false;         

    }

  }

  return true;
  
 }



/* ----------------------------------------------------------------------------
 *  Flash the red LED.
 *  
 */
void flashRedLED() {

  arduboy.setRGBled(25, 0, 0);
  arduboy.delayShort(100);
  arduboy.setRGBled(0, 0, 0);

}


/* ----------------------------------------------------------------------------
 *  Flash the green LED.
 *  
 */
void flashGreenLED() {
 
  arduboy.setRGBled(0, 25, 0);
  arduboy.delayShort(100);
  arduboy.setRGBled(0, 0, 0);

}


/* ----------------------------------------------------------------------------
 *  Delay before proceeding.  If the user clicks the A or B button, then abort. 
 *  
 */
void DelayOrButtonPress(uint8_t delayVal) {

  uint8_t delayCnt = 0;
  
  while (delayCnt < delayVal) {
    if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) { break; }
    arduboy.delayShort(100);
    delayCnt++;
  }

}



/* ----------------------------------------------------------------------------
 *  Wait for a button press. 
 *  
 *  Returns the pressed button to the caller.
 *  
 */
uint8_t WaitForButtonPress() {
  
  arduboy.pollButtons();
  
  while (true) {

    if (arduboy.pressed(A_BUTTON)) { 
      return A_BUTTON;
    }
    
    if (arduboy.pressed(B_BUTTON)) { 
      return B_BUTTON;
    }
    
    arduboy.delayShort(100);
    
  }

}


/* ----------------------------------------------------------------------------
 *  Draw a horizontal dotted line. 
 *  
 *  So much nicer than a solid line!
 *  
 */
void drawHorizontalDottedLine(uint8_t x1, uint8_t x2, uint8_t y) {

  for (uint8_t x3 = x1; x3 <= x2; x3+=2) {
    arduboy.drawPixel(x3, y, WHITE);
  }
  
}


/* ----------------------------------------------------------------------------
 *  Is the game over yet?
 *  
 */
boolean isTheGameOver(GameMode gameMode) {
  
  Player player1 = getFirstPlayer(gameMode);
  Player player2 = getSecondPlayer(gameMode);

  if (players_score[static_cast<uint8_t>(player1)] > GAME_FIRST_TO_REACH_SCORE || players_score[static_cast<uint8_t>(player2)] > GAME_FIRST_TO_REACH_SCORE) {

    if (players_score[static_cast<uint8_t>(player1)] > GAME_FIRST_TO_REACH_SCORE && player1 == Player::Computer) {
      
      drawMessageBox(F("\n  I won !"), 3, true);

    }
    else {
      
      drawMessageBox(F("Great~work"), 3);
      font4x6.setCursor(63,20);
      font4x6.setTextColor(BLACK);
      font4x6.print(players[static_cast<uint8_t>(playersTurn)]->getString());
      font4x6.print(",\nyou~won~!");
      font4x6.setTextColor(WHITE);
      arduboy.display();
    
    }

    arduboy.pollButtons();
    DelayOrButtonPress(200);
    return true;

  }
  else {
    
    return false;
    
  }

}


/* ----------------------------------------------------------------------------
 *  Has either player played all their bones?
 *  
 */
boolean isAnyoneOut(GameMode gameMode) {
  
  Player player1 = getFirstPlayer(gameMode);
  Player player2 = getSecondPlayer(gameMode);


  // Game still in play?

  for (uint8_t x = static_cast<uint8_t>(player1); x <= static_cast<uint8_t>(player2); x++) {


    // Has the player run out of bones?
    
    if (players_hand_idx[x] == 0) {

//    char message[] = "You~scored~an\nan extra~   \npoints!";
      char message[] = "You~scored~a \nbonus of~   \npoints!";
      printHand();

      //renderPlayersHand(player1, players_hand_highlight_idx, false);

      if (x == static_cast<uint8_t>(Player::Computer)) {

        players_score[static_cast<uint8_t>(player1)] = players_score[static_cast<uint8_t>(player1)] + handValue(player2);
        drawMessageBox(F("~I~have~used\n~up~all~of~my\n~bones!"), 3, true, Player::Computer);

      }

      if (x == static_cast<uint8_t>(Player::Human_1)) {

        uint16_t extraPoints = handValue(x == player2 ? player1 : player2);
        drawMessageBox(F("You~have~used\nup~all~of~your\nbones!"), 3, true);

        if (extraPoints < 100) {
          message[23] = (extraPoints / 10) + 48;
          message[24] = (extraPoints % 10) + 48;
          message[25] = ' ';
        }
        else {
          message[23] = (extraPoints / 100) + 48; extraPoints = extraPoints % 100;
          message[24] = ((extraPoints / 10) % 10) + 48;
          message[25] = (extraPoints % 10) + 48;
        }
        
        players_score[static_cast<uint8_t>(x == player1 ? player1 : player2)] = players_score[static_cast<uint8_t>(x == player1 ? player1 : player2)] + extraPoints;
        drawMessageBox(message, 3, true, Player::Human_1);

      }

      if (x == static_cast<uint8_t>(Player::Human_2)) {

        uint16_t extraPoints = handValue(player1);
        drawMessageBox(F("You~have~used\nup~all~of~your\nbones!"), 3, true, Player::Human_2);

        if (extraPoints < 100) {
          message[25] = (extraPoints / 10) + 48;
          message[26] = (extraPoints % 10) + 48;
          message[27] = ' ';
        }
        else {
          message[25] = (extraPoints / 100) + 48; extraPoints = extraPoints % 100;
          message[26] = ((extraPoints / 10) % 10) + 48;
          message[27] = (extraPoints % 10) + 48;
        }

        players_score[static_cast<uint8_t>(player2)] = players_score[static_cast<uint8_t>(player2)] + extraPoints;
        drawMessageBox(message, 3, true, Player::Human_2);

      }

      gameState = GameState::PlayGame;
      return true;
      
    }

  }

  return false;
  
}


void printHand() {

  #ifdef DEBUGs

  Serial.println("--------------------------------------------");
  Serial.print("PlayersTurn :");
  Serial.println(playersTurn);
  
  if (gameMode == GameMode::SinglePlayer) {
    Serial.print("Comp:  ");
    for (uint8_t x = 0; x < players_hand_idx[0]; x++) {
      uint8_t b = bones_ref[players_hand[0][x]];
      Serial.print( b >> 4 );
      Serial.print( b & 0x0f );
      Serial.print(" ");
    }  
    Serial.println(" ");
  }

  Serial.print("Play1: ");
  for (uint8_t x = 0; x < players_hand_idx[1]; x++) {
    uint8_t b = bones_ref[players_hand[1][x]];
    Serial.print( b >> 4 );
    Serial.print( b & 0x0f );
    Serial.print(" ");
  }  
  Serial.println(" ");

  if (gameMode == GameMode::TwoPlayer) {
    Serial.print("Play2: ");
    for (uint8_t x = 0; x < players_hand_idx[2]; x++) {
      uint8_t b = bones_ref[players_hand[2][x]];
      Serial.print( b >> 4 );
      Serial.print( b & 0x0f );
      Serial.print(" ");
    }  
    Serial.println(" ");
  }

  #endif

}