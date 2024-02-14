///////////////////////////|
//|File: ship.c
//|Author: Jerrin C. Redmon
//|Language: C
//|Version: 1.0
//|Date:January 9, 2023
///////////////////////////|

/*
 * Description:
 * creates player ship
 */
 
//----------------------------------------------------------------

// Includes //
#include <SDL2/SDL.h>

// Header Files //
#include "tilesheet.h"
#include "ship.h"
#include "shared.h"

// Create player ship //
Space_Ship *Space_Ship_Create(SDL_Renderer *renderer) {
	Space_Ship *ship = malloc(sizeof(Space_Ship));
	ship->tiles = TileSheet_create("Images/ship.bmp", renderer, 16, 16, TILESHEET_CREATETEXTURE);
	if (!ship->tiles) {
		free(ship);
		return NULL;
	}
	
	Space_Ship_Reset(ship);
	return ship;
}

// Destroy Ship //
void Space_Ship_Destroy(Space_Ship *ship) {
	TileSheet_destroy(ship->tiles);
	free(ship);
}

void Space_Ship_Reset(Space_Ship *ship) {
	ship->direction = 0;
    ship->position = WIDTH / 2;
}

// Draws/renders player ship //
void Space_Ship_Render(Space_Ship *ship, SDL_Renderer *renderer) {
    SDL_Rect dst_rect = {ship->position, HEIGHT - 25, ship->tiles->tile_width, ship->tiles->tile_height};
    SDL_Rect src_rect = TileSheet_getTileRect(ship->tiles, 0);
	SDL_RenderCopy(renderer, ship->tiles->texture, &src_rect, &dst_rect);
}
