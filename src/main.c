///////////////////////////|
//|File: main.c
//|Author: Jerrin C. Redmon
//|Language: C
//|Version: 1.0
//|Date:January 9, 2023
///////////////////////////|

/*
 * Description:
 * Space Dodge Computer Game
 */

//----------------------------------------------------------------

// Includes //
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// SDL2 //
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

// Header Files //
#include "tilesheet.h"
#include "font.h"
#include "ship.h"
#include "shared.h"

// Defines //
#define TOTAL_ROCKS 15
#define ASTEROID_ACCEL 2
#define TITLE_SCALE 3
#define GAME_OVER_SCALE 2
#define BG_VELOCITY 100

// Asteroid Struct //
struct Asteroid {
    float x,y;		// x and y position of asteroid
    float velocity;	// Speed which the asteroid move
    int size;		// Size of the asteroid
};

// player and asteroids //
static struct Asteroid rocks[TOTAL_ROCKS];
static Space_Ship *player;
static Mix_Chunk *intro, *points, *boom;

// Renderer instance //
static SDL_Renderer *renderer;

// Font //
static Font *font;

// Background //
static SDL_Texture *bg;

// Score //
Sint32 high_score = 0;


// Initialize the player and asteroids //
static void init(void) {

	Mix_PlayChannel(1, intro, 0);
    	Space_Ship_Reset(player);
    	for (size_t i = 0; i < TOTAL_ROCKS; ++i) {
			rocks[i] = (struct Asteroid) {
            	.velocity = rand() % 100 + 50,
           	 	.size = rand() % 5 + 5 // range from 5-15
       		 };
        rocks[i].x = rand() % (WIDTH - rocks[i].size);
        rocks[i].y = -(rand() % HEIGHT) - rocks[i].size;
		}
}


// Draws asteroids //
static void draw_rock(void) {

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (size_t i = 0; i < TOTAL_ROCKS; ++i) {
        SDL_Rect rock = {
            .x = rocks[i].x,
            .y = rocks[i].y,
            .w = rocks[i].size,
            .h = rocks[i].size
        };
        SDL_RenderFillRect(renderer, &rock);
    }
}


// High Score //
static Sint32 get_hscore(void) {

    FILE *fptr = fopen("scores", "r");
    if (!fptr) return 0;
    Uint32 hs = 0;
    fread(&hs, sizeof(hs), 1, fptr);
    fclose(fptr);
    return hs;
}


static void set_hscore(Sint32 score) {

    FILE *fptr = fopen("scores", "w");
    if (!fptr) return;
    Uint32 hs = 0;
    if (fread(&hs, sizeof(hs), 1, fptr) == 0 || score > hs) {
        rewind(fptr);
        fwrite(&score, sizeof(score), 1, fptr);
    }
    fclose(fptr);
}


// Render title screen //
static void title_screen(void) {

	bool quit = false, title = true;
	SDL_Surface *surface = SDL_LoadBMP("Images/title.bmp");
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);
	int tex_w, tex_h;
	SDL_QueryTexture(texture, NULL, NULL, &tex_w, &tex_h);
	const SDL_Rect title_rect = {
		.x = WIDTH / 2 - tex_w * TITLE_SCALE / 2,
		.y = HEIGHT / 3 - tex_h * TITLE_SCALE / 2,
		.w = tex_w * TITLE_SCALE,
		.h = tex_h * TITLE_SCALE
	};
	const char *title_text = "PRESS ENTER TO PLAY";
	const size_t title_text_len = strlen(title_text);
	const SDL_Point text_point = {
		.x = WIDTH / 2 - title_text_len * 4,
		.y = HEIGHT / 2
	};
	while (!quit && title) {
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT: {
					quit = true;
				} break;
				case SDL_KEYDOWN: {
					switch (e.key.keysym.sym) {
						case SDLK_ESCAPE: {
							quit = true;
						} break;
						case SDLK_RETURN: {
							title = false;
						} break;
					}
				} break;
			}
		}
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, &title_rect);
		Font_renderText(font, renderer, &text_point, title_text);
		SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(texture);
	if (quit) exit(0);
}


