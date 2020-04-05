// Not Just a HatRack!
// First Created 19/10/16
// Last Edited: 10/3/17
// (c) Luke Carter (2017)
// (@Hundstrasse www.Hundstrasse.com)

// Using the Arduboy 2 library, but should all run fine with the Arduboy library
// Uncomment & comment out  as indicated if you want to run using the old library

#include <Arduboy2.h> //comment out to use Arduboy.h
// #include <Arduboy.h> //Uncomment to use Arduboy.h

#include "hatrack_data.h"

Arduboy2 arduboy; //comment out to use Arduboy.h
// Arduboy arduboy; //Uncomment to use Arduboy.h

byte x_pos = 0; //X position
int x_speed = 0; // starting x speed
byte x_framecount = 0; // number of x move frames skipped
byte max_x_speed = 5; //maximum x_speed value

byte y_pos = 0; // Y position
int y_speed = 0; //starting y speed
byte max_y_speed = 5; // Maximum y_speed value
byte y_framecount = 0; //Number of y move frames skipped
bool y_jerk_flag = false; //Flag denoting deceleration skip on jump

bool button_hold = false; //Button hold indicator for jumping
bool right_wall_flag = false; //Touching a right hand wall
bool left_wall_flag = false; //Touching a left hand wall
bool right_wall_sflag = false; // Right wall slide flag
bool left_wall_sflag = false; // Left wall slide flag
bool roof_flag = false; //Touching the roof
bool air_flag = false; //In the air

byte level = 0; //Current level; First level is level 0
bool goal_flag = false; //Reached goal
byte goal_x = 0; // hatrack_b x position
byte goal_y = 0; //hatrack_b y position
byte current_map [8][16]; // current map data

//Teleporters warp to corresponding tele on same row
//back of tile acts like normal wall (saves space on screen!)
byte tele_loc_l [8] = {16, 16, 16, 16, 16, 16, 16, 16}; //Column position of each left tele, 16 = no tele
byte tele_loc_r [8] = {16, 16, 16, 16, 16, 16, 16, 16}; //Column position of each right tele, 16 = no tele
bool tele_flag = false; //Teleporter animation flag

byte switch_count = 0; // number of switches in level
byte goal_anim = 0; //goal reappearing animation counter

long total_frames = 0; // counts number of frames during play to give total time
int death_count = 0; // Final death counter

float calc_s () //calculates time to 2 DP
{
  float display_time = ((float)total_frames / 60);
  return display_time;
}

void draw_lvl () //Function to extract current map from flash, draw the current level, and set locations of key tiles
{
  // extract map data from flash memory
  for (int i = 0; i < 8; i ++) {
    for (int j = 0; j < 16; j ++) {
      current_map [i][j] = pgm_read_byte_near(map_data + (level * 128) + (i * 16) + (j));
    }
  }

  arduboy.clear();  //Clear screen

  //Reset Teleporter Locations
  for (int i = 0; i < 8; i ++) {
    tele_loc_l [i] = 16;
    tele_loc_r [i] = 16;
  }

  // Reset Switch count
  switch_count = 0;
  goal_anim = 0;

  //draw level
  for (int i = 0; i < 8; i ++) {
    for (int j = 0; j < 16; j ++) {

      //Brick 1
      if (current_map[i][j] == 1)
      {
        arduboy.drawBitmap((j * 8), (i * 8), brick_1, 8, 8, 1);
      }

      //Brick 2
      if (current_map[i][j] == 2)
      {
        arduboy.drawBitmap((j * 8), (i * 8), brick_2, 8, 8, 1);
      }

      //Switch Tile
      if (current_map [i][j] == 4)
      {
        arduboy.drawRect((j * 8), (i * 8), 8, 8, 1);
        switch_count ++;
        goal_anim = 15;
      }

      //Up Spike
      if (current_map[i][j] == 6)
      {
        arduboy.drawBitmap((j * 8), (i * 8), spike_u, 8, 8, 1);
      }

      //down Spike
      if (current_map[i][j] == 7)
      {
        arduboy.drawBitmap((j * 8), (i * 8), spike_d, 8, 8, 1);
      }

      //Right Facing Spike
      if (current_map[i][j] == 8)
      {
        arduboy.drawBitmap((j * 8), (i * 8), spike_r, 8, 8, 1);
      }

      //No Wall Jump Brick
      if (current_map[i][j] == 3)
      {
        arduboy.drawBitmap((j * 8), (i * 8), brick_nj, 8, 8, 1);
      }

      //Left Facing Spike
      if (current_map[i][j] == 9)
      {
        arduboy.drawBitmap((j * 8), (i * 8), spike_l, 8, 8, 1);
      }

      // LHS Tele
      if (current_map[i][j] == 10)
      {
        arduboy.drawBitmap((j * 8), (i * 8), tele_lA, 8, 8, 1);
        tele_loc_l [i] = j;
      }

      //RHS Tele
      if (current_map[i][j] == 11)
      {
        arduboy.drawBitmap((j * 8), (i * 8), tele_rA, 8, 8, 1);
        tele_loc_r [i] = j;
      }

      //Start Point
      if (current_map[i][j] == 200)
      {
        x_pos = j * 8;
        y_pos = i * 8;
      }

      //Goal position
      if (current_map[i][j] == 199)
      {
        goal_x = j * 8;
        goal_y = i * 8;
      }

    }
    if (level == 40) {
      arduboy.setCursor(16, 0);
      arduboy.print("Congratulations!");
      arduboy.setCursor(3, 8);
      arduboy.print("Continue to See Your");
      arduboy.setCursor(34, 16);
      arduboy.print("Final Time");
    }

  }

  draw_hat();

  //reset variables each time level is redrawn
  x_speed = 0;
  y_speed = 0;
  x_framecount = 0;
  y_framecount = 0;
  y_jerk_flag = false;
  goal_flag = false;
  button_hold = false;
  arduboy.display();
}

