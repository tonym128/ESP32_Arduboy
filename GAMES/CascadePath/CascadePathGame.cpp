#include <Arduboy2.h>
#include "DefinesImagesAndSounds.h"
#include "CascadePathGame.h"



CascadePathGame::CascadePathGame( Arduboy2 * display ) {
  Display = display;
  LastButtonPress = 0;
}
void CascadePathGame::Begin( ) {
  delay(1000);
  Location = 0;
  Distance = 0;
  Day = 1;
  Year = 1848;
  Pace = 0;
  Ration = 0;
  for (uint8_t a = 0; a < NumSettlers; a++) {
    Settlers[a].health = 100;
    Settlers[a].status = 0;
  }
  Display->clear();
  DrawTitleScreen();
  unsigned long CurrentTime = millis();
  uint8_t Dots = 0;
  while ( true ) {
    delay(0);
    Display->display();
    if ( Display->buttonsState() ) break;
    if ( millis() > CurrentTime + 750 ) {
      CurrentTime = millis();
      if ( Dots == 22 ) {
        Display->clear();
        DrawTitleScreen();
        Dots = 0;
      }
      else {
        Display->drawRect( TitleCoords[Dots], TitleCoords[Dots+1], 2, 2, WHITE );
        Dots+=2;
      }
    }
  }
  DistanceToNextLandmark = Distances[ Location ];
  GameMenu( 9, 0, 2 );
  GameState = CPGTravel;
}
void CascadePathGame::DrawTitleScreen( ) {
  Display->drawFastHLine( 2, 4,   6, WHITE);
  Display->drawFastHLine(19, 4, 105, WHITE);
  Display->drawFastHLine( 2,62, 124, WHITE);
  Display->drawFastVLine( 1, 5, 57, WHITE);
  Display->drawFastVLine(126, 5, 57, WHITE);
  DrawStripedBitmap( 0, 0, TitleSprite );  
}
void CascadePathGame::Cycle( ) {
  switch ( GameState ) {
    case CPGTravel: 
      Travel();
      break;
    case CPGLandmark:
      Landmark();
      break;
    case CPGHunt:
      Hunt();
      break;
  }
}

