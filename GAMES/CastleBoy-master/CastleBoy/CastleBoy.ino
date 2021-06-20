

#include "global.h"

#include "menu.h"
#include "game.h"
#include "assets.h"

uint8_t bootCounter = 0;

void inogamesetup(){
  WiFi.mode(WIFI_OFF);
  ab.begin();
  ab.setFrameRate(FPS);

  Menu::showTitle();
}

void inogameloop()
{
  if (!ab.nextFrame())
  {
    return;
  }

  ab.clear();
  
  if(bootCounter < 120)
  {
    bootCounter++;
    sprites.drawOverwrite(49, 14, logo, 0);
    ab.display();
    return;
  }

  ab.pollButtons();
  Menu::loop();

#ifdef DEBUG_LOG
  drawDebugLog();
#endif

#ifdef DEBUG_CPU
  drawDebugCpu();
#endif

#ifdef DEBUG_RAM
  drawDebugRam();
#endif

  if (flashCounter > 0)
  {
    ab.fillRect(0, 0, 128, 64, WHITE);
    flashCounter--;
  }
  ab.display();
}
void gameLogicLoop(void *)
{
  for (;;) {
    inogameloop(); 
    // ArduinoOTA.handle();
  }
}

void setup() {
  inogamesetup();
  xTaskCreatePinnedToCore(gameLogicLoop, "g", 4096, nullptr, 0, nullptr, 0);
}

void loop() {
	delay(60000);
}
