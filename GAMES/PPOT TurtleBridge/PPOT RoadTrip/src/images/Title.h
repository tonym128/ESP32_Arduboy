#pragma once

const uint8_t  RoadTrip_00[] = {
#ifndef DEBUG
74, 24,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0xf0, 0xe0, 0xfc, 0xf8, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0x1e, 0xff, 0xee, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0x3e, 0xff, 0xcc, 0xff, 0x30, 0xfe, 0x80, 0xf8, 0xe0, 0xfc, 0xf8, 0xfe, 0xfc, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0x1e, 0xff, 0xee, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0x3e, 0xff, 0xcc, 0xff, 0x30, 0xfe, 0x00, 0xf8, 0x80, 0xf0, 0xe0, 0xfc, 0xf8, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0x1e, 0xff, 0xe0, 0xff, 0x00, 0xf0, 0x00, 0xc0, 0x80, 0xf0, 0xe0, 0xfe, 0xfc, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0x1e, 0xff, 0xee, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0x7c, 0xff, 0x88, 0xfe, 0x70, 0xfc, 0x00, 0x78, 
0x00, 0x00, 0x00, 0x80, 0x00, 0xe0, 0xc0, 0xfc, 0x78, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0x47, 0xff, 0x78, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff, 0x9f, 0xff, 0x67, 0xff, 0x19, 0xff, 0x06, 0xff, 0xe1, 0xff, 0x7c, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xc7, 0xff, 0x78, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff, 0x8f, 0xff, 0x71, 0xff, 0x8e, 0xff, 0x61, 0xff, 0x78, 0xff, 0x7e, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x3f, 0xff, 0x1f, 0xff, 0xe3, 0xff, 0x7c, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xc1, 0xff, 0x3e, 0xff, 0x01, 0xff, 0xe0, 0xff, 0x7c, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x3f, 0xff, 0xc7, 0xff, 0x78, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f, 0xff, 0xff, 0xff, 0x7f, 0xff, 0x8f, 0xff, 0x71, 0xff, 0x0e, 0xff, 0x01, 0x1f, 0x00, 0x03, 0x00, 0x00, 
0x00, 0x3c, 0x18, 0xff, 0x57, 0xff, 0x51, 0xff, 0x51, 0xff, 0x51, 0xff, 0x51, 0xff, 0x4f, 0xff, 0x10, 0xff, 0x5f, 0xff, 0x51, 0xff, 0x51, 0xff, 0x51, 0xff, 0x51, 0xff, 0x5f, 0xff, 0x60, 0xff, 0x1f, 0xff, 0x00, 0x3f, 0x00, 0x3f, 0x1f, 0xff, 0x51, 0xff, 0x51, 0xff, 0x51, 0xff, 0x51, 0xff, 0x57, 0xff, 0x55, 0xff, 0x56, 0xff, 0x53, 0xff, 0x51, 0xff, 0x59, 0xff, 0x69, 0xff, 0x2d, 0xff, 0x13, 0x7f, 0x04, 0x3f, 0x19, 0xff, 0x5e, 0xff, 0x53, 0xff, 0x51, 0xff, 0x51, 0xff, 0x59, 0xff, 0x65, 0xff, 0x15, 0xff, 0x05, 0x3f, 0x19, 0xff, 0x51, 0xff, 0x51, 0xff, 0x51, 0xff, 0x51, 0xff, 0x5d, 0xff, 0x43, 0xff, 0x3c, 0xff, 0x03, 0x7f, 0x18, 0xff, 0x5f, 0xff, 0x51, 0xff, 0x51, 0xff, 0x51, 0xff, 0x51, 0xff, 0x57, 0xff, 0x55, 0xff, 0x56, 0xff, 0x53, 0xff, 0x51, 0xff, 0x51, 0xff, 0x69, 0xff, 0x2d, 0xff, 0x33, 0x7f, 0x0c, 0x7f, 0x03, 0x1f, 0x00, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
#else
1, 1, 1, 1
#endif
};