void CascadePathGame::MenuDraw( uint8_t y, uint8_t h, uint8_t scrollIndex, uint8_t start, uint8_t menuMax ) {
  BlackoutBox( y, h );
  for (uint8_t a = scrollIndex; a < menuMax; a++) {
    LoadStringBuffer(MenuStrings, start+a);
    PrintBufferAt( 20, y+4+((a-scrollIndex)*8) );
  }
}
int16_t  CascadePathGame::GameMenu( int16_t y, uint8_t ArrayIndex, uint8_t Selections ) {
  int16_t index = 0;
  uint8_t height = (Selections+1) * 8;
  height = min(height,47);
  bool scrolling = false;
  uint8_t osc = 0;
  if (Selections > 5 ) scrolling = true;
  ActivateButtonCD();
  while ( true ) {
    delay(0);
    int16_t scrollIndex = 0;
    if ( pressedCD( UP_BUTTON ) ) {
      if ( index == 0 ) index = Selections-1;
      else index--;
    }
    if ( pressedCD( DOWN_BUTTON ) ) {
      if ( index == Selections-1 ) index = 0;
      else index++;
    }
    if ( index > 2 && scrolling ) {
      scrollIndex = index - 2;
      if (scrollIndex > Selections-5) scrollIndex = Selections - 5;
    }
    uint8_t cursorY = 4+y+((index-scrollIndex)*8);
    if ( pressedCD( A_BUTTON ) ) {
      LoadStringBuffer(MenuStrings, ArrayIndex+index);
      while (osc < 6) {
        if (osc % 2) PrintBufferAt( 20, cursorY );
        else Display->fillRect(20,cursorY,100,8,0);
        Display->display();
        osc++;
        delay(75);
      }
      int16_t MenuIndex = index+ArrayIndex;
      switch ( MenuIndex ) {
        // New Game Menu
        case 0: QuickNewGame(); break;
        case 1: CustomNewGame(); break;

        // Profession Selection
        case 2: //Profession = 1; Money = 1200; break; // Doctor
        case 3: //Profession = 2; Money = 800; break; // Carpenter
        case 4: Profession = MenuIndex - 1; Money = 1600 - (400*Profession); break; // Teacher

        // Main Menu
        case 5: GameState = CPGHunt; break; // Go Hunting
        case 6: /*SubMenuAnimate( 6, 9, 32 );*/ GameMenu( 9, 12, 3 ); break; // Change Pace
        case 7: /*SubMenuAnimate( 6, 9, 32 );*/ GameMenu( 9, 15, 3 ); break; // Change Rations
        case 8: break; // Group Status
        case 9: Rest(); break; // Rest
        case 10: break; // Map
        
        case 30:
        case 11: break; // Exit Menu

        // Pace Menu
        case 12: //Pace = 0; break; // Steady Pace
        case 13: //Pace = 1; break; // Strenuous Pace
        case 14: Pace = MenuIndex - 12; break; // Grueling Pace

        // Rationing Menu
        case 15: //Ration = 0; break; // Filling
        case 16: //Ration = 1; break; // Meager
        case 17: Ration = MenuIndex - 15; break; // Bare Bones

        // River Menu 18-20 return values
        case 21: case 24: case 32: GameMenu( 9, 5, 7 ); break; // Main Menu

        // Landmark Menu
        case 22: case 31: NextLocation(); break;
        case 23: ShopMenu(); break; // menushop
        
        // Shop Menu
        case 25: //BuyDialog( CPGOxen ); PatternWipe(); break;
        case 26: //BuyDialog( CPGClothing ); PatternWipe(); break;
        case 27: //BuyDialog( CPGParts ); PatternWipe(); break;
        case 28: //BuyDialog( CPGBullets ); PatternWipe(); break;
        case 29: BuyDialog( MenuIndex - 25 ); PatternWipe(); break;
      }
      return ( MenuIndex );
    }    
    MenuDraw( y, height, scrollIndex, ArrayIndex, min(Selections,scrollIndex+5) );
    SetAndPrint( 10, cursorY, F(">") );
    Display->display();
  }
/*  
  Display->fillRect( x, y, 115, (Selections+1)*8, BLACK );
  Display->drawRect( x, y, 115, (Selections+1)*8, WHITE );
  for ( uint8_t a = 0; a < Selections; a++ ) {
    strcpy_P(StringBuffer, (char*)pgm_read_word(&(MenuStrings[ArrayIndex + a])));
    Display->setCursor( x+14, y+4+(a*8) );
    Display->print(StringBuffer);
  }
  int16_t Selection = ArrayIndex + MenuCursor( x+6, y+4, Selections );
  strcpy_P( StringBuffer, (char*)pgm_read_word(&(MenuStrings[Selection])) );
  uint8_t osc = 0;
*/
}
void CascadePathGame::MainMenu( ) {
  DrawNextStop();
  DrawDate(0);
  GameMenu( 9, 5, 7 );
}
void CascadePathGame::ShopMenu( ) {
  DrawLocation(0);
  GameMenu( 9, 25, 6 );
}
void CascadePathGame::BuyDialog( uint8_t InventorySlot ) {
  unsigned long BlinkTime = 0;
  int16_t value = 0;
  bool underscore = true;
  int16_t costPer = Prices[InventorySlot];
  int16_t maxValue = Maxes[InventorySlot] - Inventory[InventorySlot];
  int16_t maxBuy = Money / costPer;
  maxValue = min(maxValue, maxBuy);
  ActivateButtonCD();
  while (true) {
    delay(0);
    if ( millis() > BlinkTime + BlinkCD ) {
      BlinkTime = millis();
      underscore = !underscore;
    }
    if ( pressedCD( UP_BUTTON ) ) {
      if ( value == maxValue ) value = 0;
      else value++;
    }
    if ( pressedCD( DOWN_BUTTON ) ) {
      if ( value == 0 ) value = maxValue;
      else value--;
    }
    if ( pressedCD( A_BUTTON ) || pressedCD( B_BUTTON ) ) {
      Money = Money - ( value * costPer );
      Inventory[InventorySlot] = Inventory[InventorySlot] + value;
      return;
    }
    int16_t cost = costPer * value;
    int16_t balance = Money - cost;
    Display->fillRect(0,9,128,55,BLACK);
    Display->drawFastHLine(0,9,127,WHITE);
    LoadStringBuffer(BuyStrings, InventorySlot);
    PrintBufferAt( 0, 14 );
    SetAndPrint( 0, 30, F("Balance        $    \n   @ $         -\nNew Balance:   $") );
    SetAndPrint( ShopOffset[InventorySlot]+offset(4,Inventory[InventorySlot]), 14, Inventory[InventorySlot] );
    SetAndPrint( 100+offset(4,Money), 30, Money);
    SetAndPrint( 0, 38, value );
    SetAndPrint( 36, 38, costPer );
    SetAndPrint( 100+offset(4,cost), 38, cost );
    SetAndPrint( 100+offset(4,balance), 46, balance);
    Display->drawFastHLine(0,45,18-offset( 3, maxValue ),underscore);
		Display->display();
  }
}
/*void CascadePathGame::SubMenuAnimate(uint8_t x, uint8_t y, uint8_t h) {
  // Todo: needs to clear last frame of animation box before loading next menu
  for (uint8_t a = 127; a > x; a-=4) {
    if (a < 12) { Display->fillRect(115,y,13,h,0); }
    Display->fillRect(a,y,116,h,0);
    Display->drawRect(a,y,115,h,1);
    Display->display();
  }
}*/

