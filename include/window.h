#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL/SDL.h>

void window_create(int width, int height);
void window_free();

void window_refresh();

// display a SDL_Surface at location (x,y)
void window_display_image(SDL_Surface* surface, int x, int y);

// display the sprite from x0, y0 to x1, y1, as many time as needed
void window_display_array(int x0, int y0, int x1, int y1, SDL_Surface* sprite);

// set every pixel of the window to white
void window_clear();

#endif /* WINDOW_H_ */
