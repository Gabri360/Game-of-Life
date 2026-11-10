#include <SDL.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "render.h"
#include "logic.h"
#include "const.h"


static int cell_at(int p, int total, int n)
{
    if (p < 0 || p >= total) return -1;
    return (int)((((long long)p + 1) * n - 1) / total);
}

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



	int state[WIDTH][HEIGHT];
	init_state(state);

    int running = 1, life = 0;
	Uint32 last_step = SDL_GetTicks();

	//--------------------------------LOOP
    while (running) {

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q)) running = 0;
            if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_RETURN)) life = !life;
			if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && life == 0) {
				int w, h;
				SDL_GetWindowSize(win, &w, &h);
				int gw = w - 2 * padding;
				int gh = h - 2 * padding;

				int i = cell_at(e.button.x - padding, gw, WIDTH);
				int j = cell_at(e.button.y - padding, gh, HEIGHT);
				if (i >= 0 && j >= 0)
					state[i][j] = !state[i][j];
			}
			if (e.type == SDL_MOUSEMOTION && (e.motion.state & SDL_BUTTON_LMASK) && life == 0) {
				int w, h;
				SDL_GetWindowSize(win, &w, &h);
				int i = cell_at(e.motion.x - padding, w - 2 * padding, WIDTH);
				int j = cell_at(e.motion.y - padding, h - 2 * padding, HEIGHT);
				if (i >= 0 && j >= 0) state[i][j] = 1;   // qui imposta a 1, non inverte
			}
        }

		//--------------------------SIMULATION


		Uint32 now = SDL_GetTicks();
        while (now - last_step >= 100) {
            if(life) {
				game_step(state);
			}
            last_step += 100;
        }


		//-----------------------------PLOT

		draw_grid(ren, win);
		draw_state(ren, win, state);

		SDL_RenderPresent(ren);
	}

	TTF_CloseFont(font);
    TTF_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