const uint8_t  RoadTrip_01[] = {
#ifndef DEBUG
66, 24,
0x00, 0x00,	0x00, 0x00,	0x00, 0x1e,	0x0c, 0x7f,	0x2e, 0xff,	0xce, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x3e, 0xff,	0xce, 0xff,	0x2e, 0xff,	0x2e, 0x7f,	0x26, 0x7f,	0x18, 0x7f,	0x00, 0xfc,	0xc0, 0xfe,	0xfc, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x7e, 0xff,	0x8e, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x0c, 0xff,	0xf0, 0xfe,	0x00, 0xfe,	0x0c, 0x0f,	0x0e, 0x0f,	0xee, 0xef,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x1e, 0xff,	0xee, 0xff,	0x2e, 0xff,	0x2e, 0x7f,	0x32, 0x7f,	0x0c, 0xff,	0xc0, 0xfe,	0xfc, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x7e, 0xff,	0x8e, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x0c, 0xff,	0xf0, 0xfe,	0x00, 0xf8,
0x00, 0x00,	0x00, 0x80,	0x00, 0xf0,	0xe0, 0xff,	0x7e, 0xff,	0x7f, 0xff,	0x7f, 0xff,	0x7f, 0xff,	0xff, 0xff,	0x1f, 0xff,	0xe3, 0xff,	0x1c, 0xff,	0x03, 0x3f,	0x00, 0x07,	0x00, 0x80,	0x00, 0xf8,	0xf0, 0xff,	0x7e, 0xff,	0x7f, 0xff,	0x7f, 0xff,	0xff, 0xff,	0xff, 0xff,	0xff, 0xff,	0x63, 0xff,	0x7c, 0xff,	0x7f, 0xff,	0x7f, 0xff,	0xff, 0xff,	0x7f, 0xff,	0xbf, 0xff,	0xc7, 0xff,	0x38, 0xff,	0x07, 0x7f,	0x00, 0x0f,	0xc0, 0xc1,	0x78, 0xf8,	0x7f, 0xff,	0x7f, 0xff,	0x7f, 0xff,	0xff, 0xff,	0xff, 0xff,	0x0f, 0xff,	0xf1, 0xff,	0x0e, 0xff,	0x01, 0x1f,	0x00, 0x83,	0x00, 0xf8,	0xf0, 0xff,	0x7e, 0xff,	0x7f, 0xff,	0x7f, 0xff,	0x7f, 0xff,	0x7f, 0xff,	0x5f, 0xff,	0x43, 0xff,	0x7c, 0xff,	0x7f, 0xff,	0x7f, 0xff,	0x7f, 0xff,	0xff, 0xff,	0x3f, 0xff,	0xc7, 0xff,	0x38, 0xff,	0x07, 0x7f,	0x00, 0x0f,	0x00, 0x01,
0x00, 0x3c,	0x18, 0xff,	0x57, 0xff,	0x51, 0xff,	0x51, 0xff,	0x51, 0xff,	0x59, 0xff,	0x4f, 0xff,	0x31, 0xff,	0x0e, 0x7f,	0x01, 0x1f,	0x00, 0x03,	0x00, 0x00,	0x00, 0x3c,	0x18, 0xff,	0x57, 0xff,	0x51, 0xff,	0x51, 0xff,	0x51, 0xff,	0x59, 0xff,	0x4f, 0xff,	0x10, 0xff,	0x5f, 0xff,	0x51, 0xff,	0x51, 0xff,	0x51, 0xff,	0x51, 0xff,	0x5f, 0xff,	0x60, 0xff,	0x17, 0xff,	0x18, 0xff,	0x54, 0xfd,	0x54, 0xfc,	0x53, 0xff,	0x51, 0xff,	0x51, 0xff,	0x51, 0xff,	0x51, 0xff,	0x51, 0xff,	0x57, 0xff,	0x54, 0xff,	0x5d, 0xff,	0x60, 0xff,	0x00, 0xf1,	0x00, 0x3c,	0x18, 0xff,	0x57, 0xff,	0x51, 0xff,	0x51, 0xff,	0x51, 0xff,	0x51, 0xff,	0x4d, 0xff,	0x33, 0xff,	0x0d, 0x7f,	0x05, 0x1f,	0x05, 0x0f,	0x05, 0x0f,	0x05, 0x0f,	0x05, 0x0f,	0x06, 0x0f,	0x03, 0x0f,	0x00, 0x07,	0x00, 0x01,	0x00, 0x00,	0x00, 0x00,	0x00, 0x00,
#else
1, 1, 1, 1
#endif
};

