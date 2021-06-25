#include "sin.h"

 const int16_t st[] =
{
  0,
  50,
  98,
  142,
  181,
  213,
  237,
  251,
  256,
  251,
  237,
  213,
  181,
  142,
  98,
  50,
  0,
  -50,
  -98,
  -142,
  -181,
  -213,
  -237,
  -251,
  -256,
  -251,
  -237,
  -213,
  -181,
  -142,
  -98,
  -50,
};

 const int16_t tt[] =
{
 25,
 78,
 137,
 210,
 312,
 479,
 844,
 2599,
};



uint8_t atn(int16_t x, int16_t y){
  if(x == 0 && y >= 0) return 8;
  if(x == 0 && y <  0) return 24;
  int16_t t = y * 256 / x;
  uint8_t i = 0;
  if(x >  0 && y >= 0){
    while(i < 8 && tn(i) < t) i++;
    return i--;
  } else if(x <  0 && y >= 0){
    while(i < 8 && -tn(7-i) < t) i++;
    return i+8;
  } else if(x <  0 && y <  0){
    while(i < 8 && tn(i) < t) i++;
    return i+16;
  } else { // if(x >  0 && y <  0)
    while(i < 8 && -tn(7-i) < t) i++;
    return (i+24)%32;
  }
}

int16_t dist(int16_t x1,int16_t y1,int16_t x2,int16_t y2){
  uint16_t dx = (x1-x2)/256;
  uint16_t dy = (y1-y2)/256;
  return dx*dx+dy*dy;
}
/*
static uint32_t sd[4]={123456789, 362436069, 521288629, 88675123};

void init_xor128(uint32_t s) {
OB  for (uint8_t i=0; i<4; ++i) sd[i]=s=1812433253U*(s^(s>>30))+i;
}

uint32_t xor128() {
  uint32_t *a=sd;
  uint32_t t=(a[0]^(a[0]<<11));
  a[0]=a[1]; a[1]=a[2]; a[2]=a[3];
  return (a[3]=(a[3]^(a[3]>>19))^(t^(t>>8)));
}

int16_t xrandom(int16_t a, int16_t b){
  return (int16_t)(xor128()%(b-a)+a);
}
*/

int32_t sqr(int32_t x) {
  int32_t s, t;

  if (x <= 0) return 0;

  s = 1;  t = x;
  while (s < t) {  s <<= 1;  t >>= 1;  }
  do {
    t = s;
    s = (x / s + s) >> 1;
  } while (s < t);

  return t;
}

