#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL/SDL.h>
#include <map.h>
#include <constant.h>

// load game's sprites
void		 sprite_load();

SDL_Surface* sprite_get_player(enum direction direction);
SDL_Surface* sprite_get_monster(enum direction direction);
SDL_Surface* sprite_get_bomb(short type);
SDL_Surface* sprite_get_menu(short type);
SDL_Surface* sprite_get_menu_h();
SDL_Surface* sprite_get_bonus(enum bonus_type bonus_type);
SDL_Surface* sprite_get_fire(short type);
SDL_Surface* sprite_get_completed();
SDL_Surface* sprite_get_victory();
SDL_Surface* sprite_get_tree();
SDL_Surface* sprite_get_box();
SDL_Surface* sprite_get_key();
SDL_Surface* sprite_get_stone();
SDL_Surface* sprite_get_door();
SDL_Surface* sprite_get_closed_door();

SDL_Surface* sprite_get_number(short number);
SDL_Surface* sprite_get_banner_life();
SDL_Surface* sprite_get_banner_bomb();
SDL_Surface* sprite_get_banner_line();
SDL_Surface* sprite_get_banner_range();

#endif /* SPRITE_H_ */
