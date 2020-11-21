/*----------------------
    spring:
  --------------------- */
uint8_t pSprings[]   = {1, 2, 3, 4, 3, 4, 3, 2, 1, 2, 1, 2, 3, 3, 4, 3};
uint8_t pSprings_1[] = {1, 2, 3, 4, 3, 4, 3, 2, 1, 2, 1, 2, 3, 3, 4, 3};
int16_t pScroll = 128;
int16_t pScrollSpeed = 1;

/*-----------------------
    initSpring
  ----------------------*/
//TODO:引数でN面を対応する
void initSpring() {
  for (int i = 0; i < 16; i++) {
    pSprings[i] = pSprings_1[i];
  }
}

/*-----------------------
    setAllOneSpring
  ----------------------*/
void setAllOneSpring() {
  for (int i = 0; i < 16; i++) {
    pSprings[i] = 1;
  }
}

/*-----------------------
    setAddSpring
  ----------------------*/
void setAddSpring() {
  for (int i = 0; i < 16; i++) {
    if (pSprings[i] < 4) {
      pSprings[i] += 1;
    }
  }
}

/*-----------------------
    setAddMaxSpring
  ----------------------*/
void setAddMaxSpring() {
  for (int i = 0; i < 16; i++) {
    pSprings[i] = 4;
  }
}



/*-----------------------
    setDelSpring
  ----------------------*/
void setDelSpring() {
  for (int i = 0; i < 16; i++) {
    if (pSprings[i] > 0) {
      pSprings[i] -= 1;
    }
  }
}


/*-----------------------
    setAllOneSpring
  ----------------------*/
void setOddSpring() {
  for (int i = 0; i < 16; i++) {
    if (i % 2 == 0) {
      pSprings[i] = 0;
    } else {
      pSprings[i] = 1;
    }
  }
}


/*-------------------------
    collisionSpring
  ------------------------ */
void collisionSpring(bool &is_miss)
{
  sound.tone(NOTE_A1, 80);

  is_miss = true;
  for (int i = 0; i < 16; i++)
  {
    //
    int16_t x = pScroll - (i * 8);
    if (x < 0) {
      x = 128 + x;
    }
    x+=4;
    if (pX - 4 <= x && x <= pX + 12 && pSprings[i] > 0) {
      pSprings[i] -= 1;
      is_miss = false;
      break;
    }
  }

  // 全部なくなったらリセット
  int s = 0;
  for (int i = 0; i < 16; i++) {
    s += pSprings[i];
  }
  if (s == 0) {
    initSpring();
  }
}

/*-----------------------
    moveSpring
  ----------------------*/
void moveSpring()
{
////  pScroll--;
  pScroll-=pScrollSpeed;
  if (pScroll < 0) {
    pScroll = 127;
  }
}

/*-----------------------
    initSpeedScroll
  ----------------------*/
void initSpeedScroll()
{
  pScrollSpeed = 1;
}

/*-----------------------
    speedupScroll
  ----------------------*/
void speedupScroll()
{
  pScrollSpeed++;
  if (pScrollSpeed > 5){
    pScrollSpeed = 1;
  }
}

/*-----------------------
    drawSpring
  ----------------------*/
void drawSpring()
{
  arduboy.fillRect(8, 56, 127, 64, 1);
  for (int i = 0; i < 16; i++)
  {
    //
    int16_t x = pScroll - (i * 8);
    if (x < 0) {
      x = 128 + x;
    }

    switch ( pSprings[i] )
    {
      case 0:
        arduboy.drawSlowXYBitmap(x, 56, bEmpty, 8, 8, 0);
        break;
      case 1:
        arduboy.drawSlowXYBitmap(x, 56, bSpring1, 8, 8, 0);
        break;
      case 2:
        arduboy.drawSlowXYBitmap(x, 56, bSpring2, 8, 8, 0);
        break;
      case 3:
        arduboy.drawSlowXYBitmap(x, 56, bSpring3, 8, 8, 0);
        break;
      case 4:
        arduboy.drawSlowXYBitmap(x, 56, bSpring4, 8, 8, 0);
        break;
      default:
        arduboy.drawSlowXYBitmap(x, 56, bEmpty, 8, 8, 0);
        break;
    }
  }
}
