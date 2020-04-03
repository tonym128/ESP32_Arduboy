/*	LayoutView.java
 * 
 *		LevelEditor Copyright 2016 William Edward Woody, all rights reserved.
 */
package com.chaosinmotion.leveleditor.views;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.util.concurrent.Future;
import javax.swing.JComponent;
import javax.swing.JOptionPane;
import com.chaosinmotion.leveleditor.data.Level;
import com.chaosinmotion.leveleditor.data.Pt;
import com.chaosinmotion.leveleditor.test.PlayState;
import com.chaosinmotion.leveleditor.test.PlayTest;

/**
 * @author woody
 *
 */
public class LayoutView extends JComponent implements MouseListener
{
	private static final long serialVersionUID = 1L;
	
	public interface Callback
	{
		void updateContents();
	}
	
	private int toolIndex;
	private Callback callback;
	private Level level;

	private Future<?> testFuture;
	
	public static int TOOLWALL = 0;
	public static int TOOLSTART = 1;
	public static int TOOLBOX = 2;
	public static int TOOLEND = 3;

	public LayoutView(Callback cb)
	{
		callback = cb;
		addMouseListener(this);
	}
	
	public void setLevel(Level l)
	{
		level = l;
		repaint();
	}
	
	public Dimension getPreferredSize()
	{
		return getMinimumSize();
	}
	
	public Dimension getMinimumSize()
	{
		return new Dimension(256,128);
	}
	

	public void paint(Graphics g)
	{
		g.setColor(Color.white);
		g.fillRect(0, 0, 256, 128);

		g.setColor(Color.lightGray);
		
		if (level == null) return;
		
		/*
		 * Frame and lines
		 */
		for (int y = 1; y < 8; ++y) {
			g.drawLine(0, y*16, 256, y*16);
		}
		for (int x = 1; x < 16; ++x) {
			g.drawLine(x*16, 0, x*16, 128);
		}
		
		g.drawLine(0, 0, 255, 0);
		g.drawLine(255, 0, 255, 127);
		g.drawLine(255, 127, 0, 127);
		g.drawLine(0, 127, 0, 0);
		
		/*
		 * Fill
		 */
		
		g.setColor(Color.black);
		for (int y = 0; y < 8; ++y) {
			for (int x = 0; x < 16; ++x) {
				if (level.wall[x+y*16]) {
					g.fillRect(x*16, y*16, 16, 16);
				}
			}
		}
		
		/*
		 * Add start position
		 */
		
		if (level.person != null) {
			g.drawString("P", level.person.getX() * 16 + 4, level.person.getY() * 16 + 14);
		}
		for (Pt pt: level.box) {
			g.drawString("B", pt.getX() * 16 + 4, pt.getY() * 16 + 14);
		}
		for (Pt pt: level.end) {
			g.drawString("E", pt.getX() * 16 + 4, pt.getY() * 16 + 14);
		}
	}
	
	public void setTool(int index)
	{
		toolIndex = index;
	}
	
	public void mouseClicked(MouseEvent e)
	{
		if (level == null) return;
		
		int y = e.getY()/16;
		int x = e.getX()/16;
		if ((x >= 0) && (x <= 15) && (y >= 0) && (y <= 7)) {
			if (toolIndex == TOOLWALL) {
				level.wall[x+y*16] = !level.wall[x+y*16];
			} else if (toolIndex == TOOLSTART) {
				level.person = new Pt(x,y);
			} else if (toolIndex == TOOLBOX) {
				Pt pt = new Pt(x,y);
				if (level.box.contains(pt)) {
					level.box.remove(pt);
				} else {
					level.box.add(pt);
				}
			} else if (toolIndex == TOOLEND) {
				Pt pt = new Pt(x,y);
				if (level.end.contains(pt)) {
					level.end.remove(pt);
				} else {
					level.end.add(pt);
				}
			}
			callback.updateContents();
		}
		repaint();
	}

	@Override
	public void mousePressed(MouseEvent e)
	{
	}

	@Override
	public void mouseReleased(MouseEvent e)
	{
	}

	@Override
	public void mouseEntered(MouseEvent e)
	{
	}

	@Override
	public void mouseExited(MouseEvent e)
	{
	}
	
	// TODO: Test level, or cancel test if test is in progress.
	public void testLevel()
	{
		if (testFuture == null) {
			PlayState state = level.convertToPlayState();
			if (state == null) {
				JOptionPane.showMessageDialog(this, "Illegal level; level must contain a person and an equal number of boxes and endpoints");
				return;
			}
			
			PlayTest test = new PlayTest(state);
			testFuture = test.runTest(new PlayTest.Callback() {
				@Override
				public void finished(boolean success)
				{
					testFuture = null;
					
					if (success) {
						JOptionPane.showMessageDialog(LayoutView.this, "Valid level.");
					} else {
						JOptionPane.showMessageDialog(LayoutView.this, "This level is not valid.");
					}
				}
			});
		} else {
			testFuture.cancel(true);
			testFuture = null;
		}
	}
}
