#include "src/utils/Arduboy2Ext.h"


/* ----------------------------------------------------------------------------
 *  Render player's score.
 *  
 *  If ANIMATIONS is defined, flash the score three times.  Disable ANIMATIONS 
 *  to speed up testing in development mode.
 *  
 */
void renderPlayersScore(Player player, HandPosition handPosition) { 

  switch (handPosition) {
    
    case HandPosition::Top:

      renderPlayerScore(HandPosition::Top, player, false);
        
      for (uint8_t y = 0; y < 6; y++) {

        renderPlayerScore(HandPosition::Top, player, (y % 2 == 0));
        arduboy.delayShort(250);

      }
      break;

    case HandPosition::Bottom:

      renderPlayerScore(HandPosition::Bottom, player, false);
        
      for (uint8_t y = 0; y < 6; y++) {

        renderPlayerScore(HandPosition::Bottom, player, (y % 2 == 0));
        arduboy.delayShort(250);

      }
      break;

  }

}


/* ----------------------------------------------------------------------------
 *  Render player's score.
 *  
 *  If ANIMATIONS is defined, flash the score three times.  Disable ANIMATIONS 
 *  to speed up testing in development mode.
 *  
 */
void renderPlayersScore(GameMode gameMode, Player player) { 

  Player altPlayer = getAltPlayer(gameMode, player);

  if (player == Player::Computer) {

    renderPlayerScore(HandPosition::Top, player, false);
    renderPlayerScore(HandPosition::Bottom, altPlayer, false);
      
    for (uint8_t y = 0; y < 6; y++) {

      if (player == Player::Computer) {
        renderPlayerScore(HandPosition::Top, player, (y % 2 == 0));
      }
      else {
        renderPlayerScore(HandPosition::Bottom, altPlayer, (y % 2 == 0));
      }

      arduboy.delayShort(250);

    }

  }
  else {

    renderPlayerScore(HandPosition::Top, altPlayer, false);
    renderPlayerScore(HandPosition::Bottom, player, false);
      
    for (uint8_t y = 0; y < 6; y++) {

//      if (player == Player::Computer) {
//        renderPlayerScore(HandPosition::Top, altPlayer, (y % 2 == 0));
//      }
//      else {
        renderPlayerScore(HandPosition::Bottom, player, (y % 2 == 0));
//      }

      arduboy.delayShort(250);

    }

  }

}

/* ----------------------------------------------------------------------------
 *  Render a single player's score.
 *  
 */
