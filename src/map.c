#include <SDL/SDL_image.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

#include <map.h>
#include <constant.h>
#include <misc.h>
#include <sprite.h>
#include <window.h>

struct fire {
	short time;
	int x;
	int y;
	struct fire* next;
	struct fire* previous;
};

struct map {
	int width;
	int height;
	unsigned char* grid;
	struct fire* fire;
};

#define CELL(i,j) (i +  map->width * j)

struct map* map_new(int width, int height)
{
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof *map);
	if (map == NULL )
		error("map_new : malloc map failed");

	map->width = width;
	map->height = height;

	map->grid = malloc(height * width);
	if (map->grid == NULL) {
		error("map_new : malloc grid failed");
	}

	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++)
		for (j = 0; j < height; j++)
			map->grid[CELL(i,j)] = CELL_EMPTY;

	return map;
}

void map_debug(struct map* map) {

	int i, j;

	printf("Map of %i by %i :\n", map->width, map->height);

	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {

			printf("%i|", map_get_cell_type(map, i, j));

		}

		printf("\n");
	}

	printf("\n");
}

int map_is_inside(struct map* map, int x, int y)
{
	assert(map);

	// Exceeding grid limit
	if (x > (map->width - 1) || y > (map->height - 1) || x < 0 || y < 0) {
		return 0;
	}

	return 1;
}

void map_free(struct map* map)
{
	if (map == NULL )
		return;
	free(map->grid);
	free(map);
}

int map_get_width(struct map* map)
{
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map)
{
	assert(map);
	return map->height;
}

enum cell_type map_get_cell_type(struct map* map, int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 0x0f;
}

enum bonus_type map_get_bonus_type(struct map* map, int x, int y) {
	assert(map && map_is_inside(map, x, y));
	return (map->grid[CELL(x,y)] >> 4) & 0x0f;
}

void map_set_compose_type(struct map* map, int x, int y, enum cell_type type, enum bonus_type bonus) {
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type | (bonus << 4);
}

void map_set_cell_type(struct map* map, int x, int y, enum cell_type type)
{
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

void map_open_door(struct map* map) {

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_CLOSED_DOOR) {
				map_set_cell_type(map, i, j, CELL_DOOR);
				return;
			}
		}
	}

}

void add_fire_to_map(struct map* map, int x, int y, short time) {

	struct fire* fire = malloc(sizeof(*fire));

	fire->next = map->fire;
	fire->previous = NULL;
	fire->time = time;
	fire->x = x;
	fire->y = y;

	map->fire = fire;
}

struct fire* dec_fire(struct fire* fire) {

	fire->time--;

	if (fire->time <= 0) {

		if (fire->previous == NULL && fire->next == NULL) {
			free(fire);
			return NULL;
		}

		if (fire->previous != NULL)
			fire->previous->next = fire->next;

		if (fire->next != NULL)
			fire->next->previous = fire->previous;

		struct fire* fire_next = fire->next;
		free(fire);
		return fire_next;

	}
	else {
		return fire;
	}

}

void display_fire(struct map* map) {

	struct fire* fire = map->fire;
	struct fire* fire_prev = NULL;

	while (fire != NULL) {

		window_display_image(sprite_get_fire(fire->time%2), fire->x * SIZE_BLOC, fire->y * SIZE_BLOC);

		fire_prev = fire;
		fire = fire->next;
		if (dec_fire(fire_prev) == NULL)
			map->fire = NULL;
	}

}

short is_there_fire(struct map* map, int x, int y) {

	struct fire* fire = map->fire;

	while (fire != NULL) {

		if (fire->x == x && fire->y == y)
			return 1;

		fire = fire->next;

	}

	return 0;

}

void display_bonus(struct map* map, int x, int y, unsigned char type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type >> 4) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_DEC), x, y);
		break;

	case BONUS_LIFE:
		window_display_image(sprite_get_bonus(BONUS_LIFE), x, y);
		break;
	}
}

void display_scenery(struct map* map, int x, int  y, unsigned char type)
{
	switch (type >> 4) { // sub-types are encoded with the 4 most significant bits
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	}
}

void load_map(struct map* map, FILE* fLoadFile) {
	assert(map != NULL && map->height > 0 && map->width > 0);

	unsigned char cell;
	for (int i = 0; i < map->width; i++) {
		for (int j = 0; j < map->height; j++) {

			fread(&cell, sizeof(unsigned char), 1, fLoadFile);

			map->grid[CELL(i,j)] = cell;

		}

	}

}

