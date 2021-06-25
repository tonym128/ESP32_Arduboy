#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#define MAX_ASTEROIDS 16
#define MAX_EXPLOSIONS 3
#define MAX_SHOTS 5
#define SHOT_TTL 16
#define SHOT_SPEED 2
#define SIZEOF_ASTEROID_BITMAP_RECORD 34
#define SIZEOF_ASTEROID_VERTICES_RECORD 25
#define SIZEOF_SHIP_BITMAP_RECORD 10
#define SIZEOF_SHIP_VERTICES_RECORD 6
#define SIZEOF_SAUCER_VERTICES_RECORD 12
#define SIZEOF_EXPLOSION_BITMAP_RECORD 34
#define W 128
#define H 64
#define FOOTER_Y 86
#define THRUST 0.15
#define DRAG 0.02;
// trigonometric values for computing thrust at different angles
#define SIN67_5  0.9239 // cos(22.5) and sin(67.5)
#define SIN45 0.7071  // sin(45) and cos(45)
#define SIN22_5  0.3827 // sin(22.5) and cos(67.5)
#define F1 100
#define F2 970
#define FIRE 1
#define EXPLOSION 2



typedef struct Asteroid {
  byte x;
  byte y;
  byte info;  // high 4 bits are type, low 4 bits are heading
} Asteroid;

typedef struct Shot {
  byte x;
  byte y;
  byte ttl;
  byte heading;
} Shot;

typedef struct Explosion {
  byte x;
  byte y;
  byte index; // index into the explosion bitmaps
} Explosion;

#endif
