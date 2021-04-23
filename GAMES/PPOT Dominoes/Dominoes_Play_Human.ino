#include "src/utils/Arduboy2Ext.h"


/* ----------------------------------------------------------------------------
 *  Play the human's hand.
 *  
 */
void playersHand_Loop(Player player, Player secondPlayer) {

  uint8_t playerIdx = static_cast<uint8_t>(player);

  players_hand_highlight_idx = 0;
  players_hand_visible_idx = 0;
  renderGraveyard(player, NOTHING, players_hand_highlight_idx, false);

  while (gameState >= GameState::PlayHuman_HandSel) {

    arduboy.pollButtons();
    uint8_t justPressed = arduboy.justPressedButtons();


    
    // If the human cannot play a bone or draw one from the graveyard, the hand is over ..
    
    if (!canPlay(player) && bones_available_idx == 0) {

      gameState = (canPlay(secondPlayer) && !canPlay(player) ? (gameMode == GameMode::SinglePlayer ? GameState::PlayComputer : GameState::SwapPlayers) : GameState::PlayGame);

      if (gameState == GameState::PlayGame) {

        char message[] = ("Stalemate!~No\none~can~play\na~bone.");

        uint8_t player1HandVal = handValue(player);
        uint8_t player2HandVal = handValue(secondPlayer);

        if (player1HandVal < player2HandVal) {
          players_score[static_cast<uint8_t>(player)] = players_score[static_cast<uint8_t>(player)] + (player2HandVal - player1HandVal);
          drawMessageBox(message, 3, false);
          renderPlayersScore(player, HandPosition::Bottom);
          WaitForButtonPress();
        }
        else if (player1HandVal > player2HandVal) {
          players_score[static_cast<uint8_t>(secondPlayer)] = players_score[static_cast<uint8_t>(secondPlayer)] + (player1HandVal - player2HandVal);
          drawMessageBox(message, 3, false);
          renderPlayersScore(secondPlayer, HandPosition::Top);
          WaitForButtonPress();
        }
        else {
          drawMessageBox(message, 3, true);
        }

      }
      else {

//      drawMessageBox(F("You~can't~play~a\nbone~and~have\nto~pass."), 3, true);
        drawMessageBox(F(" You~can~not\n play~a~bone\n and~pass."), 3, true);

      }
      
    }

  
    // The human can play or draw so handle the actions ..
          
    if (gameState >= GameState::PlayHuman_HandSel) { 

      switch (gameState) {

        case GameState::PlayHuman_HandSel:
  
          if ((justPressed &LEFT_BUTTON) && players_hand_highlight_idx > 0) {
  
            players_hand_highlight_idx--;
            if (players_hand_highlight_idx < players_hand_visible_idx) {
              players_hand_visible_idx--;
            }

            renderPlayersHand(player, players_hand_highlight_idx, false);
            
          }

          if ((justPressed & RIGHT_BUTTON) && players_hand_highlight_idx < players_hand_idx[playerIdx] - 1) {
          
            players_hand_highlight_idx++;
            if (players_hand_highlight_idx >= players_hand_visible_idx + PLAYER_BOTTOM_VISIBLE) {
              players_hand_visible_idx++;
            }

            renderPlayersHand(player, players_hand_highlight_idx, false);
            
          }
              
          if ((justPressed & UP_BUTTON) && bones_available_idx > 0) { 

            graveyard_highlight_idx = bones_available_idx - 1;
            gameState = GameState::PlayHuman_GraveSel;
            renderGraveyard(player, graveyard_highlight_idx, NOTHING, false);
  
          }
  
          if ((justPressed & DOWN_BUTTON) && bones_available_idx > 0) { 
  
            graveyard_highlight_idx = 0;
            gameState = GameState::PlayHuman_GraveSel;
            renderGraveyard(player, graveyard_highlight_idx, NOTHING, false);
  
          }
          
          if (justPressed & A_BUTTON) { 

            if (!canPlayBone(players_hand[playerIdx][players_hand_highlight_idx])) {

              flashRedLED();
              
            }
            else {

              flashGreenLED();
             
              gameState = GameState::PlayHuman_BoardSel;
              board_highlighted_idx = Position::Center;
              renderBoard(gameMode, player, board_highlighted_idx, false);

            }
            
          }
          
          if (justPressed & B_BUTTON) { 

            if (exitGame()) {
              gameState = GameState::TitleScreen;
            }
            else {
              renderGraveyard(player, NOTHING, players_hand_highlight_idx, false);
            }
            
          }

          break;

        case GameState::PlayHuman_GraveSel:
           
          if ((justPressed & LEFT_BUTTON) && graveyard_highlight_idx > 0) {
  
            graveyard_highlight_idx--;
            renderGraveyard(player, graveyard_highlight_idx, NOTHING, false);
            
          }

          if (justPressed & RIGHT_BUTTON) {

            if (graveyard_highlight_idx < bones_available_idx - 1) {

              graveyard_highlight_idx++;
              renderGraveyard(player, graveyard_highlight_idx, NOTHING, false);

            }
         
          }
           
          if (justPressed & UP_BUTTON) {
            
            if (graveyard_highlight_idx > BONES_GRAVEYARD_BONES_PER_ROW) {

              graveyard_highlight_idx = graveyard_highlight_idx - BONES_GRAVEYARD_BONES_PER_ROW;
              renderGraveyard(player, graveyard_highlight_idx, NOTHING, false);
              
            }
            else {

              graveyard_highlight_idx = NOTHING;
              gameState = GameState::PlayHuman_HandSel;
              renderGraveyard(player, graveyard_highlight_idx, players_hand_highlight_idx, false);

            }
            
          }
           
          if (justPressed & DOWN_BUTTON) {
            
            if (graveyard_highlight_idx < bones_available_idx - BONES_GRAVEYARD_BONES_PER_ROW) {

              graveyard_highlight_idx = graveyard_highlight_idx + BONES_GRAVEYARD_BONES_PER_ROW;
              renderGraveyard(player, graveyard_highlight_idx, NOTHING, false);
              
            }
            else {

              graveyard_highlight_idx = NOTHING;
              gameState = GameState::PlayHuman_HandSel;
              renderGraveyard(player, graveyard_highlight_idx, players_hand_highlight_idx, false);

            }
            
          }

          if (justPressed & A_BUTTON) {

            players_hand[playerIdx][players_hand_idx[playerIdx]] = bones_available[bones_available_idx];
            players_hand_idx[playerIdx] = players_hand_idx[playerIdx] + 1;
            bones_available_idx--;

            players_hand_highlight_idx = players_hand_idx[playerIdx] - 1;
            players_hand_visible_idx = (players_hand_highlight_idx >= PLAYER_BOTTOM_VISIBLE ? players_hand_highlight_idx - PLAYER_BOTTOM_VISIBLE + 1: 0);
        
            board_highlighted_idx = Position::Nothing;
            graveyard_highlight_idx = NOTHING;

            gameState = GameState::PlayHuman_HandSel;
            renderGraveyard(player, NOTHING, players_hand_highlight_idx, false);
              
          }
          
          break;

        case GameState::PlayHuman_BoardSel:

          if ((justPressed & LEFT_BUTTON) && board_highlighted_idx == Position::Center && boardMode != BOARD_MODE_NO_BONES_PLAYED) {

            board_highlighted_idx = Position::West;
            renderBoard(gameMode, player, board_highlighted_idx, false);
            
          }

          if ((justPressed & LEFT_BUTTON) && board_highlighted_idx == Position::East) {

            board_highlighted_idx = Position::Center;
            renderBoard(gameMode, player, board_highlighted_idx, false);
            
          }

          if ((justPressed & RIGHT_BUTTON) && board_highlighted_idx == Position::Center && boardMode == BOARD_MODE_ONE_BONE_PLAYED) {

            board_highlighted_idx = Position::East;
            renderBoard(gameMode, player, board_highlighted_idx, false);
            
          }

          if ((justPressed & RIGHT_BUTTON) && board_highlighted_idx == Position::Center && boardMode > BOARD_MODE_TWO_BONES_PLAYED) {

            board_highlighted_idx = Position::East;
            renderBoard(gameMode, player, board_highlighted_idx, false);
            
          }
          
          if ((justPressed & RIGHT_BUTTON) && board_highlighted_idx == Position::West) {

            board_highlighted_idx = Position::Center;
            renderBoard(gameMode, player, board_highlighted_idx, false);
            
          }

          if ((justPressed & UP_BUTTON) && board_highlighted_idx == Position::Center && boardMode == BOARD_MODE_X_AXIS_PLAYED) {

            board_highlighted_idx = Position::North;
            renderBoard(gameMode, player, board_highlighted_idx, false);
            
          }

          if ((justPressed & UP_BUTTON) && board_highlighted_idx == Position::South && boardMode == BOARD_MODE_X_AXIS_PLAYED) {

            board_highlighted_idx = Position::Center;
            renderBoard(gameMode, player, board_highlighted_idx, false);
            
          }

          if ((justPressed & DOWN_BUTTON) && board_highlighted_idx == Position::Center && boardMode == BOARD_MODE_X_AXIS_PLAYED) {

            board_highlighted_idx = Position::South;
            renderBoard(gameMode, player, board_highlighted_idx, false);
            
          }

          if ((justPressed & DOWN_BUTTON) && board_highlighted_idx == Position::North && boardMode == BOARD_MODE_X_AXIS_PLAYED) {

            board_highlighted_idx = Position::Center;
            renderBoard(gameMode, player, board_highlighted_idx, false);
            
          }

          if (justPressed & A_BUTTON) {

            if (!canPlayBoard(players_hand[playerIdx][players_hand_highlight_idx], board_highlighted_idx)) {

              flashRedLED();
              
            }
            else {

              flashGreenLED();
              playBone(gameMode, player, players_hand_highlight_idx, board_highlighted_idx);
     
              players_hand_highlight_idx = NOTHING;
              players_hand_visible_idx = 0;
              board_highlighted_idx = Position::Nothing;

              if (gameMode == GameMode::SinglePlayer) {
                gameState = GameState::PlayComputer;
              }
              else {
                gameState = GameState::SwapPlayers;
              }

              renderPlayersHand(player, NOTHING, false);

            }
          
          }

          if (justPressed & B_BUTTON) {
          
            board_highlighted_idx = Position::Nothing;
            renderBoard(gameMode, player, board_highlighted_idx, false);
            gameState = GameState::PlayHuman_HandSel;
          
          }
          
          break;
          
        default: break;
      
      }

    }    


    // Pass control to the computer if the game is still in play or finish the game ..
      
    gameState = (isAnyoneOut(gameMode) ? GameState::PlayGame : gameState);
    gameState = (isTheGameOver(gameMode) ? GameState::GameOver : gameState);

    if (gameState >= GameState::PlayHuman_HandSel) {
      gameState = (canEitherPlayerMove(gameMode) ? gameState : GameState::PlayGame);
    }

    arduboy.delayShort(100);
  
  }


  // The playersTurn variable is used to record whose turn it is between rounds ..
  
  if (gameMode == GameMode::SinglePlayer) {

    playersTurn = Player::Computer;

  }

}
