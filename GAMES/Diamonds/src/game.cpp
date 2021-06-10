#include "game.h"
#include "resources.h"

#define DEFAULTPASX 12
#define DEFAULTPASY 8

void initLevel(void);

#define NORM(x) (x > 0 ? x : -x);
#define SGN(x)  (x > 0 ? 1 : -1);

char current_level;
int x5, y5;
int FieldBegin_x;
int FieldBegin_y;
int FieldBegin_z;

u8 direction;

int transparent;

int cur_shape, cur_x, cur_y, cur_rot, cur_pas;
int pas_x, pas_y;


int practice;
int score;

int bonus_time;

int life;

int nb_brick;
int nb_brick_end;

int getready;

u8 current_col;

int have_key;


// taille du tableau
#define FIELD_WIDTH  8
#define FIELD_HEIGHT 10

#define FPS          60
#define FULLBONUS    (70 * FPS)

#define SIZE_X       12
#define SIZE_Y       6

char Field[FIELD_HEIGHT + 2][FIELD_WIDTH + 2];



// helper functions

static void draw();

// game state functions

static void menuTitle();


static void gameSetup();
static void gameMain();


typedef void (*tick_func_t)();

//static tick_func_t gameTick {&menuTitle};
static tick_func_t gameTick = &menuTitle;


void tick()
{
    gameTick();
}


unsigned int rng()
{
    static unsigned int y = 0;

    y += micros(); // seeded with changing number
    y ^= y << 2; y ^= y >> 7; y ^= y << 7;
    return (y);
}

void drawOverwriteMaskDynamic(int x0, int y0, u8 *diamondsR, int flag)
{
    u8 *ofsOut = arduboy.sBuffer;

    unsigned char *ofsIn = diamondsR + 2;
    int x, y;

    ofsOut += x0;

    for (y = 0; y < 24 / 8; y++) {
        for (x = 0; x < 88; x++) {
            ofsOut[x] |= *ofsIn; // pgm_read_byte(ofsIn);
            ofsIn++;
        }
        ofsOut += 128;
    }
}

byte sequenceFrame;
const unsigned char PROGMEM frameSequences[] = {
    // bouncing Ball Frame Sequence
    0, 1, 2, 0, 0, 0, 0, 0,
    // bouncing Ball Height Sequence
    0, 0, 0, 1, 0, 0, 0, 0,
    // game title Height Sequence
    1, 2, 3, 4, 5, 4, 3, 2,
};

void menuCredits()
{
    u8 diamondsR[88 * 3 + 2];

    arduboy.clear();


    if (arduboy.everyXFrames(2)) sequenceFrame++;
    if (sequenceFrame > 7) sequenceFrame = 0;

    sprites.drawOverwrite(20, pgm_read_byte(&frameSequences[sequenceFrame + 16]), diamondsborder, 0);


    memcpy_P(diamondsR, diamonds, 88 * 3 + 2);
    for (int i = 2; i < 88 * 3 + 2; i++) {
        diamondsR[i] = diamondsR[i] & rng();
    }

//        sprites.drawOverwrite(20, 0, diamondsR, 0);    // Doesn't work (image have to be in PROGMEM with this function)

    drawOverwriteMaskDynamic(20, 0, diamondsR, 0);


    if ((arduboy.justPressed(A_BUTTON)) |
        (arduboy.justPressed(B_BUTTON)) |
        (arduboy.justPressed(UP_BUTTON)) |
        (arduboy.justPressed(DOWN_BUTTON))) {
        gameTick = &menuTitle;
    }

    arduboy.setCursor(4, 32);
    arduboy.print("Idea    Oliver Dreer");
    arduboy.setCursor(4, 40);
    arduboy.print("GFX/HP48 Doug Cannon");
    arduboy.setCursor(4, 50);
    arduboy.print("Code   RedBug/Kyuran");


    if (!sound.playing()) {
        sound.playScore(song);
    }
} // menuCredits