int16_t  CascadePathGame::offset ( uint8_t digits, int16_t value ) {
	uint8_t width = digits * 6;
	if (value > 999) return width - 24;
	if (value > 99) return width - 18;
	if (value > 9) return width - 12;
	return width - 6;
}
void CascadePathGame::WaitForTap( ) {
  delay(500);
  Display->display();
  ActivateButtonCD();
  while ( true ) {
    delay(0);
    if ( ButtonOffCD() && Display->buttonsState() ) return;
  }
}


void CascadePathGame::LoadStringBuffer( const unsigned char * const * TargetArray, int16_t Index ) {
  strcpy_P( StringBuffer, (char*) pgm_read_ptr( &(TargetArray[Index]) ) );
}


void CascadePathGame::PrintBufferAt( int16_t x, int16_t y ) {
  Display->setCursor(x,y);
  Display->print( StringBuffer );
}


int16_t  CascadePathGame::SizeOfBuffer( ) {
  for ( uint8_t a = 0; a < StringBufferSize; a++ ) {
    if ( !StringBuffer[a] ) return a;
  }
  return StringBufferSize;
}


void CascadePathGame::QuickNewGame( ) {
  Money = 1200;
  Profession = 1;
  Month = 2;
  strcpy(Settlers[0].name,"Homer");
  strcpy(Settlers[1].name,"Marge");
  strcpy(Settlers[2].name,"Bart");
  strcpy(Settlers[3].name,"Lisa");
  Inventory[ CPGOxen ] = 20;
  Pace = 2;
  Inventory[ CPGClothing ] = 8;
  Inventory[ CPGParts ] = 5;
  Inventory[ CPGBullets ] = 1;
  Inventory[ CPGFood ] = 200;
}
void CascadePathGame::CustomNewGame( ) {
  LongTextSetup();
  Display->print(F("You're about to begin\n  a great adventure,\ntraveling the Cascade\n   Path across the\n rugged landscape of\n North America. Your\ncovered wagon, pulled"));
  WaitForTap();
	
  LongTextSetup();
  Display->print(F("  by a team of oxen,\n  will travel from\nIndependence,Missouri\n    to the fertile\n  Willamette Valley\n    of the Oregon\nTerritory--a journey"));
  WaitForTap();
	
  LongTextSetup();
  SetAndPrint( 15, 26, F("of approximately\n     2,000 miles.") );
  WaitForTap();

  Display->clear();
  SetAndPrint( 7, 5, F("Choose a profession") );
  GameMenu( 18, 2, 3 );

  Display->clear();
  SetAndPrint( 7, 5, F("Enter your name") );
  NameEntry(18,13,0);

  SetAndPrint( 7, 29, F("What month will you") );
  SetAndPrint( 7, 37, F("leave?") );
  MonthEntry();

  for (uint8_t a = 0; a < 5; a++) {
    Display->clear();
    SetAndPrint( 15, 0, F("Independence, MO") );
    BuyDialog(a);
    PatternWipe();
  }
}
void CascadePathGame::NameEntry( int16_t x, int16_t y, uint8_t index) {
  long BlinkTime = millis();
  uint8_t charIndex = 0;
  bool underscore = false;
  strcpy( Settlers[index].name, "A       " );
  ActivateButtonCD();
  while (true) {
    delay(0);
    if ( millis() > BlinkTime + BlinkCD ) { BlinkTime = millis(); underscore = !underscore; }
    if ( pressedCD( UP_BUTTON ) ) {
      if (charIndex == 0) {
        if (Settlers[index].name[charIndex] == 32) Settlers[index].name[charIndex] = 90;
        else if (Settlers[index].name[charIndex] < 66) Settlers[index].name[charIndex] = 32;
        else Settlers[index].name[charIndex]--;
      }
      else {
        if (Settlers[index].name[charIndex] == 32) Settlers[index].name[charIndex] = 122;
        else if (Settlers[index].name[charIndex] < 98) Settlers[index].name[charIndex] = 32;
        else Settlers[index].name[charIndex]--;
      }
    }
    if ( pressedCD( DOWN_BUTTON ) ) {
      if (charIndex == 0) {
        if (Settlers[index].name[charIndex] == 32) Settlers[index].name[charIndex] = 65;
        else if (Settlers[index].name[charIndex] > 89) Settlers[index].name[charIndex] = 32;
        else Settlers[index].name[charIndex]++;
      }
      else {
        if (Settlers[index].name[charIndex] == 32) Settlers[index].name[charIndex] = 97;
        else if (Settlers[index].name[charIndex] > 121) Settlers[index].name[charIndex] = 32;
        else Settlers[index].name[charIndex]++;
      }
    }
    if ( pressedCD( LEFT_BUTTON ) ) {
      if (charIndex == 0) charIndex = 7;
      else charIndex--;
    }
    if ( pressedCD( RIGHT_BUTTON ) ) {
      if ( charIndex == 7 ) charIndex = 0;
      else charIndex++;
    }
    if ( pressedCD( A_BUTTON ) || pressedCD( B_BUTTON ) ) {
      uint8_t osc = 0;
      while (osc < 6) {
        if (osc % 2) {
          Display->setCursor(x,y);
          Display->print(Settlers[index].name);
        }
        else Display->fillRect(x,y,48,9,BLACK);
        Display->display();
        osc++;
        delay(75);
      }
      return;
    }
    Display->fillRect(x,y,48,9,0);
    Display->setCursor(x,y);
    Display->print(Settlers[index].name);
    Display->drawFastHLine(x+(charIndex*6),y+8,5,underscore);
    Display->display();
  }
}
void CascadePathGame::MonthEntry( ) {
  int16_t x = 18;
  int16_t y = 45;
  while ( true ) {
    delay(0);
    LoadStringBuffer(strMonths, Month);
    if ( pressedCD( LEFT_BUTTON ) || pressedCD( UP_BUTTON ) ) {
      ActivateButtonCD();
      if ( Month == 0 ) Month = 11;
      else Month--;
    }
    if ( pressedCD( RIGHT_BUTTON ) || pressedCD( DOWN_BUTTON) ) {
      if ( Month == 11 ) Month = 0;
      else Month++;
    }
    if ( pressedCD( A_BUTTON ) || pressedCD( B_BUTTON ) ) {
      uint8_t osc = 0;
      while ( osc < 6 ) {
        if ( osc % 2 ) PrintBufferAt( x, y );
        else Display->fillRect(x,y,54,8,BLACK);
        Display->display();
        delay(75);
        osc++;
      }
      return;
    }
    Display->fillRect(x,y,54,8,BLACK);
    PrintBufferAt( x, y );
    Display->display();
  }
}
void CascadePathGame::LongTextSetup( ) {
  Display->clear();
  Display->setCursor(0,4);
  Display->drawLine(0,1,127,1,1);
  Display->drawLine(0,62,127,62,1);
}

