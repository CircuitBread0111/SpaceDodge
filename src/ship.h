///////////////////////////|
//|File: ship.h
//|Author: Jerrin C. Redmon
//|Language: C
//|Version: 1.0
//|Date:January 9, 2023
///////////////////////////|

//----------------------------------------------------------------

#ifndef SHIP_H
#define SHIP_H

// max speed // 
#define SHIP_VELOCITY 130

typedef struct {
    TileSheet *tiles; 	// tilesheet for ship
    int direction; 		// current direction of ship movement
    float position; 	// current position of the ship
} Space_Ship;

Space_Ship *Space_Ship_Create(SDL_Renderer *renderer);

void Space_Ship_Destroy(Space_Ship *ship);

void Space_Ship_Reset(Space_Ship *ship);

void Space_Ship_Render(Space_Ship *ship, SDL_Renderer *renderer);

#endif
