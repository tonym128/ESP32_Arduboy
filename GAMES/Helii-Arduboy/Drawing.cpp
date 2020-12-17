/* 
 * File:   Drawing.cpp
 * Author: stephen
 * 
 * Created on August 7, 2015, 2:23 AM
 */

#include <stdio.h>
#include <math.h>
#include "globals.h"
#include "Drawing.h"
#include "CaveManager.h"

Rectangle::Rectangle()
{
}

Quad::Quad()
{
    _color = 1;
}

void Quad::SetRectangle(Rectangle r)
{
    _rect = r;
}

void Quad::SetFillColor(uint8_t color)
{
    _color = color;
}

void Quad::Draw(bool v)
{
    arduboy.fillRect(
        _rect.x,
        _rect.y,
        _rect.width,
        _rect.height,
        _color
    );
}

Sprite::Sprite()
{
    _x = _y = _width = _height = 0;
    _color = 1;
    _tintR = 0xFF;
    _tintG = 0xFF;
    _tintB = 0xFF;
    _alpha = 0xFF;
    _visible = true;
//    _image = 0;
}

int Sprite::GetX()
{
    return _x;
}

int Sprite::GetY()
{
    return _y;
}

unsigned char Sprite::GetOpacity()
{
    return _alpha;
}

void Sprite::SetX(int x)
{
    _x = x;
}

void Sprite::SetY(int y)
{
    _y = y;
}

void Sprite::SetWidth(int width)
{
    _width = width;
}

void Sprite::SetHeight(int height)
{
    _height = height;
}

void Sprite::SetRotation(float degrees)
{
    _rot = degrees * (M_PI / 180);
}
    
void Sprite::SetPosition(int x, int y)
{
    _x = x;
    _y = y;
}

void Sprite::SetVisible(bool visible)
{
    _visible = visible;
}

void Sprite::DefineCollisionRectangle(int x, int y, int w, int h)
{
    _collRect.x = x;
    _collRect.y = y;
    _collRect.width = w;
    _collRect.height = h;
}

void Sprite::Move(int dx, int dy)
{
    _x += dx;
    _y += dy;
}

void Sprite::Draw()
{
    Draw(_color);
}

void Sprite::Draw(uint8_t color)
{
    if (_visible && _width && _height) {
        // Try to draw image if possible
        //sf2d_texture * tex = 0;
        if (false) {
            // todo
        }
        // Fall back to colored rectangle if no texture present
        else {            
            arduboy.fillRect(
                _x,
                _y,
                _width,
                _height,
                color
            );
        }

        // Draw collision rect (debugging)
        if (false) {
            arduboy.fillRect(
                _x + _collRect.x,
                _y + _collRect.y,
                _collRect.width,
                _collRect.height,
                1
            );
        }
    }
}

// TODO: What were x and y again?
bool Sprite::CollidesWith(Rectangle rect, float x, float y)
{
    bool collided = (rect.x < _x + _collRect.x + _collRect.width &&
       rect.x + rect.width > _x + _collRect.x &&
       rect.y < _y + _collRect.y + _collRect.height &&
       rect.height + rect.y > _y + _collRect.y);
    
    return collided;
}

void Sprite::SetPlaceholderColor(uint8_t color)
{
    _color = color;
}

void Sprite::SetTint(u8 r, u8 g, u8 b)
{
    
}

void Sprite::SetOpacity(u8 alpha)
{
    _alpha = alpha;
}