// Game physics //
static void physics(Uint64 delta_t) {

	player->position += player->direction * SHIP_VELOCITY * (delta_t / 1000.0f);
	if (player->position < 0) player->position = 0;
	else if (player->position > WIDTH - player->tiles->tile_width) player->position = WIDTH - player->tiles->tile_width;
	for (size_t i = 0; i < TOTAL_ROCKS; ++i) {
		rocks[i].y += rocks[i].velocity * (delta_t / 1000.0f);
		rocks[i].velocity += ASTEROID_ACCEL * (delta_t / 1000.0f);
		if (rocks[i].y > HEIGHT) {
			rocks[i].y = -rocks[i].size;
			rocks[i].x = rand() % (WIDTH - rocks[i].size);
		}
	}
}


// Collisions //
static bool collision(void) {
	SDL_Rect ship_rect = {player->position, HEIGHT - 25, player->tiles->tile_width, player->tiles->tile_height};
	for (size_t i = 0; i < TOTAL_ROCKS; ++i) {
		SDL_Rect rock_rect = {rocks[i].x, rocks[i].y, rocks[i].size, rocks[i].size};
		if (SDL_HasIntersection(&ship_rect, &rock_rect)) {
			Mix_PlayChannel(-1, boom, 0);
			return true;
		}
	}
	return false;
}


// Renders Background //
static void render_bg(int pos) {
	SDL_Rect bg_rect[2] = {
		{.x = 0, .y = pos - HEIGHT, .w = WIDTH, .h = HEIGHT},
		{.x = 0, .y = pos, .w = WIDTH, .h = HEIGHT}
	};
	SDL_RenderCopy(renderer, bg, NULL, bg_rect);
	SDL_RenderCopy(renderer, bg, NULL, bg_rect + 1);
}


// Game Loop //
static void game_loop(void) {

	// scroll background //
	SDL_Surface *bg_surface = SDL_LoadBMP("Images/space.bmp");
	bg = SDL_CreateTextureFromSurface(renderer, bg_surface);
	SDL_FreeSurface(bg_surface);
	float bg_pos = 0;
	
	// game over screen //
	SDL_Surface *game_over_surface = SDL_LoadBMP("Images/game.bmp");
	SDL_Texture *game_over_texture = SDL_CreateTextureFromSurface(renderer, game_over_surface);
	SDL_FreeSurface(game_over_surface);
	int game_over_w, game_over_h;
	SDL_QueryTexture(game_over_texture, NULL, NULL, &game_over_w, &game_over_h);
	SDL_Rect game_over_rect = {
		.x = WIDTH / 2 - game_over_w * GAME_OVER_SCALE / 2,
		.y = HEIGHT / 2 - game_over_h * GAME_OVER_SCALE / 2,
		.w = game_over_w * GAME_OVER_SCALE,
		.h = game_over_h * GAME_OVER_SCALE
	};
	SDL_Point high_score_point = {
		.x = WIDTH - 10 * 8,
		.y = 0
	};
	
	// Initializes game //
	init();
   	srand(time(NULL)); //<--
	Uint64 game_time = SDL_GetTicks64();
	Uint64 points_timer = 0, start_time = 0, score = 0;
	bool quit = false;
	bool game_over = false;
	while (!quit) {
		Uint64 prev_time = game_time;
		game_time = SDL_GetTicks64();
		Uint64 delta_t = game_time - prev_time;

		// Sets player control keys //
		SDL_Event e;
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
				case SDL_QUIT: {
					quit = true;
				} break;
				case SDL_KEYDOWN: {
					if (!e.key.repeat) switch (e.key.keysym.sym) {
						case SDLK_ESCAPE: {
							quit = true;
						} break;
						case SDLK_LEFT: {
							--player->direction;
						} break;
						case SDLK_RIGHT: {
							++player->direction;
						} break;
						case SDLK_r: {
							game_over = false;
							init();
						} break;
					}
				} break;
				case SDL_KEYUP: {
					if (!e.key.repeat) switch (e.key.keysym.sym) {
						case SDLK_LEFT: {
							++player->direction;
						} break;
						case SDLK_RIGHT: {
							--player->direction;
						} break;
					}
				} break;
			}
        }

	// Render Graphics //
        if (game_over) {
			bg_pos += BG_VELOCITY * (delta_t / 1000.0);
		  	if (bg_pos >= 320) bg_pos -= 320;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            render_bg(bg_pos);
            SDL_RenderCopy(renderer, game_over_texture, NULL, &game_over_rect);
            Font_renderFormatted(font, renderer, NULL, "SCORE\n%lld", score);
            Font_renderFormatted(font, renderer, &high_score_point, "HIGH SCORE\n%010d", high_score);
        } 
        else {
            if (!Mix_Playing(1)) {
                points_timer += delta_t;
                if (points_timer >= 10000) {
                	Mix_PlayChannel(-1, points, 0);
                	points_timer -= 10000;
                }
                physics(delta_t);
		bg_pos += BG_VELOCITY * (delta_t / 1000.0);
          	if (bg_pos >= 320) bg_pos -= 320;
            } else {
                start_time = game_time;
                points_timer = 0;
            }
            score = (game_time - start_time) / 10;
            if (score > high_score) high_score = score;
            game_over = collision();
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            render_bg(bg_pos);
            Font_renderFormatted(font, renderer, NULL, "SCORE\n%lld", score);
            Font_renderFormatted(font, renderer, &high_score_point, "HIGH SCORE\n%010d", high_score);
            Space_Ship_Render(player, renderer);
            draw_rock();
		}
        SDL_RenderPresent(renderer);
	}
	SDL_DestroyTexture(game_over_texture);
	SDL_DestroyTexture(bg);
}