void menuTitle()
{
    u8 diamondsR[88 * 3 + 2];

    sprites.drawOverwrite(0, 0, diammain, 0);
    sprites.drawOverwrite(7, 0, diamondsborder, 0);


    memcpy_P(diamondsR, diamonds, 88 * 3 + 2);
    for (int i = 2; i < 88 * 3 + 2; i++) {
        diamondsR[i] = diamondsR[i] & rng();
    }

// sprites.drawOverwrite(7, 0, diamondsR, 0);    // Doesn't work (image have to be in PROGMEM with this function)
    drawOverwriteMaskDynamic(7, 0, diamondsR, 0);


    for (int y = 7; y < 88 + 7; y++) {
        arduboy.sBuffer[y] = arduboy.sBuffer[y] | 3;
    }


    sprites.drawSelfMasked(2, 2 + 20, diamlogo, 0);

    if (arduboy.justPressed(A_BUTTON)) {
        score = 0;
        practice = 0;
        gameTick = &gameSetup;
        sound.toneMutesScore(true);
    }

    if (arduboy.justPressed(B_BUTTON)) {
        practice = 1;
        gameTick = &gameSetup;
        sound.toneMutesScore(true);
    }

    if (arduboy.justPressed(UP_BUTTON)) {
        if (arduboy.audio.enabled()) {
            arduboy.audio.off();
        } else {
            arduboy.audio.on();
        }
        arduboy.audio.saveOnOff();
    }

    if (arduboy.justPressed(DOWN_BUTTON)) {
        gameTick = &menuCredits;
    }

    if (arduboy.audio.enabled()) {
        arduboy.fillRect(2 + 44, (2 + 20) + 28, 12, 5, BLACK);
    } else {
        arduboy.fillRect(2 + 34, (2 + 20) + 28, 9, 5, BLACK);
    }


    if (!sound.playing()) {
        sound.playScore(song);
    }

} // menuTitle


// Diamonds

// Game initialization

void gameSetup()
{
    arduboy.initRandomSeed();

    life = 4;
    current_level = 0;

    initLevel();

    draw();

    gameTick = &gameMain;
}



void playSound(int i)
{

    switch (i) {
        case 1:
            sound.tone(WALL_FREQ, WALL_DUR); // Good bounce
            break;
        case 2:
            sound.tone(WALL_FREQ, PADDLE_DUR); // Bounce
            break;
    }


}

int GetAction(int x, int y)
{
    int retour;

    retour = 0;

    switch (Field[y][x]) {
        case 0:
            break;
        case 1:
            if (current_col == 0) {
                nb_brick--;
                score += 3;
                Field[y][x] = 0;
                retour = 1;
            }
            retour = 1;
            break;
        case 2: // DUR (explose a la fin)

            if (nb_brick == 0) {
                score += 50;
                Field[y][x] = 0;
                nb_brick_end--;
                if (nb_brick_end == 0) {
                    /*
                     * int x0,y0;
                     * int x1,y1;
                     *
                     * x0 = (int)(cur_level / 10);
                     * y0 = (int)(cur_level - x0 * 10);
                     *
                     * for(x1=-1;x1<=FIELD_WIDTH;x1++) {
                     *  Field[-1+y0*(FIELD_HEIGHT+5)+5][x1+x0*(FIELD_WIDTH+5)+5] = 0;
                     *  Field[FIELD_HEIGHT+y0*(FIELD_HEIGHT+5)+5][x1+x0*(FIELD_WIDTH+5)+5] = 0;
                     *  }
                     * for(y1=-1;y1<=FIELD_HEIGHT;y1++) {
                     *  Field[y1+y0*(FIELD_HEIGHT+5)+5][-1+x0*(FIELD_WIDTH+5)+5] = 0;
                     *  Field[y1+y0*(FIELD_HEIGHT+5)+5][FIELD_WIDTH+x0*(FIELD_WIDTH+5)+5] = 0;
                     *  }
                     */

                    //                    part = 2;
                    //                  subpart = 1;  // Win

                    score = score + (bonus_time / FPS) * 25;

                    current_level++;
                    initLevel();

                    arduboy.setRGBled(0, 255, 0);
                }

            }
            retour = 1;
            break;
        case 3: // Toujours DUR
            retour = 1;
            break;
        case 4: // Key
            if (have_key == 0) {
                score += 25;
                have_key = 1;
                Field[y][x] = 0;
            }
            retour = 1;
            break;
        case 5: // Lock
            if (have_key == 1) {
                score += 50;
                Field[y][x] = 0;
                have_key = 0;
            }
            retour = 1;
            break;
        case 6: // BOUM
            //  Lost();

            arduboy.setRGBled(255, 0, 0);

            life--;
            if (life > 0) {
                initLevel();
            }

            retour = 1;
            break;
        case 7:
            Field[y][x] = 0;
            direction = -direction;
            retour = 1;
            //  commande_inverse
            break;
        case 8:
            if (current_col == 1) {
                nb_brick--;
                Field[y][x] = 0;
                score += 3;
                retour = 1;
            }
            retour = 1;
            break;
        case 9:
            current_col = 1;
            retour = 1;
            break;
        case 10:
            if (current_col == 2) {
                nb_brick--;
                Field[y][x] = 0;
                score += 3;
                retour = 1;
            }
            retour = 1;
            break;
        case 11:
            current_col = 2;
            retour = 1;
            break;
        case 12:
            if (current_col == 3) {
                nb_brick--;
                Field[y][x] = 0;
                score += 3;
                retour = 1;
            }
            retour = 1;
            break;
        case 13:
            current_col = 3;
            retour = 1;
            break;
        case 14:
            if (current_col == 4) {
                nb_brick--;
                Field[y][x] = 0;
                score += 3;
                retour = 1;
            }
            retour = 1;
            break;
        case 15:
            current_col = 4;
            retour = 1;
            break;
        case 16:
            retour = 1;
    } // switch

    if (retour) {
        playSound(2);
    }

    return retour;
} // GetAction