void vert_check () //Subroutine to update 'in air' and roof touching flags
{
  // Check in air
  air_flag = true;
  for (int i = 0; i < 8; i ++)
  {
    int x_block = (int)((x_pos + i) / 8);
    int y_block = (int)((y_pos + 8) / 8);
    if (y_block > 7) y_block = 0;
    if ((current_map[y_block][x_block] > 0) && (current_map[y_block][x_block] < 6)) {
      air_flag = false;

      //This is statement is included in each wall check to convert a switch tile to a normal tile when touched
      if (current_map[y_block][x_block] == 4)
      {
        current_map[y_block][x_block] = 1;
        arduboy.fillRect((x_block * 8), (y_block * 8), 8, 8, 1);
        switch_count --;
      }

    }
  }

  // Check touching roof
  roof_flag = false;
  for (int i = 0; i < 8; i ++)
  {
    int x_block = (int)((x_pos + i) / 8);
    int y_block = (int)((y_pos - 1) / 8);
    if (y_pos < 1) y_block = 7;
    if ((current_map[y_block][x_block] > 0) && (current_map[y_block][x_block] < 6)) {
      roof_flag = true;
      if (current_map[y_block][x_block] == 4)
      {
        current_map[y_block][x_block] = 1;
        arduboy.fillRect((x_block * 8), (y_block * 8), 8, 8, 1);
        switch_count --;
      }
    }
  }
}

void wall_check () //Subroutine to update wall touch & slide flags
{

  //Wall slide left check
  left_wall_flag = false ;
  left_wall_sflag = false;
  for (int i = 0; i < 8; i ++)
  {
    int x_block = (int)((x_pos - 1) / 8);
    int y_block = (int)((y_pos + i) / 8);
    if (y_block > 7) y_block = 0;
    if (((current_map[y_block][x_block] > 0) && (current_map[y_block][x_block] < 6)) || (x_pos <= 0) || ((current_map[y_block][x_block] == 11))) {
      left_wall_flag = true;
      if ((i > 3) && (current_map[y_block][x_block] != 3)) left_wall_sflag = true;
      if (current_map[y_block][x_block] == 4)
      {
        current_map[y_block][x_block] = 1;
        arduboy.fillRect((x_block * 8), (y_block * 8), 8, 8, 1);
        switch_count --;
      }
    }
  }

  //Wall slide right check
  right_wall_flag = false ;
  right_wall_sflag = false ;
  for (int i = 0; i < 8; i ++)
  {
    int x_block = (int)((x_pos + 8) / 8);
    int y_block = (int)((y_pos + i) / 8);
    if (y_block > 7) y_block = 0;
    if (((current_map[y_block][x_block] > 0) && (current_map[y_block][x_block] < 6)) || (x_pos >= (WIDTH - 8) || (current_map[y_block][x_block] == 10))) {
      right_wall_flag = true;
      if ((i > 3) && (current_map[y_block][x_block] != 3)) right_wall_sflag = true;
      if (current_map[y_block][x_block] == 4)
      {
        current_map[y_block][x_block] = 1;
        arduboy.fillRect((x_block * 8), (y_block * 8), 8, 8, 1);
        switch_count --;
      }
    }
  }
}