void renderPlayerScore(HandPosition position, Player player, bool highlight) {

  uint8_t player_score = players_score[static_cast<uint8_t>(player)];
  uint8_t player_hand_bones = players_hand_idx[player];
  
  if (position == HandPosition::Top) {

    if (!highlight) {
      
      arduboy.fillRect(SCORE_COMPUTER_LEFT, SCORE_COMPUTER_TOP, SCORE_COMPUTER_WIDTH, SCORE_COMPUTER_HEIGHT + 8, BLACK);
      Sprites::drawOverwrite(SCORE_COMPUTER_LEFT + 1, SCORE_COMPUTER_TOP, Images::digits, player_score / 100);
      player_score = player_score - (player_score / 100) * 100;
      Sprites::drawOverwrite(SCORE_COMPUTER_LEFT + 6, SCORE_COMPUTER_TOP, Images::digits, player_score / 10);
      Sprites::drawOverwrite(SCORE_COMPUTER_LEFT + 11, SCORE_COMPUTER_TOP, Images::digits, player_score % 10);
      
      Sprites::drawOverwrite(SCORE_COMPUTER_LEFT + 1, SCORE_COMPUTER_TOP + 8, Images::digits, player_hand_bones / 10);
      Sprites::drawOverwrite(SCORE_COMPUTER_LEFT + 6, SCORE_COMPUTER_TOP + 8, Images::digits, player_hand_bones % 10);
      
    }
    else {
      
#ifdef SOUNDS
  
      sound.tone(NOTE_C3,50, NOTE_D3,50, NOTE_E3,50);

#endif 
      
      arduboy.fillRect(SCORE_COMPUTER_LEFT, SCORE_COMPUTER_TOP, SCORE_COMPUTER_WIDTH, SCORE_COMPUTER_HEIGHT, WHITE);
      Sprites::drawErase(SCORE_COMPUTER_LEFT + 1, SCORE_COMPUTER_TOP, Images::digits, player_score / 100);
      player_score = player_score - (player_score / 100) * 100;
      Sprites::drawErase(SCORE_COMPUTER_LEFT + 6, SCORE_COMPUTER_TOP, Images::digits, player_score / 10);
      Sprites::drawErase(SCORE_COMPUTER_LEFT + 11, SCORE_COMPUTER_TOP, Images::digits, player_score % 10);
      
    }
    
  }
 
  if (position == HandPosition::Bottom) {

    if (!highlight) {
      arduboy.fillRect(SCORE_PLAYER_LEFT, SCORE_PLAYER_TOP, SCORE_PLAYER_WIDTH, SCORE_PLAYER_HEIGHT, BLACK);
      Sprites::drawOverwrite(SCORE_PLAYER_LEFT + 1, SCORE_PLAYER_TOP, Images::digits, player_score / 100);
      player_score = player_score - (player_score / 100) * 100;
      Sprites::drawOverwrite(SCORE_PLAYER_LEFT + 6, SCORE_PLAYER_TOP, Images::digits, player_score / 10);
      Sprites::drawOverwrite(SCORE_PLAYER_LEFT + 11, SCORE_PLAYER_TOP, Images::digits, player_score % 10); 
      
    }
    else {
      
#ifdef SOUNDS
  
      sound.tone(NOTE_C4,30, NOTE_D4,30, NOTE_E4,30);

#endif 

      arduboy.fillRect(SCORE_PLAYER_LEFT, SCORE_PLAYER_TOP, SCORE_PLAYER_WIDTH, SCORE_PLAYER_HEIGHT, WHITE);
      Sprites::drawErase(SCORE_PLAYER_LEFT + 1, SCORE_PLAYER_TOP, Images::digits, player_score / 100);
      player_score = player_score - (player_score / 100) * 100;
      Sprites::drawErase(SCORE_PLAYER_LEFT + 6, SCORE_PLAYER_TOP, Images::digits, player_score / 10);
      Sprites::drawErase(SCORE_PLAYER_LEFT + 11, SCORE_PLAYER_TOP, Images::digits, player_score % 10);

    }
    
  }

  arduboy.display();

}


/* ----------------------------------------------------------------------------
 *  Render the player's hand.
 *  
 *  Only six bones can be rendered across the screen however the players hand
 *  may contain many more.  Render the bones from index 'players_hand_visible_idx'
 *  to 'players_hand_visible_idx' plus six.
 *  
 */
void renderPlayersHand(Player player, uint8_t highlight, bool hideBones) { 

  if (player == Player::Computer) player = getAltPlayer(gameMode, player);

  uint8_t x = 0;

  arduboy.fillRect(PLAYER_BOTTOM_LEFT_ARROW_X, PLAYER_BOTTOM_HAND_Y, WIDTH, HEIGHT, BLACK);
 
  for (uint8_t y = players_hand_visible_idx; y < players_hand_idx[player]; y++) {

    if (!hideBones) {
      drawBone_Rotated(bones_ref[players_hand[static_cast<uint8_t>(player)][y]], PLAYER_BOTTOM_HAND_X + (x * BONES_SMALL_X_SPACING_LANDSCAPE), PLAYER_BOTTOM_HAND_Y, y == highlight);
    }
    else {
      Sprites::drawOverwrite(PLAYER_BOTTOM_HAND_X + (x * BONES_SMALL_X_SPACING_LANDSCAPE), PLAYER_BOTTOM_HAND_Y, Images::bone_landscape_hidden, 0);
    }

    x++;
    if (y - players_hand_visible_idx + 1 >= PLAYER_BOTTOM_VISIBLE) break;

  }

  Sprites::drawOverwrite(PLAYER_BOTTOM_LEFT_ARROW_X, PLAYER_BOTTOM_LEFT_ARROW_Y, Images::leftArrow, (players_hand_visible_idx != 0 ? 1 : 0));
  Sprites::drawOverwrite(PLAYER_BOTTOM_RIGHT_ARROW_X, PLAYER_BOTTOM_RIGHT_ARROW_Y, Images::rightArrow, (players_hand_idx[1] - players_hand_visible_idx) > PLAYER_HAND_MAX_VISIBLE ? 1 : 0);
  arduboy.display();

}


