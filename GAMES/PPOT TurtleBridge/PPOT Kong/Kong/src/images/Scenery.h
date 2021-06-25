#pragma once

namespace Images {

  const uint8_t  Girder[] = {
    11, 8,
    0x87, 0x8f, 0x9d, 0xb7, 0xe7, 0xc7, 0xa5, 0xd7, 0xaf, 0x97, 0x8d,  
  };

  const uint8_t  Girder_Small[] = {
    13, 8,
    0x25, 0x2d, 0x39, 0x31, 0x29, 0x31, 0x25, 0x2d, 0x39, 0x31, 0x29, 0x31, 0x21, 
  };

  const uint8_t  Girder_OverHead[] = {
    12, 8,
    0x62, 0x7a, 0x7a, 0x62, 0x62, 0x7a, 0x7a, 0x62, 0x62, 0x7a, 0x7a, 0x62, 
  };

  const uint8_t  Ladder[] = {
    11, 19,
    0x00, 0xfe, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0x48, 0xfe, 0x00, 
    0x00, 0xff, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0x92, 0xff, 0x00, 
    0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 
  };

  const uint8_t  Ladder_Mask[] = {
    0xff, 0xff, 0xff, 0xfc, 0xfc, 0xfc, 0xfc, 0xfc, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0x07, 0x07, 0x07, 0x01, 0x01, 0x01, 0x01, 0x01, 0x07, 0x07, 0x07, 
  };

  const uint8_t  Plate[] = {
    25, 16,

    // Plate_1_LHS
    0x01, 0x01, 0x07, 0x05, 0x05, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x05, 0x05, 0x07, 0x04, 0x04, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    // Plate_1_RHS
    0x04, 0x04, 0x07, 0x05, 0x05, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x05, 0x05, 0x07, 0x01, 0x01, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    // Plate_2
    0x01, 0x01, 0x07, 0x05, 0x05, 0x07, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x07, 0x05, 0x05, 0x07, 0x01, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    // Plate_3_LHS
    0x01, 0x01, 0x0f, 0x0b, 0x0a, 0x0e, 0x06, 0x06, 0x06, 0x04, 0x0c, 0x0c, 0x0c, 0x08, 0x18, 0x18, 0x18, 0x18, 0x70, 0x50, 0x50, 0x70, 0x40, 0x40, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    // Plate_3_RHS
    0x00, 0x40, 0x40, 0x70, 0x50, 0x50, 0x70, 0x18, 0x18, 0x18, 0x18, 0x08, 0x0c, 0x0c, 0x0c, 0x04, 0x06, 0x06, 0x06, 0x0e, 0x0a, 0x0b, 0x0f, 0x01, 0x01, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    // Plate_4_LHS
    0x01, 0x07, 0x0b, 0x0a, 0x0e, 0x06, 0x04, 0x0c, 0x0c, 0x08, 0x18, 0x18, 0x10, 0x30, 0x30, 0x20, 0xe0, 0x60, 0x40, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 

    // Plate_4_RHS
    0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x40, 0x60, 0xe0, 0x20, 0x30, 0x30, 0x10, 0x18, 0x18, 0x08, 0x0c, 0x0c, 0x04, 0x06, 0x0e, 0x0a, 0x0b, 0x07, 0x01, 
    0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    // Plate_5_LHS
    0x19, 0x36, 0x2c, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x6c, 0xd8, 0xb0, 0x60, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

    // Plate_5_RHS
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xc0, 0x60, 0x30, 0x18, 0x2c, 0x36, 0x19, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x60, 0xb0, 0xd8, 0x6c, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  };

  const uint8_t  Lever[] = {
    11, 11,
    0xf8, 0x38, 0x78, 0xe8, 0xd0, 0xa0, 0x00, 0xa0, 0x10, 0x0c, 0x0c, 
    0x07, 0x04, 0x04, 0x04, 0x05, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 

    0xf8, 0x38, 0x78, 0xe0, 0xc8, 0xa4, 0x43, 0x83, 0x00, 0x00, 0x00, 
    0x07, 0x04, 0x04, 0x04, 0x05, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 
  };

  const uint8_t  Cable_1[] = {
    20, 3,
    0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 
  };

  const uint8_t  Cable_1_Mask[] = {
    0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 0x07, 0x07, 0x00, 0x07, 
  };

  const uint8_t  Cable_2[] = {
    20, 2,
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 
  };

  const uint8_t  Cable_2_Mask[] = {
    20, 2,
    0x03, 0x03, 0x00, 0x03, 0x03, 0x03, 0x00, 0x03, 0x03, 0x03, 0x00, 0x03, 0x03, 0x03, 0x00, 0x03, 0x03, 0x03, 0x00, 0x03, 
  };

  const uint8_t  Mode[] = {
    19, 8,
    0x00, 0x00, 0x7c, 0x54, 0x44, 0x00, 0x78, 0x14, 0x78, 0x00, 0x48, 0x54, 0x24, 0x00, 0x0c, 0x70, 0x0c, 0x00, 0x00, 

    0x00, 0x00, 0x7c, 0x10, 0x7c, 0x00, 0x78, 0x14, 0x78, 0x00, 0x7c, 0x14, 0x68, 0x00, 0x7c, 0x44, 0x38, 0x00, 0x00, 
  };

  const uint8_t  Mode_Mask[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  };

  const uint8_t  LivesLeft[] = {
    8, 7,
    0x00, 0x14, 0x3c, 0x36, 0x3a, 0x16, 0x0c, 0x00, 
  };

  const uint8_t  LivesLeft_Mask[] = {
    0x14, 0x3e, 0x7e, 0x7f, 0x7f, 0x3f, 0x1e, 0x0c, 
  };

  const uint8_t  Buildings[] = {
    63, 16,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x18, 0xd8, 0x58, 0x18, 0xd8, 0xd8, 0x18, 0xd8, 0xd8, 0x18, 0xd8, 0xd8, 0x18, 0xf8, 0x78, 0x80, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0x03, 0xdb, 0xdb, 0x03, 0xdb, 0xcb, 0x03, 0xdb, 0xdb, 0x03, 0xdb, 0x5b, 0x03, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x20, 0x00, 0x00, 0x20, 0x20, 0x00, 0x20, 0x20, 0x20, 0x20, 0x3f, 0x00, 0x36, 0x16, 0x00, 0x36, 0x36, 0x00, 0x36, 0x32, 0x00, 0x36, 0x36, 0x00, 0x3f, 0x00, 0x36, 0x16, 0x00, 0x36, 0x36, 0x00, 0x36, 0x32, 0x00, 0x36, 0x36, 0x00, 0x3f, 0x00, 0x36, 0x32, 0x00, 0x36, 0x36, 0x00, 0x36, 0x16, 0x00, 0x36, 0x36, 0x00, 0x3f, 0x3f, 0x20, 0x20, 0x20, 0x00, 0x20, 0x20, 0x20, 0x00, 0x20, 
  };

}