void goal_check ()   //Check if top row pixels 2 and 7 are in goal
{
  if (switch_count == 0) {
    if (current_map[(int)((y_pos) / 8)][(int)((x_pos + 1) / 8)] == 199) {
      if (current_map[(int)((y_pos) / 8)][(int)((x_pos + 6) / 8)] == 199) {
        goal_flag = true;
      }
    }
  }

  // Spike check; Checks all 4 corners
  int ul_test = current_map[(int)((y_pos) / 8)][(int)((x_pos) / 8)];
  int ll_test = current_map[(int)((y_pos + 7) / 8)][(int)((x_pos) / 8)];
  int lr_test = current_map[(int)((y_pos + 7) / 8)][(int)((x_pos + 7) / 8)];
  int ur_test = current_map[(int)((y_pos) / 8)][(int)((x_pos + 7) / 8)];
  if (((ul_test > 5) && (ul_test < 10)) || ((lr_test > 5) && (lr_test < 10)) || ((ur_test > 5) && (ur_test < 10)) || ((ll_test > 5) && (ll_test < 10))) {
    death_anim();
    draw_lvl ();
  }
}

void tele_check ()    //Check for Teleport and Vertical screen wrap
{
  if (current_map [(int)((y_pos) / 8)][(int)((x_pos) / 8)] == 10) {
    x_pos = (8 * tele_loc_r [(int)((y_pos) / 8)]) - 8;
  }
  if (current_map [(int)((y_pos + 7) / 8)][(int)((x_pos + 7) / 8)] == 11) {
    x_pos = (8 * tele_loc_l [(int)((y_pos) / 8)]) + 8;
  }

  //if (y_pos < 0) y_pos = (HEIGHT - 8);
  //if (y_pos > (HEIGHT - 8)) y_pos = 0;

}

void move_char_x()   //X movement & speed change
{
  //speed change conditions and adjustments (in non-wallslide condition)
  if (((! left_wall_sflag) && (! right_wall_sflag)) || (! air_flag))
  {
    if (arduboy.pressed(RIGHT_BUTTON))
    {
      x_speed += 1;
      if (x_speed < 0) x_speed += 2;
    }
    if (arduboy.pressed(LEFT_BUTTON))
    {
      x_speed -= 1;
      if (x_speed > 0) x_speed -= 2;
    }
  }

  // Decelerate on button release on ground
  if ((! arduboy.pressed(RIGHT_BUTTON)) && (! arduboy.pressed(LEFT_BUTTON)) && (x_speed != 0) && (! air_flag))
  {
    if (x_speed > 0) x_speed -= 1;
    if (x_speed < 0) x_speed += 1;
  }

  // Limit Speed to Max
  if (x_speed > max_x_speed) x_speed = max_x_speed;
  if (x_speed < (max_x_speed * (-1))) x_speed = (max_x_speed * (-1));

  //Character positon move
  if (x_speed > 0)
  {
    if (! right_wall_flag)
    {
      x_pos += 1;
      wall_check();
      //Double px @ max speed
      if (x_speed == max_x_speed && (! right_wall_flag))
      {
        x_pos += 1;
        wall_check();
      }
    }
    else
    {
      x_speed = 0;
    }
  }
  if (x_speed < 0)
  {
    if (! left_wall_flag)
    {
      x_pos -= 1;
      wall_check();
      //Double px @ max speed
      if (x_speed == (max_x_speed * (-1)) && (! left_wall_flag))
      {
        x_pos -= 1;
        wall_check();
      }
    }
    else
    {
      x_speed = 0;
    }
  }

  //Reset Frameskip Count
  x_framecount = 0;
}

