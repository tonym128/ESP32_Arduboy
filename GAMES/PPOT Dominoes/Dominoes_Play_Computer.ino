#include "src/utils/Arduboy2Ext.h"

/* ----------------------------------------------------------------------------
 *  Play the computer's hand.
 *  
 */
void computersHand_Loop() {

  uint8_t boneIdx = NOTHING;
  Position bonePosition = Position::Nothing;
  
  players_hand_highlight_idx = 0;
  players_hand_visible_idx = 0;
  board_highlighted_idx = Position::Nothing;

  while (gameState == GameState::PlayComputer) {

    bool drawBone = false;

    
    // Can a scoring play be made ?
    
    highestPossibleScore(Player::Computer, &boneIdx, &bonePosition);
    
    if (boneIdx != NOTHING) {
    
      drawMessageBox_WithHourglass(players_hand[static_cast<uint8_t>(Player::Computer)][boneIdx]);
      playBone(gameMode, Player::Computer, boneIdx, bonePosition);
       
    }
    else {


      // If not, what move will drive the PIP count down?
      
      lowestPossibleScore(Player::Computer, &boneIdx, &bonePosition);
        
      if (boneIdx != NOTHING) {

        drawMessageBox_WithHourglass(players_hand[static_cast<uint8_t>(Player::Computer)][boneIdx]);
        playBone(gameMode, Player::Computer, boneIdx, bonePosition);
        gameState == GameState::PlayHuman_HandSel;
           
      }
      else {

        
        // No bones can be played so draw ..

        if (bones_available_idx > 0) {

          players_hand[static_cast<uint8_t>(Player::Computer)][players_hand_idx[static_cast<uint8_t>(Player::Computer)]] = bones_available[bones_available_idx];
          drawMessageBox(F("~I~can't~go.~I\n~will~draw~an\n~extra~bone."), 3, false);

          players_hand_idx[static_cast<uint8_t>(Player::Computer)] = players_hand_idx[static_cast<uint8_t>(Player::Computer)] + 1;
          bones_available_idx--;
          renderPlayerScore(HandPosition::Top, Player::Computer, false);
          renderGraveyard(Player::Computer, NOTHING, NOTHING, false);
          drawBone = true;
        
        }
        else {  // No bones to draw so pass ..

          gameState = (canPlay(Player::Human_1) ? GameState::PlayComputer : GameState::PlayGame);

          if (gameState == GameState::PlayComputer) {

            drawMessageBox(F("~I~can't~go.~I\n~will~have~to\n~pass."), 3, true);
            gameState == GameState::PlayHuman_HandSel;

          }
          else {

            char message[] = "No~one~can~go\nand~this~game\nis~a~draw!";

            uint8_t player1HandVal = handValue(Player::Human_1);
            uint8_t player2HandVal = handValue(Player::Computer);

            if (player1HandVal < player2HandVal) {
              players_score[static_cast<uint8_t>(Player::Human_1)] = players_score[static_cast<uint8_t>(Player::Human_1)] + (player2HandVal - player1HandVal);
              drawMessageBox(message, 3, false);
              renderPlayersScore(Player::Human_1, HandPosition::Bottom);
              WaitForButtonPress();
            }
            else if (player1HandVal < player2HandVal) {
              players_score[static_cast<uint8_t>(Player::Computer)] = players_score[static_cast<uint8_t>(Player::Computer)] + (player1HandVal - player2HandVal);
              drawMessageBox(message, 3, false);
              renderPlayersScore(Player::Computer, HandPosition::Top);
              WaitForButtonPress();
            }
            else {
              drawMessageBox(message, 3, true);
            }

          }

        }
        
      }
    
    }

    boneIdx = NOTHING;
    bonePosition = bonePosition;

    
    // Pass control to the human if the game is still in play or finish the game ..
    
    if (!drawBone) {

      gameState = (isAnyoneOut(gameMode) ? GameState::PlayGame : gameState);
      gameState = (isTheGameOver(gameMode) ? GameState::GameOver : gameState);

      if (gameState == GameState::PlayComputer) {
        gameState = (canEitherPlayerMove(gameMode) ? GameState::PlayHuman_HandSel: GameState::PlayComputer);
      }
      
    }
    
    arduboy.delayShort(100);
  
  }


  // The playersTurn variable is used to record whose turn it is between rounds ..
  
  playersTurn = Player::Human_1;

}
