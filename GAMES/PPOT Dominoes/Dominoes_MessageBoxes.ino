#include "src/utils/Arduboy2Ext.h"

#define MESSAGE_BOX_DELAY_INIT      500
#define MESSAGE_BOX_DELAY           20
#define MESSAGE_BOX_DELAY_SHORT     5

#define MESSAGE_BOX_X               BONES_GRAVEYARD_X
#define MESSAGE_BOX_X_MAX           BONES_GRAVEYARD_X_MAX
#define MESSAGE_BOX_Y               BONES_GRAVEYARD_Y
#define MESSAGE_BOX_Y_MAX           BONES_GRAVEYARD_Y_MAX - 4
#define MESSAGE_BOX_Y_2_LINE        MESSAGE_BOX_Y + 10
#define MESSAGE_BOX_Y_3_LINE        MESSAGE_BOX_Y + 7

#define MESSAGE_BOX_EXIT_CIRCLE_X   115
#define MESSAGE_BOX_EXIT_CIRCLE_Y   38
#define MESSAGE_BOX_EXIT_CIRCLE_RAD 5


void drawTalkBubble(uint8_t lines) {

  arduboy.fillRect(BONES_GRAVEYARD_X, BONES_GRAVEYARD_Y, BONES_GRAVEYARD_X_MAX, BONES_GRAVEYARD_Y_MAX, BLACK);

  uint8_t y1 = (lines == 2 ? 3 : 2);
  uint8_t y2 = (lines == 2 ? 23 : 26);

  Sprites::drawSelfMasked(MESSAGE_BOX_X, MESSAGE_BOX_Y + y1, Images::Talk, 0);
  Sprites::drawSelfMasked(MESSAGE_BOX_X, MESSAGE_BOX_Y + y2, Images::Talk, 1);

}

/* ----------------------------------------------------------------------------
    Render a message box with an animated hourglass.

    Render a message box with an animated hourglass when the computer is about
    to play a bone.  The animation can get quite annoying - especially when
    debugging so it can conditionally be suppressed by removing the definition
    ANIMATIONS.  The user can skip the animation by pressing the A or B button.
    Once the animation is complete, a confirmation of the bone being played 
    is shown.

*/
void drawMessageBox_WithHourglass(uint8_t boneIdx) {

#ifdef ANIMATIONS
  uint8_t rotations = 2;
#endif

  arduboy.fillRect(BONES_GRAVEYARD_X, BONES_GRAVEYARD_Y, BONES_GRAVEYARD_X_MAX, BONES_GRAVEYARD_Y_MAX, BLACK);

#ifdef ANIMATIONS

  while (rotations > 0) {

    if ((arduboy.nextFrame())) {

      drawTalkBubble(3);
      Sprites::drawErase(MESSAGE_BOX_X + 23, MESSAGE_BOX_Y + 7, Images::hourglass, frame);

      if (arduboy.everyXFrames(3)) frame++;

      if (frame > 11 || (rotations == 1 && frame == 9)) {

        rotations--;
        frame = 0;

      }

      arduboy.display();

    }

    if (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) {
      break;
    }

  }

#endif

  frame = 0;

  drawTalkBubble(3);
  font4x6.setTextColor(BLACK);
  font4x6.setCursor(MESSAGE_BOX_X + 4, MESSAGE_BOX_Y_2_LINE);
  font4x6.print(F("I will play\nthe    ~bone."));
  drawBone_Rotated_Erase(bones_ref[boneIdx], MESSAGE_BOX_X + 22, 21);
  font4x6.setTextColor(WHITE);

  arduboy.display();
  DelayOrButtonPress(MESSAGE_BOX_DELAY);

}


/* ----------------------------------------------------------------------------
 *  Render a simple message.
 */
void drawMessageBox(String message, uint8_t lines, bool waitForButtonPress) {

  drawMessageBox(message, lines, waitForButtonPress, Player::None);

}

void drawMessageBox(String message, uint8_t lines, bool waitForButtonPress, Player renderPlayerScore) {

  arduboy.delayShort(MESSAGE_BOX_DELAY_INIT);

  drawTalkBubble(lines);

  font4x6.setTextColor(BLACK);
  font4x6.setCursor(MESSAGE_BOX_X + 4, (lines == 2 ? MESSAGE_BOX_Y_2_LINE : MESSAGE_BOX_Y_3_LINE));
  font4x6.print(message);
  font4x6.setTextColor(WHITE);

  arduboy.display();


  // Should a players score be updated?

  if (renderPlayerScore != Player::None) {
    renderPlayersScore(gameMode, renderPlayerScore);
  }


  // Should the message box be dismissed after a delay or wait until a button is pressed?

  if (waitForButtonPress) {

    WaitForButtonPress();

  }
  else {

    DelayOrButtonPress((renderPlayerScore == Player::None ? MESSAGE_BOX_DELAY : MESSAGE_BOX_DELAY_SHORT));

  }

}


void drawMessageBox(String message, uint8_t lines) {

  drawTalkBubble(lines);
  font4x6.setTextColor(BLACK);
  font4x6.setCursor(MESSAGE_BOX_X + 4, (lines == 2 ? MESSAGE_BOX_Y_2_LINE : MESSAGE_BOX_Y_3_LINE));
  font4x6.print(message);
  font4x6.setTextColor(WHITE);

}

/* ----------------------------------------------------------------------------
 *  Render an exit message box.
 */
boolean exitGame() {

  drawTalkBubble(3);
  font4x6.setTextColor(BLACK);
  font4x6.setCursor(MESSAGE_BOX_X + 4, MESSAGE_BOX_Y_3_LINE);
  font4x6.print(F("~Abandon~game\n~and~return?"));
    
  font4x6.setCursor(MESSAGE_BOX_X + 4, MESSAGE_BOX_Y_3_LINE + 22);
  font4x6.print(F("~Confirm"));
  Sprites::drawErase(113, 27, Images::BOnly, 0);
  font4x6.setTextColor(WHITE);
  arduboy.display();

  arduboy.delayShort(250);

  return (WaitForButtonPress() == B_BUTTON ? true : false);

}

