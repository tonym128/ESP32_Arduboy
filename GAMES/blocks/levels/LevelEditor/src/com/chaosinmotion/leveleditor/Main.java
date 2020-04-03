/*	Main.java
 * 
 *		LevelEditor Copyright 2016 William Edward Woody, all rights reserved.
 */
package com.chaosinmotion.leveleditor;

import javax.swing.JFrame;
import javax.swing.SwingUtilities;

/**
 * @author woody
 *
 */
public class Main
{
	public static void main(String[] args)
	{
		SwingUtilities.invokeLater(new Runnable() {
			@Override
			public void run()
			{
				createAndShowGUI();
			}
		});
	}
	
	private static void createAndShowGUI()
	{
		JFrame frame = new MainWindow();
		frame.setVisible(true);
	}
}
