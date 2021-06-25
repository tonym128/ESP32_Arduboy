#define ANIMATION_FRAMES 40
#define PI_HALF (3.1415 / 2)

uint8_t perfect;
uint8_t gamestate = 5;
uint8_t frames;
uint8_t platformFirstMove;
uint8_t platformSecondMove = 2;
uint8_t soundTimer5;
uint8_t soundTimer4;
uint8_t soundTimer3;
uint8_t soundTimer2; 
uint8_t coinFonTime;
uint8_t perfectCount;
uint8_t soundTimer;
uint8_t soundOn;
uint8_t playerMarker;
uint8_t markerPos;
uint8_t lineMarkerX;
uint8_t lineMarkerY;
uint8_t lineMarkerEndX;
uint8_t randCoin;
uint8_t Timer2;
uint8_t frameCoin;
uint8_t Timer;
int allMoney;
int cloudXTwo = 80;
int cloudXOne = 30;
int coinsCount;
int coinRandF = - 10;
int coinRandS = -10;
int countPlatform;
int randFirstWidth = 30;
int randFirstPos;
int randSecondWidth = 20; 
int randSecondPos = 65;
int lineOnX = 29;
int lineOnY = 54;
int bridgeLength;
bool level;
bool flipPlayer;
bool middleOn;
bool bridgeIsDown;
bool bridgeOnPlatform;
bool buttOn;
bool collFirst;
bool collSecond;
bool playerOnPlatform;
bool allowDrawLine = true;
bool spriteAnim;
bool platformStrate = true;
bool ninjaB = true;
bool victoriaB;
bool mushB;
bool senseiB;
bool pandaB;
bool filmoteB;
bool MrBlinkyB;
bool PharapB;
bool drummyfishB;
bool VampiricsB;
bool mlxxxpB;
bool ESPboyB;
uint8_t const *selectedPlayer = nullptr;
uint8_t const *selectedPlayer2 = nullptr;
uint8_t buyPlayer[] = {ninjaB, victoriaB, mushB, senseiB, pandaB, filmoteB, MrBlinkyB, PharapB, drummyfishB, VampiricsB, mlxxxpB, ESPboyB};
uint8_t chosenPlayer;
uint8_t costPlayer[] = {0, 20, 30, 45, 45, 50, 50, 50, 50, 50, 50, 50};
const char xCursor[] = {50, 42, 44, 46, 50, 45, 38, 48, 35, 37, 48, 47};
const char *names[] = {"NINJA", "VICTORIA", "MUSHBOY", "SENSEI", "PANDA", "FILMOTE", "MR.BLINKY", "PHARAP", "DRUMMYFISH", "VAMPIRICS", "MLXXXp", "ESPboy"};

const unsigned char * const players[]
{
  ninja,
  girl,
  mush,
  sensei,
  panda,
  filmote,
  MrBlinky,
  Pharap,
  drummyfish,
  Vampirics,
  mlxxxp,
  ESPboy,
};

const unsigned char * const players2[]
{
  ninja2,
  girl2,
  mush2,
  sensei,
  panda2,
  filmote2,
  MrBlinky2,
  Pharap2,
  drummyfish2,
  Vampirics2,
  mlxxxp2,
  ESPboy2,
};

Rect rectFirst;
Rect rectSecond;
Rect player{15, 41, 8, 15};
Rect endLine{-10, -10, 2, 2};
Rect rectFirstMiddle{13, 55, 4, 2}; 
Rect rectSecondMiddle{83, 55, 4, 2}; 
Rect rectCoin{-10, 45, 5, 5};
