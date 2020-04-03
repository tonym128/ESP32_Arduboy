//
//
//

#ifndef _SHIP_H
#define _SHIP_H

#include "Arduboy2.h"
#include "common.h"

//=============================================================================
typedef enum {
    SHIP_ST_NONE,
    SHIP_ST_READY,
    SHIP_ST_SHOOTING,
    SHIP_ST_EXPLODED,
    SHIP_ST_LOST
} eShipState;

//=============================================================================
class Ship {
 public:
    Position pos;

    eShipState state = SHIP_ST_NONE;
    int pat;

    int dont_shoot;
    Position *missile;

    Ship() { missile = NULL; };
    ~Ship() {};

    void init();

    void explode();

    void check_input(int frame);
    void do_state(int frame);
    void draw(int frame);

    void cancel_missile();
    bool is_alive();
};

#endif // !_SHIP_H

//================================ END-OF-FILE ================================
