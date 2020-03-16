#ifndef INTRO_H
#define INTRO_H

#include "global.h"
#include "assets.h"

#define INTRO_TEXT_FRAME_LIMIT 180

#define INTRO_LOGO_FRAMES 5
#define INTRO_LOGO_FRAME_UPDATE 30

#define INTRO_CRAWL_FRAME_UPDATE 3
#define INTRO_CRAWL_FRAME_LIMIT -180

void stateIntroText() {
  ab.setCursor(0, 24);
  for (unsigned char i = 0; i < TEXT_INTRO_LINES; i++) {
    ab.println(strcpy_P(tBuffer, (char*)pgm_read_dword(&(textIntro[i]))));
  }
  if (ab.everyXFrames(INTRO_TEXT_FRAME_LIMIT)) {
    gameState = STATE_INTRO_CRAWL;
  }
}

void stateIntroCrawl() {
  if(crawlFrameCount == INTRO_CRAWL_FRAME_LIMIT) {
    gameState = STATE_MENU;
  } else {
    Sprites::drawOverwrite(24, crawlFrameCount, StarWarsLogo, 0);
    ab.setCursor(24, crawlFrameCount + 50);
    for (unsigned char i = 0; i < CRAWL_LINES; i++) {
      ab.println(strcpy_P(tBuffer, (char*)pgm_read_dword(&(crawlText[i]))));
    }
    if (ab.everyXFrames(INTRO_CRAWL_FRAME_UPDATE)) {
      crawlFrameCount--;
    }
  }
}

#endif
