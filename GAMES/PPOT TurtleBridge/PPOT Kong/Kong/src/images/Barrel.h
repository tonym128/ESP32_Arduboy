#pragma once

namespace Images {

  const uint8_t  BarrelImg[] = {
  9, 9,
  0x00, 0x38, 0x54, 0x82, 0xd6, 0x82, 0x54, 0x38, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  0x00, 0x38, 0x4c, 0xc2, 0x92, 0x86, 0x64, 0x38, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 

  0x00, 0x38, 0x64, 0x86, 0x92, 0xc2, 0x4c, 0x38, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  };

  const uint8_t  Barrel_Mask[] = {
  0x38, 0x7c, 0xfe, 0xff, 0xff, 0xff, 0xfe, 0x7c, 0x38, 
  0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 
  };

}