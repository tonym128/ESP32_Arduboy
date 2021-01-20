/* Code concept from filmote thanks filmote
 * Pharap and MLXXXP helped too thanks!
 * 
 *  ARDUBOY FOREVER!
 * 
 * 
 */
 
#include <FixedPointsCommon.h>

#define boardWidth 5
#define boardHeight 10
#define boardOffset 47
#define partSize 6

struct Particle {

    private:

        int16_t x = 0;
        int16_t y = 0;
        SQ7x8 velx = 0;
        SQ7x8 vely = 0;
        uint8_t counterInit = 0;
        uint8_t counter = 0;
        uint8_t pSize = 1;
        uint8_t type = 1;
        uint8_t wordNum;

    public:

        static constexpr SQ7x8 rThresh = 0.000001 * 16;
        static constexpr SQ7x8 cF = 0.96;
        static constexpr SQ7x8 gravity = 0.12;

        int16_t getX()                          { return this->x; }
        int16_t getY()                          { return this->y; }
        int8_t getCounter()                     { return this->counter; }
        uint8_t getSize()                       { return this->pSize; }
        uint8_t getWord()                       { return this->wordNum; }
        uint8_t getType()                       { return this->type; }

        void setX(int16_t val)                  { this->x = val; }
        void setY(int16_t val)                  { this->y = val; }
        void setCounter(int16_t val)             { this->counter = val; this->counterInit = val; }
        void setVelX(SQ7x8 val)                  { this->velx = val; }
        void setVelY(SQ7x8 val)                  { this->vely = val; }
        void setSize(uint8_t val)                { this->pSize = val; }
        void setType(int val)                { this->type = val; }
        void setWord(int val)                { this->wordNum = val; } 

    public:

        void update() {
          

          // move your particle based on direction, whatever ..

          this->vely -= gravity * this->pSize;
          this->velx *= cF;
          
          uint8_t boundL = 0;
          uint8_t boundR = WIDTH;

          if( this->type == 1 ){
            boundL = boardOffset+2;
            boundR = (boardWidth*partSize)+2+boardOffset;
          }
          else if( this->type ==0){
            boundL = 0;
            boundR = WIDTH;
          }
          else if( this->type ==2){
            boundL = 0;  
            boundR = boardOffset;
          }
          else if( this->type ==3){
            boundL = (boardWidth*partSize)+boardOffset+4;  
            boundR = WIDTH;
          }
                    
          if(this->x > boundR - this->pSize ){
            this->x = boundR - this->pSize ;
            this->velx = -this->velx;
            
          }
          if(this->x < boundL){
            this->x = boundL;
            this->velx = -this->velx;
          }
          
          if(this->y > (boardHeight*partSize)+4){ // above (below) board height - bottom of screen
            this->y = (boardHeight*partSize)+4;

            if(this->vely > rThresh)
              this->vely = -this->vely * rThresh;
            else
              this->vely = 0;
          }

          if( this->type ==2 || this->type ==3 ){
            // let stuff go negative yo
          }
          else{
             if(this->y < 0){ // below (above) board - top of screen
              if(this->vely > rThresh)
                this->vely = -this->vely * rThresh;
              else
                this->vely = 0;            
            }           
          }
          
          this->x += static_cast<int8_t>(this->velx);
          this->y -= static_cast<int8_t>(this->vely);

          // shift size
          
          if (this->counter < counterInit * 0.5 ){
            if( this->pSize > 2) this->pSize = 2;
          }
          if (this->counter <  counterInit * 0.2 ){
            if( this->pSize > 1) this->pSize = 1;
          }
          
          if (this->counter > 0) this->counter--;

        }

        bool render() {

            return (this->counter > 0);

        }

};
