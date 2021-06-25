#include "src/utils/Arduboy2Ext.h"


/* ----------------------------------------------------------------------------
 *  The computer will attempt to:
 *  
 *  1)  Play a domino that scores.
 *  2)  Play a domino that blocks an end of the play using a bone whose pips are 
 *      fully visible in the his hand or the 'used' boness. 
 *  3)  Play a domino that reduces the pip count thus preventing other players
 *      scoring highly. 
 *  
 */

/* ----------------------------------------------------------------------------
 *  Calculate the best score-producing hand.
 *  
 *  Iterate through the player's hand and return the best score-producing hand
 *  if one exists.  Returns the cards index (in the hand, zero-based) and the 
 *  cardinal location on the playing field to place it.
 *    
 */
void highestPossibleScore(Player player, uint8_t *retIndex, Position *retDirection) {

  uint8_t retValue = 0;
  uint8_t leftPips = NOTHING;
  uint8_t rightPips = NOTHING;
  bool isDouble = false;
    
  for (uint8_t x = 0; x < players_hand_idx[static_cast<uint8_t>(player)]; x++) {

      leftPips = leftPips;
      rightPips = rightPips;
      isDouble = (leftPips == rightPips);
      
      switch (boardMode) {
      
        case BOARD_MODE_NO_BONES_PLAYED:
          highestPossibleScore_CenterOnly(x, leftPips, rightPips, &retValue, retIndex, retDirection);
          break;
          
        case BOARD_MODE_ONE_BONE_PLAYED:

          if (bone_c_pips_outer == leftPips)    { highestPossibleScore_East(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_c_pips_outer == rightPips)   { highestPossibleScore_East(isDouble, x, leftPips, &retValue, retIndex, retDirection); }
          if (bone_c_pips_inner == leftPips)    { highestPossibleScore_West(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_c_pips_inner == rightPips)   { highestPossibleScore_West(isDouble, x, leftPips, &retValue, retIndex, retDirection); }
                    
          break;

        case BOARD_MODE_TWO_BONES_PLAYED:
          
          if (bone_c_pips_outer == leftPips)    { highestPossibleScore_Center(x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_c_pips_outer == rightPips)   { highestPossibleScore_Center(x, leftPips, &retValue, retIndex, retDirection); }
          if (bone_w_pips_outer == leftPips)    { highestPossibleScore_West(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_w_pips_outer == rightPips)   { highestPossibleScore_West(isDouble, x, leftPips, &retValue, retIndex, retDirection); }
          
          break;

      case BOARD_MODE_DOUBLE_BONE_PLAYED:

        if (bone_e != NOTHING) {                         // Center is occupied and east is played ..
          
          if (bone_e_pips_outer == leftPips) {  

            if (bone_w != NOTHING)              { highestPossibleScore_East(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
            if (bone_w == NOTHING)              { highestPossibleScore_East_CenterOnly(x, rightPips, &retValue, retIndex, retDirection); }
          
          }
          
          if (bone_e_pips_outer == rightPips) { 

            if (bone_w != NOTHING)              { highestPossibleScore_East(isDouble, x, leftPips, &retValue, retIndex, retDirection); }
            if (bone_w == NOTHING)              { highestPossibleScore_East_CenterOnly(x, leftPips, &retValue, retIndex, retDirection); }
          
          }
         
        }

        if (bone_w != NOTHING) {                         // Center is occupied and west is played ..
          
          if (bone_w_pips_outer == leftPips) { 

            if (bone_e != NOTHING)              { highestPossibleScore_West(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
            if (bone_e == NOTHING)              { highestPossibleScore_West_CenterOnly(x, rightPips, &retValue, retIndex, retDirection); }
          
          }
          
          if (bone_w_pips_outer == rightPips) { 

            if (bone_e != NOTHING)              { highestPossibleScore_West(isDouble, x, leftPips, &retValue, retIndex, retDirection); }
            if (bone_e == NOTHING)              { highestPossibleScore_West_CenterOnly(x, leftPips, &retValue, retIndex, retDirection); }
          
          }
         
        }

        if (bone_w == NOTHING && bone_e == NOTHING) {    // Center is occupied but the east and the west are empty ..

          if (bone_c_pips_outer == leftPips)    { highestPossibleScore_West_CenterOnly(x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_c_pips_outer == rightPips)   { highestPossibleScore_West_CenterOnly(x, leftPips, &retValue, retIndex, retDirection); }

        }          

        break;
    
      case BOARD_MODE_X_AXIS_PLAYED:

        if (bone_e_pips_outer == leftPips)      { highestPossibleScore_East(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
        if (bone_e_pips_outer == rightPips)     { highestPossibleScore_East(isDouble, x, leftPips, &retValue, retIndex, retDirection); }
        if (bone_w_pips_outer == leftPips)      { highestPossibleScore_West(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
        if (bone_w_pips_outer == rightPips)     { highestPossibleScore_West(isDouble, x, leftPips, &retValue, retIndex, retDirection); }

        if (bone_n != NOTHING) {                         
          
          if (bone_n_pips_outer == leftPips)    { highestPossibleScore_North(x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_n_pips_outer == rightPips)   { highestPossibleScore_North(x, leftPips, &retValue, retIndex, retDirection); }
          
        }
        else {
          
          if (bone_c_pips_outer == leftPips)    { highestPossibleScore_North(x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_c_pips_outer == rightPips)   { highestPossibleScore_North(x, leftPips, &retValue, retIndex, retDirection); }
        
        }

        if (bone_s != NOTHING) { 
          
          if (bone_s_pips_outer == leftPips)    { highestPossibleScore_South(x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_s_pips_outer == rightPips)   { highestPossibleScore_South(x, leftPips, &retValue, retIndex, retDirection); }
          
        }
        else {
          
          if (bone_c_pips_outer == leftPips)    { highestPossibleScore_South(x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_c_pips_outer == rightPips)   { highestPossibleScore_South(x, rightPips, &retValue, retIndex, retDirection); }
        
        }

        break;
     
    }

  }  

}


/* ----------------------------------------------------------------------------
 *  Calculate the best score-producing hand (North).
 *  
 *  Can the nominated bone be played in the North position?  If so, will it
 *  result in a score that is lower than all previously calculated ones ??
 *    
 */
void highestPossibleScore_North(uint8_t boneIdx, uint8_t pips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {

  uint8_t val = bone_w_pips_outer + bone_e_pips_outer + (bone_s != NOTHING ? bone_s_pips_outer : 0) + (bone_n != NOTHING ? bone_n_pips_outer : 0) + pips;

  if (val % 5 == 0 && val > *retValue) { 
    *retValue = val; 
    *retDirection = Position::North;
    *retIndex = boneIdx;
  }

}


/* ----------------------------------------------------------------------------
 *  Calculate the best score-producing hand (South).
 *  
 *  Can the nominated bone be played in the South position?  If so, will it
 *  result in a score that is lower than all previously calculated ones ??
 *    
 */
void highestPossibleScore_South(uint8_t boneIdx, uint8_t pips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {

  uint8_t val = bone_w_pips_outer + bone_e_pips_outer + (bone_n != NOTHING ? bone_n_pips_outer : 0) + pips;

  if (val % 5 == 0 && val > *retValue) {  
    *retValue = val; 
    *retDirection = Position::South;
    *retIndex = boneIdx;
  }

}


/* ----------------------------------------------------------------------------
 *  Calculate the best score-producing hand (Center).
 *  
 *  Can the nominated bone be played in the Center position?  If so, will it
 *  result in a score that is lower than all previously calculated ones ??
 *    
 */
void highestPossibleScore_Center(uint8_t boneIdx, uint8_t pips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {

  uint8_t val = bone_w_pips_outer + bone_e_pips_outer + (bone_n != NOTHING ? bone_n_pips_outer : 0) + pips;

  if (val % 5 == 0 && val > *retValue) {  
    *retValue = val; 
    *retDirection = Position::South;
    *retIndex = boneIdx;
  }

}


/* ----------------------------------------------------------------------------
 *  Calculate the best score-producing hand (Center Only).
 *  
 *  Can the nominated bone be played in the West position against the centre 
 *  bone? If so, will it result in a score that is lower than all previously 
 *  calculated ones ??  
 *    
 */
void highestPossibleScore_CenterOnly(uint8_t boneIdx, uint8_t leftPips, uint8_t rightPips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {

  uint8_t val = leftPips + rightPips;

  if (val % 5 == 0 && val > *retValue) { 
    *retValue = val; 
    *retDirection = Position::Center;
    *retIndex = boneIdx;
  }

}


/* ----------------------------------------------------------------------------
 *  Calculate the best score-producing hand (West).
 *  
 *  Can the nominated bone be played in the West position?  If so, will it
 *  result in a score that is lower than all previously calculated ones ??
 *    
 */
void highestPossibleScore_West(boolean isDouble, uint8_t boneIdx, uint8_t pips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {

  switch (boardMode) {
  
    case BOARD_MODE_ONE_BONE_PLAYED:
      {
        uint8_t val = bone_c_pips_outer + (pips * (isDouble ? 2 : 1));
      
        if (val % 5 == 0 && val > *retValue) { 
          *retValue = val; 
          *retDirection = Position::West;
          *retIndex = boneIdx;
        }

      }
      
      break;
      
    case BOARD_MODE_TWO_BONES_PLAYED:
      {
        uint8_t val = bone_c_pips_outer + pips;

        if (val % 5 == 0 && val > *retValue) { 
          *retValue = val; 
          *retDirection = Position::West;
          *retIndex = boneIdx;
        }

      }
      
      break;

    case BOARD_MODE_DOUBLE_BONE_PLAYED:
      {
        uint8_t val = bone_e_pips_outer + pips;
        if (val % 5 == 0 && val > *retValue) { 
          *retValue = val; 
          *retDirection = Position::West;
          *retIndex = boneIdx;
        }

      }      
      break;

    case BOARD_MODE_X_AXIS_PLAYED:
      {
        uint8_t val = (bone_n != NOTHING ? bone_n_pips_outer : 0) + (bone_s != NOTHING ? bone_s_pips_outer : 0) + bone_e_pips_outer + pips;

        if (val % 5 == 0 && val > *retValue) { 
          *retValue = val; 
          *retDirection = Position::West;
          *retIndex = boneIdx;
        }
        
      }
      break;
      
  }
  
}


/* ----------------------------------------------------------------------------
 *  Calculate the best score-producing hand (West, Center Only).
 *  
 *  Can the nominated bone be played in the West position against the centre 
 *  bone? If so, will it result in a score that is lower than all previously 
 *  calculated ones ??  
 *    
 */
void highestPossibleScore_West_CenterOnly(uint8_t boneIdx, uint8_t pips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {

  uint8_t val = bone_c_pips_outer + bone_c_pips_inner + pips;

  if (val % 5 == 0 && val > *retValue) { 
    *retValue = (bone_c_pips_outer + bone_c_pips_inner + pips); 
    *retDirection = Position::West;
    *retIndex = boneIdx;
  }

}


/* ----------------------------------------------------------------------------
 *  Calculate the best score-producing hand (East).
 *  
 *  Can the nominated bone be played in the East position?  If so, will it
 *  result in a score that is lower than all previously calculated ones ??
 *    
 */
void highestPossibleScore_East(boolean isDouble, uint8_t boneIdx, uint8_t pips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {

  switch (boardMode) {
  
    case BOARD_MODE_ONE_BONE_PLAYED:
      {
        uint8_t val = bone_c_pips_inner + (pips * (isDouble ? 2 : 1));

        if (val % 5 == 0 && val > *retValue) { 
          *retValue = val; 
          *retDirection = Position::East;
          *retIndex = boneIdx;
        }

      }
      
      break;
      
    case BOARD_MODE_DOUBLE_BONE_PLAYED:     
      {
        uint8_t val = (bone_w_pips_outer + pips);

        if (val % 5 == 0 && val > *retValue) { 
          *retValue = (bone_w_pips_outer + pips); 
          *retDirection = Position::East;
          *retIndex = boneIdx;
        }

      }
      
      break;
      
    case BOARD_MODE_X_AXIS_PLAYED:      
      {
        uint8_t val = (bone_n != NOTHING ? bone_n_pips_outer : 0) + (bone_s != NOTHING ? bone_s_pips_outer : 0) + bone_w_pips_outer + pips;
        
        if (val % 5 == 0 && val > *retValue) { 
          *retValue = val; 
          *retDirection = Position::East;
          *retIndex = boneIdx;
        }

      }
      
      break;
      
  }
  
}


/* ----------------------------------------------------------------------------
 *  Calculate the best score-producing hand (East, Center Only).
 *  
 *  Can the nominated bone be played in the East position against the centre 
 *  bone? If so, will it result in a score that is lower than all previously 
 *  calculated ones ??  
 *    
 */
void highestPossibleScore_East_CenterOnly(uint8_t boneIdx, uint8_t pips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {
  
  uint8_t val = bone_c_pips_outer + bone_c_pips_inner + pips;

  if (val % 5 == 0 && val > *retValue) { 
    *retValue = val; 
    *retDirection = Position::East;
    *retIndex = boneIdx;
  }

}


/* ----------------------------------------------------------------------------
 *  Calculate the lowest possible score.
 *  
 *  If a scoring hand cannot be played, look for a bone that reduces the overall
 *  pip score as much as possible.  A play that results in a block is given more
 *  weight than other moves.
 *  
 */
void lowestPossibleScore(Player player, uint8_t *retIndex, Position *retDirection) {

  uint8_t retValue = NOTHING;

  
  // Work out which bones are in play or in the computer's hand.  These are then 
  // used to work out whether a blocking play can be made ..
  
  populateBoneCounts(player);
  
  
  // Cycle through all bones in the hand ..
  
  for (uint8_t x = 0; x < players_hand_idx[static_cast<uint8_t>(player)]; x++) {

    uint8_t boneIdx = players_hand[static_cast<uint8_t>(player)][x]; 
    uint8_t leftPips = leftPips_ByBoneId(boneIdx);
    uint8_t rightPips = rightPips_ByBoneId(boneIdx);
    bool isDouble = (leftPips == rightPips);
    
    switch (boardMode) {
      
      case BOARD_MODE_NO_BONES_PLAYED:
        lowestPossibleScore_CenterOnly(x, leftPips, rightPips, &retValue, retIndex, retDirection);
        break;
          
      case BOARD_MODE_ONE_BONE_PLAYED:

        if (bone_c_pips_outer == leftPips)      { lowestPossibleScore_East(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
        if (bone_c_pips_outer == rightPips)     { lowestPossibleScore_East(isDouble, x, leftPips, &retValue, retIndex, retDirection); }
        if (bone_c_pips_inner == leftPips)      { lowestPossibleScore_West(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
        if (bone_c_pips_inner == rightPips)     { lowestPossibleScore_West(isDouble, x, leftPips, &retValue, retIndex, retDirection); }
          
        break;

      case BOARD_MODE_TWO_BONES_PLAYED:
         
        if (bone_c_pips_outer == leftPips)      { lowestPossibleScore_Center(x, rightPips, &retValue, retIndex, retDirection); }
        if (bone_c_pips_outer == rightPips)     { lowestPossibleScore_Center(x, leftPips, &retValue, retIndex, retDirection); }
        if (bone_w_pips_outer == leftPips)      { lowestPossibleScore_West(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
        if (bone_w_pips_outer == rightPips)     { lowestPossibleScore_West(isDouble, x, leftPips, &retValue, retIndex, retDirection); }
          
        break;
        
      case BOARD_MODE_DOUBLE_BONE_PLAYED:
       
        if (bone_e != NOTHING) { 

          if (bone_e_pips_outer == leftPips)    { lowestPossibleScore_East(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_e_pips_outer == rightPips)   { lowestPossibleScore_East(isDouble, x, leftPips, &retValue, retIndex, retDirection); }
         
        }
        else {

          if (bone_c_pips_outer == leftPips)    { lowestPossibleScore_East(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_c_pips_outer == rightPips)   { lowestPossibleScore_East(isDouble, x, leftPips, &retValue, retIndex, retDirection); }

        }

        if (bone_w != NOTHING) { 

          if (bone_w_pips_outer == leftPips)    { lowestPossibleScore_West(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_w_pips_outer == rightPips)   { lowestPossibleScore_West(isDouble, x, leftPips, &retValue, retIndex, retDirection); }
         
        }
        else {

          if (bone_c_pips_outer == leftPips)    { lowestPossibleScore_West(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_c_pips_outer == rightPips)   { lowestPossibleScore_West(isDouble, x, leftPips, &retValue, retIndex, retDirection); }

        }
        
        if (bone_w == NOTHING && bone_e == NOTHING) { // Center is occupied but the east and the west are empty ..

          if (bone_c_pips_outer == leftPips)    { lowestPossibleScore_West(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_c_pips_outer == rightPips)   { lowestPossibleScore_West(isDouble, x, leftPips, &retValue, retIndex, retDirection); }

        }          

        break;
    
      case BOARD_MODE_X_AXIS_PLAYED:
       
        if (bone_e_pips_outer == leftPips)      { lowestPossibleScore_East(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
        if (bone_e_pips_outer == rightPips)     { lowestPossibleScore_East(isDouble, x, leftPips, &retValue, retIndex, retDirection); }
        if (bone_w_pips_outer == leftPips)      { lowestPossibleScore_West(isDouble, x, rightPips, &retValue, retIndex, retDirection); }
        if (bone_w_pips_outer == rightPips)     { lowestPossibleScore_West(isDouble, x, leftPips, &retValue, retIndex, retDirection); }

        if (bone_n != NOTHING) { // North ..
         
          if (bone_n_pips_outer == leftPips)    { lowestPossibleScore_North(x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_n_pips_outer == rightPips)   { lowestPossibleScore_North(x, leftPips, &retValue, retIndex, retDirection); }
          
        }
        else {
         
          if (bone_c_pips_outer == leftPips)    { lowestPossibleScore_North(x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_c_pips_outer == rightPips)   { lowestPossibleScore_North(x, leftPips, &retValue, retIndex, retDirection); }
        
        }

        if (bone_s != NOTHING) { // South ..
          
          if (bone_s_pips_outer == leftPips)    { lowestPossibleScore_South(x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_s_pips_outer == rightPips)   { lowestPossibleScore_South(x, leftPips, &retValue, retIndex, retDirection); }
          
        }
        else {
          
          if (bone_c_pips_outer == leftPips)    { lowestPossibleScore_North(x, rightPips, &retValue, retIndex, retDirection); }
          if (bone_c_pips_outer == rightPips)   { lowestPossibleScore_North(x, leftPips, &retValue, retIndex, retDirection); }
        
        }

        break;
     
    }

  }  

}


/* ----------------------------------------------------------------------------
 *  Calculate the lowest possible score (North).
 *  
 *  Can the nominated bone be played in the North position?  If so, could it 
 *  block the other player or drive the pip count down to a value that is lower 
 *  than all previously calculated ones ??  Moves that block the other player 
 *  are preferred to non-blocking moves.
 *    
 */
void lowestPossibleScore_North(uint8_t boneIdx, uint8_t pips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {
    
  uint8_t val = canBlock(pips) ? 0 : (bone_w_pips_outer + bone_e_pips_outer + (bone_s != NOTHING ? bone_s_pips_outer : 0) + pips);

  if (val < *retValue) { 
    *retValue = val; 
    *retDirection = Position::North;
    *retIndex = boneIdx;
  }

}


/* ----------------------------------------------------------------------------
 *  Calculate the lowest possible score (South).
 *  
 *  Can the nominated bone be played in the South position?  If so, could it 
 *  block the other player or drive the pip count down to a value that is lower 
 *  than all previously calculated ones ??  Moves that block the other player 
 *  are preferred to non-blocking moves.
 *    
 */
void lowestPossibleScore_South(uint8_t boneIdx, uint8_t pips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {

  uint8_t val = canBlock(pips) ? 0 : (bone_w_pips_outer + bone_e_pips_outer + (bone_n != NOTHING ? bone_n_pips_outer : 0) + pips);

  if (val < *retValue) {  
    *retValue = val; 
    *retDirection = Position::South;
    *retIndex = boneIdx;
  }

}


/* ----------------------------------------------------------------------------
 *  Calculate the lowest possible score (Center).
 *  
 *  Can the nominated bone be played in the Center position?  If so, could it 
 *  block the other player or drive the pip count down to a value that is lower 
 *  than all previously calculated ones ??  Moves that block the other player 
 *  are preferred to non-blocking moves.
 *    
 */
void lowestPossibleScore_Center(uint8_t boneIdx, uint8_t pips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {

  uint8_t val = (canBlock(pips) ? 0 : bone_w_pips_outer + pips);

  if (val < *retValue) { 
    *retValue = val;
    *retDirection = Position::Center;
    *retIndex = boneIdx;
  }
  
}


/* ----------------------------------------------------------------------------
 *  Calculate the lowest possible score (Center Only).
 *  
 *  Can the nominated bone be played in the Center position?  If so, could it 
 *  block the other player or drive the pip count down to a value that is lower 
 *  than all previously calculated ones ??  Moves that block the other player 
 *  are preferred to non-blocking moves.
 *    
 */
void lowestPossibleScore_CenterOnly(uint8_t boneIdx, uint8_t leftPips, uint8_t rightPips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {

  uint8_t val = leftPips + rightPips;

  if (val < *retValue) { 
    *retValue = val;
    *retDirection = Position::Center;
    *retIndex = boneIdx;
  }
  
}

/* ----------------------------------------------------------------------------
 *  Calculate the lowest possible score (West).
 *  
 *  Can the nominated bone be played in the West position?  If so, could it 
 *  block the other player or drive the pip count down to a value that is lower 
 *  than all previously calculated ones ??  Moves that block the other player 
 *  are preferred to non-blocking moves.
 *    
 */
void lowestPossibleScore_West(boolean isDouble, uint8_t boneIdx, uint8_t pips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {

  switch (boardMode) {
    
    case BOARD_MODE_TWO_BONES_PLAYED:
      {
        uint8_t val = (canBlock(pips) ? 0 : bone_c_pips_outer + pips);

        if (val < *retValue) { 
          *retValue = val;
          *retDirection = Position::West;
          *retIndex = boneIdx;
        } 
        
      }

      break;
  
    case BOARD_MODE_ONE_BONE_PLAYED:
      {
        uint8_t val = (canBlock(pips) ? 0 : bone_c_pips_outer + (pips * (isDouble ? 2 : 1)));

        if (val < *retValue) { 
          *retValue = val;
          *retDirection = Position::West;
          *retIndex = boneIdx;
        }

      }

      break;
     
    case BOARD_MODE_DOUBLE_BONE_PLAYED:
      {
        uint8_t val = canBlock(pips) ? 0 : ((bone_e != NOTHING ? bone_e_pips_outer : bone_c_pips_outer + bone_c_pips_inner) + pips);

        if (val < *retValue) { 
          *retValue = val; 
          *retDirection = Position::West;
          *retIndex = boneIdx;
        }

      }

      break;

    case BOARD_MODE_X_AXIS_PLAYED:
      {
        uint8_t val = canBlock(pips) ? 0 : ((bone_n != NOTHING ? bone_n_pips_outer : 0) + (bone_s != NOTHING ? bone_s_pips_outer : 0) + bone_e_pips_outer + pips);

        if (val < *retValue) { 
          *retValue = val; 
          *retDirection = Position::West;
          *retIndex = boneIdx;
        }

      }
      
      break;
        
  }

}


/* ----------------------------------------------------------------------------
 *  Calculate the lowest possible score (West, Center Only).
 *  
 *  Can the nominated bone be played in the West position?  If so, could it 
 *  block the other player or drive the pip count down to a value that is lower 
 *  than all previously calculated ones ??  Moves that block the other player 
 *  are preferred to non-blocking moves.
 *    
 */
void lowestPossibleScore_West_CenterOnly(uint8_t boneIdx, uint8_t pips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {

  uint8_t val = canBlock(pips) ? 0 : (bone_c_pips_inner + bone_c_pips_outer + pips);

  if (val < *retValue) { 
    *retValue = val; 
    *retDirection = Position::West;
    *retIndex = boneIdx;
  }

}


/* ----------------------------------------------------------------------------
 *  Calculate the lowest possible score (East).
 *  
 *  Can the nominated bone be played in the East position?  If so, could it 
 *  block the other player or drive the pip count down to a value that is lower 
 *  than all previously calculated ones ??  Moves that block the other player 
 *  are preferred to non-blocking moves.
 *    
 */
void lowestPossibleScore_East(boolean isDouble, uint8_t boneIdx, uint8_t pips, uint8_t *retValue, uint8_t *retIndex, Position *retDirection) {

  switch (boardMode) {
    
    case BOARD_MODE_ONE_BONE_PLAYED:
      {
        uint8_t val = (canBlock(pips) ? 0 : bone_c_pips_inner + (pips * (isDouble ? 2 : 1)));

        if (val < *retValue) {
          *retValue = val;
          *retDirection = Position::East;
          *retIndex = boneIdx;
        }

      }          
      break;
      
    case BOARD_MODE_DOUBLE_BONE_PLAYED:
      {
        uint8_t val = canBlock(pips) ? 0 :((bone_w != NOTHING ? bone_w_pips_outer : bone_c_pips_outer + bone_c_pips_inner) + pips);

        if (val < *retValue) { 
          *retValue = val; 
          *retDirection = Position::East;
          *retIndex = boneIdx;
        }

      }     
      break;

    case BOARD_MODE_X_AXIS_PLAYED:
      {
        uint8_t val = canBlock(pips) ? 0 : ((bone_n != NOTHING ? bone_n_pips_outer : 0) + (bone_s != NOTHING ? bone_s_pips_outer : 0) + bone_w_pips_outer + pips);

        if (val < *retValue) { 
          *retValue = val; 
          *retDirection = Position::East;
          *retIndex = boneIdx;
        }
      }
      break;
      
  }
  
}


/* ----------------------------------------------------------------------------
 *  Play a bone.
 *  
 *  Play the bone at 'index' in the hand of the 'player' in cardinal location 
 *  'position'.
 *  
 */
void playBone(GameMode gameMode, Player player, uint8_t index, Position position) {

  printHand();
  
  // Update the board's end bones ..
  
  switch (position) {
      
    case Position::Center:
    
      if (bone_c == NOTHING) {
        bone_c = players_hand[static_cast<uint8_t>(player)][index];
        bone_c_pips_inner = leftPips_ByBoneId(bone_c);
        bone_c_pips_outer = rightPips_ByBoneId(bone_c);
      }
      else {
        bones_played[bones_played_idx] = bone_c;
        bones_played_idx++;
        bone_c = players_hand[static_cast<uint8_t>(player)][index];
        bone_c_pips_inner = bone_c_pips_outer;
        bone_c_pips_outer = playBone_NewPipValue(bone_c_pips_inner, bone_c);
      }
      break;
        
    case Position::North:
    
      if (bone_n == NOTHING) {
        bone_n = players_hand[static_cast<uint8_t>(player)][index];
        bone_n_pips_inner = bone_c_pips_outer;
        bone_n_pips_outer = playBone_NewPipValue(bone_n_pips_inner, bone_n);
      }
      else {
        bones_played[bones_played_idx] = bone_n;
        bones_played_idx++;
        bone_n = players_hand[static_cast<uint8_t>(player)][index];
        bone_n_pips_inner = bone_n_pips_outer;
        bone_n_pips_outer = playBone_NewPipValue(bone_n_pips_inner, bone_n);
      }
      break;
      
    case Position::South:
    
      if (bone_s == NOTHING) {
        bone_s = players_hand[static_cast<uint8_t>(player)][index];
        bone_s_pips_inner = bone_c_pips_outer;
        bone_s_pips_outer = playBone_NewPipValue(bone_s_pips_inner, bone_s);
      }
      else {
        bones_played[bones_played_idx] = bone_s;
        bones_played_idx++;
        bone_s = players_hand[static_cast<uint8_t>(player)][index];
        bone_s_pips_inner = bone_s_pips_outer;
        bone_s_pips_outer = playBone_NewPipValue(bone_s_pips_inner, bone_s);
      }
      break;      

    case Position::East:
    
      if (bone_e == NOTHING) {
        bone_e = players_hand[static_cast<uint8_t>(player)][index];
        if (bone_c == NOTHING) {
          bone_e_pips_inner = bone_e_pips_inner;
          bone_e_pips_outer = playBone_NewPipValue(bone_e_pips_inner, bone_e);
        }
        else {
          bone_e_pips_inner = bone_c_pips_outer;
          bone_e_pips_outer = playBone_NewPipValue(bone_e_pips_inner, bone_e);
        }
      }
      else {        
        bones_played[bones_played_idx] = bone_e;
        bones_played_idx++;
        bone_e = players_hand[static_cast<uint8_t>(player)][index];
        bone_e_pips_inner = bone_e_pips_outer;
        bone_e_pips_outer = playBone_NewPipValue(bone_e_pips_inner, bone_e);
      }
      break;
      
    case Position::West:

      if (bone_w == NOTHING) {
        bone_w = players_hand[static_cast<uint8_t>(player)][index];
        if (bone_c == NOTHING) {
          bone_w_pips_inner = bone_w_pips_inner;
          bone_w_pips_outer = playBone_NewPipValue(bone_w_pips_inner, bone_w);
        }
        else {
          bone_w_pips_inner = bone_c_pips_inner;
          bone_w_pips_outer = playBone_NewPipValue(bone_w_pips_inner, bone_w);
        }
      }
      else {
        bones_played[bones_played_idx] = bone_w;
        bones_played_idx++;
        bone_w = players_hand[static_cast<uint8_t>(player)][index];
        bone_w_pips_inner = bone_w_pips_outer;
        bone_w_pips_outer = playBone_NewPipValue(bone_w_pips_inner, bone_w);

      }
      break;

    default: break;
                  
  }
 
  
  // Shuffle the bones along in the players hand to the left ..

  for (uint8_t x = index; x < players_hand_idx[static_cast<uint8_t>(player)] - 1; x++) {
            
      players_hand[player][x] = players_hand[static_cast<uint8_t>(player)][x + 1]; 
      
  }

  players_hand[static_cast<uint8_t>(player)][players_hand_idx[static_cast<uint8_t>(player)] - 1] = 0;
  players_hand_idx[static_cast<uint8_t>(player)]--;

  

  // Update screen ..

  switch (boardMode) {
  
    case BOARD_MODE_NO_BONES_PLAYED:
      boardMode = (hasADoubleBeenPlayed(BOARD_MODE_NO_BONES_PLAYED) ? BOARD_MODE_DOUBLE_BONE_PLAYED : BOARD_MODE_ONE_BONE_PLAYED);
      break;
  
    case BOARD_MODE_ONE_BONE_PLAYED:
      boardMode = (hasADoubleBeenPlayed(BOARD_MODE_ONE_BONE_PLAYED) ? BOARD_MODE_DOUBLE_BONE_PLAYED : BOARD_MODE_TWO_BONES_PLAYED);
      break;
  
    case BOARD_MODE_TWO_BONES_PLAYED:
      boardMode = (hasADoubleBeenPlayed(BOARD_MODE_TWO_BONES_PLAYED) ? BOARD_MODE_DOUBLE_BONE_PLAYED : BOARD_MODE_TWO_BONES_PLAYED);
      break;
      
    case BOARD_MODE_DOUBLE_BONE_PLAYED:  
      boardMode = (hasXAxisBeenPlayed() ? BOARD_MODE_X_AXIS_PLAYED : BOARD_MODE_DOUBLE_BONE_PLAYED);
      break;
      
    case BOARD_MODE_X_AXIS_PLAYED:  
      break;

  }

  renderBoard(gameMode, player, Position::Nothing, false);


  // Update score ..

  if (getPipCount() % 5 == 0 && getPipCount() != 0) {

    renderGraveyard(player, NOTHING, players_hand_highlight_idx, false);
    players_score[static_cast<uint8_t>(player)] = players_score[static_cast<uint8_t>(player)] + getPipCount();
    renderPlayersScore(gameMode, player);
    renderPlayersHand(player, players_hand_highlight_idx, false);
  
  }
  else {
    renderGraveyard(player, NOTHING, players_hand_highlight_idx, false);
  }

}


/* ----------------------------------------------------------------------------
 *  Can the nominated bone be played in the position?
 *  
 */
boolean canPlayBoard(uint8_t boneIdx, uint8_t position) {

  uint8_t leftPips = leftPips_ByBoneId( boneIdx );
  uint8_t rightPips = rightPips_ByBoneId( boneIdx );
  
  switch (boardMode) {
  
    case BOARD_MODE_NO_BONES_PLAYED:

      return true;

    case BOARD_MODE_ONE_BONE_PLAYED:

      if (position == Position::East) {
        
        if (bone_e == NOTHING && bone_c != NOTHING && ( bone_c_pips_outer == leftPips || bone_c_pips_outer == rightPips)) {
          return true;
        }
  
        if (bone_e != NOTHING && ( bone_e_pips_inner == leftPips || bone_e_pips_inner == rightPips)) {
          return true;
        }

      }
      
      if (position == Position::West) {

        if (bone_w == NOTHING && bone_c != NOTHING && ( bone_c_pips_inner == leftPips || bone_c_pips_inner == rightPips)) {
          return true;
        }

        if (bone_w != NOTHING && ( bone_w_pips_outer == leftPips || bone_w_pips_outer == rightPips)) {
          return true;
        }
            
      }
      
      // if (position == Position::Center) {
          
      //   if (bone_c_pips_outer == NOTHING || bone_c_pips_outer == leftPips || bone_c_pips_outer == rightPips) {
      //     return true;
      //   }
      
      // }
      
      break;
      
    case BOARD_MODE_TWO_BONES_PLAYED:

      if (position == Position::West && (bone_w_pips_outer == leftPips || bone_w_pips_outer == rightPips)) {
        return true;
      }
    
      if (position == Position::Center && (bone_c_pips_outer == leftPips || bone_c_pips_outer == rightPips)) {
        return true;
      }

      break;
      
    case BOARD_MODE_DOUBLE_BONE_PLAYED:
    
      if (position == Position::East) {
        
        if (bone_e_pips_outer == NOTHING && (bone_c_pips_outer == leftPips || bone_c_pips_outer == rightPips)) {
          return true;
        }
    
        if (bone_e_pips_outer != NOTHING && (bone_e_pips_outer == leftPips || bone_e_pips_outer == rightPips)) {
          return true;
        }

      }
   
      if (position == Position::West) {
    
        if (bone_w_pips_outer == NOTHING && (bone_c_pips_inner == leftPips || bone_c_pips_inner == rightPips)) {
          return true;
        }
    
        if (bone_w_pips_outer != NOTHING && (bone_w_pips_outer == leftPips || bone_w_pips_outer == rightPips)) {
          return true;
        }

      }
      
      break;

    case BOARD_MODE_X_AXIS_PLAYED:

      if (position == Position::North) {
      
        if (bone_n_pips_outer == NOTHING && (bone_c_pips_outer == leftPips || bone_c_pips_outer == rightPips)) {
          return true;
        }
    
        if (bone_n_pips_outer != NOTHING && (bone_n_pips_outer == leftPips || bone_n_pips_outer == rightPips)) {
          return true;
        }
      
      }

      if (position == Position::South) {
      
        if (bone_s_pips_outer == NOTHING && (bone_c_pips_outer == leftPips || bone_c_pips_outer == rightPips)) {
          return true;
        }
    
        if (bone_s_pips_outer != NOTHING && (bone_s_pips_outer == leftPips || bone_s_pips_outer == rightPips)) {
          return true;
        }

      }

      if (position == Position::East && (bone_e_pips_outer == leftPips || bone_e_pips_outer == rightPips)) {
        return true;
      }
    
      if (position == Position::West && (bone_w_pips_outer == leftPips || bone_w_pips_outer == rightPips)) {
        return true;
      }

      break;
      
  }

  return false;
  
}

