#pragma once

namespace Images {

    #ifndef DEBUG
      const uint8_t  hourglass[] = {
      26, 32,

      0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF9, 0x09, 0x49, 0x89, 0x49, 0x89, 0x49, 0x89, 0x49, 0x89, 0x49, 0x09, 0xF9, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x19, 0x32, 0xE5, 0xCA, 0x15, 0xCA, 0xE5, 0x32, 0x19, 0x0C, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0C, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x06, 0x0C, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x27, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x27, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF9, 0x09, 0x09, 0x89, 0x09, 0x89, 0x09, 0x89, 0x09, 0x89, 0x09, 0x09, 0xF9, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x19, 0x32, 0xE5, 0xCA, 0x55, 0xCA, 0xE5, 0x32, 0x19, 0x0C, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0C, 0x06, 0x03, 0x01, 0x00, 0x55, 0x00, 0x01, 0x03, 0x06, 0x0C, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x27, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x24, 0x27, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF9, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0xF9, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x19, 0x32, 0xE5, 0xCA, 0x95, 0xCA, 0xE5, 0x32, 0x19, 0x0C, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0C, 0x06, 0x03, 0x01, 0x00, 0xAA, 0x00, 0x01, 0x03, 0x06, 0x0C, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x27, 0x24, 0x24, 0x24, 0x26, 0x24, 0x26, 0x24, 0x26, 0x24, 0x24, 0x24, 0x27, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF9, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0xF9, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x18, 0x32, 0xE4, 0xCA, 0x54, 0xCA, 0xE4, 0x32, 0x18, 0x0C, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0C, 0x06, 0x03, 0x81, 0x00, 0x95, 0x00, 0x81, 0x03, 0x06, 0x0C, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x27, 0x24, 0x26, 0x25, 0x26, 0x25, 0x26, 0x25, 0x26, 0x25, 0x26, 0x24, 0x27, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF9, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0xF9, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x18, 0x30, 0xE4, 0xC8, 0x94, 0xC8, 0xE4, 0x30, 0x18, 0x0C, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0C, 0x06, 0x03, 0x81, 0x40, 0x8A, 0x40, 0x81, 0x03, 0x06, 0x0C, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x27, 0x24, 0x26, 0x25, 0x26, 0x25, 0x26, 0x25, 0x26, 0x25, 0x26, 0x24, 0x27, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF9, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0xF9, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x18, 0x30, 0xE0, 0xC8, 0x50, 0xC8, 0xE0, 0x30, 0x18, 0x0C, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0C, 0x86, 0x43, 0x81, 0x40, 0x85, 0x40, 0x81, 0x43, 0x86, 0x0C, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x27, 0x24, 0x26, 0x25, 0x26, 0x25, 0x26, 0x25, 0x26, 0x25, 0x26, 0x24, 0x27, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF9, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0xF9, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x18, 0x30, 0xE0, 0xC0, 0x90, 0xC0, 0xE0, 0x30, 0x18, 0x0C, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0C, 0x86, 0x43, 0xA1, 0x40, 0xAA, 0x40, 0xA1, 0x43, 0x86, 0x0C, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x27, 0x24, 0x26, 0x25, 0x26, 0x25, 0x26, 0x25, 0x26, 0x25, 0x26, 0x24, 0x27, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF9, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0xF9, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x18, 0x30, 0xE0, 0xC0, 0x40, 0xC0, 0xE0, 0x30, 0x18, 0x0C, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0C, 0x86, 0x43, 0xA1, 0x50, 0xA1, 0x50, 0xA1, 0x43, 0x86, 0x0C, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x27, 0x24, 0x26, 0x25, 0x26, 0x25, 0x26, 0x25, 0x26, 0x25, 0x26, 0x24, 0x27, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xF9, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0x09, 0xF9, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x0C, 0x18, 0x30, 0xE0, 0xC0, 0x00, 0xC0, 0xE0, 0x30, 0x18, 0x0C, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x0C, 0x86, 0x43, 0xA1, 0x50, 0xA8, 0x50, 0xA1, 0x43, 0x86, 0x0C, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x27, 0x24, 0x26, 0x25, 0x26, 0x25, 0x26, 0x25, 0x26, 0x25, 0x26, 0x24, 0x27, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0xA0, 0x50, 0x28, 0x1C, 0x28, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x08, 0x1C, 0x0A, 0x15, 0x22, 0x41, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x3F, 0x60, 0x40, 0x40, 0x40, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x3F, 0x40, 0x94, 0x28, 0x54, 0x28, 0x94, 0x48, 0xA4, 0x51, 0x2A, 0x14, 0x0E, 0x04, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x0E, 0x05, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80,
      0xFF, 0x00, 0x00, 0xFF, 0x01, 0x01, 0x01, 0x01, 0x03, 0x06, 0x0C, 0x18, 0xB0, 0xB0, 0x18, 0x0C, 0x86, 0x43, 0xA1, 0x51, 0xA1, 0x51, 0xFF, 0x00, 0x00, 0xFF,
      0x3F, 0x20, 0x20, 0x3F, 0x10, 0x10, 0x10, 0x10, 0x18, 0x0C, 0x06, 0x03, 0x01, 0x01, 0x03, 0x06, 0x0C, 0x19, 0x12, 0x15, 0x12, 0x15, 0x3F, 0x20, 0x20, 0x3F,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x28, 0x1C, 0x28, 0x50, 0xA0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x40, 0x40, 0x40, 0x40, 0x60, 0x3F, 0x00, 0x28, 0x94, 0xA8, 0x94, 0xA8, 0x94, 0x49, 0x22, 0x15, 0x0A, 0x1C, 0x08, 0x00,
      0x04, 0x0E, 0x14, 0x2A, 0x51, 0xA0, 0x40, 0x80, 0x00, 0x00, 0x00, 0x80, 0x40, 0x3F, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x05, 0x0E, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

      };
    #else
      const uint8_t  hourglass[] = {
      1,1,
      1,1,1, 1,1,1, 1,1,1, 1,1,1
      };
    #endif

}