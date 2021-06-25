#include "Arduino.h"
#include "physics.h"

#define WIDTH 8
#define HEIGHT 8

bool intersectSpriteSprite(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
	return ((x1 < x2+WIDTH) &&
		(x1+WIDTH > x2) &&
		(y1 < y2+HEIGHT) &&
		(y1+HEIGHT > y2));
}

bool collideSpriteSprite(int16_t &x1, int16_t &y1, bool horiz, int8_t &vel, int16_t x2, int16_t y2)
{
	if( (x1 < x2+WIDTH) &&
		(x1+WIDTH > x2) &&
		(y1 < y2+HEIGHT) &&
		(y1+HEIGHT > y2))
	{
		if(horiz)
		{
			if(vel < 0)
			{
				x1 = x2+WIDTH;
			}
			else
			{
				x1 = x2-WIDTH;
			}
		}
		else
		{
			if(vel < 0)
			{
				y1 = y2+HEIGHT;
			}
			else
			{
				y1 = y2-HEIGHT;
			}
		}
		vel = 0;
		return true;
	}
	return false;
}

bool collideSpriteWall(int16_t &x1, int16_t &y1, bool horiz, int8_t &vel, int16_t x2, int16_t y2, int16_t w2, int16_t h2)
{
	if( (x1 < x2+w2) &&
	(x1+WIDTH > x2) &&
	(y1 < y2+h2) &&
	(y1+HEIGHT > y2))
	{
		if(horiz)
		{
			if(vel < 0)
			{
				x1 = x2+w2;
			}
			else
			{
				x1 = x2-WIDTH;
			}
		}
		else
		{
			if(vel < 0)
			{
				y1 = y2+h2;
			}
			else
			{
				y1 = y2-HEIGHT;
			}
		}
		vel = 0;
		return true;
	}
	return false;
}