const uint8_t  HighScore[] = {
#ifndef DEBUG
96, 16,
0x00, 0x00, 0x0c, 0x0c, 0x0c, 0xfc, 0xfc, 0x0c, 0x0c, 0x0c, 0xf0, 0xf8, 0x1c, 0x0c, 0x0c, 0x0c, 0x1c, 0xf8, 0xf0, 0x00, 0x00, 0xfc, 0xfc, 0xcc, 0xcc, 0xcc, 0xfc, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xfc, 0xfc, 0x0c, 0x0c, 0x0c, 0x1c, 0xf8, 0xf0, 0x00, 0x00, 0xfc, 0xfc, 0xcc, 0xcc, 0xcc, 0xcc, 0x7c, 0x38, 0x00, 0x00, 0xfc, 0xfc, 0x00, 0x04, 0x3c, 0xf8, 0xc0, 0x00, 0xc0, 0xf8, 0x3c, 0x04, 0x00, 0xfc, 0xfc, 0xcc, 0xcc, 0xcc, 0xcc, 0xcc, 0x00, 0x00, 0xfc, 0xfc, 0xcc, 0xcc, 0xcc, 0xcc, 0x7c, 0x38, 0x00, 0x00, 0x78, 0xfc, 0xcc, 0xcc, 0xcc, 0xdc, 0x98, 0x00, 0x00, 
0x00, 0x40, 0x80, 0x80, 0x80, 0x8a, 0x8a, 0x80, 0x80, 0x80, 0x82, 0x82, 0x8a, 0x88, 0x88, 0x88, 0x8a, 0x82, 0x82, 0x80, 0x80, 0x8a, 0x8a, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x8a, 0x8a, 0x88, 0x88, 0x88, 0x8a, 0x82, 0x82, 0x80, 0x80, 0x8a, 0x8a, 0x80, 0x80, 0x80, 0x82, 0x8a, 0x88, 0x88, 0x80, 0x8a, 0x8a, 0x80, 0x80, 0x80, 0x80, 0x8a, 0x8a, 0x8a, 0x80, 0x80, 0x80, 0x80, 0x8a, 0x8a, 0x88, 0x88, 0x88, 0x88, 0x88, 0x80, 0x80, 0x8a, 0x8a, 0x80, 0x80, 0x80, 0x82, 0x8a, 0x88, 0x88, 0x80, 0x82, 0x8a, 0x88, 0x88, 0x88, 0x8a, 0x82, 0x40, 0x00, 
#else
1, 1, 1, 1
#endif
};

const uint8_t  Arrow[] = {
1, 8,
0xFF,
};

const uint8_t  Title_HighScore[] = {
37, 5,
0x1f, 0x04, 0x1f, 0x00, 0x11, 0x1f, 0x11, 0x00, 0x1f, 0x11, 0x19, 0x00, 0x1f, 0x04, 0x1f, 0x00, 0x00, 0x17, 0x15, 0x1d, 0x00, 0x1f, 0x11, 0x11, 0x00, 0x1f, 0x11, 0x1f, 0x00, 0x1f, 0x05, 0x1b, 0x00, 0x1f, 0x15, 0x11, 0x00, 
};

const uint8_t  Title_Start[] = {
19, 5,
0x17, 0x15, 0x1d, 0x00, 0x01, 0x1f, 0x01, 0x00, 0x1f, 0x05, 0x1f, 0x00, 0x1f, 0x05, 0x1b, 0x00, 0x01, 0x1f, 0x01, 
};