// Main //
int main(int argc, char *argv[]) {

	// Creates game window //
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
		return 1;
	}
	SDL_Window *window = SDL_CreateWindow(
	    "Space Dodge",										// title
	    SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,	// window position
	    WIDTH * SCALE, HEIGHT * SCALE,						// window size
	    SDL_RENDERER_PRESENTVSYNC							// window flags
    );
	if (!window) {
		fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
		return 1;
	}
	
	// Create Renderer //
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
		return 1;
	}
    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT);

	// Create Mixer //
	if (Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 2048) < 0) {
	    fprintf(stderr, "Mix_OpenAudio: %s\n", SDL_GetError());
	    return 1;
	}
	intro = Mix_LoadWAV("Music/Sounds/intro.wav");
	if (!intro) {
	    fprintf(stderr, "Could not load left sound: %s\n", SDL_GetError());
	    return 1;
	}
	points = Mix_LoadWAV("Music/Sounds/points.wav");
	if (!intro) {
	    fprintf(stderr, "Could not load left sound: %s\n", SDL_GetError());
	    return 1;
	}
	boom = Mix_LoadWAV("Music/Sounds/boom.wav");
	if (!intro) {
	    fprintf(stderr, "Could not load left sound: %s\n", SDL_GetError());
	    return 1;
	}
	
	// Create Font //
	font = Font_create("Images/font.bmp", renderer, 1);
	if (!font) {
	    fprintf(stderr, "Could not load font: %s\n", SDL_GetError());
	    return 1;
	}
	
	// Create Player //
	player = Space_Ship_Create(renderer);
	if (!player) {
		fprintf(stderr, "Could not create ship: %s\n", SDL_GetError());
	    return 1;
	}
	
	// Game Program //
	title_screen();
	high_score = get_hscore();
	game_loop();
	set_hscore(high_score);
	
	// End of Game Program //
	Space_Ship_Destroy(player);
	Font_destroy(font);
	Mix_FreeChunk(intro);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_Quit();
	SDL_Quit();
	return 0;
}
