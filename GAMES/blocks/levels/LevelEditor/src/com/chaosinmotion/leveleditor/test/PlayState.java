/*	PlayState.java
 * 
 *		LevelEditor Copyright 2016 William Edward Woody, all rights reserved.
 */
package com.chaosinmotion.leveleditor.test;

import java.util.Arrays;
import java.util.Comparator;
import java.util.HashSet;
import com.chaosinmotion.leveleditor.data.Pt;

/**
 * @author woody
 *
 */
public class PlayState
{
	private static final int WALL = 254;
	private static final int NOTHING = 255;
	
	private Walls walls;
	private Pt person;
	private Pt[] box;
	private HashSet<Pt> end;
	
	public PlayState(Walls w, Pt p, HashSet<Pt> b, HashSet<Pt> e)
	{
		walls = w;
		person = p;
		
		box = b.toArray(new Pt[b.size()]);
		Arrays.sort(box, new Comparator<Pt>() {
			@Override
			public int compare(Pt a, Pt b)
			{
				if (a.getX() < b.getX()) return -1;
				if (a.getX() > b.getX()) return 1;
				if (a.getY() < b.getY()) return -1;
				if (a.getY() > b.getY()) return 1;
				return 0;
			}
		});

		end = e;
	}
	
	public PlayState(Walls w, Pt p, Pt[] b, HashSet<Pt> e)
	{
		walls = w;
		person = p;
		box = b;
		end = e;
	}

	
	@Override
	public int hashCode()
	{
		final int prime = 31;
		int result = 1;
		result = prime * result + Arrays.hashCode(box);
		result = prime * result + ((person == null) ? 0 : person.hashCode());
		return result;
	}

	@Override
	public boolean equals(Object obj)
	{
		if (this == obj) return true;
		if (obj == null) return false;
		if (getClass() != obj.getClass()) return false;
		PlayState other = (PlayState) obj;
		if (!Arrays.equals(box, other.box)) return false;
		if (person == null) {
			if (other.person != null) return false;
		} else if (!person.equals(other.person)) return false;
		return true;
	}

	/**
	 * Return true if we are a valid endpoint
	 * @return
	 */
	public boolean isValidEnd()
	{
		for (Pt p: box) {
			if (!end.contains(p)) return false;
		}
		return true;
	}
	
	public String toString()
	{
		return person.getX() + "," + person.getY();
	}
	
	/**
	 * Hit detect
	 * @param x
	 * @param y
	 * @return
	 */
	private int hit(int x, int y)
	{
		if ((x < 0) || (x > 15) || (y < 0) || (y > 7)) return WALL;
		if (walls.hitDetect(x, y)) return WALL;
		
		int i,len = box.length;
		for (i = 0; i < len; ++i) {
			if ((box[i].getX() == x) && (box[i].getY() == y)) return i;
		}
		return NOTHING;
	}
	
	/**
	 * Generate the next state for the index (from 0 to 3), assuming the
	 * state can be generated.
	 * @param i
	 * @return
	 */
	public PlayState nextState(int i)
	{
		int dx = 0;
		int dy = 0;
		switch (i) {
			default:
			case 0:
				dx = -1;
				break;
			case 1:
				dx = 1;
				break;
			case 2:
				dy = -1;
				break;
			case 3:
				dy = 1;
				break;
		}
		
		int x = person.getX() + dx;
		int y = person.getY() + dy;
		
		int h = hit(x,y);
		if (h == WALL) return null;
		if (h == NOTHING) {
			Pt np = new Pt(x,y);
			return new PlayState(walls,np,box,end);
		}
		
		// We hit a box. See if the box walks into anything
		int bx = person.getX() + dx * 2;
		int by = person.getY() + dy * 2;
		if (NOTHING != hit(bx,by)) return null;
		
		// Point
		Pt np = new Pt(x,y);
		HashSet<Pt> nbox = new HashSet<Pt>();
		int j,len = box.length;
		for (j = 0; j < len; ++j) {
			if (j == h) {
				nbox.add(new Pt(bx,by));
			} else {
				nbox.add(box[j]);
			}
		}
		return new PlayState(walls,np,nbox,end);
	}
}