const uint8_t  GameOver_00[] = {
64, 23,
0x00, 0x00,	0x00, 0x00,	0x00, 0x00,	0x00, 0x00,	0x00, 0xf0,	0xf0, 0xfc,	0xfc, 0xfe,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x0e, 0xff,	0xd6, 0xff,	0xd6, 0xff,	0xd6, 0xff,	0xd6, 0xff,	0xd6, 0xff,	0x16, 0xff,	0x16, 0xbf,	0x18, 0xbf,	0x18, 0xfc,	0xc0, 0xfc,	0xf0, 0xfe,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x7e, 0xff,	0x80, 0xff,	0xf8, 0xfd,	0x00, 0xfc,	0xf0, 0xfe,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x3e, 0xff,	0xc0, 0xff,	0xf0, 0xfc,	0xfc, 0xfe,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x1e, 0xff,	0xe0, 0xff,	0x08, 0xfd,	0xf0, 0xfe,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x7e, 0xff,	0x8e, 0xff,	0xf6, 0xff,	0x36, 0xff,	0x16, 0x3f,	0x18, 0x3f,	0x18, 0x3c,	0x00, 0x1c,
0x00, 0x00,	0x00, 0xc0,	0xc0, 0xf8,	0xb8, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xff, 0xff,	0x3f, 0xff,	0xc7, 0xff,	0xb8, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0x7f, 0xff,	0x8f, 0xff,	0x70, 0xff,	0x9f, 0xff,	0xe3, 0xff,	0xb8, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0x7f, 0xff,	0x1f, 0xff,	0xe3, 0xff,	0xbc, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xff, 0xff,	0x0f, 0xff,	0x30, 0xff,	0xc7, 0xff,	0xb8, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xff, 0xff,	0x3f, 0xff,	0xff, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0x7f, 0xff,	0xbf, 0xff,	0xff, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xff, 0xff,	0x3f, 0xff,	0xc7, 0xff,	0x38, 0xff,	0xc7, 0xff,	0xb8, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xff, 0xff,	0x3f, 0xff,	0xcf, 0xff,	0xee, 0xff,	0x2e, 0xff,	0x22, 0x7f,	0x38, 0x7f,	0x08, 0x7c,	0x00, 0x08,	0x00, 0x00,	0x00, 0x00,
0x00, 0x06,	0x06, 0x0f,	0x09, 0x1f,	0x08, 0x3f,	0x28, 0x7f,	0x28, 0x7f,	0x29, 0x7f,	0x2a, 0x7f,	0x2b, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x2c, 0x7f,	0x23, 0x7f,	0x2c, 0x7f,	0x03, 0x7f,	0x0c, 0x5f,	0x0b, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x2c, 0x7f,	0x35, 0x7f,	0x25, 0x7f,	0x09, 0x3f,	0x08, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x2c, 0x7f,	0x23, 0x7f,	0x20, 0x7f,	0x0c, 0x7f,	0x0b, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x37, 0x7f,	0x28, 0x7f,	0x0f, 0x3f,	0x28, 0x7f,	0x26, 0x7f,	0x21, 0x7f,	0x0e, 0x7f,	0x09, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x2c, 0x7f,	0x33, 0x7f,	0x3c, 0x7f,	0x01, 0x3f,	0x0e, 0x1f,	0x09, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x29, 0x7f,	0x2a, 0x7f,	0x2d, 0x7f,	0x30, 0x7f,	0x30, 0x78,	0x00, 0x38,	0x00, 0x00,	0x00, 0x00,	0x00, 0x00,	0x00, 0x00,	0x00, 0x00,
};

