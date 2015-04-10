#include <SDL/SDL_image.h>
#include <assert.h>

#include <sprite.h>
#include <misc.h>

// Sprites general
#define MAP_CASE        "sprite/wood_box.png"
#define MAP_KEY			"sprite/key.png"
#define MAP_OPENED_DOOR	"sprite/door.png"
#define MAP_CLOSED_DOOR	"sprite/closed_door.png"

// Monsters
#define MNST_LEFT     "sprite/monster_left.png"
#define MNST_UP       "sprite/monster_up.png"
#define MNST_RIGHT    "sprite/monster_right.png"
#define MNST_DOWN     "sprite/monster_down.png"

// Bombs
#define BOMB1   	"sprite/bomb1.png"
#define BOMB2  	  	"sprite/bomb2.png"
#define BOMB3  	 	"sprite/bomb3.png"
#define BOMB4  		"sprite/bomb4.png"

// Scenery elements
#define MAP_STONE		"sprite/stone.png"
#define MAP_TREE        "sprite/tree.png"

// Sprites of Banner
#define BANNER_LINE		"sprite/banner_line.png"
#define BANNER_LIFE		"sprite/banner_life.png"
#define BANNER_BOMB		"sprite/bomb3.png"
#define BANNER_RANGE	"sprite/banner_range.png"
#define BANNER_0		"sprite/banner_0.jpg"
#define BANNER_1		"sprite/banner_1.jpg"
#define BANNER_2		"sprite/banner_2.jpg"
#define BANNER_3		"sprite/banner_3.jpg"
#define BANNER_4		"sprite/banner_4.jpg"
#define BANNER_5		"sprite/banner_5.jpg"
#define BANNER_6		"sprite/banner_6.jpg"
#define BANNER_7		"sprite/banner_7.jpg"
#define BANNER_8		"sprite/banner_8.jpg"
#define BANNER_9		"sprite/banner_9.jpg"

// Sprites of Bombs
#define BOMB_TTL1       "sprite/bomb1.png"
#define BOMB_TTL2       "sprite/bomb2.png"
#define BOMB_TTL3       "sprite/bomb3.png"
#define BOMB_TTL4       "sprite/bomb4.png"

// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"
#define IMG_BONUS_LIFE    		  "sprite/bonus_life.png"

// Sprite for the fire
#define MAP_FIRE_1 	"sprite/fire1.png"
#define MAP_FIRE_2 	"sprite/fire2.png"

// Sprites of Players
#define PLAYER_LEFT     "sprite/player_left.png"
#define PLAYER_UP       "sprite/player_up.png"
#define PLAYER_RIGHT    "sprite/player_right.png"
#define PLAYER_DOWN     "sprite/player_down.png"

// Sprites of game states
#define IMG_STATE_COMPLETED "sprite/completed.png"
#define IMG_STATE_VICTORY 	"sprite/victory.png"
#define IMG_MENU_1			"sprite/first_menu.png"
#define IMG_MENU_2			"sprite/second_menu.png"
#define IMG_GAME_OVER		"sprite/game_over.png"

// banner
SDL_Surface* numbers[10];
SDL_Surface* banner_life;
SDL_Surface* banner_bomb;
SDL_Surface* banner_range;
SDL_Surface* banner_line;

// map
SDL_Surface* box;
SDL_Surface* goal;
SDL_Surface* key;
SDL_Surface* opened_door;
SDL_Surface* closed_door;
SDL_Surface* stone;
SDL_Surface* tree;
SDL_Surface* monster_img[4];
SDL_Surface* bomb_img[4];
SDL_Surface* fire[2];
SDL_Surface* completed;
SDL_Surface* menu[3];
SDL_Surface* state_victory;

// bonus
#define NB_BONUS 5
SDL_Surface* bonus[NB_BONUS];

// player
SDL_Surface* player_img[4];

void banner_load() {
	// numbers imgs
	numbers[0] = load_image(BANNER_0);
	numbers[1] = load_image(BANNER_1);
	numbers[2] = load_image(BANNER_2);
	numbers[3] = load_image(BANNER_3);
	numbers[4] = load_image(BANNER_4);
	numbers[5] = load_image(BANNER_5);
	numbers[6] = load_image(BANNER_6);
	numbers[7] = load_image(BANNER_7);
	numbers[8] = load_image(BANNER_8);
	numbers[9] = load_image(BANNER_9);

	// other banner sprites
	banner_life = load_image(BANNER_LIFE);
	banner_bomb = load_image(BANNER_BOMB);
	banner_range = load_image(BANNER_RANGE);
	banner_line = load_image(BANNER_LINE);
}

void banner_unload() {
	// numbers imgs
	for (int i = 0; i < 10; i++) {
		SDL_FreeSurface(numbers[i]);
	}

	// other banner sprites
	SDL_FreeSurface(banner_life);
	SDL_FreeSurface(banner_bomb);
	SDL_FreeSurface(banner_range);
	SDL_FreeSurface(banner_life);
}

void map_load() {
	// Sprite loading
	tree = load_image(MAP_TREE);
	box = load_image(MAP_CASE);
	key = load_image(MAP_KEY);
	stone = load_image(MAP_STONE);
	opened_door = load_image(MAP_OPENED_DOOR);
	closed_door = load_image(MAP_CLOSED_DOOR);
}

