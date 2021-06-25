/*	PlayTest.java
 * 
 *		LevelEditor Copyright 2016 William Edward Woody, all rights reserved.
 */
package com.chaosinmotion.leveleditor.test;

import java.util.HashSet;
import java.util.LinkedList;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import javax.swing.SwingUtilities;

/**
 * @author woody
 *
 */
public class PlayTest
{
	private LinkedList<PlayState> queue;
	private HashSet<PlayState> states;
	
	public interface Callback
	{
		void finished(boolean success);
	}
	
	public PlayTest(PlayState start)
	{
		states = new HashSet<PlayState>();
		queue = new LinkedList<PlayState>();
		
		queue.add(start);
	}
	
	private static ExecutorService executor;
	private static synchronized ExecutorService getExecutor()
	{
		if (executor == null) {
			executor = Executors.newCachedThreadPool();
		}
		return executor;
	}
	
	public Future<?> runTest(final Callback callback)
	{
		return getExecutor().submit(new Runnable() {
			@Override
			public void run()
			{
				final boolean result = test();
				SwingUtilities.invokeLater(new Runnable() {
					@Override
					public void run()
					{
						callback.finished(result);
					}
				});
			}
		});
	}
	
	/**
	 * Test to see if we hit an end state
	 * @return
	 */
	private boolean test()
	{
		for (;;) {
			if (queue.isEmpty()) return false;
			
			PlayState s = queue.removeFirst();
			
			if (s.isValidEnd()) return true;
			if (!states.contains(s)) {
				states.add(s);
				
				/*
				 * Formulate the four directions
				 */
				
				for (int i = 0; i < 4; ++i) {
					PlayState ns = s.nextState(i);
					if ((ns != null) && (!states.contains(ns))) {
						queue.addFirst(ns);
					}
				}
			}
		}
	}
}
