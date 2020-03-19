// This contains the regular and 90-degree-clockwise rotated versions
// of the 'block' graphics. They are all 8x10 pixels.
// (The arduboy.drawBitmap function requires rotated graphics as unput)

PROGMEM const unsigned char mask[10]={
B00001100,
B00111100,
B11111110,
B11111111,
B11111111,
B11111111,
B11111111,
B01111111,
B00111100,
B00110000
};

PROGMEM const unsigned char mask90c[16]={
124,252,254,254,255,255,252,248,0,0,3,3,1,1,0,0
};

PROGMEM const unsigned char block[10]={
B00000000,
B00001000,
B00111100,
B00111100,
B00010010,
B00000100,
B00101010,
B00010100,
B00000000,
B00000000
};

PROGMEM const unsigned char block90c[16]={
0,0,76,156,78,172,80,0,0,0,0,0,0,0,0,0
};

PROGMEM const unsigned char black[10]={
B00000000,
B01000000,
B01000000,
B01000011,
B00101100,
B00010000,
B00010000,
B00010000,
B00000000,
B00000000
};

PROGMEM const unsigned char black90c[16]={
0,14,16,224,16,16,8,8,0,0,0,0,0,0,0,0
};



