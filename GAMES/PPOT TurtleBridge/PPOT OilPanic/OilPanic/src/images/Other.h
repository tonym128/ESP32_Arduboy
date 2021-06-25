
#pragma once

namespace Images {

    const uint8_t  ThrowOil_LH[] = {
    4, 8,
    0x0c, 0x03, 0x30, 0x0c, 

    0x1b, 0x00, 0x1b, 0x00, 

    0x06, 0x18, 0x03, 0x0c, 
    };

    const uint8_t  ThrowOil_RH[] = {
    4, 8,
    0x0c, 0x30, 0x03, 0x0c, 

    0x00, 0x1b, 0x00, 0x1b, 

    0x0c, 0x03, 0x18, 0x06, 
    };

    const uint8_t  GameOver[] = {
    60, 14,
    0xff, 0x01, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x0d, 0xf5, 0xb5, 0x2d, 0xfd, 0x0d, 0xb5, 0xb5, 0x0d, 0xfd, 0x05, 0xed, 0xdd, 0xed, 0x05, 0xfd, 0x05, 0xb5, 0xb5, 0xf5, 0xfd, 0xfd, 0xfd, 0x0d, 0xf5, 0xf5, 0x0d, 0xfd, 0x05, 0xfd, 0xfd, 0xfd, 0x05, 0xfd, 0x05, 0xb5, 0xb5, 0xf5, 0xfd, 0x05, 0xb5, 0xb5, 0x4d, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x01, 0x07, 0xfc, 
    0x1f, 0x10, 0x37, 0x27, 0x27, 0x27, 0x27, 0x26, 0x25, 0x25, 0x26, 0x27, 0x24, 0x27, 0x27, 0x24, 0x27, 0x24, 0x27, 0x27, 0x27, 0x24, 0x27, 0x24, 0x25, 0x25, 0x25, 0x27, 0x27, 0x27, 0x26, 0x25, 0x25, 0x26, 0x27, 0x27, 0x26, 0x25, 0x26, 0x27, 0x27, 0x24, 0x25, 0x25, 0x25, 0x27, 0x24, 0x27, 0x27, 0x24, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x27, 0x20, 0x20, 0x3f, 
    };

    const uint8_t  GameOver_Mask[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 
    0x1f, 0x1f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 
    };

    const uint8_t  Font[] = {
    3, 8,
    0x1F, 0x11, 0x1F, // 0  
    0x00, 0x1F, 0x00,
    0x1D, 0x15, 0x17,  
    0x11, 0x15, 0x1F,  
    0x07, 0x04, 0x1F,  
    0x17, 0x15, 0x1D,  
    0x1F, 0x15, 0x1D,  
    0x01, 0x01, 0x1F,  
    0x1F, 0x15, 0x1F,  
    0x17, 0x15, 0x1F, // 9
    };
    

