#include <Arduino.h>
#include "sound.h"

void sound_play (int noteFrequency)
{
  int x;
  // Convert the frequency to microseconds
  float microsecondsPerWave = 1000000/noteFrequency;
  // Calculate how many milliseconds there are per HIGH/LOW cycles.
  float millisecondsPerCycle = 100000/(microsecondsPerWave * 2);
  // Multiply noteDuration * number or cycles per millisecond

  for (x=0;x<millisecondsPerCycle;x++)
  {
    digitalWrite(SOUND_PIN,HIGH);
    delayMicroseconds(microsecondsPerWave);
    digitalWrite(SOUND_PIN,LOW);
    delayMicroseconds(microsecondsPerWave);
  }
}

void sound_hit_stone() {
   sound_play(16);
}

void sound_hit_bird() {
   sound_play(697);
}

void sound_extra_life()
{
   sound_play(988);
   sound_play(3951);
}
