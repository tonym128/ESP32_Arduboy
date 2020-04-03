/*  levels.cpp
 *  
 *      Constants with the levels for game play
 */

/*  Blocks: A simple Arduboy game.
 *
 *  Copyright © 2016 by William Edward Woody
 *
 *  This program is free software: you can redistribute it and/or modify it 
 *  under the terms of the GNU General Public License as published by the 
 *  Free Software Foundation, either version 3 of the License, or (at your 
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but 
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License 
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along 
 *  with this program. If not, see <http://www.gnu.org/licenses/>
 */
 
#include <avr/pgmspace.h>
#include "levels.h"

/****************************************************************************/
/*                                                                          */
/*  Constants                                                               */
/*                                                                          */
/****************************************************************************/

/*  LevelMap
 *
 *      Each level is a 16 byte bitmap with a bit set for each wall item
 */

const static PROGMEM uint16_t LevelMap[] = {
    // Level 1
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,

    // Level 26
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,
    0x0000,

    // Level 2
    0x0000,
    0x0000,
    0x0100,
    0x0100,
    0x0100,
    0x0100,
    0x0000,
    0x0000,

    // Level 20
    0xFFFF,
    0xFFFF,
    0xF9E7,
    0xC003,
    0xF9E7,
    0xFFFF,
    0xFFFF,
    0xFFFF,

    // Level 3
    0xFFFF,
    0x8001,
    0x8001,
    0x8101,
    0x8101,
    0x8001,
    0x8001,
    0xFFFF,

    // Level 17
    0xFFFF,
    0x8201,
    0x8201,
    0x8201,
    0x8341,
    0x8041,
    0x8041,
    0xFFFF,

    // Level 12
    0xFFFF,
    0xFCFF,
    0xFCFF,
    0xE0FF,
    0xFEFF,
    0xFEFF,
    0xFEFF,
    0xFFFF,

    // Level 13
    0xFFFF,
    0xFFFF,
    0xFCFF,
    0xE0FF,
    0xFE7F,
    0xFC01,
    0xFC7F,
    0xFFFF,

    // Level 4
    0xFFFF,
    0xF47F,
    0xF47F,
    0xF641,
    0xF001,
    0xF401,
    0xF601,
    0xFFFF,

    // Level 29
    0xFFFF,
    0xED87,
    0xEDB7,
    0x8001,
    0xEDB7,
    0xEDB7,
    0x8001,
    0xFFFF,

    // Level 6
    0xFFFF,
    0xFFFF,
    0xFF9F,
    0xC01F,
    0xC61F,
    0xFCFF,
    0xFC01,
    0xFFFF,

    // Level 14
    0xFFFF,
    0xF9FF,
    0xF91F,
    0xF01F,
    0xF91F,
    0xF9FF,
    0xF9FF,
    0xFFFF,

    // Level 5
    0xFFFF,
    0x8F09,
    0x8909,
    0x8109,
    0x8909,
    0x8801,
    0x8F01,
    0xFFFF,

    // Level 7
    0xFFFF,
    0xFFFF,
    0xFE3F,
    0xF83F,
    0xF93F,
    0xF9FF,
    0xFFFF,
    0xFFFF,

    // Level 18
    0xFFFF,
    0x9819,
    0x9819,
    0x8181,
    0x8181,
    0x9819,
    0x9819,
    0xFFFF,

    // Level 8
    0xFFFF,
    0xF0FF,
    0xF08F,
    0xF00F,
    0xFFCF,
    0xFFCF,
    0xFFCF,
    0xFFFF,

    // Level 9
    0xFFFF,
    0x83F1,
    0x93F1,
    0x93FB,
    0x8001,
    0x9211,
    0x921F,
    0xFFFF,

    // Level 16
    0xFFFF,
    0x8C63,
    0x8C23,
    0x81A3,
    0xB1B7,
    0xB007,
    0x8667,
    0xFFFF,

    // Level 15
    0xFFFF,
    0xE007,
    0xF557,
    0xE007,
    0xF557,
    0xE007,
    0xE007,
    0xFFFF,

    // Level 10
    0xFFFF,
    0xFFFF,
    0x891F,
    0x801F,
    0xC9E3,
    0xC803,
    0xFFE3,
    0xFFFF,

    // Level 11
    0xFFFF,
    0xFC11,
    0xF851,
    0xF011,
    0xE15B,
    0xC303,
    0xC7F3,
    0xFFFF,

    // Level 19
    0xFFFF,
    0xC001,
    0xE0F1,
    0xB199,
    0x9B0D,
    0x8001,
    0x9B19,
    0xFFFF,

    // Level 21
    0xFFFF,
    0xFF91,
    0xF181,
    0xF011,
    0xF99F,
    0x81FF,
    0x99FF,
    0xFFFF,

    // Level 22
    0x8001,
    0x99C1,
    0x9501,
    0x9589,
    0x9509,
    0x99C9,
    0x8001,
    0x8001,

    // Level 23
    0x0000,
    0x770E,
    0x4448,
    0x664C,
    0x4448,
    0x4748,
    0x0000,
    0x0000,

    // Level 24
    0xFFFF,
    0x8841,
    0x8841,
    0x8841,
    0x8841,
    0x8001,
    0x8001,
    0xFFFF,

    // Level 25
    0xFFFF,
    0xE3F1,
    0xE3F1,
    0x80F1,
    0x80FB,
    0xEC01,
    0xE601,
    0xFFFF,

    // Level 27
    0xFFFF,
    0xF9F1,
    0x8801,
    0x81F9,
    0xF9FF,
    0xF9FF,
    0xFFFF,
    0xFFFF,

    // Level 28
    0xFFFF,
    0x9249,
    0x9249,
    0x9241,
    0x9209,
    0x904D,
    0x8061,
    0xFFFF,

    // Level 30
    0xFFFF,
    0xFC01,
    0xCD85,
    0x8C15,
    0x80D5,
    0x8C1D,
    0xFFFD,
    0xFFFF,

    // Level 31
    0xFFFF,
    0x8101,
    0x8D01,
    0x813B,
    0x89B1,
    0xF831,
    0xFF31,
    0xFFFF,
};

