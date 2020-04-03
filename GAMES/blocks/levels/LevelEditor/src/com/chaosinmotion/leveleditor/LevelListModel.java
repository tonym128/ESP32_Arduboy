/*	LevelsData.java
 * 
 *		LevelEditor Copyright 2016 William Edward Woody, all rights reserved.
 */
package com.chaosinmotion.leveleditor;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Iterator;
import javax.swing.AbstractListModel;
import com.chaosinmotion.leveleditor.data.Level;

/**
 * @author woody
 *
 */
public class LevelListModel extends AbstractListModel<Level> implements Iterable<Level>
{
	private static final long serialVersionUID = 1L;
	private ArrayList<Level> levels;

	public LevelListModel()
	{
		levels = new ArrayList<Level>();
		
		addNewLevel();
		addNewLevel();
	}

	/* (non-Javadoc)
	 * @see javax.swing.ListModel#getSize()
	 */
	@Override
	public int getSize()
	{
		return levels.size();
	}

	/* (non-Javadoc)
	 * @see javax.swing.ListModel#getElementAt(int)
	 */
	@Override
	public Level getElementAt(int index)
	{
		return levels.get(index);
	}
	
	
	public boolean loadFromFile(File f)
	{
		ArrayList<Level> readLevels = new ArrayList<Level>();

		try {
			FileInputStream fis = new FileInputStream(f);
			InputStreamReader isr = new InputStreamReader(fis,"UTF-8");
			BufferedReader br = new BufferedReader(isr);
			
			br.readLine();		// ignore first line
			for (;;) {
				String name = br.readLine();
				if (name == null) break;
				
				Level l = new Level(br);
				l.levelName = name;
				readLevels.add(l);
			}
			
			fis.close();
		}
		catch (Exception ex) {
			return false;
		}
		
		levels = readLevels;
		fireContentsChanged(this, 0, levels.size()-1);
		return true;
	}
	
	public boolean saveToFile(File f)
	{
		try {
			FileOutputStream fos = new FileOutputStream(f);
			OutputStreamWriter osw = new OutputStreamWriter(fos,"UTF-8");
			PrintWriter pw = new PrintWriter(osw);
			
			pw.println("Level Editor Level");
			for (Level l: levels) {
				pw.println(l.levelName);
				l.write(pw);
			}
			
			pw.close();
			fos.close();
		}
		catch (Exception ex) {
			return false;
		}
		
		return true;
	}
	
	public void addNewLevel()
	{
		int ix = levels.size();
		String lname = "Level " + (ix + 1);
		Level l = new Level(lname);
		levels.add(l);
		
		fireIntervalAdded(this, ix, ix);
	}
	
	public void move(int src, int dst)
	{
		if (dst > src) --dst;
		
		Level l = levels.get(src);
		levels.remove(src);
		levels.add(dst, l);
		
		fireContentsChanged(this, 0, levels.size());
	}

	@Override
	public Iterator<Level> iterator()
	{
		return levels.iterator();
	}
}
