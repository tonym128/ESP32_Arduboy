#pragma once

#include "../utils/Enums.h"

#define BARREL_POSITION_3_START       0
#define BARREL_POSITION_3_COUNT       30
#define BARREL_POSITION_3_END         BARREL_POSITION_3_START + BARREL_POSITION_3_COUNT - 1 /* 29 */
#define BARREL_POSITION_2_START       BARREL_POSITION_3_START + BARREL_POSITION_3_COUNT     /* 30 */
#define BARREL_POSITION_2_COUNT       29
#define BARREL_POSITION_2_END         BARREL_POSITION_2_START + BARREL_POSITION_2_COUNT - 1 /* 58 */
#define BARREL_POSITION_1_START       BARREL_POSITION_2_START + BARREL_POSITION_2_COUNT     /* 59 */
#define BARREL_POSITION_1_COUNT       19
#define BARREL_POSITION_1_END         BARREL_POSITION_1_START + BARREL_POSITION_1_COUNT - 1 /* 78 */
#define BARREL_NUMBER_OF_ELEMENTS     3


namespace Coordinates {

  struct BarrelData {

    int8_t x;
    int8_t y;
    uint8_t skip;
    Rotation rotation;

  };

  const uint8_t PROGMEM Barrel[660] = {

    //From Position 3 ..

    69, 18, static_cast<uint8_t>(Rotation::None),	
    69, 20, static_cast<uint8_t>(Rotation::None),	
    69, 22, static_cast<uint8_t>(Rotation::None),	

    69, 24, static_cast<uint8_t>(Rotation::None),	
    69, 26, static_cast<uint8_t>(Rotation::None),	
    69, 28, static_cast<uint8_t>(Rotation::None),	
    69, 30, static_cast<uint8_t>(Rotation::None),	

    69, 32, static_cast<uint8_t>(Rotation::None),	
    69, 34, static_cast<uint8_t>(Rotation::None),	
    69, 36, static_cast<uint8_t>(Rotation::None),	
    69, 38, static_cast<uint8_t>(Rotation::None),	

    69, 40, static_cast<uint8_t>(Rotation::None),	
    69, 42, static_cast<uint8_t>(Rotation::None),	
    69, 44, static_cast<uint8_t>(Rotation::None),	
    69, 46, static_cast<uint8_t>(Rotation::None),	

    69, 48, static_cast<uint8_t>(Rotation::None),	
    69, 50, static_cast<uint8_t>(Rotation::None),	
    69, 52, static_cast<uint8_t>(Rotation::None),	
    67, 52, static_cast<uint8_t>(Rotation::Left),	

    65, 52, static_cast<uint8_t>(Rotation::Left),	
    63, 52, static_cast<uint8_t>(Rotation::Left),	
    61, 53, static_cast<uint8_t>(Rotation::Left),	
    57, 53, static_cast<uint8_t>(Rotation::Left),	

    55, 53, static_cast<uint8_t>(Rotation::Left),	
    53, 53, static_cast<uint8_t>(Rotation::Left),	
    51, 54, static_cast<uint8_t>(Rotation::Left),	
    49, 54, static_cast<uint8_t>(Rotation::Left),	

    47, 54, static_cast<uint8_t>(Rotation::Left),	
    45, 54, static_cast<uint8_t>(Rotation::Left),	
    43, 54, static_cast<uint8_t>(Rotation::Left) | (19 << 3),	


    // From Position 2 ..

    44, 19, static_cast<uint8_t>(Rotation::None),	
    44, 21, static_cast<uint8_t>(Rotation::None),	

    44, 23, static_cast<uint8_t>(Rotation::None),	
    44, 25, static_cast<uint8_t>(Rotation::None),	
    44, 27, static_cast<uint8_t>(Rotation::None),	
    44, 29, static_cast<uint8_t>(Rotation::None),	

    44, 31, static_cast<uint8_t>(Rotation::None),	
    44, 33, static_cast<uint8_t>(Rotation::None),	
    44, 35, static_cast<uint8_t>(Rotation::None),	
    44, 37, static_cast<uint8_t>(Rotation::None),	

    44, 39, static_cast<uint8_t>(Rotation::None),	
    44, 41, static_cast<uint8_t>(Rotation::None),	
    44, 43, static_cast<uint8_t>(Rotation::None),	
    44, 45, static_cast<uint8_t>(Rotation::None),	

    44, 47, static_cast<uint8_t>(Rotation::None),	
    44, 49, static_cast<uint8_t>(Rotation::None),	
    44, 51, static_cast<uint8_t>(Rotation::None),	
    44, 53, static_cast<uint8_t>(Rotation::Left),	

    44, 55, static_cast<uint8_t>(Rotation::Left),	
    41, 55, static_cast<uint8_t>(Rotation::Left),	// From Position 3
    39, 55, static_cast<uint8_t>(Rotation::Left),	
    37, 55, static_cast<uint8_t>(Rotation::Left),	

    35, 55, static_cast<uint8_t>(Rotation::Left),	
    31, 56, static_cast<uint8_t>(Rotation::Left),	
    29, 56, static_cast<uint8_t>(Rotation::Left),	
    27, 56, static_cast<uint8_t>(Rotation::Left),	

    25, 56, static_cast<uint8_t>(Rotation::Left),	
    23, 56, static_cast<uint8_t>(Rotation::Left),	
    21, 57, static_cast<uint8_t>(Rotation::Left) | (19 << 3),	
//    19, 57, static_cast<uint8_t>(Rotation::Left),	 //in position 1


    // From Position 1 ..

    18, 19, static_cast<uint8_t>(Rotation::None),	
    18, 21, static_cast<uint8_t>(Rotation::None),
        	
    18, 23, static_cast<uint8_t>(Rotation::None),	
    18, 25, static_cast<uint8_t>(Rotation::None),	
    18, 27, static_cast<uint8_t>(Rotation::None),	
    18, 29, static_cast<uint8_t>(Rotation::None),

    18, 31, static_cast<uint8_t>(Rotation::None),	
    18, 33, static_cast<uint8_t>(Rotation::None),	
    18, 35, static_cast<uint8_t>(Rotation::None),	
    18, 37, static_cast<uint8_t>(Rotation::None),

    18, 39, static_cast<uint8_t>(Rotation::None),	
    18, 41, static_cast<uint8_t>(Rotation::None),	
    18, 43, static_cast<uint8_t>(Rotation::None),	
    18, 45, static_cast<uint8_t>(Rotation::None),	

    18, 47, static_cast<uint8_t>(Rotation::None),	
    18, 49, static_cast<uint8_t>(Rotation::None),	
    18, 51, static_cast<uint8_t>(Rotation::None),	
    18, 53, static_cast<uint8_t>(Rotation::None),

    18, 55, static_cast<uint8_t>(Rotation::None),	

    // Common to all ..

    18, 57, static_cast<uint8_t>(Rotation::Left),	// From Position 2
    17, 57, static_cast<uint8_t>(Rotation::Left),	
    15, 57, static_cast<uint8_t>(Rotation::Left),	
    13, 57, static_cast<uint8_t>(Rotation::Left),	
    11, 58, static_cast<uint8_t>(Rotation::Left),	
    9, 58, static_cast<uint8_t>(Rotation::Left),	
    7, 58, static_cast<uint8_t>(Rotation::Left),	
    5, 58, static_cast<uint8_t>(Rotation::Left),	
    3, 58, static_cast<uint8_t>(Rotation::Left),	
    1, 59, static_cast<uint8_t>(Rotation::Left),	
    static_cast<uint8_t>(-1), 61, static_cast<uint8_t>(Rotation::Left),	
    static_cast<uint8_t>(-1), 63, static_cast<uint8_t>(Rotation::Left),	
    static_cast<uint8_t>(-1), 65, static_cast<uint8_t>(Rotation::Left),	

    static_cast<uint8_t>(-1), 67, static_cast<uint8_t>(Rotation::Left),	
    static_cast<uint8_t>(-1), 69, static_cast<uint8_t>(Rotation::Left),	
    static_cast<uint8_t>(-1), 71, static_cast<uint8_t>(Rotation::Left),	
      
    static_cast<uint8_t>(-1), 73, static_cast<uint8_t>(Rotation::Left),	
    static_cast<uint8_t>(-1), 75, static_cast<uint8_t>(Rotation::Left),	
    static_cast<uint8_t>(-1), 77, static_cast<uint8_t>(Rotation::Left),	
    static_cast<uint8_t>(-1), 79, static_cast<uint8_t>(Rotation::Left),	
      
    static_cast<uint8_t>(-1), 81, static_cast<uint8_t>(Rotation::Left),	
    static_cast<uint8_t>(-1), 83, static_cast<uint8_t>(Rotation::Left),	
    1, 83, static_cast<uint8_t>(Rotation::Right),	
    3, 83, static_cast<uint8_t>(Rotation::Right),	
    5, 83, static_cast<uint8_t>(Rotation::Right),	

    7, 83, static_cast<uint8_t>(Rotation::Right),	
    9, 84, static_cast<uint8_t>(Rotation::Right),	
    11, 84, static_cast<uint8_t>(Rotation::Right),	
    13, 84, static_cast<uint8_t>(Rotation::Right),	//16
      
    15, 84, static_cast<uint8_t>(Rotation::Right),	
    17, 84, static_cast<uint8_t>(Rotation::Right),	
    19, 85, static_cast<uint8_t>(Rotation::Right),	
    21, 85, static_cast<uint8_t>(Rotation::Right),	
      
    23, 85, static_cast<uint8_t>(Rotation::Right),	
    25, 85, static_cast<uint8_t>(Rotation::Right),	
    27, 85, static_cast<uint8_t>(Rotation::Right),	
    29, 86, static_cast<uint8_t>(Rotation::Right),	//24
      
    31, 86, static_cast<uint8_t>(Rotation::Right),	
    33, 86, static_cast<uint8_t>(Rotation::Right),	
    35, 86, static_cast<uint8_t>(Rotation::Right),	
    37, 86, static_cast<uint8_t>(Rotation::Right),	
      
    39, 87, static_cast<uint8_t>(Rotation::Right),	
    41, 87, static_cast<uint8_t>(Rotation::Right),	
    43, 87, static_cast<uint8_t>(Rotation::Right),	
    45, 87, static_cast<uint8_t>(Rotation::Right),	//32
      
    47, 87, static_cast<uint8_t>(Rotation::Right),	
    49, 88, static_cast<uint8_t>(Rotation::Right),	
    51, 88, static_cast<uint8_t>(Rotation::Right),	
    53, 88, static_cast<uint8_t>(Rotation::Right),	
      
    55, 88, static_cast<uint8_t>(Rotation::Right),	
    57, 88, static_cast<uint8_t>(Rotation::Right),	
    59, 89, static_cast<uint8_t>(Rotation::Right),	
    61, 89, static_cast<uint8_t>(Rotation::Right),	//40
      
    63, 89, static_cast<uint8_t>(Rotation::Right),	
    65, 89, static_cast<uint8_t>(Rotation::Right),	
    67, 89, static_cast<uint8_t>(Rotation::Right),	
    69, 90, static_cast<uint8_t>(Rotation::Right),	
      
    71, 90, static_cast<uint8_t>(Rotation::Right),	
    73, 90, static_cast<uint8_t>(Rotation::Right),	
    75, 90, static_cast<uint8_t>(Rotation::Right),	
    77, 90, static_cast<uint8_t>(Rotation::Right),	//48
      
    79, 91, static_cast<uint8_t>(Rotation::Right),	
    81, 91, static_cast<uint8_t>(Rotation::Right),	
    83, 91, static_cast<uint8_t>(Rotation::Right),	
    85, 91, static_cast<uint8_t>(Rotation::Right),	
      
    87, 91, static_cast<uint8_t>(Rotation::Right),	
    89, 92, static_cast<uint8_t>(Rotation::Right),	
    91, 92, static_cast<uint8_t>(Rotation::Right),	
    93, 92, static_cast<uint8_t>(Rotation::Right),	//56
      
    95, 92, static_cast<uint8_t>(Rotation::Right),	
    97, 92, static_cast<uint8_t>(Rotation::Right),	
    99, 93, static_cast<uint8_t>(Rotation::Right),	
    100, 94, static_cast<uint8_t>(Rotation::Right),	
      
    101, 95, static_cast<uint8_t>(Rotation::Right),	
    101, 97, static_cast<uint8_t>(Rotation::Right),	
    101, 99, static_cast<uint8_t>(Rotation::Right),	
    101, 101, static_cast<uint8_t>(Rotation::Right),	//64
      
    101, 103, static_cast<uint8_t>(Rotation::Right),	
    101, 105, static_cast<uint8_t>(Rotation::Right),	
    101, 107, static_cast<uint8_t>(Rotation::Right),	
    101, 109, static_cast<uint8_t>(Rotation::Right),	
      
    101, 111, static_cast<uint8_t>(Rotation::Right),	
    101, 113, static_cast<uint8_t>(Rotation::Right),	
    101, 115, static_cast<uint8_t>(Rotation::Right),	
    99, 115, static_cast<uint8_t>(Rotation::Left),	//72
      
    97, 115, static_cast<uint8_t>(Rotation::Left),	
    95, 115, static_cast<uint8_t>(Rotation::Left),	
    93, 116, static_cast<uint8_t>(Rotation::Left),	
    91, 116, static_cast<uint8_t>(Rotation::Left),	
      
    89, 116, static_cast<uint8_t>(Rotation::Left),	
    87, 116, static_cast<uint8_t>(Rotation::Left),	
    85, 116, static_cast<uint8_t>(Rotation::Left),	
    83, 117, static_cast<uint8_t>(Rotation::Left),	//80
      
    81, 117, static_cast<uint8_t>(Rotation::Left),	
    79, 117, static_cast<uint8_t>(Rotation::Left),	
    77, 117, static_cast<uint8_t>(Rotation::Left),	
    75, 117, static_cast<uint8_t>(Rotation::Left),	
      
    73, 118, static_cast<uint8_t>(Rotation::Left),	
    71, 118, static_cast<uint8_t>(Rotation::Left),	
    69, 118, static_cast<uint8_t>(Rotation::Left),	
    67, 118, static_cast<uint8_t>(Rotation::Left),	//88
      
    65, 118, static_cast<uint8_t>(Rotation::Left),	
    63, 119, static_cast<uint8_t>(Rotation::Left),	
    61, 119, static_cast<uint8_t>(Rotation::Left),	
    59, 119, static_cast<uint8_t>(Rotation::Left),	
      
    57, 119, static_cast<uint8_t>(Rotation::Left),	
    55, 119, static_cast<uint8_t>(Rotation::Left),	
    53, 120, static_cast<uint8_t>(Rotation::Left),	
    51, 120, static_cast<uint8_t>(Rotation::Left),	//96
      
    49, 120, static_cast<uint8_t>(Rotation::Left),	
    47, 120, static_cast<uint8_t>(Rotation::Left),	
    45, 120, static_cast<uint8_t>(Rotation::Left),	
    43, 121, static_cast<uint8_t>(Rotation::Left),	
      
    41, 121, static_cast<uint8_t>(Rotation::Left),	
    39, 121, static_cast<uint8_t>(Rotation::Left),	
    37, 121, static_cast<uint8_t>(Rotation::Left),	
    35, 121, static_cast<uint8_t>(Rotation::Left),	//104
      
    33, 122, static_cast<uint8_t>(Rotation::Left),	
    31, 122, static_cast<uint8_t>(Rotation::Left),	
    29, 122, static_cast<uint8_t>(Rotation::Left),	
    27, 122, static_cast<uint8_t>(Rotation::Left),	
      
    25, 122, static_cast<uint8_t>(Rotation::Left),	
    23, 123, static_cast<uint8_t>(Rotation::Left),	
    21, 123, static_cast<uint8_t>(Rotation::Left),	
    19, 123, static_cast<uint8_t>(Rotation::Left),	//112
      
    17, 123, static_cast<uint8_t>(Rotation::Left),	
    15, 123, static_cast<uint8_t>(Rotation::Left),	
    13, 124, static_cast<uint8_t>(Rotation::Left),	
    11, 124, static_cast<uint8_t>(Rotation::Left),	
      
    9, 124, static_cast<uint8_t>(Rotation::Left),	
    7, 124, static_cast<uint8_t>(Rotation::Left),	
    5, 124, static_cast<uint8_t>(Rotation::Left),	
    3, 125, static_cast<uint8_t>(Rotation::Left),	//120
      
    1, 126, static_cast<uint8_t>(Rotation::Left),	//last corner
    1, 128, static_cast<uint8_t>(Rotation::Left),	
    1, 130, static_cast<uint8_t>(Rotation::Left),	
    1, 132, static_cast<uint8_t>(Rotation::Left),	
      
    1, 134, static_cast<uint8_t>(Rotation::Left),	
    1, 136, static_cast<uint8_t>(Rotation::Left),	
    1, 138, static_cast<uint8_t>(Rotation::Left),	
    1, 140, static_cast<uint8_t>(Rotation::Left),	//128

    0, 0, static_cast<uint8_t>(Rotation::None),	//End

  };