void move_char_y ()   // Y Movement & Speed Change
{
  //check if in air and accelerate towards ground
  if (air_flag) //In air prog branch
  {
    if (!(arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)))
    {
      //to stop upward acceleration on button release
      button_hold = false;
    }

    //decelerate slower as long as button held and speed above threshold
    //Achieved by removing slowdown every other frame (using y_jerk_flag)
    if ((button_hold) && (y_speed > 0) && (y_jerk_flag == false))
    {
      y_jerk_flag = true;
    }
    else
    {
      //decelerate
      y_speed -= 1;
      y_jerk_flag = false;
    }

    //Left Hand Wall Jump
    if ((left_wall_sflag) && (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) && (! button_hold))
    {
      y_speed = max_y_speed;
      x_speed = 3; // X walljump speed
      button_hold = true;
      y_jerk_flag = false;
    }

    //Right Hand Wall Jump
    if ((right_wall_sflag) && (arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)) && (! button_hold))
    {
      y_speed = max_y_speed;
      x_speed = (-3); // X walljump speed
      button_hold = true;
      y_jerk_flag = false;
    }
  }

  else // On Ground prog branch
  {
    y_speed = 0;

    if ((arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON)))
    {
      //initial jump @ max y speed
      y_speed = max_y_speed;
      button_hold = true; //reset button hold flag
      y_jerk_flag = false; //reset y acc. frameskip
    }
  }

  //Character Position Move
  if (y_speed > 0)
  {
    if (! roof_flag)
    {

      if (y_pos == 0) {
        y_pos = (HEIGHT - 8);
      }
      else
      {
        y_pos -= 1;
      }
      vert_check();
      //Double px @ Max Speed
      if ((abs(y_speed) == max_y_speed) && (! roof_flag))
      {
        if (y_pos == 0) {
          y_pos = (HEIGHT - 8);
        }
        else
        {
          y_pos -= 1;
        }
        vert_check();
      }
    }
    else
    {
      y_speed = 0;
    }

  }
  if (y_speed < 0)
  {
    if (air_flag)
    {
      y_pos += 1;
      if (y_pos > (HEIGHT - 8)) y_pos = 0;
      vert_check();
      //Double px @ Max Speed
      if ((abs(y_speed) == max_y_speed) && (air_flag))
      {
        y_pos += 1;
        if (y_pos > (HEIGHT - 8)) y_pos = 0;
        vert_check();
      }
    }
    else
    {
      y_speed = 0;
    }
  }

  //Reset Frameskip Count
  y_framecount = 0;

}

void draw_hat () {  //Character draw (& hatrack redraw)

  // redraw hatrack each frame
  // only if no switches remaining
  if (switch_count == 0) {
    arduboy.drawBitmap (goal_x, goal_y, hatrack_b, 8, 8, 1);
    arduboy.drawBitmap (goal_x, goal_y - 8, hatrack_t, 8, 8, 1);
    //hatrack appears from ground up
    if (goal_anim != 0) {
      arduboy.fillRect(goal_x, goal_y - 8, 8, goal_anim, 0);
      goal_anim --;
    }
  }

  if (left_wall_sflag && air_flag) arduboy.drawBitmap(x_pos, y_pos, hat_wall_left, 8, 8, 1);
  else if (right_wall_sflag && air_flag) arduboy.drawBitmap(x_pos, y_pos, hat_wall_right, 8, 8, 1);
  else if (x_speed > 4) arduboy.drawBitmap(x_pos, y_pos, hat_right, 8, 8, 1);
  else if (x_speed < -4) arduboy.drawBitmap(x_pos, y_pos, hat_left, 8, 8, 1);
  else arduboy.drawBitmap(x_pos, y_pos, hat, 8, 8, 1);
}

void hat_toss () { //Level End sequence
  // zero speeds for vanilla sprite
  x_speed = 0;
  y_speed = 0;

  // move hat to animation start point (should be only 1 or 2 pixels jump)
  arduboy.fillRect(x_pos, y_pos, 8, 8, 0);
  x_pos = goal_x;
  y_pos = goal_y;
  byte toss_frame = 0;
  byte toss_xframe = 0;
  draw_hat ();
  arduboy.display();
  while (true) {
    if (arduboy.nextFrame()) {
      arduboy.fillRect(x_pos, y_pos, 8, 8, 0);

      if (toss_frame == 17) {
        arduboy.drawBitmap (goal_x, goal_y, hatrack_b, 8, 8, 1);
        arduboy.drawBitmap (goal_x, goal_y - 8, hatrack_t, 8, 8, 1);
        arduboy.drawBitmap(x_pos, y_pos, hat_tilt, 8, 8, 1);
        arduboy.display();
        delay (1000); // end of level pause
        return;
      }

      if (toss_frame > 13) {
        y_pos ++;
      }
      else {
        y_pos --;
      }

      if (toss_xframe == 4) {
        x_pos --;
        toss_xframe = 0;
      }
      else {
        toss_xframe ++;
      }

      draw_hat ();
      arduboy.display();
      toss_frame ++;
    }
  }
}