/* ----------------------------------------------------------------------------
 *  Render the graveyard.
 *  
 *  The graveyard includes available bones and those already played.  As the 
 *  bones are 17 pixels high, they overwrite the player's hand and this must
 *  be redrawn.
 *  
 *  The graveyard can only 'accommodate' 21 bones, as defined by the constants
 *  BONES_GRAVEYARD_BONES_PER_ROW and BONES_GRAVEYARD_BONES_ROWS.  Therefore
 *  rendering stops after the third row.  This gives the computer an advantage
 *  when getting towards the end of a long game as he can see every bone that
 *  has been played whereas the human may not be able to.  Tough luck!
 *  
 */
void renderGraveyard(Player currentPlayer, uint8_t highlight, uint8_t playersHandHighlight, bool hideBones) { 

  arduboy.fillRect(BONES_GRAVEYARD_X, BONES_GRAVEYARD_Y, BONES_GRAVEYARD_X_MAX, BONES_GRAVEYARD_Y_MAX, BLACK); 

  uint8_t x = BONES_GRAVEYARD_X;
  uint8_t y = BONES_GRAVEYARD_Y;
  uint8_t rows = 1;
  
  
  // Draw the available bones ..
  
  for (uint8_t z = 0; z < bones_available_idx; z++) {
    
    drawBone_Hidden(x, y, z == highlight);
    x = x + BONES_SMALL_X_SPACING_PORTRAIT;
    
    if (x > (WIDTH - BONES_SMALL_X_SPACING_PORTRAIT)) { 
      x = BONES_GRAVEYARD_X; 
      y = y + BONES_SMALL_Y_SPACING_PORTRAIT;
      rows++;
    }
    
  }

  
  // Draw the played bones ..

  for (uint8_t z = 0; z < bones_played_idx; z++) {
    
    drawBone_Small(bones_ref[bones_played[z]], x, y, false);
    x = x + BONES_SMALL_X_SPACING_PORTRAIT;
    
    if (x > (WIDTH - BONES_SMALL_X_SPACING_PORTRAIT)) { 
      x = BONES_GRAVEYARD_X; 
      y = y + BONES_SMALL_Y_SPACING_PORTRAIT;
      rows++;
    }
    
    
    // Do not render a fourth row as there is not enough real-estate ..
    
    if (rows > BONES_GRAVEYARD_BONES_ROWS) { break; }
    
  }

  renderPlayersHand(currentPlayer, playersHandHighlight, hideBones);
  arduboy.display();    
  
}


/* ----------------------------------------------------------------------------
 *  Render the board.
 *  
 *  Render the playing board including played bones, available positions where
 *  bones can be played and the player's scores.  As the rendering of the board
 *  overwrites the player's hand this is also re-rendered.
 *  
 */
