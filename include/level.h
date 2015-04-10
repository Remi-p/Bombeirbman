#ifndef LEVEL_H_
#define LEVEL_H_

#include <map.h>
#include <stdio.h>

struct level;

// return level number num
struct level*  level_get_level(int num);

// Check if we still have remaining maps, and if so, upgrade
short level_continu(struct level* level);

// Load the level (+map) data
void level_load(struct level* level, FILE* fSaveFile);

// Save the level (+map) data
void level_save(struct level* level, FILE* fSaveFile);

// Give the next level number
int next_level_number(struct level* level);

// Check if a file exist if map is defined, tell the number of maps if map == 0
short count_maps(int level, int map);

// return current map of the level
struct map* level_get_curr_map(struct level* level);

// return map number num of the level
struct map*  level_get_map(struct level* level, int num);

// free the level
void  level_free(struct level* level);

// display the level
void  level_display(struct level* level);

// free level struct
void   level_free(struct level* level);

#endif /* LEVEL_H_ */
