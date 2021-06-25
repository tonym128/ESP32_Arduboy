/*
 *      CaveManager.h
 *      
 *      Copyright 2009 Stephen Eisenhauer <mail@stepheneisenhauer.com>
 * 
 * 		Class header for CaveManager objects.
 */

#include "Drawing.h"
#include "Column.h"

#ifndef CAVEMGR_H
#define CAVEMGR_H

#define CAVE_HEIGHT 64
#define NUM_COLUMNS 16
#define NUM_OBSTACLES 0
#define OBSTACLE_SPACING (int)((WIDTH + (2.0 * MARGIN)) / NUM_OBSTACLES)
#define CAVE_SPEED 3
#define MARGIN 16
#define COL_WIDTH (int)((WIDTH + (2.0 * MARGIN)) / NUM_COLUMNS)
#define INITIAL_CAVE_TOP 8
#define INITIAL_CAVE_SPACING 48
#define MIN_CAVE_SPACING 20
#define MAX_CAVE_TOP_CHANGE 2

class CaveManager {
private:

	friend class Column;				// TODO: Not sure this is needed

	Column roof[NUM_COLUMNS];			// Roof columns
	Column ground[NUM_COLUMNS];			// Ground columns
	Column obstacles[NUM_OBSTACLES];	// Obstacle columns

	float caveTop;			// Holds the y-coordinate for the top of the cave
	float caveSpacing;		// Holds the current height of the cave

	unsigned short first;				// Index of the first column

public:

	// Constructors and destructor
	CaveManager();			// default constructor
	~CaveManager();			// destructor

	// Initializer
	void resetAll();		// All columns to starting positions!

	bool CollidesWithPlayer();

	// Operations
	void update();	// Recalculate positions
	void Draw();	// Draw the columns to the screen

};

#endif
