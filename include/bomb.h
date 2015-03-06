#ifndef BOMB_H_
#define BOMB_H_

#include <map.h>
#include <constant.h>

struct bomb;

// Creates a new bomb with a given number of available bombs
struct bomb* bomb_init(int x, int y, struct bomb* next);
void   bomb_free(struct bomb* bomb);

// Returns the current position of the bomb
int bomb_get_x(struct bomb* bomb);
int bomb_get_y(struct bomb* bomb);

// Make the bomb explode
void bomb_explode(struct bomb* previous, struct bomb* bomb);

// Updating the bomb in the FPS rate
void bombs_update(struct map* map, struct bomb* bomb);

// Display one bomb on the screen
void bomb_display(struct bomb* bomb);

// Display the bombs on the screen
void bombs_display(struct bomb* bomb);

#endif /* BOMB_H_ */