const uint8_t  GameOver_01[] = {
59, 23,
0x00, 0x00,	0x00, 0x00,	0x00, 0x00,	0x00, 0x00,	0x00, 0xf0,	0xf0, 0xfc,	0xfc, 0xfe,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x1e, 0xff,	0xee, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x1e, 0xff,	0xe0, 0xfe,	0x38, 0xfc,	0xc0, 0xfe,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x1e, 0xff,	0x80, 0xff,	0xe0, 0xfc,	0xfc, 0xfe,	0xfe, 0xff,	0xfe, 0xff,	0x7e, 0xff,	0x8e, 0xff,	0x02, 0xff,	0xf0, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x7e, 0xff,	0x8e, 0xff,	0xf6, 0xff,	0x36, 0xff,	0x16, 0x3f,	0x18, 0x3f,	0x08, 0xfc,	0xf0, 0xfe,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x1e, 0xff,	0xee, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0xfe, 0xff,	0x1e, 0xff,	0xe0, 0xfe,	0x78, 0xfc,	0x00, 0x78,
0x00, 0x00,	0x00, 0xc0,	0xc0, 0xf8,	0xb8, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xff, 0xff,	0x3f, 0xff,	0xc7, 0xff,	0xb8, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0x7f, 0xff,	0x8f, 0xff,	0xf0, 0xff,	0x3f, 0xff,	0x03, 0xff,	0xf8, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xa3, 0xff,	0xbc, 0xff,	0xbf, 0xff,	0xff, 0xff,	0x3f, 0xff,	0xc7, 0xff,	0x39, 0xff,	0xc6, 0xff,	0xb8, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xff, 0xff,	0x3f, 0xff,	0xcf, 0xff,	0xee, 0xff,	0x2e, 0xff,	0x22, 0x7f,	0x38, 0xff,	0xc0, 0xfc,	0xb8, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xa7, 0xff,	0xb8, 0xff,	0xbf, 0xff,	0xbf, 0xff,	0xff, 0xff,	0x1f, 0xff,	0xef, 0xff,	0x70, 0xff,	0x3f, 0x7f,	0x03, 0x3f,	0x00, 0x03,	0x00, 0x00,
0x00, 0x06,	0x06, 0x0f,	0x09, 0x1f,	0x08, 0x3f,	0x28, 0x7f,	0x28, 0x7f,	0x29, 0x7f,	0x2a, 0x7f,	0x29, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x34, 0x7f,	0x3b, 0x7f,	0x1c, 0x3f,	0x0f, 0x1f,	0x01, 0x0f,	0x00, 0x1f,	0x0e, 0x1f,	0x09, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x2e, 0x7f,	0x33, 0x7f,	0x3c, 0x7f,	0x0f, 0x3f,	0x01, 0x1f,	0x0e, 0x1f,	0x09, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x29, 0x7f,	0x2a, 0x7f,	0x2d, 0x7f,	0x30, 0x7f,	0x30, 0x78,	0x00, 0x3e,	0x0e, 0x1f,	0x09, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x2c, 0x7f,	0x23, 0x7f,	0x2c, 0x7f,	0x0b, 0x7f,	0x28, 0x7f,	0x28, 0x7f,	0x2c, 0x7f,	0x23, 0x7f,	0x3c, 0x7f,	0x0f, 0x7f,	0x00, 0x0f,	0x00, 0x00,	0x00, 0x00,	0x00, 0x00,	0x00, 0x00,
};

const uint8_t  TransmissionType_Manual[] = {
32, 32,
0x00, 0xfc, 0x02, 0x02, 0xc2, 0xc2, 0xc2, 0x82, 0x02, 0x02, 0x02, 0x82, 0xc2, 0xc2, 0xc2, 0x02, 0x02, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0x02, 0x02, 0xfc, 0x00, 
0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0x0f, 0x1f, 0x3e, 0x1f, 0x0f, 0xff, 0xff, 0xff, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x00, 
0x00, 0xff, 0x00, 0x00, 0xff, 0x8b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x8b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x8b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 
0x00, 0x3f, 0x40, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x40, 0x3f, 0x00, 
};

const uint8_t  TransmissionType_Auto[] = {
32, 32,
0x00, 0xfc, 0x02, 0x02, 0x82, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0x82, 0x02, 0x02, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0xc2, 0x02, 0x02, 0xfc, 0x00, 
0x00, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0x71, 0x71, 0x71, 0x71, 0x71, 0xff, 0xff, 0xff, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0xff, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0xff, 0x00, 
0x00, 0xff, 0x00, 0x00, 0xff, 0x8b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x8b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x8b, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 
0x00, 0x3f, 0x40, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x40, 0x3f, 0x00, 
};

const uint8_t  TransmissionType_Mask[] = {
0xfc, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfc, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x3f, 
};

const uint8_t  TransmissionType_Highlight[] = {
36, 36,
0x00, 0xf8, 0x04, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x04, 0xf8, 0x00, 
0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 
0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 
0x00, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 
0x00, 0x01, 0x02, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x02, 0x01, 0x00, 
};

