/////////////////////////|
//|File:Game.java
//|Author:Jerrin C. Redmon
//|Language:Java (v.11)
//|Version:1.0
//|Date:October 3, 2022
/////////////////////////|

//Imports
import java.awt.Color;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Rectangle;
import java.awt.Toolkit;
import java.util.Random;

//___________________________________________________________________________

/**
 * Game represent the game engine, creates the windows, renders the graphics
 * checks the ticks and provide the game loop.
 * @author Jerrin C. Redmon
 * @version 1.0
 */
public class Game implements Runnable {
	
	//Variables
	public final static int TICKS_PER_SEC = 60;
	private boolean running;
	private Window display;
	public int width; 
	public int height;
	public String title;
	public Random random;
	private long startTime;
	private int score = 0;
	private boolean gameStatus;
	Ship control;
	Rock[] rocky;
	
	/**
	 * @param title sets title of the game window
	 * @param width sets width of the game window
	 * @param height sets height of the game window
	 */
	public Game(String title, int width, int height) {
		this.width = width;
		this.height = height;
		this.title = title;
		running = true;
		gameStatus = true;
	}
	
	/**
	 * Initializes the the game
	 */
	public void init() {
		display = new Window(); 
		control = new Ship(275, 400);
		display.frame.addKeyListener(control); 
		rocky = new Rock[10];
		for (int i = 0; i < 10; i++) {
			rocky[i] = new Rock();		
		}
	}
	
	/**
	 * Method which renders the graphics to the game window
	 */
	private void render() {
		Graphics g = display.bs.getDrawGraphics();
		g.clearRect(0, 0, width, height);
		g.setColor(Color.BLACK);
		g.fillRect(0,0, width, height);
		g.setColor(Color.BLUE);
		g.setFont(Font.decode("Monospaced 20"));
		g.drawString(String.format("SCORE: %d", score), 0, 20);
		control.renderShip(g);
		for (int i = 0; i < 10; i++)
			rocky[i].renderRock(g);
		if (!gameStatus) {
			gameOver(g);
		}
		g.dispose();
		display.bs.show();
	}
	
	/**
	 * Method which updates the game every tick
	 */
	private void update() {
		control.update();
		Rectangle ship = new Rectangle((int) control.x, (int) control.y, Ship.SHIP_SIZE, Ship.SHIP_SIZE);
		for (int i = 0; i < 10; i++) {
			rocky[i].updateRock();
			Rectangle rock = new Rectangle((int) rocky[i].rockX, (int) rocky[i].rockY, Rock.ROCK_SIZE, Rock.ROCK_SIZE);
			if (ship.intersects(rock)) {
				gameStatus = false;
			}
		}		
	}
	
	/**
	 * Method which ends the game
	 */
	public void stop() {
		running = false;
	}
	
	/**
	 * Method which draws the game over screen
	 * @param g Graphics 
	 */
	private void gameOver(Graphics g) {
		g.setColor(Color.BLACK);
		g.fillRect(0,0, width, height);
		g.setColor(Color.RED);
		g.setFont(Font.decode("Monospaced 50"));
		g.drawString("GAME OVER", 175,200);
		g.setColor(Color.BLUE);
		g.setFont(Font.decode("Monospaced 20"));
		g.drawString(String.format("SCORE: %d", score), 250, 250);
		stop();		
	}
	
	/**
	 * Runs the thread
	 */
	public void run() {
		double timePerTick = 1000000000/TICKS_PER_SEC;
		double delta = 0;
		long now;
		long lastTime = System.nanoTime();
		startTime = lastTime;
		while(running) {
			now = System.nanoTime();
			delta += (now - lastTime) / timePerTick;
			lastTime = now;
			if(delta >= 1) {
				update();
				generateScore(now - startTime);
				render();
				Toolkit.getDefaultToolkit().sync();
				delta--;
			}
		}
	}
	
	/**
	 * summary Method which keeps trick of the score
	 * @param time The game timer
	 */
	private void generateScore(long time) {
		score = (int) (time * 10e-9);
	}
	
}
