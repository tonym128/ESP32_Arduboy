//
//
//

#ifndef _COMMON_H
#define _COMMON_H

#include "Arduboy2.h"

//=============================================================================
#define FRAMES_IN_SECOND (60)

#define SCREEN_X 20
#define SCREEN_W (128 - 40)

#define POS_SCALE (256)

//=============================================================================
typedef enum {
    GAME_MODE_PRACTICE,
    GAME_MODE_NORMAL
} eGameMode;

//=============================================================================
class Position {
 public:
    int x;
    int y;

    Position() {
        x = 0;
        y = 0;
    };
    Position(int _x, int _y) {
        x = _x * POS_SCALE;
        y = _y * POS_SCALE;
    };

    void set(int _x, int _y) {
        x = _x * POS_SCALE;
        y = _y * POS_SCALE;
    };
    void set_x(int _x) {
        x = _x * POS_SCALE;
    };
    void set_y(int _y) {
        y = _y * POS_SCALE;
    };
    int get_x() {
        return (x / POS_SCALE);
    };
    int get_y() {
        return (y / POS_SCALE);
    };
    int move_x(int d) {
        x += d;
        return (x / POS_SCALE);
    };
    int move_x(float d) {
        x += (int)(d * (float)POS_SCALE);
        return (x / POS_SCALE);
    };
    int move_y(int d) {
        y += d;
        return (y / POS_SCALE);
    };
    int move_y(float d) {
        y += (int)(d * (float)POS_SCALE);
        return (y / POS_SCALE);
    };

    static int get_frames(int diff, float v) {
        return (int)((float)diff / v);
    };

    static float get_v(int diff, int frames) {
        return ((float)diff / (float)frames);
    };
};

//=============================================================================
class Size {
 public:
    int w;
    int h;
    Size(int _w, int _h) {
        w = _w;
        h = _h;
    };
};

//=============================================================================
class Region {
 public:
    Position *pos;
    Size *size;

    Region(Position *_pos, Size *_size) {
        pos = _pos;
        size = _size;
    };
    Region(int _x, int _y, int _w, int _h) {
        pos->set_x(_x);
        pos->set_x(_y);
        size->w = _w;
        size->h = _h;
    };
    bool in_region(Position *dest) {
        int x1 = pos->get_x();
        int y1 = pos->get_y();
        int x2 = dest->get_x();
        int y2 = dest->get_y();
        if (x2 >= x1 && y2 >= y1
            && x2 < (x1 + size->w) && y2 < (y1 + size->h)) {
            return (true);
        }
        return (false);
    };
    bool in_region(Region *dest) {
        int x1 = pos->get_x();
        int y1 = pos->get_y();
        int x2 = dest->pos->get_x();
        int y2 = dest->pos->get_y();
        if (x2 < (x1 + size->w) && (x2 + dest->size->w) > x1
            && y2 < (y1 + size->h) && (y2 + dest->size->h) > y1) {
            return (true);
        }
        return (false);
    };
};

//=============================================================================
class Sprite {
 public:
    int x;
    int y;
    int8_t pat;

    void init(int _x, int _y, int8_t _pat) {
        x = _x;
        y = _y;
        pat = _pat;
    };
    void draw();
};

#endif // !_COMMON_H

//================================ END-OF-FILE ================================