void map_unload() {
	SDL_FreeSurface(tree);
	SDL_FreeSurface(box);
	SDL_FreeSurface(goal);
	SDL_FreeSurface(key);
	SDL_FreeSurface(stone);
	SDL_FreeSurface(opened_door);
	SDL_FreeSurface(closed_door);
}

void state_load() {
	completed = load_image(IMG_STATE_COMPLETED);
//	state_victory = load_image(IMG_STATE_VICTORY);
	menu[0] = load_image(IMG_MENU_1);
	menu[1] = load_image(IMG_MENU_2);
	menu[2] = load_image(IMG_GAME_OVER);
}

void state_unload() {
	SDL_FreeSurface(completed);
	SDL_FreeSurface(state_victory);
	SDL_FreeSurface(menu[0]);
	SDL_FreeSurface(menu[1]);
	SDL_FreeSurface(menu[2]);
}

void fire_load() {
	fire[0] = load_image(MAP_FIRE_1);
	fire[1] = load_image(MAP_FIRE_2);
}

void fire_unload() {
	SDL_FreeSurface(fire[0]);
	SDL_FreeSurface(fire[1]);
}

void bonus_load() {
	bonus[BONUS_BOMB_RANGE_INC] = load_image(IMG_BONUS_BOMB_RANGE_INC);
	bonus[BONUS_BOMB_RANGE_DEC] = load_image(IMG_BONUS_BOMB_RANGE_DEC);
	bonus[BONUS_BOMB_NB_INC] = load_image(IMG_BONUS_BOMB_NB_INC);
	bonus[BONUS_BOMB_NB_DEC] = load_image(IMG_BONUS_BOMB_NB_DEC);
	bonus[BONUS_LIFE] = load_image(IMG_BONUS_LIFE);
}

void bonus_unload() {
	for (int i = 0; i < NB_BONUS; i++)
		SDL_FreeSurface(bonus[i]);
}

void player_load() {
	player_img[WEST] = load_image(PLAYER_LEFT);
	player_img[EAST] = load_image(PLAYER_RIGHT);
	player_img[NORTH] = load_image(PLAYER_UP);
	player_img[SOUTH] = load_image(PLAYER_DOWN);
}

void player_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(player_img[i]);
}

void monster_load() {
	monster_img[WEST] = load_image(MNST_LEFT);
	monster_img[EAST] = load_image(MNST_RIGHT);
	monster_img[NORTH] = load_image(MNST_UP);
	monster_img[SOUTH] = load_image(MNST_DOWN);
}

void monster_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(monster_img[i]);
}

void bomb_load() {
	bomb_img[1] = load_image(BOMB1);
	bomb_img[2] = load_image(BOMB2);
	bomb_img[3] = load_image(BOMB3);
	bomb_img[4] = load_image(BOMB4);
}

void bomb_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(bomb_img[i]);
}

void sprite_load() {
	map_load();
	bonus_load();
	banner_load();
	player_load();
	monster_load();
	bomb_load();
	fire_load();
	state_load();
}

void sprite_free() {
	map_unload();
	bonus_unload();
	banner_unload();
	player_unload();
	monster_unload();
	bomb_unload();
	fire_unload();
	state_unload();
}

SDL_Surface* sprite_get_number(short number) {
	assert(number >= 0 && number <= 9);
	return numbers[number];
}

SDL_Surface* sprite_get_player(enum direction direction) {
	assert(player_img[direction]);
	return player_img[direction];
}

SDL_Surface* sprite_get_completed() {
	assert(completed);
	return completed;
}

/*
SDL_Surface* sprite_get_victory() {
	assert(victory);
	return victory;
}*/

SDL_Surface* sprite_get_monster(enum direction direction) {
	assert(monster_img[direction]);
	return monster_img[direction];
}

SDL_Surface* sprite_get_bomb(short type) {
	assert(bomb_img[type]);
	return bomb_img[type];
}

SDL_Surface* sprite_get_menu(short type) {
	assert(menu[type]);
	return menu[type];
}

SDL_Surface* sprite_get_banner_life() {
	assert(banner_life);
	return banner_life;
}

SDL_Surface* sprite_get_banner_bomb() {
	assert(banner_bomb);
	return banner_bomb;
}

SDL_Surface* sprite_get_banner_line() {
	assert(banner_line);
	return banner_line;
}

SDL_Surface* sprite_get_banner_range() {
	assert(banner_range);
	return banner_range;
}

SDL_Surface* sprite_get_bonus(enum bonus_type bonus_type) {
	assert(bonus[bonus_type]);
	return bonus[bonus_type];
}

SDL_Surface* sprite_get_fire(short type) {
	assert(fire[type]);
	return fire[type];
}

SDL_Surface* sprite_get_tree() {
	assert(tree);
	return tree;
}

SDL_Surface* sprite_get_box() {
	assert(box);
	return box;
}

SDL_Surface* sprite_get_key() {
	assert(key);
	return key;
}

SDL_Surface* sprite_get_stone() {
	assert(stone);
	return stone;
}

SDL_Surface* sprite_get_door() {
	assert(opened_door);
	return opened_door;
}

SDL_Surface* sprite_get_closed_door() {
	assert(closed_door);
	return closed_door;
}
