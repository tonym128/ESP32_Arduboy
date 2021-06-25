#include "src/utils/Arduboy2Ext.h"


/* ----------------------------------------------------------------------------
 *  Draw a bone at location x, y.  
 *  
 *  This function accepts a bone value (left and righ pip value in a single hex
 *  number) and renders it without regard to orientation.  This is used when
 *  rendering the played bones in the graveyard. 
 *  
 */
void drawBone_Small(uint8_t boneValue, uint8_t x, uint8_t y, bool highlighted) {
  
  drawBone_Small(leftPips_ByBoneValue(boneValue), rightPips_ByBoneValue(boneValue), x, y, highlighted);
  
}


/* ----------------------------------------------------------------------------
 *  Draw a bone at location x, y.  
 *  
 *  This function accepts two parameters - left / top pips and right / bottom
 *  pips and renders a bone in this orientation.  This is used when rendering
 *  the board where orientation matters. 
 *  
 */
void drawBone_Small(uint8_t pipsTop, uint8_t pipsBottom, uint8_t x, uint8_t y, bool highlighted) {

  if (highlighted) {
    
    Sprites::drawSelfMasked(x, y, Images::bone_portrait_highlighted, 0);
    Sprites::drawErase(x + 2, y + 2, Images::bone_pips, pipsTop);
    Sprites::drawErase(x + 2, y + 10, Images::bone_pips, pipsBottom);
    
  }
  else {

    Sprites::drawSelfMasked(x, y, Images::bone_portrait, 0);
    Sprites::drawOverwrite(x + 2, y + 2, Images::bone_pips_highlight, pipsTop);
    Sprites::drawOverwrite(x + 2, y + 10, Images::bone_pips_highlight, pipsBottom);
  
  }
  
}


/* ----------------------------------------------------------------------------
 *  Draw a bone at location x, y in the landscape orientation.  
 *  
 *  This function accepts a bone value (left and righ pip value in a single hex
 *  number) and renders it without regard to orientation.  This is used when
 *  rendering the played bones in the graveyard. 
 *  
 */
void drawBone_Rotated(uint8_t boneValue, uint8_t x, uint8_t y, bool highlighted) {
  
  drawBone_Rotated(leftPips_ByBoneValue(boneValue), rightPips_ByBoneValue(boneValue), x, y, highlighted);
  
}


/* ----------------------------------------------------------------------------
 *  Draw a bone at location x, y in the landscape orientation.  
 *  
 *  This function accepts a bone value (left and righ pip value in a single hex
 *  number) and renders it without regard to orientation.  This is used when
 *  rendering the played bones in the graveyard. 
 *  
 */
void drawBone_Rotated(uint8_t pipsLeft, uint8_t pipsRight, uint8_t x, uint8_t y, bool highlighted) {

  if (highlighted) {
    
    Sprites::drawSelfMasked(x, y, Images::bone_landscape_highlighted, 0);
    Sprites::drawErase(x + 2, y + 2, Images::bone_pips, pipsLeft);
    Sprites::drawErase(x + 10, y + 2, Images::bone_pips, pipsRight);
    
  }
  else {

    Sprites::drawSelfMasked(x, y, Images::bone_landscape, 0);
    Sprites::drawOverwrite(x + 2, y + 2, Images::bone_pips_highlight, pipsLeft);
    Sprites::drawOverwrite(x + 10, y + 2, Images::bone_pips_highlight, pipsRight);
  
  }

}


void drawBone_Rotated_Erase(uint8_t boneValue, uint8_t x, uint8_t y) {

  Sprites::drawErase(x, y, Images::bone_landscape, 0);
  Sprites::drawErase(x + 2, y + 2, Images::bone_pips_highlight, leftPips_ByBoneValue(boneValue));
  Sprites::drawErase(x + 10, y + 2, Images::bone_pips_highlight, rightPips_ByBoneValue(boneValue));

}

/* ----------------------------------------------------------------------------
 *  Draw a bone outline only at location X, Y.
 *  
 */
void drawBone_Outline(uint8_t x, uint8_t y, bool highlighted) {

  if (highlighted) {
    
    Sprites::drawOverwrite(x, y, Images::bone_portrait_outline_highlighted, 0);
    
  }
  else {
    
    Sprites::drawOverwrite(x, y, Images::bone_portrait_outline, 0);
    
  }

}


/* ----------------------------------------------------------------------------
 *  Draw a bone outline only at location X, Y in the landscape orientation. 
 *  
 */
void drawBone_Outline_Rotated(uint8_t x, uint8_t y, bool highlighted) {

  if (highlighted) {
    
    Sprites::drawOverwrite(x, y, Images::bone_landscape_outline_highlighted, 0);
    
  }
  else {
    
    Sprites::drawOverwrite(x, y, Images::bone_landscape_outline, 0);
    
  }

}


/* ----------------------------------------------------------------------------
 *  Draw a hidden bone at location X, Y in the landscape orientation. 
 *  
 */
void drawBone_Hidden(uint8_t x, uint8_t y, bool highlighted) {

  if (highlighted) {

    Sprites::drawOverwrite(x, y, Images::bone_portrait_hidden_highlight, 0);
  
  }
  else {
    
    Sprites::drawOverwrite(x, y, Images::bone_portrait_hidden, 0);
  
  }

}
