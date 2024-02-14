/////////////////////////|
//|File:Launcher.java
//|Author:Jerrin C. Redmon
//|Language:Java (v.11)
//|Version:1.0
//|Date:October 3, 2022
/////////////////////////|

//Imports
import javax.swing.JFrame;
import javax.swing.JButton;
import javax.swing.JPanel;
import javax.swing.Icon;
import javax.swing.ImageIcon;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

//___________________________________________________________________________

/**
 * Main file that is in charge with launching the game
 * @author Jerrin C. Redmon
 * @version 1.0
 */
public class Launcher{
	
	//Variables
	public JFrame frame; 
	public JButton button, button2;
	public JPanel panel;
	public Game game;
	public Thread gameThread;
	public Window window;
	public Icon icon;
	private String title = "SPACE DODGE : How long can you survive?";
	private int width = 640;
	private int height = 480;

	/**
	 * Main method
	 * @param args
	 */
	public static void main(String[] args) {
		@SuppressWarnings("unused")
		Launcher launcher = new Launcher();
	}
	
	/**
	 * Constructor method for creating the window
	 */
	public Launcher() {
		frame = new JFrame(title);
		panel = new JPanel();
		icon = new ImageIcon("textures/SDTitle.png");
		button = new JButton(icon);
		panel.add(button);
		frame.setSize(width,height);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		frame.setResizable(false);
		frame.setLocationRelativeTo(null);
		frame.add(panel);
		frame.setVisible(true);
		button.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				game = new Game("test",640,480);
				gameThread = new Thread(game);
				game.init();
				gameThread.start();
			}
		});
	}
}