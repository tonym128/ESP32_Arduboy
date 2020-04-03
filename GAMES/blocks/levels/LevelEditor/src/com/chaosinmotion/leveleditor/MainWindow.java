/*	MainWindow.java
 * 
 *		LevelEditor Copyright 2016 William Edward Woody, all rights reserved.
 */
package com.chaosinmotion.leveleditor;

import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.PrintWriter;
import javax.swing.DropMode;
import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JList;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.ListSelectionModel;
import javax.swing.event.ListDataEvent;
import javax.swing.event.ListDataListener;
import javax.swing.event.ListSelectionEvent;
import javax.swing.event.ListSelectionListener;
import javax.swing.filechooser.FileNameExtensionFilter;
import com.chaosinmotion.leveleditor.data.Level;
import com.chaosinmotion.leveleditor.data.Pt;
import com.chaosinmotion.leveleditor.dnd.ListDragListener;
import com.chaosinmotion.leveleditor.dnd.ListDropListener;
import com.chaosinmotion.leveleditor.views.LayoutView;
import com.chaosinmotion.leveleditor.views.ToolPicker;

/**
 * @author woody
 *
 */
public class MainWindow extends JFrame
{
	private static final long serialVersionUID = 1L;
	
	private LayoutView layoutView;
	private LevelListModel levels;
	private JTextArea textArea;
	private boolean isDirty;
	private File file;
	