    const uint8_t  Bystanders[] = {
    20, 24,
    0x00, 0x00, 0x10, 0xa8, 0x68, 0x2e, 0x21, 0xaf, 0x21, 0x21, 0xa1, 0x21, 0x2e, 0x68, 0xa8, 0x10, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x80, 0x41, 0x42, 0x34, 0x08, 0x08, 0xf1, 0xf1, 0x08, 0x08, 0x34, 0x42, 0x41, 0x80, 0x00, 0x00, 0x00, 0x00,  
    0x00, 0x03, 0xe4, 0x18, 0x1a, 0x1c, 0x6f, 0x80, 0x70, 0x70, 0x80, 0x6f, 0x1c, 0x1a, 0x18, 0xe4, 0x03, 0x00, 0x00, 0x00, 

    0x00, 0x00, 0x10, 0xa8, 0x68, 0x2e, 0x11, 0x5f, 0x11, 0x11, 0x51, 0x11, 0x2e, 0x68, 0xa8, 0x10, 0x00, 0x00, 0x00, 0x00,  
    0x00, 0x70, 0x88, 0x89, 0x12, 0x34, 0x08, 0x08, 0xf1, 0xf1, 0x08, 0x08, 0x34, 0x12, 0x89, 0x88, 0x70, 0x00, 0x00, 0x00,  
    0x00, 0x00, 0xe0, 0x10, 0x11, 0x1a, 0x6e, 0x80, 0x70, 0x70, 0x80, 0x6c, 0x1a, 0x11, 0x10, 0xe0, 0x00, 0x00, 0x00, 0x00,  

    0x00, 0x00, 0x00, 0x00, 0x8c, 0x72, 0x09, 0x15, 0x49, 0x12, 0x2a, 0x54, 0xa8, 0x10, 0xa0, 0x50, 0xa0, 0x40, 0x00, 0x00, 
    0x20, 0x50, 0x90, 0x10, 0xa0, 0xa1, 0xa2, 0x92, 0x0d, 0x00, 0x00, 0x0d, 0x02, 0x91, 0x22, 0xe5, 0x02, 0x00, 0x00, 0x00,  
    0x00, 0x00, 0x00, 0x01, 0x00, 0xcc, 0x34, 0x33, 0x01, 0xf1, 0x11, 0xa1, 0x42, 0x1f, 0x24, 0xc3, 0x00, 0x00, 0x00, 0x00,  

    0x00, 0x00, 0x00, 0x00, 0x8c, 0x72, 0x09, 0x15, 0x49, 0x12, 0x2a, 0x54, 0xa8, 0x10, 0xa0, 0x50, 0xa0, 0x40, 0x00, 0x00, 
    0x1e, 0x22, 0x44, 0x48, 0x90, 0x91, 0xa2, 0x92, 0x0d, 0x00, 0x00, 0x0d, 0x02, 0x91, 0x22, 0x95, 0x92, 0x48, 0x24, 0x1c, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0x34, 0x33, 0x01, 0xf1, 0x11, 0xa1, 0x42, 0x1f, 0x24, 0xc3, 0x00, 0x00, 0x00, 0x00, 
    };

    const uint8_t  Bystanders_Mask[] = {
    0x00, 0x00, 0x10, 0xb8, 0xf8, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf8, 0xb8, 0x10, 0x00, 0x00, 0x00, 0x00,  
    0x00, 0x00, 0x80, 0xc1, 0xc3, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xc3, 0xc1, 0x80, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x03, 0xe7, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0x03, 0x00, 0x00, 0x00, 

    0x00, 0x00, 0x10, 0xb8, 0xf8, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xf8, 0xb8, 0x10, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x70, 0xf8, 0xf9, 0xf3, 0xf7, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf7, 0xf3, 0xf9, 0xf8, 0x70, 0x00, 0x00, 0x00,  
    0x00, 0x00, 0xe0, 0xf0, 0xf1, 0xfb, 0xff, 0xff, 0x7f, 0x7f, 0xff, 0xff, 0xfb, 0xf1, 0xf0, 0xe0, 0x00, 0x00, 0x00, 0x00, 

    0x00, 0x00, 0x00, 0x00, 0x8c, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xf0, 0xe0, 0x40, 0x00, 0x00, 
    0x20, 0x70, 0xf0, 0xf0, 0xe0, 0xe1, 0xe3, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xf1, 0xe3, 0xe7, 0x02, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x01, 0x00, 0xcc, 0xfc, 0xff, 0xff, 0xff, 0x1f, 0xbf, 0xff, 0xff, 0xe7, 0xc3, 0x00, 0x00, 0x00, 0x00, 

    0x00, 0x00, 0x00, 0x00, 0x8c, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfc, 0xf8, 0xf0, 0xe0, 0xf0, 0xe0, 0x40, 0x00, 0x00,  
    0x1e, 0x3e, 0x7c, 0x78, 0xf0, 0xf1, 0xe3, 0xf3, 0xff, 0xff, 0xff, 0xff, 0xfb, 0xf1, 0xe3, 0xf7, 0xf2, 0x78, 0x3c, 0x1c,  
    0x00, 0x00, 0x00, 0x00, 0x00, 0xcc, 0xfc, 0xff, 0xff, 0xff, 0x1f, 0xbf, 0xff, 0xff, 0xe7, 0xc3, 0x00, 0x00, 0x00, 0x00,  
    };

};