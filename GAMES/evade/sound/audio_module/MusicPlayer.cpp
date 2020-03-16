#include "Arduboy.h"
#include "MusicPlayer.h"
#include "Music.h"

int currentSong;



void stopMusic() {
    Arduboy ab;

    if (ab.tunes.playing()) {
        ab.tunes.stopScore();
    }
}

void playMusic(int song) {
    Arduboy ab;

   if (!ab.tunes.playing()) {

      if (song == 1 && currentSong != song) {
        stopMusic();
        ab.tunes.playScore(stage1Music);
      }
      else if (song == 2 && currentSong != song) {
        stopMusic();
        ab.tunes.playScore(stage2Music);
      }
          
      else if (song == 3 && currentSong != song) {
        stopMusic();
        ab.tunes.playScore(stage3Music);
      }
      else if (song == 4 && currentSong != song) {
        stopMusic();
        ab.tunes.playScore(bossMusic);
      }

    }
    
}




void playTone1() {
  Arduboy ab;
  ab.tunes.tone(987, 120);
  delay(120); 
  ab.tunes.tone(1318, 400);
}

void playTone2() {
  Arduboy ab;

  ab.tunes.tone(1318, 120);
  delay(120); 
  ab.tunes.tone(987, 400);

}


void playTone3() {
  Arduboy ab;
  ab.tunes.tone(1500, 120);
}

void playTone4() {
  Arduboy ab;
  ab.tunes.tone(900, 120);
}

void playTone5() {
  Arduboy ab;
  ab.tunes.tone(987, 120);
}

