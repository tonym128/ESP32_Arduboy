#pragma once

namespace Images {

    #ifndef DEBUG
    const uint8_t  PPOT[] = {
    65, 31,
    0xfe, 0x01, 0x05, 0xf1, 0xf1, 0xf1, 0x11, 0xd1, 0xb1, 0xd1, 0x11, 0xf1, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x51, 0x00, 0x00, 0xff, 0x00, 0x00, 0x7f, 0x7f, 0x09, 0x09, 0x0f, 0x06, 0x00, 0x7f, 0x7f, 0x19, 0x39, 0x6f, 0x46, 0x00, 0x7f, 0x7f, 0x49, 0x49, 0x41, 0x41, 0x00, 0x26, 0x6f, 0x49, 0x49, 0x7b, 0x32, 0x00, 0x26, 0x6f, 0x49, 0x49, 0x7b, 0x32, 
    0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xfe, 0xff, 0x02, 0x79, 0xcd, 0x95, 0xa5, 0xcd, 0x79, 0x01, 0x11, 0x11, 0x11, 0x01, 0xf9, 0xf9, 0x01, 0x01, 0x00, 0x00, 0xff, 0x00, 0x00, 0x7f, 0x7f, 0x09, 0x09, 0x0f, 0x06, 0x00, 0x7f, 0x7f, 0x40, 0x40, 0x40, 0x40, 0x00, 0x7c, 0x7e, 0x0b, 0x0b, 0x7e, 0x7c, 0x00, 0x07, 0x0f, 0x78, 0x78, 0x0f, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0xff, 0x00, 0x00, 0x1f, 0x3f, 0x3f, 0x33, 0x33, 0x33, 0x33, 0x33, 0x3e, 0x36, 0x36, 0x3e, 0x36, 0x3e, 0x36, 0x3e, 0x36, 0x36, 0x3e, 0x36, 0x36, 0x3e, 0x36, 0x00, 0x00, 0xff, 0x00, 0x00, 0x3e, 0x7f, 0x41, 0x41, 0x7f, 0x3e, 0x00, 0x7f, 0x7f, 0x0e, 0x1c, 0x7f, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x3f, 0x40, 0x50, 0x40, 0x40, 0x40, 0x78, 0x46, 0x41, 0x41, 0x59, 0x59, 0x41, 0x41, 0x41, 0x41, 0x59, 0x59, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x00, 0x00, 0x7f, 0x00, 0x00, 0x01, 0x01, 0x7f, 0x7f, 0x01, 0x01, 0x00, 0x7c, 0x7e, 0x0b, 0x0b, 0x7e, 0x7c, 0x00, 0x7f, 0x7f, 0x09, 0x09, 0x0f, 0x06, 0x00, 0x7f, 0x7f, 0x49, 0x49, 0x41, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    };  
    #else
    const uint8_t  PPOT[] = {
    0x00 ,0x07, 0x3d
    };
    #endif  

    #ifndef DEBUG
    const uint8_t  PPOT_Arrow[] = {
    5, 7,
    0x7f, 0x7f, 0x3e, 0x1c, 0x08, 
    };
    #else
    const uint8_t  PPOT_Arrow[] = {
    0x00 ,0x07, 0x3d
    };
    #endif  

    const uint8_t  PPOT_Spindle[] = {
    2, 8,
    0xd4, 0xd2, 
    };

    const uint8_t  Ppot_Loading[] = {
    33, 8,
    0x0f, 0x0f, 0x08, 0x08, 0x00, 0x06, 0x09, 0x0b, 0x06, 0x00, 0x0e, 0x03, 0x03, 0x0e, 0x00, 0x0f, 0x09, 0x0f, 0x06, 0x00, 0x0f, 0x0f, 0x00, 0x0f, 0x02, 0x03, 0x0f, 0x00, 0x0e, 0x0f, 0x09, 0x0d, 0x00, 
    };

    #ifndef DEBUG
    const uint8_t  PressAandB[] = {
    68, 16,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbc, 0x7e, 0xc7, 0xeb, 0xed, 0xeb, 0xc7, 0x7e, 0xbc, 0x00, 0x00, 0x3c, 0x2c, 0x00, 0x38, 0x14, 0x38, 0x00, 0x08, 0x24, 0x10, 0x00, 0x04, 0x38, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xbc, 0x7e, 0xc1, 0xd5, 0xd5, 0xd5, 0xcb, 0x7e, 0xbc, 0x00, 0x00, 0x3c, 0x10, 0x3c, 0x00, 0x38, 0x14, 0x38, 0x00, 0x3c, 0x14, 0x28, 0x00, 0x3c, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    };
    #else
    const uint8_t  PressAandB[] = {
    0x01 ,0x01, 0x01
    };
    #endif 

    #ifndef DEBUG
    const uint8_t  PressAandB_Mask[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
    };
    #else
    const uint8_t  PressAandB_Mask[] = {
    0x01 ,0x01, 0x01
    };
    #endif 

}