/*  BoxMap
 *
 *      Each level box map is an 8 byte header with the number of boxes,
 *  followed by a byte per box, with the format XXXXYYYY.
 */

const static PROGMEM uint8_t BoxMap[] = {
    // Level 1
    1,
    0x84,
    // Level 26
    2,
    0x65,
    0x64,
    // Level 2
    1,
    0x54,
    // Level 20
    1,
    0x83,
    // Level 3
    2,
    0x54,
    0x53,
    // Level 17
    2,
    0x43,
    0x44,
    // Level 12
    1,
    0x63,
    // Level 13
    1,
    0x73,
    // Level 4
    2,
    0x72,
    0x73,
    // Level 29
    4,
    0x62,
    0x73,
    0x53,
    0x64,
    // Level 6
    2,
    0x74,
    0x53,
    // Level 14
    2,
    0x62,
    0x64,
    // Level 5
    4,
    0x32,
    0x22,
    0x34,
    0x24,
    // Level 7
    2,
    0x83,
    0x64,
    // Level 18
    2,
    0x33,
    0x34,
    // Level 8
    3,
    0x62,
    0x52,
    0x53,
    // Level 9
    2,
    0x54,
    0x53,
    // Level 16
    3,
    0x65,
    0x22,
    0x94,
    // Level 15
    4,
    0x44,
    0x62,
    0x64,
    0x42,
    // Level 10
    3,
    0x34,
    0x23,
    0x73,
    // Level 11
    3,
    0x54,
    0x82,
    0x72,
    // Level 19
    2,
    0x95,
    0x52,
    // Level 21
    2,
    0x25,
    0x53,
    // Level 22
    2,
    0x36,
    0x64,
    // Level 23
    1,
    0x35,
    // Level 24
    5,
    0x82,
    0x62,
    0x73,
    0x84,
    0x64,
    // Level 25
    4,
    0x32,
    0x44,
    0x52,
    0x64,
    // Level 27
    3,
    0x62,
    0x53,
    0x64,
    // Level 28
    4,
    0xB2,
    0xB3,
    0xB4,
    0x53,
    // Level 30
    2,
    0x33,
    0x34,
    // Level 31
    3,
    0x32,
    0x23,
    0x53,
};

