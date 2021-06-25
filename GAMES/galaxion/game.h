//
//
//

#ifndef _GAME_H
#define _GAME_H

#include "Arduboy2.h"
#include "common.h"

#include "ship.h"
#include "enemy.h"

//=============================================================================
#define CAPTURE_SCREEN

//=============================================================================
typedef enum {
    GAME_ST_NONE,
    GAME_ST_LOGO,
    GAME_ST_TITLE,
    GAME_ST_START,
    GAME_ST_PLAY,
    GAME_ST_GAMEOVER,
    GAME_ST_MAX
} eGameState;

//=============================================================================
class Game {
 public:
    eGameState mState;
    int mFrame;

    int mNumShips;
    int mStageNo;
    uint16_t mHighScore;
    uint16_t mScore;

    Ship mShip;

    Convoy mConvoy;

    int mNextStage;
    int mNextShip;

    Game();
    ~Game();

    void check_input();
    void do_state();
    void draw();

    void init_stage();
    void next_stage();

    void add_score(int score);
    void destroy_ship();

    eGameMode mMode;
    int mCursor;

    void draw_logo(int frame);
    void draw_title(int frame);
    void draw_stage(int frame);
};

#endif // !_GAME_H

//================================ END-OF-FILE ================================