void renderBoard(GameMode gameMode, Player player, Position highlight, bool hideBones) {


  // Clear background ..
  
  arduboy.fillRect(0, 0, 55, 50, BLACK);
  

  // Render players scores ..
  
  if (player == Player::Computer) {
    renderPlayerScore(HandPosition::Top, player, false);
    renderPlayerScore(HandPosition::Bottom, getAltPlayer(gameMode, player), false);
  }
  else {
    renderPlayerScore(HandPosition::Top, getAltPlayer(gameMode, player), false);
    renderPlayerScore(HandPosition::Bottom, player, false);
  }


  // Render board ..
 
  if (boardMode == BOARD_MODE_NO_BONES_PLAYED) {

    drawBone_Outline_Rotated(BONE_C_POS_X_NO_BONES_PLAYED, BONE_C_POS_Y_NO_BONES_PLAYED, highlight == Position::Center);

  } else if (boardMode == BOARD_MODE_TWO_BONES_PLAYED) {

    drawBone_Rotated(bone_w_pips_outer, bone_w_pips_inner, BONE_W_POS_X_TWO_BONES_PLAYED, BONE_W_POS_Y_TWO_BONES_PLAYED, highlight == Position::West);
    drawBone_Rotated(bone_c_pips_inner, bone_c_pips_outer, BONE_C_POS_X_TWO_BONES_PLAYED, BONE_C_POS_Y_TWO_BONES_PLAYED, highlight == Position::Center);

  } else {

    if (boardMode == BOARD_MODE_X_AXIS_PLAYED){
      
      if (bone_n == NOTHING)    drawBone_Outline(BONE_N_POS_X_AXIS_PLAYED, BONE_N_POS_Y_AXIS_PLAYED, highlight == Position::North);
      if (bone_n != NOTHING)    {
        if (isDouble(bone_n))   {
          drawBone_Rotated(bone_n_pips_outer, bone_n_pips_inner, BONE_N_POS_X_DOUBLE_BONE_PLAYED - 4, BONE_N_POS_Y_DOUBLE_BONE_PLAYED + 1, highlight == Position::North);
          Sprites::drawSelfMasked(BONE_N_POS_X_DOUBLE_BONE_PLAYED, BONE_N_POS_Y_DOUBLE_BONE_PLAYED + 11, Images::Half_Top, 0);
        }
        else {
          drawBone_Small(bone_n_pips_outer, bone_n_pips_inner, BONE_N_POS_X_DOUBLE_BONE_PLAYED, BONE_N_POS_Y_DOUBLE_BONE_PLAYED, highlight == Position::North);
        }
      }        
    }
    
    if (bone_w == NOTHING)      drawBone_Outline_Rotated(BONE_W_POS_X_DOUBLE_BONE_PLAYED, BONE_W_POS_Y_DOUBLE_BONE_PLAYED, highlight == Position::West);
    if (bone_w != NOTHING)      {
      if (isDouble(bone_w))     {
        drawBone_Small(bone_w_pips_outer, bone_w_pips_inner, BONE_W_POS_X_DOUBLE_BONE_PLAYED, BONE_W_POS_Y_DOUBLE_BONE_PLAYED - 4, highlight == Position::West);
        Sprites::drawSelfMasked(BONE_W_POS_X_DOUBLE_BONE_PLAYED + 10, BONE_W_POS_Y_DOUBLE_BONE_PLAYED, Images::Half_Left, 0);
      }
      else {
        drawBone_Rotated(bone_w_pips_outer, bone_w_pips_inner, BONE_W_POS_X_DOUBLE_BONE_PLAYED, BONE_W_POS_Y_DOUBLE_BONE_PLAYED, highlight == Position::West);
      }
    }
    
    drawBone_Rotated(bone_c_pips_inner, bone_c_pips_outer, BONE_C_POS_X_DOUBLE_BONE_PLAYED, BONE_C_POS_Y_DOUBLE_BONE_PLAYED, highlight == Position::Center);
    
    if (bone_e == NOTHING)      drawBone_Outline_Rotated(BONE_E_POS_X_DOUBLE_BONE_PLAYED, BONE_E_POS_Y_DOUBLE_BONE_PLAYED, highlight == Position::East);
    if (bone_e != NOTHING)      {
      if (isDouble(bone_e))     {
        drawBone_Small(bone_e_pips_inner, bone_e_pips_outer, BONE_E_POS_X_DOUBLE_BONE_PLAYED + 7, BONE_E_POS_Y_DOUBLE_BONE_PLAYED - 4, highlight == Position::East);
        Sprites::drawSelfMasked(BONE_E_POS_X_DOUBLE_BONE_PLAYED, BONE_E_POS_Y_DOUBLE_BONE_PLAYED, Images::Half_Right, 0);
      }
      else {
        drawBone_Rotated(bone_e_pips_inner, bone_e_pips_outer, BONE_E_POS_X_DOUBLE_BONE_PLAYED, BONE_E_POS_Y_DOUBLE_BONE_PLAYED, highlight == Position::East);
      }
    }
            
    if (boardMode == BOARD_MODE_X_AXIS_PLAYED){
      
      if (bone_s == NOTHING)    drawBone_Outline(BONE_S_POS_X_AXIS_PLAYED, BONE_S_POS_Y_AXIS_PLAYED, highlight == Position::South);
      if (bone_s != NOTHING)    {
        if (isDouble(bone_s))   {
          drawBone_Rotated(bone_s_pips_inner, bone_s_pips_outer, BONE_S_POS_X_DOUBLE_BONE_PLAYED - 4, BONE_S_POS_Y_DOUBLE_BONE_PLAYED + 7, highlight == Position::South);
          Sprites::drawSelfMasked(BONE_S_POS_X_DOUBLE_BONE_PLAYED, BONE_S_POS_Y_DOUBLE_BONE_PLAYED, Images::Half_Bottom, 0);
        }
        else {          
          drawBone_Small(bone_s_pips_inner, bone_s_pips_outer, BONE_S_POS_X_DOUBLE_BONE_PLAYED, BONE_S_POS_Y_DOUBLE_BONE_PLAYED, highlight == Position::South);
        }
      }

    }
      
  }

  renderPlayersHand(player, players_hand_highlight_idx, hideBones);
  arduboy.display();
  
}
