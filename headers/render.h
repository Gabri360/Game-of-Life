#ifndef RENDER_H
#define RENDER_H


#include <SDL.h>
#include <SDL_ttf.h>

void draw_text(SDL_Renderer *ren, TTF_Font *font, const char *text, int x, int y, int px, SDL_Color color);

void draw_grid(SDL_Renderer *ren, SDL_Window *win);

#endif
