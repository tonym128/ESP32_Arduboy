/*  levels.h
 *   
 *      Constants which provide the levels
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

#ifndef __LEVELS_H
#define __LEVELS_H

#include <stdint.h>

/****************************************************************************/
/*                                                                          */
/*  Constants                                                               */
/*                                                                          */
/****************************************************************************/

#define MAXLEVELS           31

#define MAXBOXES            16      // Max boxes possible

/****************************************************************************/
/*                                                                          */
/*  Methods                                                                 */
/*                                                                          */
/****************************************************************************/

extern void GetBoundaries(uint8_t levelIndex, uint16_t *mem);
extern uint8_t GetBoxes(uint8_t levelIndex, uint8_t *boxpos);
extern uint8_t GetEnds(uint8_t levelIndex, uint8_t *endpos);
extern uint8_t GetStartPos(uint8_t levelIndex);

#endif // __LEVELS_H