const uint8_t  TransmissionType_Highlight_Mask[] = {
0xfc, 0xfe, 0xff, 0x0f, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x0f, 0xff, 0xfe, 0xfc, 
0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
0x03, 0x07, 0x0f, 0x0f, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0e, 0x0f, 0x0f, 0x07, 0x03, 
};

const uint8_t  Aim[] = {
73, 32,
0x00, 0x00, 0x00, 0x1c, 0x70, 0x1c, 0x00, 0x78, 0x48, 0x78, 0x00, 0x78, 0x40, 0x78, 0x00, 0x78, 0x08, 0x18, 0x00, 0x00, 0x00, 0xb8, 0xa8, 0xf8, 0x00, 0x78, 0x48, 0x78, 0x00, 0x30, 0x48, 0x78, 0x00, 0x02, 0x7e, 0x00, 0x00, 0x00, 0x74, 0x00, 0x50, 0x48, 0x28, 0x00, 0x00, 0x00, 0x08, 0x3c, 0x48, 0x00, 0x78, 0x48, 0x78, 0x00, 0x00, 0x00, 0xf8, 0x48, 0x30, 0x00, 0x30, 0x48, 0x78, 0x00, 0x50, 0x48, 0x28, 0x00, 0x50, 0x48, 0x28, 0x00, 0x00, 
0x08, 0x3c, 0x48, 0x00, 0x7c, 0x08, 0x70, 0x00, 0x30, 0x68, 0x50, 0x00, 0x00, 0x00, 0x78, 0x08, 0x78, 0x00, 0x78, 0x48, 0x78, 0x00, 0x78, 0x10, 0x78, 0x00, 0x74, 0x00, 0x78, 0x08, 0x78, 0x00, 0x30, 0x48, 0x78, 0x00, 0x08, 0x3c, 0x48, 0x00, 0x30, 0x68, 0x50, 0x00, 0x30, 0x48, 0x7c, 0x00, 0x00, 0x00, 0x78, 0x08, 0x78, 0x00, 0x78, 0x40, 0x78, 0x00, 0x78, 0x10, 0x78, 0x00, 0x7c, 0x48, 0x30, 0x00, 0x30, 0x68, 0x50, 0x00, 0x78, 0x08, 0x18, 
0x00, 0x00, 0x00, 0x78, 0x48, 0x78, 0x00, 0x10, 0x7c, 0x14, 0x00, 0x00, 0x00, 0x78, 0x48, 0x48, 0x00, 0x30, 0x48, 0x78, 0x00, 0x78, 0x08, 0x18, 0x00, 0x50, 0x48, 0x28, 0x00, 0x00, 0x00, 0x30, 0x68, 0x50, 0x00, 0x30, 0x48, 0x78, 0x00, 0x78, 0x48, 0x48, 0x00, 0x7c, 0x08, 0x70, 0x00, 0x00, 0x00, 0x30, 0x48, 0x7c, 0x00, 0x30, 0x48, 0x78, 0x00, 0xb8, 0xa0, 0x78, 0x00, 0x00, 0x00, 0x08, 0x3c, 0x48, 0x00, 0x78, 0x48, 0x78, 0x00, 0x00, 0x00, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x78, 0x48, 0x48, 0x00, 0x78, 0x48, 0x78, 0x00, 0x78, 0x08, 0x78, 0x00, 0x08, 0x3c, 0x48, 0x00, 0x74, 0x00, 0x78, 0x08, 0x78, 0x00, 0x78, 0x40, 0x78, 0x00, 0x30, 0x68, 0x50, 0x00, 0x00, 0x00, 0x78, 0x08, 0x18, 0x00, 0x30, 0x48, 0x78, 0x00, 0x78, 0x48, 0x48, 0x00, 0x74, 0x00, 0x78, 0x08, 0x78, 0x00, 0xb8, 0xa8, 0x78, 0x00, 0x5c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

const uint8_t  Corner_TL[] = {
3, 8,
0x03, 0xfd, 0x02, 
};

const uint8_t  Corner_TR[] = {
3, 8,
0x02, 0xfd, 0x03, 
};

const uint8_t  Corner_BL[] = {
3, 8,
0x80, 0x3f, 0x40, 
};

const uint8_t  Corner_BR[] = {
3, 8,
0x40, 0x3f, 0x80, 
};