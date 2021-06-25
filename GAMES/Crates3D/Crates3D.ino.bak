// Crates 3D. A port of the TI-83 calculator puzzle game.
// Ion version by Badja. Arduboy port by Brian Smith.
#include <ESP8266WiFi.h>
#include "Arduboy2.h"
#include "c3dstd.h"
#include "title.h"
#include "blocks.h"
Arduboy2 arduboy;

#define ALLOW_SAVE 1 // Allow progress save to EEPROM? 1=yes, 0=no

byte map3d[768];
byte start_x,start_y,start_z,end_x,end_y,end_z;
int x,y,z,angle;
byte level=1;

//-----------------------------------------------------------
void read_map(int stop_level)
{
  byte c,v;
  char title[20];
  int i,p,ci;
   
  
  p=72;  c=pgm_read_byte(c3dstd+p); title[p-72]=c; p++;
  while(c!=0) {c=pgm_read_byte(c3dstd+p); title[p-72]=c; p++;}
  
  p+=2; // skip level count

  if (stop_level==0) load(); // level 0 = load from saved state
  level=0;
  while(level!=stop_level) {
    start_y=pgm_read_byte(c3dstd+p); p++;
    start_x=pgm_read_byte(c3dstd+p); p++;
    start_z=pgm_read_byte(c3dstd+p); p++;
    end_x=pgm_read_byte(c3dstd+p); p++;
    end_y=pgm_read_byte(c3dstd+p); p++;
    end_z=pgm_read_byte(c3dstd+p); p++;
  
    // read 768 bytes run-length coded...
    ci=0; 
    while(ci!=768) {
      c=pgm_read_byte(c3dstd+p); p++;
      if (c!=145) {map3d[ci]=c; ci++;}
      else {v=pgm_read_byte(c3dstd+p);  c=pgm_read_byte(c3dstd+p+1); p+=2;
           map3d[ci]=v; ci++; c--;
           for (i=0; i<c; i++) {map3d[ci]=v; ci++;}
      }
    }
    level++;
  }
  x=start_x; y=start_y; z=start_z; angle=2;
  draw_map(x,y,z,angle);
}

//------------------------------------------------------------------
void load()
{
  int i;

  for (i=0; i<768; i++) map3d[i]=EEPROM.read(i+150);
  start_x=EEPROM.read(918); start_y=EEPROM.read(919); start_z=EEPROM.read(920);
  end_x=EEPROM.read(921); end_y=EEPROM.read(922); end_z=EEPROM.read(923); 
}

//-----------------------------------------------------------------
void save()
{
  int i;

  print(0,8,"SAVING......."); arduboy.display();
        
  for (i=0; i<768; i++) EEPROM.write(i+150,map3d[i]);
  EEPROM.write(918,x); EEPROM.write(919,y); EEPROM.write(920,z);
  EEPROM.write(921,end_x); EEPROM.write(922,end_y); EEPROM.write(923,end_z);
  EEPROM.commit();
  print(0,8,"Saved to level 0."); arduboy.display();
  angle--; if (angle<0) angle=3;
  delay(1000);
}

//------------------------------------------------------------
void draw_block_slow(int x, int y)
{
   arduboy.drawSlowXYBitmap(x,y,mask,8,10,0);
   arduboy.drawSlowXYBitmap(x,y,block,8,10,1);
}

//----------------------------------------------------------
void draw_block(int x, int y)
{
    arduboy.drawBitmap(x,y,mask90c,8,10,0);
    arduboy.drawBitmap(x,y,block90c,8,10,1);
}

//-----------------------------------------------------------
void draw_black_slow(int x, int y)
{
   arduboy.drawSlowXYBitmap(x,y,mask,8,10,0);
   arduboy.drawSlowXYBitmap(x,y,black,8,10,1);
}

//----------------------------------------------------------
void draw_black(int x, int y)
{
  arduboy.drawBitmap(x,y,mask90c,8,10,0);
  arduboy.drawBitmap(x,y,black90c,8,10,1);  
}

//-------------------------------------------------------------------
void draw_map(int xmap, int ymap, int zuser, int angle)
{ 
  int x,y,z,xs,ys,xm,ym,i,b;
  
  arduboy.fillScreen(1);
  
  for (x=0; x<35; x++) for (y=0; y<34; y++) {
    if (angle==0) {xm=xmap+x-17; ym=ymap+16-y;}
    if (angle==1) {xm=xmap+16-y; ym=ymap+17-x;}
    if (angle==2) {xm=xmap+17-x; ym=ymap+y-16;}
    if (angle==3) {xm=xmap+y-16; ym=ymap+x-17;}
    
    
    if (xm<0 || xm>31 || ym<0 || ym>23) b=0; else b=map3d[ym*32+xm];
    
    xs=80-x*4+y*2; ys=y*3+x*2-40;
    
    for (z=0; z<9; z++) {
      if (b&1) {draw_block(xs,ys);}
      if (xm==end_y && ym==end_x && z==end_z) draw_black(xs,ys);
      if (xm==xmap && ym==ymap && z==zuser) draw_black(xs,ys);
      b=b/2; ys-=5;
    }    
  }
  // add level indicator in top left....
  if (level<10) arduboy.drawChar(0,0,level+'0',0,1,1);
  else {arduboy.drawChar(0,0,level/10+'0',0,1,1); arduboy.drawChar(6,0,level%10+'0',0,1,1);}

  arduboy.display();
}

//----------------------------------------------------------------------
int read_keys() {
 if (arduboy.pressed(UP_BUTTON)) return(1);
 if (arduboy.pressed(DOWN_BUTTON)) return(3);
 if (arduboy.pressed(LEFT_BUTTON)) return(2);
 if (arduboy.pressed(RIGHT_BUTTON)) return(4);
 if (arduboy.pressed(A_BUTTON)) return(5);
 return(0);
}