void CascadePathGame::Travel( ) {
  unsigned long LastMove = millis();
  unsigned long oscTimer = LastMove;
  bool osc = false;
  uint8_t hour = 0;
  uint8_t mountainOffset = 0;
  uint8_t grassOffset = 0;
	for (uint8_t a = 16; a > 0; a-=2) {
		TravelDraw(osc,mountainOffset,grassOffset);
		FadeIn(a);
		Display->display();
	}
  if ( DistanceToNextLandmark == Distances[Location] ) {
    TravelDraw(osc,mountainOffset,grassOffset);
    BlackoutBox( 18, 24 );
    SetAndPrint( 9, 22, F("It is ") );
    Display->print( Distances[Location] );
    Display->print(F(" miles to"));
    LoadStringBuffer( locationData, Location );
    PrintBufferAt( 9, 30 );
    WaitForTap( );
  }
  ActivateButtonCD();
  WeatherMachine();
  while ( GameState == CPGTravel ) {
    delay(0);
    unsigned long CurrentTime = millis();
    if ( CurrentTime > oscTimer + BlinkCD ) {
      oscTimer = CurrentTime;
      osc = !osc;
    }
    if ( ButtonOffCD() ) {
      if ( Display->pressed( A_BUTTON ) || Display->pressed( B_BUTTON ) ) {
        MainMenu();
        if ( GameState != CPGTravel ) return;
      }
    }
    if ( CurrentTime > LastMove + 100 ) {
      LastMove = CurrentTime;
      hour++;
      grassOffset += 2;
      if ( hour > 23 ) {
        hour = 0;
        IncDate( false );
      }
      if ( hour % 6 == 0 ) mountainOffset++;
      if ( mountainOffset > 127 ) mountainOffset = 0;
      if ( grassOffset > 127 ) grassOffset = 0;
      int16_t MaxDistance;
      if ( Location < 5 ) MaxDistance = 40;
      else MaxDistance = 24;
      float hourDistance = MaxDistance * ( .50 + ( ( float ) Pace / 4 ) ) * ( (float) Inventory[CPGOxen] / Maxes[CPGOxen] );
      hourDistance = hourDistance / 24;
      if (hourDistance > DistanceToNextLandmark) {
        Distance += DistanceToNextLandmark;
        DistanceToNextLandmark = 0;
        GameState = CPGLandmark;
        PatternWipe();
        return;
      }
      else {
        Distance += hourDistance;
        DistanceToNextLandmark -= hourDistance;
      }
      Display->clear();
      TravelDraw( osc, mountainOffset, grassOffset );
      Display->display();
    }
  }
}


