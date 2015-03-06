#ifndef MONSTER_H_
#define MONSTER_H_

#include <map.h>
#include <constant.h>

struct monster;

// Creates a new monster with a given number of available bombs
struct monster* monster_init(int x, int y, struct monster* next, short rand);
void   monster_free(struct monster* monster);

// Returns the current position of the monster
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);

// Set the direction of the next move of the monster
void monster_set_current_way(struct monster * monster, enum direction direction);

// Create the variables for the monsters
struct monster* monsters_from_map(struct map* map);

// Move one monster according to the direction
void monster_move(struct monster* monster, struct map* map);

// Move all the monsters
void monsters_move(struct monster* monster, struct map* map);

// Display one monster on the screen
void monster_display(struct monster* monster);

// Display the monsters on the screen
void monsters_display(struct monster* monster);


#endif /* MONSTER_H_ */
