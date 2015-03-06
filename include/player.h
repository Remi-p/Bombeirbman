#ifndef PLAYER_H_
#define PLAYER_H_

#include <map.h>
#include <monster.h>
#include <constant.h>
#include <bomb.h>

struct player;

// Creates a new player with a given number of available bombs
struct player* player_init(int bomb_number);
void   player_free(struct player* player);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);

// Get, Decrease the number of life remaining
short player_get_life(struct player * player);
void player_dec_life(struct player * player);

// Return the time of invicibility remaining for the player
int player_is_inv(struct player* player);

// Return the boolean corresponding to the player visibility
short player_is_vis(struct player* player);

// Return a fresh bomb
struct bomb* create_bomb(struct map* map, struct bomb* previous, struct player* player);

// Decrease life player depending if (s)he is in the same cell as a monster
void player_on_monster(struct player* player, struct monster* monster, struct map* map);

// Load the player position from the map
void player_from_map(struct player* player, struct map* map);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player);

// Updating the player in the FPS rate
void player_update(struct map* map, struct player* player);

#endif /* PLAYER_H_ */
