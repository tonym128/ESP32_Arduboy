/*
 *      Column.h
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class header for Column objects.
 */

#include "Drawing.h"

#ifndef COLUMN_H
#define COLUMN_H

class Column {
	
	friend class CaveManager;

public:

	// Constructors and destructor
	Column();						// default constructor
	~Column();						// destructor

	// Accessors
	Rectangle getRect();
	float getXpos();

	void spawn(bool isGround);
	void spawn(bool isGround, float x);
	void spawn(bool isGround, float x, int height);
	void spawnObstacle(float x, float y, float height);

	bool CollidesWithPlayer();

	void pushLeft(float amount);

	void Move();	// Recalculate position
	void Draw();	// Draw the column to the screen

protected:

	// Physical
	float x_pos;
	float y_pos;

	// Drawing objects
	Rectangle theRect;
	Quad theQuad;

};

#endif