void death_anim () { //Animates growing square blanking out hat
  death_count ++;
  delay (500); // slight pause because everyone wants to know that they've just jumped into spikes
  byte count = 0;
  while (true) {
    if (arduboy.nextFrame()) {
      if (count == 0) arduboy.fillRect(x_pos + 3, y_pos + 3, 2, 2, 0);
      if (count == 5) arduboy.fillRect(x_pos + 2, y_pos + 2, 4, 4, 0);
      if (count == 10) arduboy.fillRect(x_pos + 1, y_pos + 1, 6, 6, 0);
      if (count == 15) arduboy.fillRect(x_pos, y_pos, 8, 8, 0);
      arduboy.display();
      if (count == 16) {
        delay (500); //pause before respawn
        return;
      }
      count ++;
    }
  }
}

void tele_anim () {   //Teleporter fizzling animation
  if (tele_flag) {
    tele_flag = false;
    for (int i = 0; i < 8; i ++) {
      if (tele_loc_l [i] < 16) {
        arduboy.fillRect((tele_loc_l [i] * 8), (i * 8), 8, 8, 0);
        arduboy.drawBitmap((tele_loc_l [i] * 8), (i * 8), tele_lA, 8, 8, 1);
      }
      if (tele_loc_r [i] < 16) {
        arduboy.fillRect((tele_loc_r [i] * 8), (i * 8), 8, 8, 0);
        arduboy.drawBitmap((tele_loc_r [i] * 8), (i * 8), tele_rA, 8, 8, 1);
      }
    }
  }
  else {
    tele_flag = true;
    for (int i = 0; i < 8; i ++) {
      if (tele_loc_l [i] < 16) {
        arduboy.fillRect((tele_loc_l [i] * 8), (i * 8), 8, 8, 0);
        arduboy.drawBitmap((tele_loc_l [i] * 8), (i * 8), tele_lB, 8, 8, 1);
      }
      if (tele_loc_r [i] < 16) {
        arduboy.fillRect((tele_loc_r [i] * 8), (i * 8), 8, 8, 0);
        arduboy.drawBitmap((tele_loc_r [i] * 8), (i * 8), tele_rB, 8, 8, 1);
      }
    }
  }
}

