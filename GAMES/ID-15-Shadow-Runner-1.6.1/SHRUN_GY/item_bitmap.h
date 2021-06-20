#include <pgmspace.h>

PROGMEM prog_uchar stone_bitmap[] =
{
  16,16,
  0b00000000, 0b00000000, //                 
  0b00111111, 0b11000000, //   ########      
  0b01111111, 0b11110000, //  ###########    
  0b11111111, 0b11110000, // ############    
  0b11111111, 0b11111000, // #############   
  0b11111111, 0b11111100, // ##############  
  0b11111111, 0b11011100, // ########## ###  
  0b11111111, 0b10011100, // #########  ###  
  0b11111110, 0b00111000, // #######   ###   
  0b11111100, 0b01111000, // ######   ####   
  0b11110000, 0b11110000, // ####    ####    
  0b11111111, 0b11110000, // ############    
  0b11111111, 0b11110000, // ############    
  0b11111111, 0b11000000, // ##########      
  0b01111111, 0b00000000, //  #######        
  0b00000000, 0b00000000, //                 
};

PROGMEM prog_uchar stonemask_bitmap[] =
{
  16,16,
  0b01111111, 0b11100000, //  ##########     
  0b11111111, 0b11111000, // #############   
  0b11111111, 0b11111000, // #############   
  0b11111111, 0b11111100, // ##############  
  0b11111111, 0b11111110, // ############### 
  0b11111111, 0b11111110, // ############### 
  0b11111111, 0b11111110, // ############### 
  0b11111111, 0b11111110, // ############### 
  0b11111111, 0b11111110, // ############### 
  0b11111111, 0b11111100, // ##############  
  0b11111111, 0b11111100, // ##############  
  0b11111111, 0b11111000, // #############   
  0b11111111, 0b11111000, // #############   
  0b11111111, 0b11111000, // #############   
  0b11111111, 0b11100000, // ###########     
  0b11111111, 0b10000000, // #########       
};

PROGMEM prog_uchar heart_bitmap[] =
{
  16,16,
  0b00000000, 0b00000000, //                 
  0b00000001, 0b11111000, //        ######   
  0b00000011, 0b11111100, //       ########  
  0b00000111, 0b11111110, //      ########## 
  0b00001111, 0b11111110, //     ########### 
  0b00011111, 0b11111110, //    ############ 
  0b00111111, 0b11111100, //   ############  
  0b01111111, 0b11111000, //  ############   
  0b01111111, 0b11111000, //  ############   
  0b00111111, 0b11111100, //   ############  
  0b00011111, 0b11111110, //    ############ 
  0b00001111, 0b11111110, //     ########### 
  0b00000111, 0b11111110, //      ########## 
  0b00000011, 0b11111100, //       ########  
  0b00000001, 0b11111000, //        ######   
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 

  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000001, 0b11100000, //        ####     
  0b00000011, 0b11110000, //       ######    
  0b00000111, 0b11111000, //      ########   
  0b00001111, 0b11111000, //     #########   
  0b00011111, 0b11110000, //    #########    
  0b00011111, 0b11110000, //    #########    
  0b00001111, 0b11111000, //     #########   
  0b00000111, 0b11111000, //      ########   
  0b00000011, 0b11110000, //       ######    
  0b00000001, 0b11100000, //        ####     
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
};



PROGMEM prog_uchar heartmask_bitmap[] =
{
  16,16,
  0b00000011, 0b11111100, //       ########  
  0b00000111, 0b11111110, //      ########## 
  0b00001111, 0b11111111, //     ############
  0b00011111, 0b11111111, //    #############
  0b00111111, 0b11111111, //   ##############
  0b01111111, 0b11111111, //  ###############
  0b11111111, 0b11111111, // ################
  0b11111111, 0b11111110, // ############### 
  0b11111111, 0b11111110, // ############### 
  0b11111111, 0b11111111, // ################
  0b01111111, 0b11111111, //  ###############
  0b00111111, 0b11111111, //   ##############
  0b00011111, 0b11111111, //    #############
  0b00001111, 0b11111111, //     ############
  0b00000111, 0b11111110, //      ########## 
  0b00000011, 0b11111100, //       ########  
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
  0b00000011, 0b11110000, //       ######    
  0b00000111, 0b11111000, //      ########   
  0b00001111, 0b11111100, //     ##########  
  0b00011111, 0b11111100, //    ###########  
  0b00111111, 0b11111100, //   ############  
  0b00111111, 0b11111100, //   ############  
  0b00111111, 0b11111100, //   ############  
  0b00111111, 0b11111100, //   ############  
  0b00011111, 0b11111100, //    ###########  
  0b00001111, 0b11111100, //     ##########  
  0b00000111, 0b11111000, //      ########   
  0b00000011, 0b11110000, //       ######    
  0b00000000, 0b00000000, //                 
  0b00000000, 0b00000000, //                 
};


