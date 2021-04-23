#include "src/utils/Arduboy2Ext.h"
#include "src/utils/Enums.h"

#define TITLE_DELAY_BEFORE_PROMPT      20
#define TITLE_PORTRAIT_BONE_X          1
#define TITLE_PORTRAIT_BONE_Y          1
#define TITLE_LANDSCAPE_BONE_X         18
#define TITLE_LANDSCAPE_BONE_Y         36
#define TITLE_TITLE_X                  19
#define TITLE_TITLE_Y                  2
#define TITLE_PRESSA_X                 49
#define TITLE_PRESSA_Y                 52

#define TITLE_UPPER_LINE_X             70
#define TITLE_UPPER_LINE_X_MAX         127
#define TITLE_UPPER_LINE_Y             8

#define TITLE_LOWER_LINE_X             TITLE_UPPER_LINE_X
#define TITLE_LOWER_LINE_X_MAX         TITLE_UPPER_LINE_X_MAX
#define TITLE_LOWER_LINE_Y             28

#define TITLE_HEADLINE_X               75
#define TITLE_HEADLINE_Y               15
#define TITLE_PROMPT_X                 79
#define TITLE_PROMPT_Y                 45
#define TITLE_PROMPT_CIRCLE_X          117
#define TITLE_PROMPT_CIRCLE_Y          49
#define TITLE_PROMPT_CIRCLE_RAD        5


/* ----------------------------------------------------------------------------
 *  Handle the title screen.
 */
void drawTitle_Loop() {

  uint8_t restart = 0;
  uint8_t delayA = 0;
  uint8_t topArrowOffset = 0;
  uint8_t bottomArrowOffset = 0;
  uint8_t arrowOffsetCount = 0;


  // Reset the scores ..
  memset(players_score, 0, (sizeof(players_score) / sizeof(players_score[0])));

  while (gameState == GameState::TitleScreen) {

    if (!(arduboy.nextFrame())) { delay(20); };
    arduboy.pollButtons();

    uint8_t justPressed = arduboy.justPressedButtons();


    // Clear the screen and render the splash ..
    
    arduboy.clear();
    Sprites::drawSelfMasked(TITLE_TITLE_X, TITLE_TITLE_Y, Images::Title, 0);

    switch (titleMode) {

      case TitleMode::PressA:

        if (delayA >= 55) {
          Sprites::drawSelfMasked(35, 42, Images::Bullet, 0);
          Sprites::drawSelfMasked(90, 42, Images::Bullet, 0);
        }

        if (delayA >= 110) {
          Sprites::drawSelfMasked(27, 42, Images::Bullet, 0);
          Sprites::drawSelfMasked(98, 42, Images::Bullet, 0);
        }

        if (delayA >= 165) {
          Sprites::drawSelfMasked(19, 42, Images::Bullet, 0);
          Sprites::drawSelfMasked(106, 42, Images::Bullet, 0);
        }

        if (delayA == 175) {
          Sprites::drawSelfMasked(TITLE_PRESSA_X, TITLE_PRESSA_Y, Images::PressA, 0);
        }
        else {
          delayA++;
        }

        if (justPressed & A_BUTTON) { titleMode = TitleMode::EnterNumberOfPlayers; }
        if (justPressed & B_BUTTON) { gameState = GameState::HighScore; }

        break;

      case TitleMode::EnterNumberOfPlayers:

        font4x6.setCursor(16,52);
        font4x6.print(F("Number~of~players~?"));

        switch (gameMode) {

          case GameMode::SinglePlayer:
            font4x6.setCursor(108, 52);
            font4x6.print(F("1"));
            break;

          case GameMode::TwoPlayer:
            font4x6.setCursor(108, 52);
            font4x6.print(F("2"));
            break;

        }

        Sprites::drawSelfMasked(107, 49 - topArrowOffset, Images::ArrowUp, 0);
        Sprites::drawSelfMasked(107, 60 + bottomArrowOffset, Images::ArrowDown, 0);

        if (justPressed & A_BUTTON) { gameState = GameState::GetNames; }
        if (justPressed & B_BUTTON) { titleMode = TitleMode::PressA; }
        if ((justPressed & UP_BUTTON) && gameMode == GameMode::SinglePlayer)   { gameMode = GameMode::TwoPlayer; topArrowOffset = 2; arrowOffsetCount = 10; }
        if ((justPressed & DOWN_BUTTON) && gameMode == GameMode::TwoPlayer)    { gameMode = GameMode::SinglePlayer; bottomArrowOffset = 2; arrowOffsetCount = 10; arduboy.clearButtonState(); }

        break;

    }


    arduboy.display();

    if (arrowOffsetCount > 0) {
      arrowOffsetCount--;
      if (arrowOffsetCount == 0) {
        topArrowOffset = 0;
        bottomArrowOffset = 0;
      }
    }


    // Restart ?

    if (arduboy.pressed(DOWN_BUTTON)) {

      if (restart < UPLOAD_DELAY) {
        restart++;
      }
      else {
        arduboy.exitToBootloader();
      }

    }
    else {
      restart = 0;
    }

  }
  
}