void intro_seq () { // Opening Titles and Intro screen
  arduboy.clear();
  arduboy.display();
  delay (100);
  for (int k = 0; k < 3; k++) {
    arduboy.drawSlowXYBitmap(0, 0, hundstrasse, WIDTH, HEIGHT, 1);
    arduboy.setCursor(32, 54);
    arduboy.print("@Hundstrasse");
    for (int i = 0; i < 8; i ++) {
      for (int j = 0; j < 16; j ++) {
        arduboy.fillRect(((j * 8) + 1 + k), ((i * 8) + 1 + k), (6 - (k * 2)), (6 - (k * 2)) , 0);
      }
    }
    arduboy.display();
    delay (50);
  }
  arduboy.drawSlowXYBitmap(0, 0, hundstrasse, WIDTH, HEIGHT, 1);
  arduboy.setCursor(32, 54);
  arduboy.print("@Hundstrasse");
  arduboy.display();
  delay (1500);
  for (int k = 0; k < 3; k++) {
    for (int i = 0; i < 8; i ++) {
      for (int j = 0; j < 16; j ++) {
        arduboy.fillRect(((j * 8) + 4 - k ), ((i * 8) + 4 - k), ((k + 1) * 2), ((k + 1) * 2) , 0);
      }
    }
  }
  arduboy.display();
  delay (100);
  arduboy.clear();
  arduboy.display();
  delay (500);
  arduboy.drawSlowXYBitmap(0, 0, title, WIDTH, HEIGHT, 1);
  arduboy.fillRect(0, 0, 52, 64, 0);
  arduboy.display();
  delay (500);
  arduboy.drawSlowXYBitmap(0, 0, title, WIDTH, HEIGHT, 1);
  arduboy.fillRect(0, 18, 52, 46, 0);
  arduboy.display();
  delay (500);
  arduboy.drawSlowXYBitmap(0, 0, title, WIDTH, HEIGHT, 1);
  arduboy.fillRect(0, 35, 52, 29, 0);
  arduboy.display();
  delay (500);
  arduboy.drawSlowXYBitmap(0, 0, title, WIDTH, HEIGHT, 1);
  arduboy.fillRect(0, 50, 52, 14, 0);
  arduboy.display();
  delay (500);
  arduboy.drawSlowXYBitmap(0, 0, title, WIDTH, HEIGHT, 1);
  arduboy.display();
  while (!(arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON))) {}
  arduboy.clear();
  arduboy.display();
  delay (500);
  arduboy.drawBitmap(59, 1, hat, 8, 8, 1);
  for (int i = 0; i < 8; i ++) {
    arduboy.drawBitmap(0, (i * 8), brick_2, 8, 8, 1);
  }
  for (int i = 0; i < 8; i ++) {
    arduboy.drawBitmap(120, (i * 8), brick_2, 8, 8, 1);
  }
  arduboy.display();
  delay (750);
  arduboy.setCursor(13, 10);
  arduboy.print("This is Karlville");
  arduboy.display();
  delay (750);
  arduboy.setCursor(30, 20);
  arduboy.print("He is A Hat");
  arduboy.display();
  delay (750);
  arduboy.setCursor(21, 30);
  arduboy.print("Get Him To The");
  arduboy.display();
  delay (750);
  arduboy.setCursor(39, 40);
  arduboy.print("Hat Rack");
  arduboy.display();
  delay (750);
  arduboy.drawBitmap (59, 56, hatrack_b, 8, 8, 1);
  arduboy.drawBitmap (59, 48, hatrack_t, 8, 8, 1);
  arduboy.display();
  while (!(arduboy.pressed(A_BUTTON) || arduboy.pressed(B_BUTTON))) {}
}

void game_loop () {

  //Check for next frame
  if (!(arduboy.nextFrame())) return;

  //Clear last position
  arduboy.fillRect(x_pos, y_pos, 8, 8, 0);

  //Update check flags and move if necessary
  wall_check();
  vert_check();
  if (x_framecount > (max_x_speed - (abs(x_speed) + 1)) || x_speed == 0) move_char_x();
  wall_check();
  vert_check();
  if (y_framecount > (max_y_speed - (abs(y_speed) + 1)) || y_speed == 0) move_char_y();

  //Check for tele
  tele_check ();

  //Draw next position
  draw_hat ();

  //Animate Tele
  tele_anim ();

  //incriment framecount
  x_framecount += 1;
  y_framecount += 1;
  if (level < 40 ) total_frames ++;

  // Output to Display
  arduboy.display();

  //Check Goal
  goal_check ();
}

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60); //Framerate adjusts gamespeed
  arduboy.clear();  //Clear screen
  arduboy.display();
  intro_seq();
}

//Begin Program Loop
void loop() {

  //Pre-level screen with level no. and total time
  // Urgh, why do I find constructing strings so difficult??... anyway, this seems to work...
  if (level < 40) {
    arduboy.clear();  //Clear screen
    arduboy.setCursor(30, 20);
    String temp = String("Level " + String((level + 1)));
    arduboy.print(temp);
    arduboy.setCursor(5, 50);
    temp = String("Total Time: " + String(calc_s()) + "s");
    arduboy.print(temp);
    arduboy.display();
    delay (1500);
  }

  draw_lvl();

  while (!goal_flag) {
    game_loop();
  }

  hat_toss(); //;level finish sequence

  level ++;
  if (level == 41) {
    arduboy.clear();  //Clear screen
    arduboy.setCursor(5, 10);
    String temp = String("Total Time: " + String(calc_s()) + "s");
    arduboy.print(temp);
    arduboy.setCursor(5, 25);
    temp = String("Total Deaths: " + String(death_count));
    arduboy.print(temp);
    arduboy.setCursor(39, 45);
    arduboy.print("Tweet Pic");
    arduboy.setCursor(32, 55);
    arduboy.print("@Hundstrasse");
    arduboy.display();
    while (true){
      delay (5000); // infinite loop 
    }
  }
}
