/*	ToolPicker.java
 * 
 *		LevelEditor Copyright 2016 William Edward Woody, all rights reserved.
 */
package com.chaosinmotion.leveleditor.views;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import javax.swing.JComponent;

/**
 * @author woody
 *
 */
public class ToolPicker extends JComponent implements MouseListener
{
	private static final long serialVersionUID = 1L;
	private int tool = 0;
	private Callback callback;
	
	public ToolPicker()
	{
		addMouseListener(this);
	}
	
	public interface Callback
	{
		void updateTool(int tool);
	}
	
	public void setCallback(Callback c)
	{
		callback = c;
	}
	
	public void setTool(int t)
	{
		tool = t;
		repaint();
	}
	
	private void drawTool(Graphics g, int index, String label)
	{
		if (index == tool) {
			g.setColor(Color.black);
			g.fillRect(index*16, 0, 16, 16);
			g.setColor(Color.white);
		} else {
			g.setColor(Color.black);
		}
		
		g.drawString(label, index*16+2, 14);
	}
	
	public void paint(Graphics g)
	{
		g.setColor(Color.white);
		g.fillRect(0, 0, getWidth(), getHeight());
	
		drawTool(g,0,"W");
		drawTool(g,1,"P");
		drawTool(g,2,"B");
		drawTool(g,3,"E");
	}
	
	@Override
	public Dimension getPreferredSize()
	{
		return new Dimension(64,16);
	}

	@Override
	public Dimension getMaximumSize()
	{
		// TODO Auto-generated method stub
		return getPreferredSize();
	}

	@Override
	public Dimension getMinimumSize()
	{
		// TODO Auto-generated method stub
		return getPreferredSize();
	}

	@Override
	public void mouseClicked(MouseEvent e)
	{
		int index = e.getX()/16;
		if ((index >= 0) && (index < 4)) {
			setTool(index);
			callback.updateTool(index);
		}
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
	

}
