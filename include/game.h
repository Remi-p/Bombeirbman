#ifndef GAME_H_
#define GAME_H_

#include <player.h>
#include <monster.h>
#include <bomb.h>
#include <level.h>

// Test of existence for savegame
extern short savefile_exist;

// State of the game
enum game_state {
	STATE_GAME=0,
	STATE_LEVEL_COMPLETED,
	STATE_FIRST_MENU,
	STATE_SECOND_MENU,
	STATE_GAME_OVER,
	STATE_VICTORY
};

// Abstract data type
struct game;

// Create a new game
struct game* game_new();

// Free a game
void game_free(struct game* game);

// Return the player of the current game
struct player* game_get_player(struct game* game);

// Return the current level
struct level* game_get_curr_level(struct game* game);

// Display the game on the screen
void game_display(struct game* game);

// update
int game_update(struct game* game);

#endif /* GAME_H_ */
