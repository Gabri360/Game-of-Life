#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "render.h"
#include "logic.h"
#include "const.h"




int main(void) {

	//----------------------------------INIT
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *win = SDL_CreateWindow("game_of_life", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, 1920*SCALE, 1080*SCALE,
                                       SDL_WINDOW_RESIZABLE);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED |  SDL_RENDERER_PRESENTVSYNC);
    if (!win || !ren) {
        fprintf(stderr, "SDL: %s\n", SDL_GetError());
        return 1;
    }

	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);

	char font_path[1024];
    char *base = SDL_GetBasePath();
    snprintf(font_path, sizeof font_path, "%s%s", base ? base : "", FONT_FILE);
    SDL_free(base);
	if (TTF_Init() != 0) {
		fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
		return 1;
	}
	TTF_Font *font = TTF_OpenFont(font_path, 16);
	if (!font) {
		fprintf(stderr, "TTF_OpenFont (%s): %s\n", font_path, TTF_GetError());
		return 1;
	}



    int running = 1;
	Uint32 last_step = SDL_GetTicks();

	//--------------------------------LOOP
    while (running) {

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_KEYDOWN && !e.key.repeat) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE: running = 0; break;
				case SDLK_q: running = 0; break;
                }
            }
        }

		//--------------------------SIMULATION


		Uint32 now = SDL_GetTicks();
        while (now - last_step >= 100) {
            /* step(); */
            last_step += 100;
        }



		//-----------------------------PLOT

		draw_grid(ren, win);

		SDL_RenderPresent(ren);
	}

	TTF_CloseFont(font);
    TTF_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
