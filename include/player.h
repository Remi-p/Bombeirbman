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

// Return sizeof(player) for the saving part
int player_sizeof();

// Display informations about the player
void player_debug(struct player* player);

// Returns the current position of the player
int player_get_x(struct player* player);
int player_get_y(struct player* player);

// Set the direction of the next move of the player
void player_set_current_way(struct player * player, enum direction direction);

// Increase, Decrease the number of bomb that player can put
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);

// Get, Increase, Decrease the number of bomb the player is using actually
int  player_get_nb_bomb_now(struct player * player);
void player_inc_nb_bomb_now(struct player * player);
void player_dec_nb_bomb_now(struct player * player);

// Set, Increase, Decrease the number of bomb that player can put
int  player_get_nb_bomb(struct player * player);
void player_inc_nb_bomb(struct player * player);
void player_dec_nb_bomb(struct player * player);

// Increase/Decrease and set the scope for bombs of the player
int  player_get_scope(struct player * player);
void player_inc_scope(struct player * player);
void player_dec_scope(struct player * player);

// Get, Decrease, Increase the number of life remaining
short player_get_life(struct player * player);
void player_dec_life(struct player * player);
void player_inc_life(struct player * player);

// Decrease life player if he is in the given coordinate
void fire_in_the_hole(struct player* player, int x, int y);

// Tell if the player has the key or if he doesn't
short player_has_key(struct player* player);

// Set the next_level variable of player
void player_set_next_lvl(struct player* player, short next_level);

// Tell if we need to go to the next level
short player_next_level(struct player* player);

// Reset some parts of the player
void player_reset(struct player* player);

// Return the time of invicibility remaining for the player
int player_is_inv(struct player* player);

// Return the boolean corresponding to the player visibility
short player_is_vis(struct player* player);

// Return a fresh bomb
struct bomb* create_bomb(struct map* map, struct bomb* previous, struct player* player);

// Decrease life player depending if (s)he is in the same cell as a monster
void player_on_monster(struct player* player, struct monster* monster, struct map* map);

// Load the player position from the map
void player_from_map(struct player* player, struct map* map, short second);

// Do the actions when the player is on an item
void player_on_bonus(struct player* player, struct map* map, int x, int y);

// Move the player according to the current direction
int player_move(struct player* player, struct map* map);

// Display the player on the screen
void player_display(struct player* player, short multi);

// Updating the player in the FPS rate. Return 1 if player has 0 life
short player_update(struct map* map, struct player* player);

#endif /* PLAYER_H_ */
