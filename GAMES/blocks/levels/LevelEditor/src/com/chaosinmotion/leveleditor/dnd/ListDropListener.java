/*	ListDropListener.java
 * 
 *		LevelEditor Copyright 2016 William Edward Woody, all rights reserved.
 */
package com.chaosinmotion.leveleditor.dnd;

import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import javax.swing.JList;
import javax.swing.TransferHandler;
import com.chaosinmotion.leveleditor.LevelListModel;

/**
 * @author woody
 *
 */
public class ListDropListener<T> extends TransferHandler
{
	private static final long serialVersionUID = 1L;
	private JList<T> list;
	private LevelListModel listModel;
	
	public ListDropListener(JList<T> l, LevelListModel lm)
	{
		listModel = lm;
		list = l;
	}

	@Override
	public boolean importData(TransferSupport support)
	{
		if (!canImport(support)) return false;
		
		Transferable t = support.getTransferable();
		String index;
		try {
			index = (String)t.getTransferData(DataFlavor.stringFlavor);
		}
		catch (Exception e) {
			return false;
		}
		
		int src = Integer.parseInt(index);
		int dst = ((JList.DropLocation)support.getDropLocation()).getIndex();
		
		listModel.move(src,dst);
		
		return true;	// TODO
	}

	@Override
	public boolean canImport(TransferSupport support)
	{
		if (support.isDataFlavorSupported(DataFlavor.stringFlavor)) {
			JList.DropLocation dl = (JList.DropLocation)support.getDropLocation();
			if (dl.getIndex() != -1) return true;
		}
		return false;
	}
	
}
