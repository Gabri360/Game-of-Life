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


	SDL_Color text_color = {200, 200, 200, 255};
	SDL_Color state_color = {50, 150, 60, 255};
	SDL_Color state2_color = {50, 150, 60, 100};

	int state[WIDTH][HEIGHT];
	int next_state[WIDTH][HEIGHT];
	init_state(state);
	init_state(next_state);

    int running = 1, life = 0, gen = 0;
	Uint32 last_step = SDL_GetTicks();
	Uint32 t = 100;

	//--------------------------------LOOP
    while (running) {

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_ESCAPE || e.key.keysym.sym == SDLK_q)) running = 0;
            if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_RETURN)) life = !life;
			if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_UP)) t = (Uint32)fmax(10,t-10);
			if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_DOWN)) t = (Uint32)fmin(200,t+10);
			if (e.type == SDL_KEYDOWN && (e.key.keysym.sym == SDLK_r)) {init_state(state); init_state(next_state); life = 0; gen = 0;}
			if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT && life == 0) {
				int w, h;
				SDL_GetWindowSize(win, &w, &h);
				int gw = w - 2 * padding;
				int gh = h - 2 * padding;

				int i = cell_at(e.button.x - padding, gw, WIDTH);
				int j = cell_at(e.button.y - padding, gh, HEIGHT);
				if (i >= 0 && j >= 0)
					next_state[i][j] = !next_state[i][j];
			}
			if (e.type == SDL_MOUSEMOTION && (e.motion.state & SDL_BUTTON_LMASK) && life == 0) {
				int w, h;
				SDL_GetWindowSize(win, &w, &h);
				int i = cell_at(e.motion.x - padding, w - 2 * padding, WIDTH);
				int j = cell_at(e.motion.y - padding, h - 2 * padding, HEIGHT);
				if (i >= 0 && j >= 0) next_state[i][j] = 1;
			}
        }

		//--------------------------SIMULATION


		Uint32 now = SDL_GetTicks();

        while (now - last_step >= t) {
            if(life) {
				copy_state(state,next_state);
				game_step(state, next_state);
				gen++;
			}
            last_step += t;
        }


		//-----------------------------PLOT

		draw_grid(ren, win);
		draw_state(ren, win, next_state, state_color);
		draw_state(ren, win, state, state2_color);


		float fps = 1000.0f/(float)t;
		int scale = 20;
		char buf[64];
		snprintf(buf, sizeof buf, "%.1f FPS", fps);
		draw_text(ren, font, buf, padding, padding, scale, text_color);

		if(life)
			snprintf(buf, sizeof buf, "Running");
		else
			snprintf(buf, sizeof buf, "Pause");
		draw_text(ren, font, buf, padding, padding + (int)(scale * 1.1), scale, text_color);

		snprintf(buf, sizeof buf, "Gen: %d", gen);
		draw_text(ren, font, buf, padding, padding + (int)(scale * 1.1)*2, scale, text_color);

		SDL_RenderPresent(ren);
	}

	TTF_CloseFont(font);
    TTF_Quit();
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