PROGMEM prog_uchar bird_bitmap[] =
{
  19,24,
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b11000000, //                 ##      
  0b00000000, 0b00000000, 0b11100000, //                 ###     
  0b00000000, 0b00001000, 0b11110000, //             #   ####    
  0b00000000, 0b00001111, 0b11111100, //             ##########  
  0b00000000, 0b00000111, 0b11111100, //              #########  
  0b00000000, 0b00000111, 0b11111100, //              #########  
  0b00000000, 0b00000111, 0b11111000, //              ########   
  0b00000000, 0b00001111, 0b00110000, //             ####  ##    
  0b00000000, 0b00001111, 0b00000000, //             ####        
  0b00000000, 0b00001111, 0b00000000, //             ####        
  0b00000000, 0b00001111, 0b00000000, //             ####        
  0b00000000, 0b00001111, 0b10000000, //             #####       
  0b00000000, 0b00001111, 0b10000000, //             #####       
  0b00000000, 0b00001111, 0b10000000, //             #####       
  0b00000000, 0b00001001, 0b00000000, //             #  #        
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000011, 0b00000000, //               ##        
  0b00000000, 0b00001111, 0b00000000, //             ####        
  0b00000000, 0b00001111, 0b10000000, //             #####       
  0b00000000, 0b00000111, 0b10000000, //              ####       
  0b00000000, 0b00000111, 0b10000000, //              ####       
  0b00000000, 0b00000111, 0b10000000, //              ####       
  0b00000000, 0b00001111, 0b00000000, //             ####        
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00010000, 0b00000000, //            #            
  0b00000000, 0b00011000, 0b00000000, //            ##           
  0b00000000, 0b00111100, 0b00000000, //           ####          
  0b00000000, 0b00111110, 0b00000000, //           #####         
  0b00000000, 0b01111110, 0b00000000, //          ######         
  0b00000000, 0b01111110, 0b00000000, //          ######         
  0b00000000, 0b01111110, 0b00000000, //          ######         
  0b00000000, 0b00011110, 0b00000000, //            ####         
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00011110, 0b00000000, //            ####         
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00001100, 0b00000000, //             ##          
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b11100000, 0b00000000, //         ###             
  0b00000001, 0b11101000, 0b00000000, //        #### #           
  0b00000011, 0b11101100, 0b00000000, //       ##### ##          
  0b00000111, 0b11110110, 0b00000000, //      ####### ##         
  0b00001111, 0b11111110, 0b00000000, //     ###########         
  0b00011111, 0b11111110, 0b00000000, //    ############         
  0b00011111, 0b11111110, 0b00000000, //    ############         
  0b00001100, 0b01111110, 0b00000000, //     ##   ######         
  0b00000000, 0b11111110, 0b00000000, //         #######         
  0b00000001, 0b11111110, 0b00000000, //        ########         
  0b00000001, 0b11111110, 0b00000000, //        ########         
  0b00000001, 0b11111100, 0b00000000, //        #######          
  0b00000001, 0b10011100, 0b00000000, //        ##  ###          
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b01100000, 0b00000000, //          ##             
  0b00000001, 0b11100100, 0b00000000, //        ####  #          
  0b00000111, 0b11110110, 0b00000000, //      ####### ##         
  0b00001111, 0b11110011, 0b00000000, //     ########  ##        
  0b00011111, 0b11111011, 0b00000000, //    ########## ##        
  0b00111111, 0b11111111, 0b00000000, //   ##############        
  0b00111111, 0b11111111, 0b00000000, //   ##############        
  0b00011100, 0b11111111, 0b00000000, //    ###  ########        
  0b00000100, 0b11111111, 0b00000000, //      #  ########        
  0b00000011, 0b11111111, 0b00000000, //       ##########        
  0b00000011, 0b11111110, 0b00000000, //       #########         
  0b00000011, 0b11111110, 0b00000000, //       #########         
  0b00000000, 0b10001100, 0b00000000, //         #   ##          
  0b00000000, 0b00000100, 0b00000000, //              #          
  0b00000000, 0b00000100, 0b00000000, //              #          
  0b00000000, 0b00000100, 0b00000000, //              #          
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00100000, 0b00000000, //           #             
  0b00000000, 0b11101000, 0b00000000, //         ### #           
  0b00000001, 0b11111100, 0b00000000, //        #######          
  0b00000011, 0b11110110, 0b00000000, //       ###### ##         
  0b00000111, 0b11111110, 0b00000000, //      ##########         
  0b00000111, 0b11111110, 0b00000000, //      ##########         
  0b00000111, 0b11111110, 0b00000000, //      ##########         
  0b00000011, 0b01111110, 0b00000000, //       ## ######         
  0b00000001, 0b00111110, 0b00000000, //        #  #####         
  0b00000000, 0b01111110, 0b00000000, //          ######         
  0b00000000, 0b11111110, 0b00000000, //         #######         
  0b00000000, 0b11111100, 0b00000000, //         ######          
  0b00000000, 0b11011100, 0b00000000, //         ## ###          
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00000110, 0b00000000, //              ##         
  0b00000000, 0b00000110, 0b00000000, //              ##         
  0b00000000, 0b00000110, 0b00000000, //              ##         
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00001100, 0b00000000, //             ##          
  0b00000000, 0b00001100, 0b00000000, //             ##          
  0b00000000, 0b00001100, 0b00000000, //             ##          
  0b00000000, 0b00001100, 0b00000000, //             ##          
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000001, 0b10000000, //                ##       
  0b00000000, 0b00000001, 0b11000000, //                ###      
  0b00000000, 0b00001001, 0b11000000, //             #  ###      
  0b00000000, 0b00001111, 0b11100000, //             #######     
  0b00000000, 0b00000111, 0b11110000, //              #######    
  0b00000000, 0b00000111, 0b11110000, //              #######    
  0b00000000, 0b00000111, 0b11100000, //              ######     
  0b00000000, 0b00001111, 0b01000000, //             #### #      
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00001111, 0b00000000, //             ####        
  0b00000000, 0b00001111, 0b00000000, //             ####        
  0b00000000, 0b00001111, 0b00000000, //             ####        
  0b00000000, 0b00001101, 0b00000000, //             ## #        
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00001000, 0b00000000, //             #           
  0b00000000, 0b00000000, 0b00000000, //                         
};

