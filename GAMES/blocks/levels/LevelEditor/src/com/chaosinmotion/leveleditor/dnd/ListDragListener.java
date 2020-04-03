/*	ListDragListener.java
 * 
 *		LevelEditor Copyright 2016 William Edward Woody, all rights reserved.
 */
package com.chaosinmotion.leveleditor.dnd;

import java.awt.datatransfer.StringSelection;
import java.awt.dnd.DnDConstants;
import java.awt.dnd.DragGestureEvent;
import java.awt.dnd.DragGestureListener;
import java.awt.dnd.DragSource;
import java.awt.dnd.DragSourceDragEvent;
import java.awt.dnd.DragSourceDropEvent;
import java.awt.dnd.DragSourceEvent;
import java.awt.dnd.DragSourceListener;
import javax.swing.JList;

/**
 * @author woody
 *
 */
public class ListDragListener<T> implements DragSourceListener, DragGestureListener
{
	private JList<T> list;
	private DragSource ds;
	
	public ListDragListener(JList<T> l)
	{
		list = l;
		ds = new DragSource();
		ds.createDefaultDragGestureRecognizer(list, DnDConstants.ACTION_MOVE, this);
	}

	/* (non-Javadoc)
	 * @see java.awt.dnd.DragGestureListener#dragGestureRecognized(java.awt.dnd.DragGestureEvent)
	 */
	@Override
	public void dragGestureRecognized(DragGestureEvent dge)
	{
		StringSelection transferable = new StringSelection(Integer.toString(list.getSelectedIndex()));
		ds.startDrag(dge, DragSource.DefaultCopyDrop, transferable, this);
	}

	/* (non-Javadoc)
	 * @see java.awt.dnd.DragSourceListener#dragEnter(java.awt.dnd.DragSourceDragEvent)
	 */
	@Override
	public void dragEnter(DragSourceDragEvent dsde)
	{
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see java.awt.dnd.DragSourceListener#dragOver(java.awt.dnd.DragSourceDragEvent)
	 */
	@Override
	public void dragOver(DragSourceDragEvent dsde)
	{
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see java.awt.dnd.DragSourceListener#dropActionChanged(java.awt.dnd.DragSourceDragEvent)
	 */
	@Override
	public void dropActionChanged(DragSourceDragEvent dsde)
	{
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see java.awt.dnd.DragSourceListener#dragExit(java.awt.dnd.DragSourceEvent)
	 */
	@Override
	public void dragExit(DragSourceEvent dse)
	{
		// TODO Auto-generated method stub
		
	}

	/* (non-Javadoc)
	 * @see java.awt.dnd.DragSourceListener#dragDropEnd(java.awt.dnd.DragSourceDropEvent)
	 */
	@Override
	public void dragDropEnd(DragSourceDropEvent dsde)
	{
		// TODO Auto-generated method stub
		
	}
	
}