  const uint8_t PROGMEM Barrel_Splash[] = {


    //From Position 3 ..

    3, static_cast<uint8_t>(-9), static_cast<uint8_t>(Rotation::None),	
    3, static_cast<uint8_t>(-8), static_cast<uint8_t>(Rotation::None),	
    3, static_cast<uint8_t>(-7), static_cast<uint8_t>(Rotation::None),	
    3, static_cast<uint8_t>(-6), static_cast<uint8_t>(Rotation::None),	

    3, static_cast<uint8_t>(-5), static_cast<uint8_t>(Rotation::None),	
    3, static_cast<uint8_t>(-4), static_cast<uint8_t>(Rotation::None),	
    3, static_cast<uint8_t>(-3), static_cast<uint8_t>(Rotation::None),	
    3, static_cast<uint8_t>(-2), static_cast<uint8_t>(Rotation::None),	

    3, static_cast<uint8_t>(-1), static_cast<uint8_t>(Rotation::None),	
    3, 0, static_cast<uint8_t>(Rotation::None),	
    3, 1, static_cast<uint8_t>(Rotation::Right),	
    5, 1, static_cast<uint8_t>(Rotation::Right),	

    7, 1, static_cast<uint8_t>(Rotation::Right),	
    9, 1, static_cast<uint8_t>(Rotation::Right),	
    11, 1, static_cast<uint8_t>(Rotation::Right),	
    13, 2, static_cast<uint8_t>(Rotation::Right),	

    15, 2, static_cast<uint8_t>(Rotation::Right),	
    17, 2, static_cast<uint8_t>(Rotation::Right),	
    19, 2, static_cast<uint8_t>(Rotation::Right),	
    21, 2, static_cast<uint8_t>(Rotation::Right),	//20

    23, 3, static_cast<uint8_t>(Rotation::Right),	
    25, 3, static_cast<uint8_t>(Rotation::Right),	
    27, 3, static_cast<uint8_t>(Rotation::Right),	
    29, 3, static_cast<uint8_t>(Rotation::Right),	

    31, 3, static_cast<uint8_t>(Rotation::Right),	
    33, 4, static_cast<uint8_t>(Rotation::Right),	
    35, 4, static_cast<uint8_t>(Rotation::Right),	
    37, 4, static_cast<uint8_t>(Rotation::Right),	

    39, 4, static_cast<uint8_t>(Rotation::Right),	
    41, 4, static_cast<uint8_t>(Rotation::Right),	
    43, 5, static_cast<uint8_t>(Rotation::Right),	
    45, 5, static_cast<uint8_t>(Rotation::Right),	

    47, 5, static_cast<uint8_t>(Rotation::Right),	
    49, 5, static_cast<uint8_t>(Rotation::Right),	
    51, 5, static_cast<uint8_t>(Rotation::Right),	
    53, 6, static_cast<uint8_t>(Rotation::Right),	

    55, 6, static_cast<uint8_t>(Rotation::Right),	
    57, 6, static_cast<uint8_t>(Rotation::Right),	
    59, 6, static_cast<uint8_t>(Rotation::Right),	
    61, 6, static_cast<uint8_t>(Rotation::Right),	//40

    63, 7, static_cast<uint8_t>(Rotation::Right),	
    65, 7, static_cast<uint8_t>(Rotation::Right),	
    67, 7, static_cast<uint8_t>(Rotation::Right),	
    69, 7, static_cast<uint8_t>(Rotation::Right),	

    71, 7, static_cast<uint8_t>(Rotation::Right),	
    73, 8, static_cast<uint8_t>(Rotation::Right),	
    75, 8, static_cast<uint8_t>(Rotation::Right),	
    77, 8, static_cast<uint8_t>(Rotation::Right),	

    79, 8, static_cast<uint8_t>(Rotation::Right),	
    81, 8, static_cast<uint8_t>(Rotation::Right),	
    83, 9, static_cast<uint8_t>(Rotation::Right),	
    85, 9, static_cast<uint8_t>(Rotation::Right),	

    87, 9, static_cast<uint8_t>(Rotation::Right),	
    89, 9, static_cast<uint8_t>(Rotation::Right),	
    91, 9, static_cast<uint8_t>(Rotation::Right),	
    93, 10, static_cast<uint8_t>(Rotation::Right),	

    95, 10, static_cast<uint8_t>(Rotation::Right),	
    97, 10, static_cast<uint8_t>(Rotation::Right),	
    99, 10, static_cast<uint8_t>(Rotation::Right),	
    101, 10, static_cast<uint8_t>(Rotation::Right),	//60

    103, 11, static_cast<uint8_t>(Rotation::Right),	
    105, 11, static_cast<uint8_t>(Rotation::Right),	
    107, 11, static_cast<uint8_t>(Rotation::Right),	
    109, 11, static_cast<uint8_t>(Rotation::Right),	

    111, 11, static_cast<uint8_t>(Rotation::Right),	
    113, 12, static_cast<uint8_t>(Rotation::Right),	
    115, 13, static_cast<uint8_t>(Rotation::None),	
    115, 15, static_cast<uint8_t>(Rotation::None),	

    115, 17, static_cast<uint8_t>(Rotation::None),	
    115, 19, static_cast<uint8_t>(Rotation::None),	
    115, 21, static_cast<uint8_t>(Rotation::None),	
    115, 23, static_cast<uint8_t>(Rotation::None),	

    115, 25, static_cast<uint8_t>(Rotation::None),	
    115, 27, static_cast<uint8_t>(Rotation::None),	
    115, 29, static_cast<uint8_t>(Rotation::None),	
    115, 31, static_cast<uint8_t>(Rotation::None),	

    115, 33, static_cast<uint8_t>(Rotation::None),	
    115, 35, static_cast<uint8_t>(Rotation::None),	
    113, 35, static_cast<uint8_t>(Rotation::Left),	
    111, 35, static_cast<uint8_t>(Rotation::Left),	//80	

    109, 35, static_cast<uint8_t>(Rotation::Left),	
    107, 36, static_cast<uint8_t>(Rotation::Left),	
    105, 36, static_cast<uint8_t>(Rotation::Left),	
    103, 36, static_cast<uint8_t>(Rotation::Left),	

    101, 36, static_cast<uint8_t>(Rotation::Left),	
    99, 36, static_cast<uint8_t>(Rotation::Left),	
    97, 37, static_cast<uint8_t>(Rotation::Left),	
    95, 37, static_cast<uint8_t>(Rotation::Left),	

    93, 37, static_cast<uint8_t>(Rotation::Left),	
    91, 37, static_cast<uint8_t>(Rotation::Left),	
    89, 37, static_cast<uint8_t>(Rotation::Left),	
    87, 38, static_cast<uint8_t>(Rotation::Left),	

    85, 38, static_cast<uint8_t>(Rotation::Left),	
    83, 38, static_cast<uint8_t>(Rotation::Left),	
    81, 38, static_cast<uint8_t>(Rotation::Left),	
    79, 38, static_cast<uint8_t>(Rotation::Left),	

    77, 39, static_cast<uint8_t>(Rotation::Left),	
    75, 39, static_cast<uint8_t>(Rotation::Left),	
    73, 39, static_cast<uint8_t>(Rotation::Left),	
    71, 39, static_cast<uint8_t>(Rotation::Left),	//100

    69, 39, static_cast<uint8_t>(Rotation::Left),	
    67, 40, static_cast<uint8_t>(Rotation::Left),	
    65, 40, static_cast<uint8_t>(Rotation::Left),	
    63, 40, static_cast<uint8_t>(Rotation::Left),	

    61, 40, static_cast<uint8_t>(Rotation::Left),	
    59, 40, static_cast<uint8_t>(Rotation::Left),	
    57, 41, static_cast<uint8_t>(Rotation::Left),	
    55, 41, static_cast<uint8_t>(Rotation::Left),	

    53, 41, static_cast<uint8_t>(Rotation::Left),	
    51, 41, static_cast<uint8_t>(Rotation::Left),	
    49, 41, static_cast<uint8_t>(Rotation::Left),	
    47, 42, static_cast<uint8_t>(Rotation::Left),	

    45, 42, static_cast<uint8_t>(Rotation::Left),	
    43, 42, static_cast<uint8_t>(Rotation::Left),	
    41, 42, static_cast<uint8_t>(Rotation::Left),	
    39, 42, static_cast<uint8_t>(Rotation::Left),

    37, 43, static_cast<uint8_t>(Rotation::Left),	
    35, 43, static_cast<uint8_t>(Rotation::Left),	
    33, 43, static_cast<uint8_t>(Rotation::Left),	
    31, 43, static_cast<uint8_t>(Rotation::Left),	//120	

    29, 43, static_cast<uint8_t>(Rotation::Left),	
    27, 44, static_cast<uint8_t>(Rotation::Left),	
    25, 44, static_cast<uint8_t>(Rotation::Left),	
    23, 44, static_cast<uint8_t>(Rotation::Left),	

    21, 44, static_cast<uint8_t>(Rotation::Left),	
    19, 44, static_cast<uint8_t>(Rotation::Left),	
    17, 45, static_cast<uint8_t>(Rotation::Left),	
    15, 45, static_cast<uint8_t>(Rotation::Left),	

    13, 45, static_cast<uint8_t>(Rotation::Left),	
    11, 45, static_cast<uint8_t>(Rotation::Left),	
    9, 45, static_cast<uint8_t>(Rotation::Left),	
    7, 46, static_cast<uint8_t>(Rotation::Left),	

    5, 47, static_cast<uint8_t>(Rotation::Left),	
    5, 49, static_cast<uint8_t>(Rotation::Left),	
    5, 51, static_cast<uint8_t>(Rotation::Left),	
    5, 53, static_cast<uint8_t>(Rotation::None),	 

    5, 55, static_cast<uint8_t>(Rotation::None),	
    5, 57, static_cast<uint8_t>(Rotation::None),	
    5, 59, static_cast<uint8_t>(Rotation::None),	
    5, 61, static_cast<uint8_t>(Rotation::None),	 //140

    5, 63, static_cast<uint8_t>(Rotation::None),	
    5, 65, static_cast<uint8_t>(Rotation::None),	

  };

  #define BARREL_POSITION_SPLASH_END 142  

  inline void readBarrelData(Coordinates::BarrelData &barrelData, uint8_t index) {

    int8_t x = pgm_read_byte(&Coordinates::Barrel[(index * BARREL_NUMBER_OF_ELEMENTS)]);
    int8_t y = pgm_read_byte(&Coordinates::Barrel[(index * BARREL_NUMBER_OF_ELEMENTS) + 1]);
    uint8_t data = pgm_read_byte(&Coordinates::Barrel[(index * BARREL_NUMBER_OF_ELEMENTS) + 2]);

    barrelData.x = x;
    barrelData.y = y;
    barrelData.rotation = static_cast<Rotation>(data & 0x07);
    barrelData.skip = data  >> 3;
    
  } 
  
}