	public MainWindow()
	{
		super("Blocks Editor");
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		getContentPane().setLayout(new FrameLayout());

		layoutView = new LayoutView(new LayoutView.Callback() {
			@Override
			public void updateContents()
			{
				isDirty = true;
				updateTextContents();
			}
		});
		getContentPane().add("grid", layoutView);
		
		// TODO: Scrolling?
		textArea = new JTextArea();
		JScrollPane sp = new JScrollPane(textArea);
		getContentPane().add("compiled", sp);
						
		JPanel buttons = new JPanel(new FlowLayout());
		ToolPicker picker = new ToolPicker();
		buttons.add(picker);
		picker.setCallback(new ToolPicker.Callback() {
			@Override
			public void updateTool(int tool)
			{
				layoutView.setTool(tool);
			}
		});
		
		JButton add = new JButton("Add");
		buttons.add(add);
		add.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e)
			{
				levels.addNewLevel();
			}
		});

		JButton save = new JButton("Save");
		buttons.add(save);
		save.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e)
			{
				saveFile();
			}
		});

		JButton load = new JButton("Load");
		buttons.add(load);
		load.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e)
			{
				loadFile();
			}
		});

		JButton compile = new JButton("Test");
		buttons.add(compile);
		compile.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e)
			{
				layoutView.testLevel();
			}
		});

		getContentPane().add("controls",buttons);
		
		levels = new LevelListModel();
		final JList<Level> list = new JList<Level>(levels);
		JScrollPane lsp = new JScrollPane(list);
		getContentPane().add("list", lsp);
		
		list.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		list.setDragEnabled(true);
		list.setDropMode(DropMode.INSERT);
		list.setTransferHandler(new ListDropListener<Level>(list,levels));
		new ListDragListener<Level>(list);
		
		list.addListSelectionListener(new ListSelectionListener() {
			@Override
			public void valueChanged(ListSelectionEvent e)
			{
				int index = list.getSelectedIndex();
				Level l = levels.getElementAt(index);
				layoutView.setLevel(l);
			}
		});

		/*
		 * I also want to listen for changes to update state
		 */
		levels.addListDataListener(new ListDataListener() {
			@Override
			public void intervalAdded(ListDataEvent e)
			{
				updateTextContents();
			}

			@Override
			public void intervalRemoved(ListDataEvent e)
			{
				updateTextContents();
			}

			@Override
			public void contentsChanged(ListDataEvent e)
			{
				updateTextContents();
			}
		});
		
		pack();
	}
	
	private boolean saveFile()
	{
		if (file == null) {
			FileNameExtensionFilter filter = new FileNameExtensionFilter("Level Editor File","lvle");
			JFileChooser save = new JFileChooser();
			save.addChoosableFileFilter(filter);
			int retVal = save.showSaveDialog(this);
			if (retVal != JFileChooser.APPROVE_OPTION) return false;
			
			File f = save.getSelectedFile();
			if (levels.saveToFile(f)) {
				file = f;
				isDirty = false;
				return true;
			} else {
				return false;
			}
		} else {
			if (levels.saveToFile(file)) {
				isDirty = false;
				return true;
			} else {
				return false;
			}
		}
	}
	
	private void loadFile()
	{
		if (isDirty) {
			String[] options = { "Save", "Don't Save", "Cancel" };
			int index = JOptionPane.showOptionDialog(this, 
					"Save your levels first", "Save", JOptionPane.YES_NO_CANCEL_OPTION, 
					JOptionPane.WARNING_MESSAGE, null, options, options[2]);
			if (index == 0) {
				if (!saveFile()) return;
			} else if (index == 2) {
				return;
			}
		}
		
		/* If we get here we're going to overwrite our current contents */
		FileNameExtensionFilter filter = new FileNameExtensionFilter("Level Editor File","lvle");
		JFileChooser open = new JFileChooser();
		open.addChoosableFileFilter(filter);
		int retVal = open.showOpenDialog(this);
		if (retVal != JFileChooser.APPROVE_OPTION) return;
		
		File f = open.getSelectedFile();
		if (levels.loadFromFile(f)) {
			isDirty = false;
			file = f;
		}
	}
	
	private void updateTextContents()
	{
		try {
			ByteArrayOutputStream baos = new ByteArrayOutputStream();
			PrintWriter w = new PrintWriter(baos);
			
			w.println("#define MAXLEVELS " + levels.getSize());
			w.println();
			
			w.println("/*  LevelMap");
			w.println(" *");
			w.println(" *      Each level is a 16 byte bitmap with a bit set for each wall item");
			w.println(" */");
			w.println();
			
			w.println("const static PROGMEM uint16_t LevelMap[] = {");
			boolean first = true;
			for (Level l: levels) {
				if (first) {
					first = false;
				} else {
					w.println();
				}
				w.println("    // " + l.levelName);
				for (int y = 0; y < 8; ++y) {
					int m = 0;
					for (int x = 0; x < 16; ++x) {
						m = m << 1;
						if (l.wall[x + y*16]) {
							m |= 1;
						}
					}
					w.format("    0x%04X,\n", m);
				}
			}
			w.println("};");
			w.println();
			
			w.println("/*  BoxMap");
			w.println(" *");
			w.println(" *      Each level box map is an 8 byte header with the number of boxes,");
			w.println(" *  followed by a byte per box, with the format XXXXYYYY.");
			w.println(" */");
			w.println();
			w.println("const static PROGMEM uint8_t BoxMap[] = {");
			for (Level l: levels) {
				w.println("    // " + l.levelName);
				w.println("    " + l.box.size() + ",");
				for (Pt pt: l.box) {
					w.format("    0x%X%X,\n",pt.getX(),pt.getY());
				}
			}
			w.println("};");
			w.println();
			
			w.println("/*  EndMap");
			w.println(" *");
			w.println(" *      Same format as BitMap; this gives the position where the ");
			w.println(" *  boxes should go.");
			w.println(" */");
			w.println();
			w.println("const static PROGMEM uint8_t EndMap[] = {");
			for (Level l: levels) {
				w.println("    " + l.end.size() + ",");
				for (Pt pt: l.end) {
					w.format("    0x%X%X,\n",pt.getX(),pt.getY());
				}
			}
			w.println("};");
			w.println();
			
			w.println("/*  StartPos");
			w.println(" *");
			w.println(" *      One byte per level in the same XXXXYYYY format as used with boxes.");
			w.println(" *  this gives the initial position of the player");
			w.println(" */");
			w.println();
			w.println("const static PROGMEM uint8_t StartPos[] = {");
			for (Level l: levels) {
				if (l.person != null) {
					w.format("    0x%X%X,\n", l.person.getX(),l.person.getY());
				} else {
					w.println("    0x73, // not set");
				}
			}
			w.println("};");
			w.println();
			
			w.flush();
			String out = baos.toString("UTF-8");
			textArea.setText(out);
		}
		catch (Exception ex) {
			
		}
	}
}
