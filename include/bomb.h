#ifndef BOMB_H_
#define BOMB_H_

#include <map.h>
#include <constant.h>
#include <player.h>
#include <monster.h>

struct player;

struct bomb;

// Creates a new bomb with a given number of available bombs
struct bomb* bomb_init(int x, int y, struct bomb* next, short portee, struct player* player);
void   bomb_free(struct bomb* bomb);

// Take the bombs from the map (when loading)
struct bomb* bomb_from_map(struct map* map, struct player* player);

// Returns the current position of the bomb
int bomb_get_x(struct bomb* bomb);
int bomb_get_y(struct bomb* bomb);

// Delete the bombs
void delete_bombs(struct bomb* bomb);

// Make the bomb explode
short bomb_explode(struct bomb* previous, struct bomb* bomb);

// Burn for burn, wound for wound, bruise for bruise.
short exp_fire(struct map* map, int x, int y, struct player* player, struct player* player_2, struct monster* monster);

// Take charge of the explosion
void explosion(struct map* map, int x, int y, short portee, struct player* player, struct player* player_2, struct monster* monster);

// Updating the bomb in the FPS rate
short bombs_update(struct map* map, struct bomb* bomb, struct player* player, struct player* player_2, struct monster* monster);

// Display one bomb on the screen
void bomb_display(struct bomb* bomb);

// Display the bombs on the screen
void bombs_display(struct bomb* bomb);

#endif /* BOMB_H_ */
