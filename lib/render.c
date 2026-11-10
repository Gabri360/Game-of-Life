#include "render.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


void draw_text(SDL_Renderer *ren, TTF_Font *font, const char *text, int x, int y, int px, SDL_Color color)
{
    TTF_SetFontSize(font, px);
    SDL_Surface *surf = TTF_RenderUTF8_Blended(font, text, color);
    if (!surf) return;
    SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, surf);
    SDL_Rect dst = { x, y, surf->w, surf->h };
    SDL_FreeSurface(surf);
    if (!tex) return;
    SDL_RenderCopy(ren, tex, NULL, &dst);
    SDL_DestroyTexture(tex);
}

void draw_grid(SDL_Renderer *ren, SDL_Window *win)
{

	int w, h;
	SDL_GetWindowSize(win, &w, &h);

	SDL_SetRenderDrawColor(ren, 20, 20, 25, 255);
	SDL_RenderClear(ren);

	SDL_SetRenderDrawColor(ren, 50, 50, 60, 255);
	SDL_Rect r = {padding, padding, w - 2 * padding , h - 2 * padding };
	SDL_RenderFillRect(ren, &r);
	SDL_SetRenderDrawColor(ren, 50, 150, 60, 255);
	SDL_RenderDrawRect(ren, &r);


	SDL_SetRenderDrawColor(ren, 50, 150, 60, 255);
	int cell_width = (w - 2*padding);
	int cell_height = (h - 2*padding);

	int x;
	for(int i=1;i<WIDTH;i++) {
		x = padding + (int)(((long long)cell_width*i)/WIDTH);
		SDL_RenderDrawLine(ren, x, padding, x, h - padding);
	}
	for(int i=1;i<HEIGHT;i++) {
		x = padding + (int)(((long long)cell_height*i)/HEIGHT);
		SDL_RenderDrawLine(ren, padding, x, w - padding, x);
	}

}


void draw_state(SDL_Renderer *ren, SDL_Window *win, int state[][HEIGHT])
{
	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	int gw = w - 2*padding;
	int gh = h - 2*padding;

	SDL_SetRenderDrawColor(ren, 50, 150, 60, 255);
	for (int i = 0; i < WIDTH; i++) {
		int x0 = padding + (int)(((long long)gw * i) / WIDTH);
		int x1 = padding + (int)(((long long)gw * (i + 1)) / WIDTH);
		for (int j = 0; j < HEIGHT; j++) {
			if (!state[i][j]) continue;
			int y0 = padding + (int)(((long long)gh * j) / HEIGHT);
			int y1 = padding + (int)(((long long)gh * (j + 1)) / HEIGHT);
			SDL_Rect r = { x0, y0, x1 - x0, y1 - y0 };
			SDL_RenderFillRect(ren, &r);
		}
	}
}