void CascadePathGame::TravelDraw( bool osc, uint8_t mountainOffset, uint8_t grassOffset ) {
  Display->drawBitmap(grassOffset,     45, mountains, 128, 16, 1);
  Display->drawBitmap(grassOffset-128, 45, mountains, 128, 16, 1);
  for (uint8_t a = 0; a < 128; a++) Display->drawBitmap( a, 45, grassMask, 1, 16, BLACK );
  DrawNextStop();
  DrawDate(0);
  Display->drawBitmap(64,25,wgtop,63,16,WHITE);
  if (osc)  Display->drawBitmap(64,41,wgbottom2,59,8,WHITE);
  else      Display->drawBitmap(64,41,wgbottom1,59,8,WHITE);
  Display->drawBitmap(mountainOffset,    10,mountains,128,16,1);
  Display->drawBitmap(mountainOffset-128,10,mountains,128,16,1);
  if (DistanceToNextLandmark < 64) {
    DrawStripedBitmap( 64 - pgm_read_byte(&(locationImagesDimensions[Location*4])) - DistanceToNextLandmark,
                       48 - pgm_read_byte(&(locationImagesDimensions[(Location*4)+1])),
                       (const uint8_t*)pgm_read_ptr(&(locationImages[Location])) );
  }
}


void CascadePathGame::DrawNextStop( ) {
  SetAndPrint( 0, 56, F("Next Stop:      Miles") );
  SetAndPrint( 75+offset(3,DistanceToNextLandmark), 56, (int) DistanceToNextLandmark );
}
void CascadePathGame::DrawDate( uint8_t y ) {
  LoadStringBuffer(strMonths, Month);
  uint8_t x = (SizeOfBuffer()+8)*6;
  if (Day > 9) x += 6;
  x = (128-x) / 2;
  PrintBufferAt( x, y );
  Display->print(F(" "));
  Display->print(Day);
  Display->print(F(", "));
  Display->print(Year);
}
void CascadePathGame::IncDate( bool rest ) {
  Day++;
  if ( Day > Days[Month] ) {
    Day = 1;
    Month++;
    if ( Month > 11 ) {
      Month = 0;
      Year++;
    }
  }
  WeatherMachine();
  int16_t Weathering = 7 - Weather - ( Inventory[ CPGClothing ] / AliveSettlers() );
  int16_t Damage = max( 0, Weathering );
  if ( !rest ) Damage += Pace * 2;
  DamageSettlers( Damage );
  FeedSettlers( );
}
int16_t  CascadePathGame::AliveSettlers( ) {
  int16_t count = 0;
  for ( uint8_t a = 0; a < NumSettlers; a++ )
    if ( Settlers[a].health > 0 ) count++;
  return count;
}
void CascadePathGame::DamageSettlers( int16_t Damage ) {
  for ( uint8_t a = 0; a < NumSettlers; a++ ) {
    if ( Settlers[a].health > Damage ) {
      Settlers[a].health -= Damage;
    }
    else {
      if ( Settlers[a].status ) {
        // Kill A
      }
      else {
        // Roll to injure A
      }
    }
  }
}
void CascadePathGame::FeedSettlers() {
  for ( uint8_t a = 0; a < NumSettlers; a++ ) {
    if ( Settlers[a].health ) {
      for ( int16_t b = 0; b < 3-Ration; b++ ) {
        if ( Inventory[CPGFood] ) {
          Settlers[a].health += 5;
          Inventory[CPGFood]--;
        }
      }
    }
  }
}

