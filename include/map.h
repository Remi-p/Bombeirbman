#ifndef MAP_H_
#define MAP_H_

#include <stdio.h>
// For directions
#include <constant.h>

enum cell_type {
	CELL_EMPTY=0,
	CELL_GOAL,  // 1
	CELL_SCENERY, // 2
	CELL_PLAYER, // 3
	CELL_CASE,   // 4
	CELL_BONUS, // 5
	CELL_MONSTER, // 6
	CELL_BOMB, // 7
	CELL_KEY, // 8
	CELL_DOOR, // 9
	CELL_CLOSED_DOOR // 10
};

enum bonus_type {
	BONUS_BOMB_RANGE_INC=1,
	BONUS_BOMB_RANGE_DEC, //2
	BONUS_BOMB_NB_INC, // 3
	BONUS_BOMB_NB_DEC, // 4
	BONUS_LIFE, // 5
	BONUS_MONSTER // 6
};

enum scenery_type {
	SCENERY_STONE, // 0
	SCENERY_TREE,  // 1
};

enum compose_type {
	CELL_STONE = CELL_SCENERY | (SCENERY_STONE << 4), 				//  0000 0010
	CELL_TREE  = CELL_SCENERY | (SCENERY_TREE << 4), 				//  0001 0010

    CELL_CASE_RANGEINC = CELL_CASE | (BONUS_BOMB_RANGE_INC << 4), 	//  0001 0100
    CELL_CASE_RANGEDEC = CELL_CASE | (BONUS_BOMB_RANGE_DEC << 4), 	//  0010 0100
	CELL_CASE_BOMBINC  = CELL_CASE | (BONUS_BOMB_NB_INC << 4), 		//  0011 0100
    CELL_CASE_BOMBDEC  = CELL_CASE | (BONUS_BOMB_NB_DEC << 4), 		//  0100 0100
    CELL_CASE_LIFE     = CELL_CASE | (BONUS_LIFE << 4), 			//  0101 0100
    CELL_CASE_MONSTER  = CELL_CASE | (BONUS_MONSTER << 4), 			//  0110 0100

    CELL_BONUS_RANGEINC = CELL_BONUS | (BONUS_BOMB_RANGE_INC << 4), 	//  0001 0101
    CELL_BONUS_RANGEDEC = CELL_BONUS | (BONUS_BOMB_RANGE_DEC << 4), 	//  0010 0101
	CELL_BONUS_BOMBINC  = CELL_BONUS | (BONUS_BOMB_NB_INC << 4), 		//  0011 0101
    CELL_BONUS_BOMBDEC  = CELL_BONUS | (BONUS_BOMB_NB_DEC << 4), 		//  0100 0101
    CELL_BONUS_LIFE     = CELL_BONUS | (BONUS_LIFE << 4)	 			//  0101 0101
};

struct map;

// Create a new empty map
struct map* map_new(int width, int height);
void map_free(struct map* map);


// Return the height and width of a map
int map_get_width(struct map* map);
int map_get_height(struct map* map);

// Return the type of a cell
enum cell_type map_get_cell_type(struct map* map, int x, int y);

// Return the type of a bonus
enum bonus_type map_get_bonus_type(struct map* map, int x, int y);

// Same as map_set_cell_type, but with the 4 first bits.
void map_set_compose_type(struct map* map, int x, int y, enum cell_type type, enum bonus_type bonus);

// Set the type of a cell
void  map_set_cell_type(struct map* map, int x, int y, enum cell_type type);

// Open the (first) closed door
void map_open_door(struct map* map);

// Add the fire of a bomb explosion
void add_fire_to_map(struct map* map, int x, int y, short time);

// Decrease the time of fires
struct fire* dec_fire(struct fire* fire);

// Display all the fires from that map
void display_fire(struct map* map);

// Return 0 if there is no fire on the cell, or 1 else
short is_there_fire(struct map* map, int x, int y);

// Test if (x,y) is within the map
int map_is_inside(struct map* map, int x, int y);

// Return a default 12x12 static map
struct map* map_get_default();

// Load a map
void load_map(struct map* map, FILE* fSaveFile);

// Save a map
void save_map(struct map* map, FILE* fSaveFile);

// Display the map on the screen
void map_display(struct map* map);

// Extract the data from a file
struct map* map_from_file(int niveau, int carte);

// Move a case to a given direction
int case_move(enum direction current_dir, int x, int y, struct map* map);

#endif /* MAP_H_ */
