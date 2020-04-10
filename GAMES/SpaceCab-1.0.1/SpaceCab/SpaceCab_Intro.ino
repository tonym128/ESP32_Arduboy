#include "src/Utils/Arduboy2Ext.h"

void vsBoot() {
  
  Sprites::drawOverwrite(30, 17, bootlogo, 0);

  switch (bootCounter) {

    case 0:
      
      if (!fadeOutEffect.isComplete()) {

        fadeOutEffect.draw(arduboy);
        fadeOutEffect.update();

      }

      if (fadeOutEffect.isComplete())   state = GameState::SplashScreen_Init;

      break;

    case 1:

      fadeOutEffect.reset(0, HEIGHT, 1);
      --bootCounter;
      break;

    default:
  
      --bootCounter;
      break;
      
  }

}

void titleScreen(Level &level, Player &player, Customer &customer) {
  
  int16_t customerXVal = customer.getX() + level.xOffset.getInteger();
  int16_t customerYVal = customer.getY() + level.yOffset.getInteger();

  if (gotoCounter != 0)   gotoCounter--;

  if (ouchCounter != 0) { 

    ouchCounter--;
    if (ouchCounter == 0) {
      gateToRender = (gateToRender == 0 ? 1 : 0);
      launchCustomer(level, customer, gateToRender, GO_TO_GATE);
    }

  }

  Sprites::drawOverwrite(0, 7, SpaceCabSplash_1, 0);
  Sprites::drawOverwrite(29, 31, SpaceCabSplash_2, 0);
  drawLevel_Intro(level, gateToRender);

  playerDisplay(player);
  customerDisplay(level, player, customer, customerXVal, customerYVal);
  handleInput(player);


  checkCollisionWithCustomer(level, player, customer);

  drawGoto(level, player, customer, customerXVal, customerYVal);
  drawOuch(level, customer, customerXVal, customerYVal);

  if (player.getY() >= 41 && player.getYDelta()> 0) {
    player.setLandingGearDown(true);
  } 
  
}