void CascadePathGame::Landmark( ) {
  Display->clear();
  DrawLocation(0);
  DrawDate(9);
  Display->drawFastHLine(0,18,128,1);
  if ( LocationType[Location] ) DrawStripedBitmap( 64 - ( pgm_read_byte(&(locationImagesDimensions[Location*4])) / 2 ),
                                                   48 - pgm_read_byte(&(locationImagesDimensions[(Location*4)+1])),
                                                   (const uint8_t*)pgm_read_ptr(&(locationImages[Location])) );
  switch ( LocationType[Location] ) {
    case 0: // river
      RiverDialog();
      break;
    case 1: // Fort Options (18,21)
      WaitForTap();
      GameMenu( 18, 22, 3 );
      break;
    case 2: // Landmark Options (21,23)
    case 3: // Supposed to be dalles
      WaitForTap();
      GameMenu( 18, 31, 2);
      break;
  }
}
void CascadePathGame::RiverDialog( ) {
  float rh;
  int16_t rw;
  switch ( Location ) {
    case 0:  rh = 4;  rw = 620;  break;
    case 1:  rh = 2;  rw = 225;  break;
    case 7:  rh = 20; rw = 400;  break;
    case 10: rh = 6;  rw = 1000; break;
  }

  float RiverHeight = rh - ( ( rh / 16 ) * random(Weather) * 2 ) + random( (rh/2) + 1 );
  int16_t   RiverWidth  = rw + ( ( RiverHeight - rh ) * ( rw / 20 ) );
  SetAndPrint( 0, 22, F("Weather: ") );
  LoadStringBuffer( WeatherStrings, Weather );
  Display->print( StringBuffer ); 
  Display->print(F("\nRiver width: "));
  Display->print( RiverWidth );
  Display->print(F("ft\nRiver depth: "));
  Display->print(RiverHeight);
  Display->print(F("ft"));
  WaitForTap();
  int16_t action = GameMenu(18,18,4);
  if ( action != 21 ) RiverCrossing( action, RiverHeight );
}
void CascadePathGame::RiverCrossing( uint8_t action, float riverHeight ) {
  randomSeed( millis() );
  int16_t frame = 0;
  while ( frame < 90 ) {
    Display->clear();
    for (uint8_t x = 0; x < 9; x++) {
      for (uint8_t y = 0; y < 5; y++) {
        Display->drawBitmap( ((x-1)*16) + (frame%16), ((y-1)*16) + (frame%16), watertile,16,16,WHITE);
      }
    }
    // Cheap triangles
    uint8_t LoopOneFrames	= 30;
    uint8_t LoopTwoFrames	= 60;
    if ( frame < LoopOneFrames ) {
      for ( int16_t y = 63 - (LoopOneFrames-frame), x = 127; y < 64; y++, x-=2 ) {
        Display->drawFastVLine( x,  y, 64, WHITE );
        Display->drawFastVLine(x-1, y, 64, WHITE );
      }
    }
    if ( frame > LoopTwoFrames ) {
      for ( int16_t y = (frame-LoopTwoFrames), x=0; y >= 0; y--, x+=2 ) {
        Display->drawFastVLine( x,  0, y, WHITE);
        Display->drawFastVLine(x+1, 0, y, WHITE);
      }
    }
    if ( frame == 45 ) {
      if ( action == 18 ) {
        if ( riverHeight > 3 ) { RiverCrash( riverHeight ); return; }
        else if ( riverHeight > 2 ) {
          if ( !random(20) ) { RiverCrash( riverHeight ); return; }
        }
      }
      if ( action == 19 ) {
        if ( riverHeight < 2) { RiverCrash( riverHeight ); return; }
        else if ( !random(3) ) { RiverCrash( riverHeight ); return; }
      }
    }
    switch (action) {
      case 18: DrawStripedBitmap( 0,0,FordRiverSprite ); break;
      case 19: DrawStripedBitmap( 0,0,CaulkRiverSprite ); break;
      case 20: DrawStripedBitmap( 0,0,FerryRiverSprite ); break;
    }
    Display->display();
    frame++;
    delay(50);
  }
  PatternWipe();
  NextLocation();
}
void CascadePathGame::RiverCrash( float riverHeight ) {
  //Display->drawBitmap(37,16,wagoncrash,53,32,WHITE );
  DrawStripedBitmap( 0,0,WagonCrashSprite );
  Display->display();
  delay(1500);
  uint8_t loss = 1;
  if (riverHeight > 6) loss++;
  if (riverHeight > 9) loss++;
  uint8_t y = (60-(8*(loss+2)))/2;
  BlackoutBox( y, 20 + ( loss*8 ) );
  SetAndPrint( 9, y+2,  F("Your wagon sunk.") );
  SetAndPrint( 9, y+10, F("You lost:") );
  for (uint8_t a = 0; a < loss; a++) {
    Display->setCursor(9,y+18+(a*8));
    RiverLoss();
  }
  WaitForTap();
  PatternWipe();
  NextLocation();
}
void CascadePathGame::RiverLoss( ) {
  while (true) {
    delay(0);
    uint8_t i = random(21);
    uint8_t index = i%5;
    if ( i == 20 ) {
      i = random(1,4);
      if (Settlers[i].health) {
        Settlers[i].health = 0;
        Display->print(Settlers[i].name);
        Display->print(F(" died."));
/*
      switch (i) {
        // case 0: display.print("You died."); break;
        case 1: display.print("Your spouse died."); break;
        case 2: display.print("Your son died."); break;
        case 3: display.print("Your daughter died."); break;
      }
*/
        return;
      }
    }
    else if ( Inventory[ index ] ) {
      uint8_t loss = random( Inventory[ index ] / 2 ) + 1;
      Inventory[ index ] -= loss;
      Display->print( loss );
      LoadStringBuffer( LossStrings, index );
      Display->print( StringBuffer );
      return;
    }
  }
}
void CascadePathGame::DrawLocation( uint8_t y ) {
  LoadStringBuffer(locationData, Location);
  uint8_t x = SizeOfBuffer() * 6; // LocationLen[Location] * 6;
  x = (128-x) / 2;
  PrintBufferAt( x, y );
}
void CascadePathGame::NextLocation( ) {
  Location++;
  DistanceToNextLandmark = Distances[Location];
  GameState = CPGTravel;
}
void CascadePathGame::WeatherMachine( ) {
  uint8_t Summering;
  randomSeed( Day );
  if ( Month < 6 ) Summering = Month;
  else Summering = 11 - Month;
  Weather = random( Summering , Summering+2 );
}

