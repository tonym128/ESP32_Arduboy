/*	FrameLayout.java
 * 
 *		LevelEditor Copyright 2016 William Edward Woody, all rights reserved.
 */
package com.chaosinmotion.leveleditor;

import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.LayoutManager;

/**
 * @author woody
 *
 */
public class FrameLayout implements LayoutManager
{
	private Component list;
	private Component grid;
	private Component compiled;
	private Component controls;

	/* (non-Javadoc)
	 * @see java.awt.LayoutManager#addLayoutComponent(java.lang.String, java.awt.Component)
	 */
	@Override
	public void addLayoutComponent(String name, Component comp)
	{
		if (name.equals("list")) list = comp;
		if (name.equals("grid")) grid = comp;
		if (name.equals("compiled")) compiled = comp;
		if (name.equals("controls")) controls = comp;
	}

	/* (non-Javadoc)
	 * @see java.awt.LayoutManager#removeLayoutComponent(java.awt.Component)
	 */
	@Override
	public void removeLayoutComponent(Component comp)
	{
		if (grid == comp) grid = null;
		else if (compiled == comp) compiled = null;
		else if (controls == comp) controls = null;
	}

	/* (non-Javadoc)
	 * @see java.awt.LayoutManager#preferredLayoutSize(java.awt.Container)
	 */
	@Override
	public Dimension preferredLayoutSize(Container parent)
	{
		return minimumLayoutSize(parent);
	}

	/* (non-Javadoc)
	 * @see java.awt.LayoutManager#minimumLayoutSize(java.awt.Container)
	 */
	@Override
	public Dimension minimumLayoutSize(Container parent)
	{
		int h = 0;
		if (controls != null) {
			h = controls.getPreferredSize().height;
		}
		int w = 0;
		if (list != null) {
			w = 200;
		}
		if (controls != null) {
			int tmp = controls.getPreferredSize().width;
			if (tmp > w) w = tmp;
		}
		return new Dimension(384 + w,128 + h);
	}

	/* (non-Javadoc)
	 * @see java.awt.LayoutManager#layoutContainer(java.awt.Container)
	 */
	@Override
	public void layoutContainer(Container parent)
	{
		Dimension size = parent.getSize();

		int h = 0;
		if (controls != null) {
			h = controls.getPreferredSize().height;
		}
		int w = 0;
		if (list != null) {
			w = 200;
		}

		int y = h + ((size.height-h) - 128)/2;
		if (grid != null) {
			grid.setBounds(w, y, 256, 128);
		}
		
		if (controls != null) {
			controls.setBounds(0, 0, size.width, h);
		}
		
		if (compiled != null) {
			compiled.setBounds(256+w, h, size.width-256-w, size.height-h);
		}
		
		if (list != null) {
			list.setBounds(0,h,w,size.height-h);
		}
	}
	
}
