#pragma once

namespace Curves {

    namespace Roads {
            
        namespace Vertical {
        
            const uint8_t Curve_00[] = { 1, 2, 3, 2, 1, }; // 5
            const uint8_t Curve_01[] = { 1, 1, 2, 3, 3, 2, 1, 1, }; // 8
            const uint8_t Curve_02[] = { 1, 1, 2, 3, 4, 4, 4, 3, 2, 1, 1, }; // 11
            const uint8_t Curve_03[] = { 1, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 1, }; // 15
            const uint8_t Curve_04[] = { 1, 1, 2, 3, 4, 5, 6, 7, 8, 8, 8, 8, 8, 7, 6, 5, 4, 3, 2, 1, }; // 20

            const uint8_t *Data[] = { Curve_00, Curve_01, Curve_02, Curve_03, Curve_04 };
            const uint8_t Length[] = { 5, 8, 11, 15, 20 };
            const uint8_t Count = 5;
        
        }
            
        namespace Horizontal {
        
            const uint8_t Curve_00[] = {  //35
                1, 1, 2, 2, 3, 
                3, 4, 5, 6, 7, 
                9, 11, 12, 13, 14,
                14, 15, 15, 15, 14,
                14, 13, 12, 11, 9,
                7, 6, 5, 4, 3,
                3, 2, 2, 1, 1, 
            };

            const uint8_t Curve_01[] = {  //50
                1, 1, 2, 2, 3, 
                3, 4, 5, 6, 7, 
                9, 11, 14, 17, 19, 
                21, 22, 23, 24, 25,
                25, 26, 26, 27, 27,
                27, 27, 26, 26, 25, 
                25, 24, 23, 22, 21,
                19, 17, 14, 11, 9, 
                7, 6, 5, 4, 3, 
                3, 2, 2, 1, 1,
            };
            const uint8_t *Data[] = { Curve_00, Curve_01 };
            const uint8_t Length[] = { 35, 50 };
            const uint8_t Count = 2;
        
        }

    }

    namespace OtherCars {

        const uint8_t Length[] = { 16, 32 };
        const uint8_t Count = 2;

    }

}

