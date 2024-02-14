/////////////////////////|
//|File:Ship.java
//|Author:Jerrin C. Redmon
//|Language:Java (v.11)
//|Version:1.0
//|Date:October 3, 2022
/////////////////////////|

//Import
import java.awt.Color;
import java.awt.Graphics;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;

//___________________________________________________________________________

/**
 * Creates the ship which the user can control using the arrow keys
 * @author Jerrin C. Redmon
 * @version 1.0
 */
public class Ship extends KeyAdapter {

		//Variables
		final int SPEED = 150;
		float x;
		float y;
		float moveX;
		float moveY;
		int direction = 0;
		public static final int SHIP_SIZE = 35;
		final int MOVE_LEFT = 1;
		final int MOVE_RIGHT = 2;
		final int MOVE_UP = 4;
		final int MOVE_DOWN = 8;
		
		/**
		 * Constructs a new ship
		 * @param window_w game window width
		 * @param window_h game window height
		 */
		public Ship(int window_w, int window_h) {
			x = window_w;
			y = window_h;
			moveX = 0;
			moveY = 0;
		}
		
		/**
		 * Creates a ship to be rendered in the game window
		 * @param g Graphics
		 */
		public void renderShip(Graphics g) {
			g.setColor(Color.GRAY);
			g.fillRect((int) x, (int) y, SHIP_SIZE, SHIP_SIZE);
		}
		
		/**
		 * Method for updating the ship's position
		 */
		public void update() {
			moveX = 0;
			moveY = 0;
			if ((direction & MOVE_LEFT) != 0) moveX--;
			if ((direction & MOVE_RIGHT) != 0) moveX++;
			if ((direction & MOVE_UP) != 0) moveY--;
			if ((direction & MOVE_DOWN) != 0) moveY++;
			x += moveX * SPEED / Game.TICKS_PER_SEC;
			y += moveY * SPEED / Game.TICKS_PER_SEC;
		}
		
		/**
		 * Method for controlling the ship
		 */
		 public void keyPressed(KeyEvent e) {

		        int key = e.getKeyCode();

		        if (key == KeyEvent.VK_LEFT) {
		            direction |= MOVE_LEFT;
		        }

		        if (key == KeyEvent.VK_RIGHT) {
		            direction |= MOVE_RIGHT;
		        }

		        if (key == KeyEvent.VK_UP) {
		            direction |= MOVE_UP;
		        }

		        if (key == KeyEvent.VK_DOWN) {
		            direction |= MOVE_DOWN;
		        }
		    }
		 	
		 	/**
		 	 * Method for controlling the ship
		 	 */
		    public void keyReleased(KeyEvent e) {
		        
		        int key = e.getKeyCode();

		        if (key == KeyEvent.VK_LEFT) {
		            direction &= ~MOVE_LEFT;
		        }

		        if (key == KeyEvent.VK_RIGHT) {
		        	direction &= ~MOVE_RIGHT;
		        }

		        if (key == KeyEvent.VK_UP) {
		            direction &= ~MOVE_UP;
		        }

		        if (key == KeyEvent.VK_DOWN) {
		        	direction &= ~MOVE_DOWN;
		        }
		    }
		}