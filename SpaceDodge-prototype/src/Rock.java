/////////////////////////|
//|File:Rock.java
//|Author:Jerrin C. Redmon
//|Language:Java (v.11)
//|Version:1.0
//|Date:October 3, 2022
/////////////////////////|

//Imports
import java.awt.Color;
import java.awt.Graphics;
import java.util.Random;

//___________________________________________________________________________

/**
 * Creates the rock obstacle in game and sets its properties
 * @author Jerrin C. Redmon
 * @version 1.0
 */
public class Rock {

	//Variables
	float velocity = 275.0f;
	final float ACCELERATION = 0.01f;
	int launchRock;
	float rockX;
	float rockY;
	public static final int ROCK_SIZE = 15;
	private Random random;
	
	/**
	 * Creates a new rock
	 */
	public Rock() {
		random = new Random();
		rockX = random.nextInt(650);
		rockY = 0;
	}
	/**
	 * Method to render the rock to the game window
	 * @param g Graphics
	 */
	public void renderRock(Graphics g) {
		g.setColor(Color.WHITE);
		g.fillRect((int) rockX, (int) rockY, ROCK_SIZE, ROCK_SIZE);	
	}
	
	/**
	 * Method for updating the rock's speed and direction
	 */
	public void updateRock() {
		velocity += ACCELERATION;
		rockY += velocity / Game.TICKS_PER_SEC;
		if (rockY > 600) {
			rockX = random.nextInt(650);
			rockY = 0;
		}
	}
}