void CascadePathGame::Hunt( ) {
  if ( !DistanceToNextLandmark ) {
    BlackoutBox( 18, 24 );
    SetAndPrint( 19, 22, F("You cannot hunt") );
    SetAndPrint( 49, 30, F("here.") );
    WaitForTap();
    GameState = CPGLandmark;
    return;
  }
  int16_t Ammo = 0;
  unsigned long CurrentTime = millis();
  unsigned long LastSpawn = CurrentTime;
  randomSeed( CurrentTime );
  
  CPGSprite * Player  = new CPGSprite( 64, 32, hunter, CPGPlayer, CPGRight );
  for ( uint8_t a = 0; a < random(4); a++ ) {
    if ( random(2) ) Player->AddSprite( random(WIDTH), random(HEIGHT), HuntingGrass, CPGPlant, CPGNoface);
    else Player->AddSprite( random(WIDTH), random(HEIGHT), HuntingTree, CPGPlant, CPGNoface);
  }
  while ( GameState == CPGHunt ) {
    delay(0);
    if ( millis() > CurrentTime + (1000/30) ) {
      CurrentTime = millis();
      if ( CurrentTime > LastSpawn + 30000 || !random( (30000-(CurrentTime-LastSpawn))/100 ) ) {
        uint8_t x, y, type, facing = random(1,5);
        const unsigned char * tile;
        switch ( facing ) {
          case CPGUp:    x = random(WIDTH); y = 63;
                         tile = squirrel; type = CPGSquirrel; break;
          case CPGDown:  x = random(WIDTH); y = 0;
                         tile = squirrel; type = CPGSquirrel; break;
          case CPGLeft:  x = 127; y = random(HEIGHT-16);
                         tile = buffalo; type = CPGBuffalo; break;
          case CPGRight: x = 0; y = random(HEIGHT-16);
                         tile = deer; type = CPGDeer; break;
        }
        Player->AddSprite( x, y, tile, type, facing );
        LastSpawn = CurrentTime;
      }
      if ( Display->pressed( UP_BUTTON ) )    { Player->y--; Player->facing = CPGUp; }
      if ( Display->pressed( DOWN_BUTTON ) )  { Player->y++; Player->facing = CPGDown; }
      if ( Display->pressed( LEFT_BUTTON ) )  { Player->x--; Player->facing = CPGLeft; }
      if ( Display->pressed( RIGHT_BUTTON ) ) { Player->x++; Player->facing = CPGRight; }
      if ( pressedCD( A_BUTTON ) ) {
        if ( !Ammo ) {
          if ( Inventory[CPGBullets] ) {
            Inventory[CPGBullets]--;
            Ammo = 20;
          }
        }
        if ( Ammo ) {
          Player->AddSprite( Player->x + (Player->w/2), Player->y + (Player->h/2), bullet, CPGBullet, Player->facing );
          Ammo--;
        }
      }
      Player->CycleNext( Player );
      Display->clear();
      for ( CPGSprite * p = Player; p; p = p->next ) p->Draw( Display );
      SetAndPrint(0, 0, F("Ammo:") );
      SetAndPrint(30, 0, Inventory[CPGBullets]*20+Ammo );
      Display->display();
      if ( Player->IsOffscreen() ) GameState = CPGTravel;
    }
  }
  BlackoutBox( 18, 24 );
  SetAndPrint( 19, 22, F("You shot ") );
  Display->print( (int) Player->meat );
  SetAndPrint( 19, 30, F("lbs of food.") );
  WaitForTap();
  if ( Player->meat > 200 ) {
    BlackoutBox( 18, 24 );
    SetAndPrint( 19, 22, F("You can only") );
    SetAndPrint( 19, 30, F("carry 200 back.") );
    WaitForTap();
  }
  Inventory[CPGFood] += min(200, Player->meat);
  if ( Inventory[CPGFood] > Maxes[CPGFood] ) Inventory[CPGFood] = Maxes[CPGFood];
  delete Player;
}

void CascadePathGame::Rest( ) {
  IncDate( true );
}

void CascadePathGame::FadeIn( uint8_t pMax ) {
	for (uint8_t pCount = 0; pCount < pMax; pCount++) {
		uint8_t patternX = patternCoords[pCount*2];
		uint8_t patternY = patternCoords[(pCount*2)+1];
		for (uint8_t x = patternX; x < 128; x+=4) {
			for (uint8_t y = patternY; y < 64; y+=4) {
				Display->drawPixel(x,y,BLACK);
			}
		}
	}
}
void CascadePathGame::PatternWipe( ) {
  for (uint8_t pCount = 0; pCount < 16; pCount++) {
    uint8_t patternX = patternCoords[pCount*2];
    uint8_t patternY = patternCoords[(pCount*2)+1];
    for (uint8_t x = patternX; x < 128; x+=4) {
      for (uint8_t y = patternY; y < 64; y+=4) {
        Display->drawPixel(x,y,BLACK);
      }
    }
    Display->display();
  }
}
void CascadePathGame::BlackoutBox( int16_t y, int16_t h ) {
  Display->fillRect ( 6, y, 115, h, BLACK );
  Display->drawRect ( 6, y, 115, h, WHITE );
}
void CascadePathGame::SetAndPrint( int16_t x, int16_t y, const __FlashStringHelper* text ) {
  Display->setCursor( x, y );
  Display->print(text);
}
void CascadePathGame::SetAndPrint( int16_t x, int16_t y, int16_t i ) {
  Display->setCursor( x, y );
  Display->print( i );
}
void CascadePathGame::DrawStripedBitmap( int16_t tX, int16_t tY, const uint8_t * address ) {
  uint8_t masks = pgm_read_byte( address++ );
  for ( uint8_t a = 0; a < masks; a++ ) {
    int16_t x = tX + pgm_read_byte( address++ );
    int16_t y = tY + pgm_read_byte( address++ );
    uint8_t w = pgm_read_byte( address++ );
    uint8_t h = pgm_read_byte( address++ );    
    Display->fillRect( x, y, w, h, BLACK );
  }
  uint8_t stripes = pgm_read_byte( address++ );
  for ( uint8_t a = 0; a < stripes; a++ ) {
    int16_t x = tX + pgm_read_byte( address++ );
    int16_t y = tY + pgm_read_byte( address++ );
    uint8_t w = pgm_read_byte( address++ );
    uint8_t h = pgm_read_byte( address++ );
    Display->drawBitmap( x, y, address, w, h, WHITE );
    address += ( w * h ) / 8;
  }
}