int TestMove(int x, int y)
{
    int coli;

    // 84
    // 12 // Size_x:12, size_y:6

    coli = GetAction(x / SIZE_X, y / SIZE_Y) * 8 +
        GetAction((x + 5) / SIZE_X, y / SIZE_Y) * 4 +
        GetAction((x + 5) / SIZE_X, (y + 4) / SIZE_Y) * 2 +
        GetAction(x / SIZE_X, (y + 4) / SIZE_Y);

    switch (coli) {
        case 1:
            pas_x = NORM(pas_x);
            pas_y = -NORM(pas_y);
            break;
        case 8:
            pas_x = NORM(pas_x);
            pas_y = NORM(pas_y);
            break;
        case 4:
            pas_x = -NORM(pas_x);
            pas_y = NORM(pas_y);
            break;
        case 2:
            pas_x = -NORM(pas_x);
            pas_y = -NORM(pas_y);
            break;

        case 11:
        case 7:
        case 14: // diag
        case 13:
        case 5:
        case 10:
            pas_x = -pas_x;
            pas_y = -pas_y;
            break;

        case 3:
            pas_y = -NORM(pas_y);
            break;

        case 12: // horiz
            pas_y = NORM(pas_y);
            break;

        case 6:
            pas_x = -NORM(pas_x);
            break;

        case 9: // vert
            pas_x = NORM(pas_x);
            break;

        case 15:
        case 0: // Nothing
            break;
    } // switch


    if ((coli == 0) | (coli == 15)) {
        transparent = 0;
        return 1;
    } else {
        if (transparent != 0) {
            return 1;
        } else {
            return 0;
        }
    }
} // TestMove




void handleBall(void)
{
    if (bonus_time > 0) {
        bonus_time--;
    }

    if (arduboy.pressed(RIGHT_BUTTON)) {
        if (pas_x == 0) {
            pas_x = DEFAULTPASX;
        } else if (pas_x < 0) pas_x = pas_x - (pas_x - 1) / 2;
    } else if (arduboy.pressed(LEFT_BUTTON)) {
        if (pas_x == 0) {
            pas_x = -DEFAULTPASX;
        } else if (pas_x > 0) pas_x = pas_x - (pas_x + 1) / 2;
    } else {
        if (pas_x > 0) pas_x = pas_x - (pas_x + 1) / 2;
        if (pas_x < 0) pas_x = pas_x - (pas_x - 1) / 2;
    }
    // }

    if (TestMove((cur_x + pas_x) / 16, (cur_y + pas_y) / 16) == 1) {
        cur_x += pas_x;
        cur_y += pas_y;
    }


} // handleBall

void initLevel(void)
{

    char str[5];

    memcpy_P(str, begin[current_level], 5);

    x5 = ((str[0] - '0') + (str[1] - '0') * 16 - 5) / 12;
    y5 = ((str[2] - '0') + (str[3] - '0') * 16 - 5) / 6;

    nb_brick = 0;
    nb_brick_end = 0;


    int x, y;

    for (x = 0; x < FIELD_WIDTH + 2; x++) {
        for (y = 0; y < FIELD_HEIGHT + 2; y++) {
            Field[y][x] = 3;
        }
    }

    for (x = 0; x < FIELD_WIDTH; x++) {
        char line[10];

        memcpy_P(line, level[x + current_level * 8], 10);


        for (y = 0; y < FIELD_HEIGHT; y++) {

            int z = line[y];

            if ((z >= '0') & (z <= '9')) z = z - '0';
            if ((z >= 'A') & (z <= 'F')) z = z - 'A' + 10;

            switch (z) {
                case 1:
                case 8:
                case 10:
                case 12:
                case 14:
                    nb_brick++;
                    break;
                case 2:
                    nb_brick_end++;
                    break;
            }

            Field[y + 1][x + 1] = z;
        }
    }



    if (Field[y5 + 1][x5 + 1] != 0) {
        if ((str[4] == '1') & (Field[y5 + 1 - 1][x5 + 1] == 0)) {
            y5 = y5 - 1;
        } else if ((str[4] == '2') & (Field[y5 + 1 + 1][x5 + 1] == 0)) {
            y5 = y5 + 1;
        } else if (Field[y5 + 1 + 1][x5 + 1] == 0) {
            y5 = y5 + 1;
        } else if (Field[y5 + 1 - 1][x5 + 1] == 0) {
            y5 = y5 - 1;
        } else if (Field[y5 + 1 + 2][x5 + 1] == 0) {
            y5 = y5 + 2;
        } else if (Field[y5 + 1 - 2][x5 + 1] == 0) {
            y5 = y5 - 2;
        }
    }



    FieldBegin_x = ( (x5 + 1) * SIZE_X + SIZE_X / 4 ) * 16; // max 96
    FieldBegin_y =  ( (y5 + 1) * SIZE_Y ) * 16;// max 60
    FieldBegin_z = str[4] - '0';

    // 240, 96  -> 15, 6

    cur_x = FieldBegin_x;
    cur_y = FieldBegin_y;
    if (FieldBegin_z == 1) {
        pas_y = -DEFAULTPASY;
    } else {
        pas_y = DEFAULTPASY;
    }

    bonus_time = FULLBONUS;

    current_col = 0;

    have_key = 0;

    direction = 1;

    transparent = 0;

    getready = FPS * 2;

} // initLevel



