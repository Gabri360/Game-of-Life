#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "render.h"
#include "logic.h"


#define SCALE 0.9
#define FPS 60
#define MAX_STEPS_PER_FRAME 4096
#define FONT_FILE "assets/DejaVuSansMono.ttf"
#define FONT_PX 14

int main(void) {

	//----------------------------------INIT
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    SDL_Window *win = SDL_CreateWindow("game_of_life", SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED, 1920*SCALE, 1080*SCALE,
                                       SDL_WINDOW_RESIZABLE);
    SDL_Renderer *ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!win || !ren) {
        fprintf(stderr, "SDL: %s\n", SDL_GetError());
        return 1;
    }

	char font_path[1024];
    char *base = SDL_GetBasePath();
    snprintf(font_path, sizeof font_path, "%s%s", base ? base : "", FONT_FILE);
    SDL_free(base);

	Uint64 freq = SDL_GetPerformanceFrequency();
	Uint64 frame_ticks = freq / FPS;
	Uint64 next_frame = SDL_GetPerformanceCounter();



    int running = 1;

	//--------------------------------LOOP
    while (running) {

        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
            if (e.type == SDL_KEYDOWN && !e.key.repeat) {
                switch (e.key.keysym.sym) {
                case SDLK_ESCAPE: running = 0; break;
                }
            }
        }

		//--------------------------SIMULATION





		//-----------------------------PLOT

		SDL_SetRenderDrawColor(ren, 20, 20, 25, 255);
		SDL_RenderClear(ren);



		SDL_RenderPresent(ren);
        //-----------------------------FPS
        next_frame += frame_ticks;
        Uint64 now = SDL_GetPerformanceCounter();

        if (now < next_frame) {
            double ms = (double)(next_frame - now) * 1000.0 / (double)freq;
            if (ms > 1.0) SDL_Delay((Uint32)(ms - 1.0));
            while (SDL_GetPerformanceCounter() < next_frame) {}
        } else if (now - next_frame > frame_ticks * 5) {
            next_frame = now;
        }
	}

	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(win);
	SDL_Quit();
	return 0;
}
