/*	Level.java
 * 
 *		LevelEditor Copyright 2016 William Edward Woody, all rights reserved.
 */
package com.chaosinmotion.leveleditor.data;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashSet;
import com.chaosinmotion.leveleditor.test.PlayState;
import com.chaosinmotion.leveleditor.test.Walls;

/**
 * @author woody
 *
 */
public class Level
{
	public String levelName;
	public boolean wall[];
	public Pt person;
	public HashSet<Pt> box;
	public HashSet<Pt> end;
	
	public Level(String name)
	{
		levelName = name;
		wall = new boolean[128];
		box = new HashSet<Pt>();
		end = new HashSet<Pt>();

		for (int i = 0; i < 16; ++i) {
			wall[i] = true;
			wall[i+7*16] = true;
		}
		for (int i = 0; i < 8; ++i) {
			wall[i*16] = true;
			wall[i*16+15] = true;
		}
	}
	
	private static Pt parse(String str) throws IOException
	{
		String[] l = str.split(",");
		if (l.length != 2) throw new IOException("Unexpected format");
		
		int x = Integer.parseInt(l[0]);
		int y = Integer.parseInt(l[1]);
		return new Pt(x,y);
	}
	
	public Level(BufferedReader r) throws IOException
	{
		levelName = "";
		wall = new boolean[128];
		box = new HashSet<Pt>();
		end = new HashSet<Pt>();
		
		// First 8 strings: wall
		for (int y = 0; y < 8; ++y) {
			String str = r.readLine();
			for (int x = 0; x < 16; ++x) {
				wall[y*16+x] = (str.charAt(x) == '#');
			}
		}
		
		// Next is person location
		String pt = r.readLine();
		if (pt.equals("null")) {
			person = null;
		} else {
			person = parse(pt);
		}
		
		// Parse boxes
		int num = Integer.parseInt(r.readLine());
		for (int i = 0; i < num; ++i) {
			box.add(parse(r.readLine()));
		}
		num = Integer.parseInt(r.readLine());
		for (int i = 0; i < num; ++i) {
			end.add(parse(r.readLine()));
		}
	}
	
	public void write(PrintWriter w)
	{
		for (int y = 0; y < 8; ++y) {
			StringBuilder b = new StringBuilder();
			for (int x = 0; x < 16; ++x) {
				b.append(wall[y*16+x] ? '#' : '.');
			}
			w.println(b.toString());
		}
		
		if (person != null) {
			w.println(person.getX() + "," + person.getY());
		} else {
			w.println("null");
		}
		
		w.println(box.size());
		for (Pt pt: box) {
			w.println(pt.getX() + "," + pt.getY());
		}
		w.println(end.size());
		for (Pt pt: end) {
			w.println(pt.getX() + "," + pt.getY());
		}
	}
	
	public String toString()
	{
		return levelName;
	}
	
	public PlayState convertToPlayState()
	{
		if (person == null) return null;
		if (box.size() < 1) return null;
		if (box.size() != end.size()) return null;
		
		Walls w = new Walls(wall);
		return new PlayState(w,person,box,end);
	}
}
