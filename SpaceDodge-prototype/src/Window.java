/////////////////////////|
//|File:Window.java
//|Author:Jerrin C. Redmon
//|Language:Java (v.11)
//|Version:1.0
//|Date:October 3, 2022
/////////////////////////|

//Imports
import javax.swing.JButton;
import javax.swing.JFrame;
import java.awt.Canvas;
import java.awt.Dimension;
import java.awt.image.BufferStrategy;

//___________________________________________________________________________

/**
 * Creates the JFrame and canvas for the game window
 * @author Jerrin C. Redmon
 * @version 1.0
 */
public class Window {

	//Variables
	public JFrame frame; 
	public JButton button;
	private Canvas canvas;
	private String title = "Controls: Use the arrow keys for movement ";
	private int width = 640;
	private int height = 480;
	public final BufferStrategy bs;
	
	/**
	 * Constructs a new window
	 */
	public Window() {
		frame = new JFrame(title);
		button = new JButton("restart");
		frame.setSize(width,height);
		frame.setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		frame.setResizable(false);
		frame.setLocationRelativeTo(null);
		frame.setVisible(true);
		canvas = new Canvas();
		canvas.setPreferredSize(new Dimension(width, height));
		canvas.setMaximumSize(new Dimension(width, height));
		canvas.setMinimumSize(new Dimension(width, height));
		frame.add(canvas);
		frame.pack();
		canvas.createBufferStrategy(2);
		bs = canvas.getBufferStrategy();
	}
	/**
	 * Method which allows access to the canvas
	 * @return canvas
	 */
	public Canvas getCanvas() {//allows access to canvas
		return canvas;
	}
}