//-----------------------------------------------------------
// check if there is a block at location (x,y,z)....
int check_block(int x, int y, int z)
{
  // first, bounds-checking...
  if (x<0 || x>31 || y<0 || y>23 || z<0 || z>7) return(0);
  // read map...
  if (map3d[y*32+x] & (1<<z)) return(1);
  return(0);
}

//------------------------------------------------------------
int check_move(int newx, int newy, int newz)
{
  if (check_block(newx,newy,newz-1) && !check_block(newx,newy,newz)) return(1);
  // maybe move up?
  if (check_block(newx,newy,newz) && !check_block(newx,newy,newz+1)) {z++; return(1);}
  // maybe move down?
  if (!check_block(newx,newy,newz) && !check_block(newx,newy,newz-1))
        if (check_block(newx,newy,newz-2)) {z--; return(1);}
  return(0);
}

//--------------------------------------------------------------
// move a block from (oldx,oldy,oldz) to (newx,newy,oldz)...
void move_block(int oldx, int oldy, int oldz, int newx, int newy)
{
  // first, delete the old block...
  map3d[oldy*32+oldx]&=~(1<<oldz);
  // now add a block in the new location...
  map3d[newy*32+newx]|=(1<<oldz);
  // check for dropping and implement.....
  while(oldz>=0 && !check_block(newx,newy,oldz-1)) {
    draw_map(x,y,z,angle);
    map3d[newy*32+newx]&=~(1<<oldz);
    oldz--;
    if (oldz>=0) map3d[newy*32+newx]|=(1<<oldz);
  }
}

//------------------------------------------------------------
void move(int dir)

{
  // check if 'B' key being pressed also....

  if (dir==0) {
     if (arduboy.pressed(B_BUTTON) && check_block(x,y+1,z) && !check_block(x,y+2,z) &&
         check_block(x,y+1,z-1) && y<22) {move_block(x,y+1,z,x,y+2); y++;}
     else if (check_move(x,y+1,z)) y++;
  }
  if (dir==1) {
     if (arduboy.pressed(B_BUTTON) && check_block(x+1,y,z) && !check_block(x+2,y,z) &&
         check_block(x+1,y,z-1) && x<30) {move_block(x+1,y,z,x+2,y); x++;}
     else if (check_move(x+1,y,z)) x++;
  }
  if (dir==2) {
     if (arduboy.pressed(B_BUTTON) && check_block(x,y-1,z) && !check_block(x,y-2,z) &&
        check_block(x,y-1,z-1) && y>1) {move_block(x,y-1,z,x,y-2); y--;}
     else if (check_move(x,y-1,z)) y--;
  }
  if (dir==3) {
     if (arduboy.pressed(B_BUTTON) && check_block(x-1,y,z) && !check_block(x-2,y,z) &&
         check_block(x-1,y,z-1) && x>1) {move_block(x-1,y,z,x-2,y); x--;}
     else if (check_move(x-1,y,z)) x--;
  }
}

//---------------------------------------------------------
void print(int x, int y, char *s)
{
  while(*s!=0) {arduboy.drawChar(x,y,*s,0,1,1); x+=6; s++;}
}

//-----------------------------------------------------------
void title_screen()
{
  int i;
  
  end_z=100;
  for (i=0; i<768; i++) map3d[i]=pgm_read_byte(title+i);
  draw_map(15,7,20,2);
  while(arduboy.notPressed(B_BUTTON)) {
    print(0,0," Level: ");
    if (level<10) {arduboy.drawChar(42,0,level+'0',0,1,1); arduboy.drawChar(48,0,32,0,1,1);}
    else {arduboy.drawChar(42,0,level/10+'0',0,1,1); arduboy.drawChar(48,0,level%10+'0',0,1,1);}
    
    print(2,8,"Hold-A=Quit");
    print(2,16,"A=Rotate");
    print(2,24,"B=Push");
    arduboy.display();
  
    if (arduboy.pressed(LEFT_BUTTON) && level>0) {level--; while(arduboy.pressed(LEFT_BUTTON));}
    if (arduboy.pressed(RIGHT_BUTTON) && level<16) {level++; while(arduboy.pressed(RIGHT_BUTTON));}
    if (!ALLOW_SAVE && level==0) level=1;
    delay(50);
  } 
}


//------------------------------------------------------------------
int gameloop() {

  int k,first=1;
  while(1) {
    read_map(level);
  
    while(!(y==end_x && x==end_y && z==end_z+1)) {
      draw_map(x,y,z,angle);
      k=read_keys();
      while(!k) {first=1; delay(10); k=read_keys();} // idle loop
      if (ALLOW_SAVE && arduboy.pressed(A_BUTTON) && arduboy.pressed(B_BUTTON)) save();
      if (first) {delay(100); first=0;} // allows for fine movement
      if (k>0 && k<5) move((k-1+angle)%4);
      if (k==5) { // 'A' key pressed... rorate view or reset level...
        angle++; if (angle==4) angle=0;
        while(arduboy.pressed(A_BUTTON)) {delay(10); k++; if (k==200) return(0);}
      }   
    }
    draw_map(x,y,z,angle);
    print(0,0," Solved! "); arduboy.display();
    while(!arduboy.pressed(B_BUTTON)) delay(2);
    level++; if (level==17) level=1;
  }
}

//-------------------------------------------------------------------
void setup() {
 WiFi.mode(WIFI_OFF); //disable wifi to save some battery power
 EEPROM.begin(2000);
 arduboy.begin();
}

//----------------------------------------------------------------------
void loop()
{    
    title_screen();
    gameloop();       
}