/*  EndMap
 *
 *      Same format as BitMap; this gives the position where the 
 *  boxes should go.
 */

const static PROGMEM uint8_t EndMap[] = {
    1,
    0xA4,
    2,
    0xA4,
    0xA5,
    1,
    0xA4,
    1,
    0xB3,
    2,
    0xA3,
    0xA4,
    2,
    0xB3,
    0xB4,
    1,
    0x76,
    1,
    0xD5,
    2,
    0xD4,
    0xB4,
    4,
    0x43,
    0x65,
    0xE6,
    0x61,
    2,
    0xE6,
    0xD6,
    2,
    0xA2,
    0xA4,
    4,
    0xE1,
    0xD1,
    0xE2,
    0xD2,
    2,
    0x82,
    0x93,
    2,
    0xE1,
    0xD1,
    3,
    0xB2,
    0xA2,
    0xB3,
    2,
    0xE1,
    0xD3,
    3,
    0xD1,
    0xD2,
    0xD3,
    4,
    0xC2,
    0xC4,
    0xA2,
    0xA4,
    3,
    0xD4,
    0xC4,
    0xB4,
    3,
    0xE1,
    0xE2,
    0xE3,
    2,
    0xE1,
    0xE2,
    2,
    0xD2,
    0xC3,
    2,
    0xC1,
    0xC2,
    1,
    0x91,
    5,
    0xC2,
    0xA2,
    0xB3,
    0xC4,
    0xA4,
    4,
    0xE1,
    0xE2,
    0xD1,
    0xD2,
    3,
    0xD1,
    0xE2,
    0xC2,
    4,
    0xE1,
    0xE2,
    0xD1,
    0xD2,
    2,
    0xC3,
    0xC4,
    3,
    0xE6,
    0xD6,
    0xC6,
};

/*  StartPos
 *
 *      One byte per level in the same XXXXYYYY format as used with boxes.
 *  this gives the initial position of the player
 */

const static PROGMEM uint8_t StartPos[] = {
    0x44,
    0x43,
    0x44,
    0x53,
    0x44,
    0x33,
    0x33,
    0x33,
    0x43,
    0x63,
    0x73,
    0x43,
    0x23,
    0x53,
    0x13,
    0xA6,
    0xC1,
    0x61,
    0x33,
    0x53,
    0x52,
    0x25,
    0x26,
    0x51,
    0x25,
    0x33,
    0xD4,
    0x63,
    0x83,
    0xE6,
    0x43,
};

/****************************************************************************/
/*                                                                          */
/*  Methods                                                                 */
/*                                                                          */
/****************************************************************************/

/*  GetBoundaries
 *
 *      Get the boundaries for each level
 */
 
void GetBoundaries(uint8_t levelIndex, uint16_t *mem)
{
    memcpy_P(mem,LevelMap + levelIndex * 8,16);
}

/*  ParseMap
 *
 *      Runs the common code for getting the boxes or end map locations.
 */

static uint8_t ParseMap(uint8_t levelIndex, const uint8_t *src, uint8_t *dest)
{
    uint8_t len;
    while (levelIndex > 0) {
        len = pgm_read_byte(src);
        src += len+1;
        --levelIndex;
    }
    len = pgm_read_byte(src);
    ++src;
    memcpy_P(dest,src,len);
    return len;
}

/*  GetBoxes
 * 
 *      Copy an array of box positions within our level. These are the initial
 *  box positions. This assumes the boxpos array is at least MAXBOXES in size
 */
uint8_t GetBoxes(uint8_t levelIndex, uint8_t *boxpos)
{
    return ParseMap(levelIndex,BoxMap,boxpos);   
}

/*  GetEnds
 * 
 *      Get the location where the boxes are to be moved.
 */
uint8_t GetEnds(uint8_t levelIndex, uint8_t *endpos)
{
    return ParseMap(levelIndex,EndMap,endpos);
}

/*  GetStartPos
 *   
 *      Get the location of the player at the start of the level.
 */
uint8_t GetStartPos(uint8_t levelIndex)
{
    return pgm_read_byte(StartPos + levelIndex);
}
