#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <stddef.h>
#include <inttypes.h>
#include <Arduino.h>
#include <avr/pgmspace.h>

extern void playMusic(int song);
extern void stopMusic();

extern void playTone1();
extern void playTone2();
extern void playTone3();
extern void playTone4();
extern void playTone5();


#endif
