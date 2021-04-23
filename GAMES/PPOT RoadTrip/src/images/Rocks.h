#pragma once

const uint8_t PROGMEM Rock_00[] = {
3, 3,
0x00, 0x06,	0x00, 0x07,	0x00, 0x06,
};

const uint8_t PROGMEM Rock_01[] = {
5, 5,
0x00, 0x10,	0x00, 0x1f,	0x02, 0x1f,	0x00, 0x1f,	0x00, 0x10,
};

const uint8_t PROGMEM Rock_02[] = {
7, 7,
0x00, 0x40,	0x00, 0x7e,	0x1e, 0x7f,	0x06, 0x7f,	0x16, 0x7f,	0x00, 0x7e,	0x00, 0x40,
};

const uint8_t PROGMEM Rock_03[] = {
10, 9,
0x00, 0x00,	0x00, 0xf0,	0x70, 0xfe,	0x7e, 0xff,	0x1e, 0xff,	0x4e, 0xff,	0x2e, 0xff,	0x50, 0xfe,	0x00, 0xf0,	0x00, 0x00,
0x00, 0x01,	0x00, 0x01,	0x00, 0x01,	0x00, 0x01,	0x00, 0x01,	0x00, 0x01,	0x00, 0x01,	0x00, 0x01,	0x00, 0x01,	0x00, 0x01,
};

const uint8_t *Rocks[] = { Rock_00, Rock_01, Rock_02, Rock_03 };
const uint8_t Rocks_Width[] = { 3, 5, 7, 10 };