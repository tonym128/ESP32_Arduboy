#include <avr/pgmspace.h>

#define sign(x) (((x) > 0) ? 1 : ((x) < 0) ? -1: 0)
#define sn(a) (int16_t)pgm_read_word_near(st + (a)%32)
#define cs(a) (int16_t)pgm_read_word_near(st + ((a)%32+8)%32)
#define tn(a) (int16_t)pgm_read_word_near(tt + (a))
//#define dist(x1,y1,x2,y2) ((x1)-(x2))*((x1)-(x2))+((y1)-(y2))*((y1)-(y2))
extern const int16_t st[];
extern const int16_t tt[];
uint8_t atn(int16_t x, int16_t y);
int16_t dist(int16_t x1,int16_t y1,int16_t x2,int16_t y2);
int32_t sqr(int32_t x);
/*
void init_xor128(uint32_t s);
uint32_t xor128();
int16_t xrandom(int16_t a, int16_t b);
*/
