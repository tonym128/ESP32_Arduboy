/*	Walls.java
 * 
 *		LevelEditor Copyright 2016 William Edward Woody, all rights reserved.
 */
package com.chaosinmotion.leveleditor.test;

/**
 * @author woody
 *
 */
public class Walls
{
	private boolean[] walls;
	
	public Walls(boolean[] w)
	{
		walls = new boolean[128];
		System.arraycopy(w, 0, walls, 0, 128);
	}
	
	public boolean hitDetect(int x, int y)
	{
		return walls[x + y*16];
	}
}