PROGMEM prog_uchar birdmask_bitmap[] =
{
  19,24,
  0b00000000, 0b00000001, 0b11100000, //                ####     
  0b00000000, 0b00000001, 0b11110000, //                #####    
  0b00000000, 0b00011101, 0b11111000, //            ### ######   
  0b00000000, 0b00011111, 0b11111110, //            ############ 
  0b00000000, 0b00011111, 0b11111110, //            ############ 
  0b00000000, 0b00011111, 0b11111110, //            ############ 
  0b00000000, 0b00001111, 0b11111110, //             ########### 
  0b00000000, 0b00011111, 0b11111110, //            ############ 
  0b00000000, 0b00011111, 0b11111100, //            ###########  
  0b00000000, 0b00011111, 0b11111000, //            ##########   
  0b00000000, 0b00011111, 0b10000000, //            ######       
  0b00000000, 0b00011111, 0b11000000, //            #######      
  0b00000000, 0b00011111, 0b11000000, //            #######      
  0b00000000, 0b00011111, 0b11000000, //            #######      
  0b00000000, 0b00011111, 0b11000000, //            #######      
  0b00000000, 0b00011111, 0b11000000, //            #######      
  0b00000000, 0b00011111, 0b10000000, //            ######       
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000111, 0b10000000, //              ####       
  0b00000000, 0b00011111, 0b10000000, //            ######       
  0b00000000, 0b00011111, 0b11000000, //            #######      
  0b00000000, 0b00011111, 0b11000000, //            #######      
  0b00000000, 0b00011111, 0b11000000, //            #######      
  0b00000000, 0b00001111, 0b11000000, //             ######      
  0b00000000, 0b00011111, 0b11000000, //            #######      
  0b00000000, 0b00011111, 0b11000000, //            #######      
  0b00000000, 0b00011111, 0b10000000, //            ######       
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00111000, 0b00000000, //           ###           
  0b00000000, 0b00111100, 0b00000000, //           ####          
  0b00000000, 0b01111110, 0b00000000, //          ######         
  0b00000000, 0b01111111, 0b00000000, //          #######        
  0b00000000, 0b11111111, 0b00000000, //         ########        
  0b00000000, 0b11111111, 0b00000000, //         ########        
  0b00000000, 0b11111111, 0b00000000, //         ########        
  0b00000000, 0b11111111, 0b00000000, //         ########        
  0b00000000, 0b11111111, 0b00000000, //         ########        
  0b00000000, 0b00111111, 0b00000000, //           ######        
  0b00000000, 0b00111111, 0b00000000, //           ######        
  0b00000000, 0b00111111, 0b00000000, //           ######        
  0b00000000, 0b00111110, 0b00000000, //           #####         
  0b00000000, 0b00111110, 0b00000000, //           #####         
  0b00000000, 0b00011110, 0b00000000, //            ####         
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000001, 0b11110000, 0b00000000, //        #####            
  0b00000011, 0b11111100, 0b00000000, //       ########          
  0b00000111, 0b11111110, 0b00000000, //      ##########         
  0b00001111, 0b11111111, 0b00000000, //     ############        
  0b00011111, 0b11111111, 0b00000000, //    #############        
  0b00111111, 0b11111111, 0b00000000, //   ##############        
  0b00111111, 0b11111111, 0b00000000, //   ##############        
  0b00111111, 0b11111111, 0b00000000, //   ##############        
  0b00111111, 0b11111111, 0b00000000, //   ##############        
  0b00011111, 0b11111111, 0b00000000, //    #############        
  0b00000011, 0b11111111, 0b00000000, //       ##########        
  0b00000011, 0b11111111, 0b00000000, //       ##########        
  0b00000011, 0b11111111, 0b00000000, //       ##########        
  0b00000011, 0b11111110, 0b00000000, //       #########         
  0b00000011, 0b11111110, 0b00000000, //       #########         
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b11110000, 0b00000000, //         ####            
  0b00000011, 0b11111110, 0b00000000, //       #########         
  0b00001111, 0b11111111, 0b00000000, //     ############        
  0b00011111, 0b11111111, 0b10000000, //    ##############       
  0b00111111, 0b11111111, 0b10000000, //   ###############       
  0b01111111, 0b11111111, 0b10000000, //  ################       
  0b01111111, 0b11111111, 0b10000000, //  ################       
  0b01111111, 0b11111111, 0b10000000, //  ################       
  0b01111111, 0b11111111, 0b10000000, //  ################       
  0b00111111, 0b11111111, 0b10000000, //   ###############       
  0b00001111, 0b11111111, 0b10000000, //     #############       
  0b00000111, 0b11111111, 0b10000000, //      ############       
  0b00000111, 0b11111111, 0b00000000, //      ###########        
  0b00000111, 0b11111111, 0b00000000, //      ###########        
  0b00000001, 0b11011110, 0b00000000, //        ### ####         
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00001110, 0b00000000, //             ###         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b01110000, 0b00000000, //          ###            
  0b00000001, 0b11111100, 0b00000000, //        #######          
  0b00000011, 0b11111110, 0b00000000, //       #########         
  0b00000111, 0b11111111, 0b00000000, //      ###########        
  0b00001111, 0b11111111, 0b00000000, //     ############        
  0b00001111, 0b11111111, 0b00000000, //     ############        
  0b00001111, 0b11111111, 0b00000000, //     ############        
  0b00001111, 0b11111111, 0b00000000, //     ############        
  0b00001111, 0b11111111, 0b00000000, //     ############        
  0b00000111, 0b11111111, 0b00000000, //      ###########        
  0b00000011, 0b11111111, 0b00000000, //       ##########        
  0b00000001, 0b11111111, 0b00000000, //        #########        
  0b00000001, 0b11111111, 0b00000000, //        #########        
  0b00000001, 0b11111110, 0b00000000, //        ########         
  0b00000001, 0b11111110, 0b00000000, //        ########         
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00000000, 0b00000000, //                         
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00001111, 0b00000000, //             ####        
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00011111, 0b00000000, //            #####        
  0b00000000, 0b00011110, 0b00000000, //            ####         
  0b00000000, 0b00011110, 0b00000000, //            ####         
  0b00000000, 0b00011110, 0b00000000, //            ####         
  0b00000000, 0b00011110, 0b00000000, //            ####         
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00000011, 0b11000000, //               ####      
  0b00000000, 0b00000011, 0b11100000, //               #####     
  0b00000000, 0b00011111, 0b11100000, //            ########     
  0b00000000, 0b00011111, 0b11110000, //            #########    
  0b00000000, 0b00011111, 0b11111000, //            ##########   
  0b00000000, 0b00011111, 0b11111000, //            ##########   
  0b00000000, 0b00001111, 0b11111000, //             #########   
  0b00000000, 0b00011111, 0b11111000, //            ##########   
  0b00000000, 0b00011111, 0b11110000, //            #########    
  0b00000000, 0b00011111, 0b11100000, //            ########     
  0b00000000, 0b00011111, 0b10000000, //            ######       
  0b00000000, 0b00011111, 0b10000000, //            ######       
  0b00000000, 0b00011111, 0b10000000, //            ######       
  0b00000000, 0b00011111, 0b10000000, //            ######       
  0b00000000, 0b00011111, 0b10000000, //            ######       
  0b00000000, 0b00011111, 0b10000000, //            ######       
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
  0b00000000, 0b00011100, 0b00000000, //            ###          
};








