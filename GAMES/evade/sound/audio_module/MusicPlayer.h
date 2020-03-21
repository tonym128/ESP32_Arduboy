#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <stddef.h>
#include <inttypes.h>
#include <Arduino.h>
#ifdef ESP8266
#include <avr/pgmspace.h>
//#define pgm_read_ptr pgm_read_word
#else
#include <stdint.h>
#include <string.h>
#define PROGMEM
#define PSTR
#define pgm_read_byte(x) (*((uint8_t*)x))
#define pgm_read_word(x) (*((uint16_t*)x))
#define pgm_read_ptr(x) (*((uintptr_t*)x))
#define strlen_P(x) strlen(x)
#endif

extern void playMusic(int song);
extern void stopMusic();

extern void playTone1();
extern void playTone2();
extern void playTone3();
extern void playTone4();
extern void playTone5();


#endif
