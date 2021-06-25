/*	Point.java
 * 
 *		LevelEditor Copyright 2016 William Edward Woody, all rights reserved.
 */
package com.chaosinmotion.leveleditor.data;

/**
 * @author woody
 *
 */
public class Pt
{
	private int x;
	private int y;
	
	public Pt(int xpos, int ypos)
	{
		x = xpos;
		y = ypos;
	}
	
	public int getX()
	{
		return x;
	}
	
	public int getY()
	{
		return y;
	}

	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + x;
		result = prime * result + y;
		return result;
	}

	@Override
	public boolean equals(Object obj)
	{
		if (this == obj) return true;
		if (obj == null) return false;
		if (getClass() != obj.getClass()) return false;
		Pt other = (Pt) obj;
		if (x != other.x) return false;
		if (y != other.y) return false;
		return true;
	}
	
}