void CascadePathGame::ProcessButtons( ) {
}
void CascadePathGame::ButtonPress( uint8_t pButton ) {
  if ( ButtonState[ pButton ] ) return;
  ButtonState[ pButton ] = true;
}
void CascadePathGame::ButtonRelease( uint8_t pButton ) {
  if ( !ButtonState[ pButton ] ) return;
  ButtonState[ pButton ] = false;
}
bool CascadePathGame::ButtonOffCD( ) {
  if ( millis() > LastButtonPress + ButtonCD ) return true;
  return false;
}
void CascadePathGame::ActivateButtonCD( ) {
  LastButtonPress = millis();
}

bool CascadePathGame::pressedCD( uint8_t buttons ) {
  if ( !ButtonOffCD() ) return false;
  if ( ( Display->buttonsState() & buttons ) == buttons ) {
    ActivateButtonCD( );
    return true;
  }
  return false;
}

CPGSprite::CPGSprite( int16_t tx, int16_t ty, const unsigned char * ttile, byte ttype, byte tfacing ) {
  frame = 0;
  x = tx;
  y = ty;
  w = pgm_read_byte(ttile++);
  h = pgm_read_byte(ttile++);
  speed = pgm_read_byte(ttile++);
  maxFrame = pgm_read_byte(ttile++);
  tile = ttile;
  facing = tfacing;
  type = ttype;
  switch ( type ) {
    case CPGSquirrel: meat = random( 1, 5); break;
    case CPGDeer:     meat = random(40,60); break;
    case CPGBuffalo:  meat = random(200,300); break;
    default: meat = 0;
  }
  next = NULL;
}
void CPGSprite::AddSprite( int16_t tx, int16_t ty, const unsigned char * ttile, byte ttype, byte tfacing ) {
  if ( next ) next->AddSprite(tx,ty,ttile,ttype,tfacing);
  else        next = new CPGSprite(tx,ty,ttile,ttype,tfacing);
}


bool CPGSprite::CycleNext( CPGSprite * First ) {
  if ( !next ) return false;
  if ( next->type != CPGPlant ) {
    switch ( next->facing ) {
      case CPGNoface: break;
      case CPGUp:     next->y -= next->speed; break;
      case CPGDown:   next->y += next->speed; break;
      case CPGLeft:   next->x -= next->speed; break;
      case CPGRight:  next->x += next->speed; break;
    }
  }
  if ( next->type == CPGBullet ) {
    for ( CPGSprite *p = First; p->next; p = p->next ) {
      if ( p->next->type > CPGAnimal ) { 
        if ( p->next->IsIn( next->x, next->y ) ) {
          First->meat += p->next->meat;
          p->DeleteNext();
        }
      }
    }
  }
  if ( next ) next->CycleNext( First );
  if ( next && next->IsOffscreen() ) DeleteNext();
  return true;
}


void CPGSprite::Draw( Arduboy2 * d ) {
  d->drawBitmap( x, y, tile + ( (frame/3) * (w*h/8)), w, h, WHITE );
  frame++;
  if ( frame/3 == maxFrame ) frame = 0;
}


void CPGSprite::DeleteNext() {
  CPGSprite *NextOfNext = NULL;
  if ( next->next ) NextOfNext = next->next;
  next->next = NULL;
  delete next;
  next = NextOfNext;
}


bool CPGSprite::IsOffscreen() {
  if ((x+w) < 0 || x >= WIDTH || (y+h) < 0 || (y >= HEIGHT)) return true;
  else return false;
}


bool CPGSprite::IsIn( int16_t tX, int16_t tY ) {
  if ( tX >= x &&
       tX <= x+w &&
       tY >= y &&
       tY <= y+h ) return true;
  return false;
}