// Running game

void gameMain()
{
    sprites.drawOverwrite(0, 0, diammain, 0);

    char line[3];
    sprintf(line, "%02d", current_level + 1);

    arduboy.setCursor(117, 12);
    arduboy.print(line);

    draw();

    if (life == 0) {
        sprites.drawOverwrite(2 + (96 - 72) / 2, 2 + (60 - 15) / 2, diamgameover, 0);

    } else if (getready > 0) {
        getready--;

        sprites.drawOverwrite(2 + (96 - 72) / 2, 2 + (60 - 15) / 2, diamgetready, 0);

        if (getready == 0) {
            arduboy.setRGBled(0, 0, 0);
        }

    } else {

        handleBall();

    }

    //  Bonus: 102,30 (max width: 25)


    int length;

    // (FULLBONUS/FPS-bonus_time/FPS)
    // bonus_time = FULLBONUS , FULLBONUS-bonus_time- = 0                 --> length = 0
    // bonus_time = 0         , FULLBONUS-bonus_time = FULLBONUS, *25 / FULLBONUS          --> length = 25


    length = ((FULLBONUS - bonus_time) * 25) / FULLBONUS;

    arduboy.fillRect(102 + 25 - length, 30, length, 2, BLACK);

    int lifetodelete = 4 - life;

    // vie 4: 122,58  vie 3: 115 vie 4: 108

    arduboy.fillRect(129 - lifetodelete * 7, 58, lifetodelete * 7, 5, BLACK);


#ifndef DEBUG
    {
        char line[32];

        if (practice) {
            arduboy.setCursor(101, 2);
            strcpy(line, "Pra.");
        } else {
            arduboy.setCursor(105, 2);
            sprintf(line, "%04d", score);
        }

        arduboy.print(line);
    }
#endif

    if ((arduboy.justPressed(UP_BUTTON)) && (practice)) {
        if (current_level > 0) {
            current_level--;
        } else {
            current_level = NBLEVEL - 1;
        }
        initLevel();
    }

    if ((arduboy.justPressed(DOWN_BUTTON)) && (practice)) {
        if (current_level < NBLEVEL - 1) {
            current_level++;
        } else {
            current_level = 0;
        }
        initLevel();
    }

    if (arduboy.justPressed(B_BUTTON)) {
        gameTick = &menuTitle;
        sound.toneMutesScore(false);
    }

    if (arduboy.justPressed(A_BUTTON)) {
        if (life == 0) {
            gameTick = &menuTitle;
            sound.toneMutesScore(false);
        }
    }

} // gameMain2

void drawTilePixel(u8 x0, u8 y0, u8 z)
{
    sprites.drawSelfMasked(x0, y0, diamicon, z);
} // drawTilePixel


void drawTile(u8 x0, u8 y0, u8 z)
{
    drawTilePixel(x0 * SIZE_X + 2, y0 * SIZE_Y + 2, z);
} // drawTile

void draw()
{
    if (current_col == 0) {
        drawTilePixel(101, 37, 1);
    } else {
        drawTilePixel(101, 37, current_col * 2 + 6);
    }

    u8 x, y;

    for (x = 0; x < FIELD_WIDTH; x++) {
        for (y = 0; y < FIELD_HEIGHT; y++) {
            drawTile(x, y, Field[y + 1][x + 1]);
        }
    }


    // Draw bullet

    int x3, y3;

    x3 = (cur_x / 16 - SIZE_X) + 2;
    y3 = (cur_y / 16 - SIZE_Y) + 2;

    arduboy.fillRect(x3 + 0, y3 + 1, 1 + 3 + 1, 0 + 3 + 0, WHITE);
    arduboy.fillRect(x3 + 1, y3 + 0, 0 + 3 + 0, 1 + 3 + 1, WHITE);

} // draw2
