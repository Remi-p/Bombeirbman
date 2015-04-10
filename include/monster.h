#ifndef MONSTER_H_
#define MONSTER_H_

#include <map.h>
#include <constant.h>

struct monster;


// Creates a new monster with a given number of available bombs
struct monster* monster_init(int x, int y, struct monster* next, short rand);
void   monster_free(struct monster* monster);

// Give the number of monsters (used for saving)
int monster_nb(struct monster* monster);

// Display informations about the monster(s)
void monster_debug(struct monster* next);

// Returns the current position of the monster
int monster_get_x(struct monster* monster);
int monster_get_y(struct monster* monster);

// Set the direction of the next move of the monster
void monster_set_current_way(struct monster * monster, enum direction direction);

// Create the variables for the monsters
struct monster* monsters_from_map(struct map* map);

// Add a monster to a list
struct monster* add_monster(struct monster* monster, int x, int y);

// Move one monster according to the direction
void monster_move(struct monster* monster, struct map* map, short force);

// Move all the monsters - and clean the array ('to_kill')
struct monster* monsters_move(struct monster* monster, struct map* map);

// Kill a given monster
struct monster* kill_monster(struct map* map, struct monster* monster);

// Delete all the monsters from the list
void kill_the_monsters(struct monster* monster);

// Decrease the life of a monster if he's in the given coordinates
void kill_the_monster_here(struct monster* monster, int x, int y);

// Return 1 if there is a monster, 0 otherwise.
// Also move the monster from the cell.
short is_there_a_monster_here(struct monster* monster, int x, int y, struct map* map);

// Display one monster on the screen
void monster_display(struct monster* monster);

// Display the monsters on the screen
void monsters_display(struct monster* monster);


#endif /* MONSTER_H_ */