void save_map(struct map* map, FILE* fSaveFile) {
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	unsigned char cell;
	for (int i = 0; i < map->width; i++) {
		for (int j = 0; j < map->height; j++) {

			cell = map->grid[CELL(i,j)];

			fwrite(&cell, sizeof(unsigned char), 1, fSaveFile);

		}

	}

}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
		for (int j = 0; j < map->height; j++) {
			x = i * SIZE_BLOC;
			y = j * SIZE_BLOC;

			unsigned char type = map->grid[CELL(i,j)];

			switch (type & 0x0f) {
			case CELL_SCENERY:
				display_scenery(map, x, y, type);
				break;
			case CELL_CASE:
				window_display_image(sprite_get_box(), x, y);
				break;
			case CELL_BONUS:
				display_bonus(map, x, y, type);
				break;
			case CELL_KEY:
				window_display_image(sprite_get_key(), x, y);
				break;
			case CELL_DOOR:
				window_display_image(sprite_get_door(), x, y);
				break;
			case CELL_CLOSED_DOOR:
				window_display_image(sprite_get_closed_door(), x, y);
				break;
			case CELL_MONSTER:
				//window_display_image(sprite_get_monster(SOUTH), x, y);
				break;
			}
		}

	}
}

struct map* map_from_file(int niveau, int carte, short multi) {

	// Mainly inspirated by :
	// http://stackoverflow.com/questions/1658530/load-numbers-from-text-file-in-c
	// http://www.codingunit.com/c-tutorial-file-io-using-text-files

	char buf[FIRST_LINE_WIDTH];
	FILE *map_txt;
	char *filename;
	 // Allocate a string with enough space for the filename
	if (multi) {
		filename = malloc(strlen("data/map_multi_00"));
		sprintf(filename, "data/map_multi_%i", carte);
	}
	else {
		filename = malloc(strlen("data/map_00_00"));
		sprintf(filename, "data/map_%i_%i", niveau, carte);
	}

	map_txt = fopen(filename, "r");

	if (!map_txt) // It didn't worked
		return NULL;

	// We get the first line
	if (fgets(buf, FIRST_LINE_WIDTH, map_txt) == NULL)
		return NULL;

	// We get the params
	char *numb;

	numb = strtok(buf, ":");
	int x = atoi(numb);
	numb = strtok(NULL, ":");
	// NULL is for saying to the function that we still used our variable
	int y = atoi(numb);

	struct map* map = map_new(x, y);

	// Now we can do the loop

	// Variables :
	int line_width = x*3+1;
	char line[line_width];
	unsigned char themap[x * y];

	for (int j = 0; j < y; j++) { // Lines

		if (fgets(line, line_width, map_txt) == NULL)
			return NULL;

		// We need to start that outside (we'll use NULL after)
		numb = strtok(line, ",");
		themap[j*x] = (char)strtol(numb, NULL, 16);

		for (int i = 1; i < x; i++) { // Columns
			numb = strtok(NULL, ",");
			themap[i + j*x] = (char)strtol(numb, NULL, 16);
			//printf("|| %i : %i", (i + j*x),themap[i + j*x]);
		}

	}

	for (int i = 0; i < x * y; i++)
		map->grid[i] = themap[i];

	return map;

}

struct map* map_get_default(void)
{
	/*
	unsigned char themap[MAP_WIDTH * MAP_HEIGHT] = {
			CELL_PLAYER, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,
			CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE_MONSTER, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_DOOR, CELL_EMPTY, CELL_STONE, CELL_CASE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_CLOSED_DOOR, CELL_STONE, CELL_STONE, CELL_STONE, CELL_EMPTY, CELL_EMPTY, CELL_STONE, CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY , CELL_EMPTY, CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_TREE, CELL_CASE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_EMPTY, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_TREE, CELL_TREE, CELL_TREE, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY,  CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
			CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_BONUS_BOMBINC, CELL_BONUS_RANGEDEC, CELL_BONUS_RANGEINC, CELL_BONUS_BOMBDEC, CELL_BONUS_LIFE, CELL_STONE,  CELL_EMPTY, CELL_EMPTY,
			CELL_CASE_RANGEINC, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE, CELL_STONE,  CELL_CASE_LIFE, CELL_EMPTY,
			CELL_MONSTER,  CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_EMPTY, CELL_MONSTER
		};
		*/

	return map_from_file(1,1,0);
}

static int case_move_aux(struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		// We don't move our case
		return 0;
		break;

	case CELL_CASE:
		return 0;
		break;

	case CELL_BOMB:
		return 0;
		break;

	case CELL_BONUS:
		break;

	case CELL_GOAL:
		break;

	case CELL_MONSTER:
		return 0;
		break;

	case CELL_PLAYER:
		break;

	default:
		break;
	}

	// Case can be moved
	return 1;
}

int case_move(enum direction current_dir, int x, int y, struct map* map) {

	int prev_x = x;
	int prev_y = y;
	int move = 0;

	switch (current_dir) {
	case NORTH:
		if (case_move_aux(map, x, --y)) {
			move = 1;
		}
		break;

	case SOUTH:
		if (case_move_aux(map, x, ++y)) {
			move = 1;
		}
		break;

	case WEST:
		if (case_move_aux(map, --x, y)) {
			move = 1;
		}
		break;

	case EAST:
		if (case_move_aux(map, ++x, y)) {
			move = 1;
		}
		break;
	}

	if (move) {
		map_set_compose_type(map, x, y, CELL_CASE, map_get_bonus_type(map, prev_x, prev_y));
		// map_set_cell_type(map, prev_x, prev_y, CELL_EMPTY); // Will be replaced by player
	}
	return move;
}
