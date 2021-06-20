#include "Arduboy.h"
Arduboy arduboy;

#include "MusicPlayer.h"



void inogamesetup()
{
  arduboy.begin();
  arduboy.setTextSize(1);
  arduboy.setCursor(0,0);
  arduboy.print(" ");
  arduboy.display();
}


int x = 0, 
    y = 0,
    playing = 0,
    currentTrack;


void loop ()
{
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame())) {
    return;
  }
  
  if (arduboy.pressed(UP_BUTTON) && playing == 0) {
    currentTrack = 1;
    playing = 1;
  } 
  else if (arduboy.pressed(DOWN_BUTTON) && playing == 0) {
    currentTrack = 2;
    playing = 1;
  } 
  else if (arduboy.pressed(LEFT_BUTTON) && playing == 0) {
    currentTrack = 3;
    playing = 1;
  } 
  else if (arduboy.pressed(RIGHT_BUTTON) && playing == 0) {
    currentTrack = 4;
    playing = 1;
  }

  // Gotta keep calling this so that the audio loops. Else, it will end when the song is over.
  if (playing == 1) {
    playMusic(currentTrack);
  }
  
  if (arduboy.pressed(A_BUTTON)|| arduboy.pressed(B_BUTTON)) {
    playing = 0;
    stopMusic();
  } 
    
  arduboy.clear();
  arduboy.setCursor(x,y);

  arduboy.print("Up: stage1Music\nDn: stage2Music\nLt: stage3Music\nRt: bossMusic\nAB: stop\n");
  
  if (playing == 0) {
    arduboy.print("\nStopped...");
    stopMusic();
  }
  else if (playing == 1) {
    arduboy.print("\nPlaying...\nPress A to stop");
  }
  

  arduboy.display();

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
