/* 
 * File:   Drawing.h
 * Author: stephen
 *
 * Created on August 7, 2015, 2:23 AM
 */

#ifndef DRAWING_H
#define	DRAWING_H

#include <Arduboy2.h>

// Pure geometry data
class Rectangle {
public:
    int16_t x;
    int8_t y;
    uint8_t width;
    uint8_t height;
    Rectangle();
private:

};

// Drawable quadrilateral
class Quad {
public:
    Quad();
    void SetRectangle(Rectangle r);
    void SetFillColor(uint8_t color);
    void Draw(bool v=0);
private:
    Rectangle _rect;
    uint8_t _color;
};

class Sprite {
public:
    Sprite();
    int GetX();
    int GetY();
    u8 GetOpacity();
    void SetX(int x);
    void SetY(int y);
    void SetWidth(int width);
    void SetHeight(int height);
    void SetRotation(float degrees);
    void SetPosition(int x, int y);
    void SetVisible(bool visible);
//    void SetImage(Image * image, int _tile_w = 0, int _tile_h = 0);
    void DefineCollisionRectangle(int x, int y, int w, int h);
    bool CollidesWith(Rectangle rect, float x, float y);
    void SetPlaceholderColor(u8 _color);
    void SetTint(u8 r, u8 g, u8 b);
    void SetOpacity(u8 alpha);
    void Move(int dx, int dy);
    void Draw();
    void Draw(uint8_t color);
private:
    int _x;
    int _y;
    float _width;
    float _height;
    float _rot;
    int _visible;
//    Image * _image;
    uint8_t _color;
    u8 _alpha;
    Rectangle _collRect;
    u8 _tintR;
    u8 _tintG;
    u8 _tintB;
};

#endif	/* DRAWING_H */
