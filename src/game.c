#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>

struct game {
	struct level* curr_level; // current level
	struct player* player;
	struct player* player_2; // Will only be available in multiplayer
	struct monster* monster;
	struct bomb* bomb;
	int state_timer;
	enum game_state game_state;
	short pause;
	short counter;
	short multiplayer;
};

void game_default(struct game* game) {

	assert(game);

	game->monster = monsters_from_map(level_get_curr_map(game->curr_level));

	game->bomb = NULL;
	game->state_timer = 0;
	game->pause = 0;

}

void game_debug(struct game* game) {
	
	assert(game);
	
	printf("Game:\n");

	printf("Address:%p\n", game);
	printf("Level:%p / Player:%p / Player_2:%p\n", game->curr_level, game->player, game->player_2);
	printf("Monster:%p / Bomb:%p\n", game->monster, game->bomb);
	
	printf("State_timer:%i / Game_state:%i\n", game->state_timer, game->game_state);
	printf("Pause:%i / Counter:%i / Multiplayer:%i", game->pause, game->counter, game->multiplayer);

	printf("\n");
	
}

struct game* game_new() {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	
	// Only a few things are needed before the actual start of the game
	game->multiplayer = 0;
	game->game_state = STATE_FIRST_MENU;
	game->counter = 0;
	
	game->curr_level = level_get_level(1, game->multiplayer);
	game->bomb = NULL;
	game->monster = NULL;
	game->player = NULL;
	game->player_2 = NULL;
	
	return game;
}

void game_free(struct game* game) {

	assert(game);

	// Free space
	player_free(game->player);
	player_free(game->player_2);
	level_free(game->curr_level);
	delete_bombs(game->bomb);
	kill_the_monsters(game->monster);

	// Addresse = NULL
	game->player = NULL;
	game->player_2 = NULL;
	game->curr_level = NULL;
	game->bomb = NULL;
	game->monster = NULL;
}

void game_reset(struct game* game, short multiplayer) {

	assert(game);

	game_free(game);

	game->curr_level = level_get_level(1, multiplayer);
	game_default(game);

	game->player = player_init(NB_BOMBS);
	if (multiplayer) game->player_2 = player_init(NB_BOMBS);
	else			 game->player_2 = NULL;

	player_from_map(game->player, level_get_map(game->curr_level, 0), 0);
	if (multiplayer) player_from_map(game->player_2, level_get_map(game->curr_level, 0), 1);

	game->game_state = STATE_FIRST_MENU;
	game->multiplayer = multiplayer;

}

void save_game(struct game game) {
	
	assert(&game);

	// From :
	// http://www.gamedev.net/topic/633653-save-and-read-data-from-file/#entry4995912
	// http://stackoverflow.com/questions/13330482/reading-and-writing-a-mallocd-linked-list-from-to-a-binary-file

	// Open the file for writing binary
	FILE *fSaveFile = fopen(SAVEGAME, "wb");

	if (fSaveFile) {
		// Write the structure to the file

		// --------- Here is the file format : -----------
		// struct player
		// int : corresponding to the level number
		// int : corresponding to the map number
		// char : corresponding to all the cells (n times)
		// -----------------------------------------------
		// Note: Monsters and bombs are coded in the map.
		// Game variable is reset using the others variables

		fwrite(game.player, player_sizeof(), 1, fSaveFile);

		level_save(game.curr_level, fSaveFile);

		fclose(fSaveFile);
	}
	else {
		printf("Error opening savefile!\n");
		exit(0);
	}
}

void load_game(struct game* game) {

	assert(game);

	// Open the file for reading binary
	FILE *fLoadFile = fopen(SAVEGAME, "rb");

	if (fLoadFile) {

		game_free(game);

		// Load : player
		game->player = player_init(NB_BOMBS);
		fread(game->player, player_sizeof(), 1, fLoadFile);

		// Load : maps from the level number
		int level_number;
		fread(&level_number, sizeof(int), 1, fLoadFile);
		game->curr_level = level_get_level(level_number, game->multiplayer);

		// Load : modified map
		level_load(game->curr_level, fLoadFile);

		// Reset values (and load monsters)
		game_default(game);

		// Load bombs
		game->bomb = bomb_from_map(level_get_curr_map(game->curr_level), game->player);

		fclose(fLoadFile);

	}
	else {
		printf("Error opening savefile!\n");
	}

}

struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

struct level* game_get_curr_level(struct game* game) {
	assert(game);

	return game->curr_level;
}

void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = level_get_curr_map(game_get_curr_level(game));

	//int height = map_get_height(map)) * SIZE_BLOC;
	int height = 12 * SIZE_BLOC;
	int y = height;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 6 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = height + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_life(game->player)), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	window_display_image(
			sprite_get_number(player_get_nb_bomb_now(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_scope(game_get_player(game))), x, y);

	if (player_has_key(game->player)) {
		x = 4 * white_bloc + 5 * SIZE_BLOC;
		window_display_image(sprite_get_key(), x, y);
	}

	x = 0;
	window_display_image(
			sprite_get_number(next_level_number(game_get_curr_level(game))-1),
			x, y);
}

void game_display(struct game* game) {
	assert(game);

	if (game->game_state != STATE_GAME)
		return;

	window_clear();

	if (!game->multiplayer)
		game_banner_display(game);
	level_display(game_get_curr_level(game));
	bombs_display(game->bomb);
	if (player_is_vis(game->player))
		player_display(game->player, 0);
	if (game->player_2 != NULL && player_is_vis(game->player_2))
		player_display(game->player_2, 1);
	monsters_display(game->monster);
	display_fire(level_get_curr_map(game_get_curr_level(game)));

	window_refresh();
}

short menu_input_keyboard(struct game* game) {

	assert(game);

	SDL_Event event;

	while (SDL_PollEvent(&event)) {

		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
		
			if (event.key.keysym.sym == SDLK_ESCAPE)
				return 1;
				
			if (game->game_state == STATE_FIRST_MENU) {
				
				switch (event.key.keysym.sym) {
					case SDLK_o:
					case SDLK_0:
						game_reset(game, 0);
						game->game_state = STATE_SECOND_MENU;
						break;
					case SDLK_t:
						game_reset(game, 1);
						game->game_state = STATE_SECOND_MENU;
						break;
					default:
						break;
				}
			}
			
			else if (game->game_state == STATE_SECOND_MENU) {

				switch (event.key.keysym.sym) {
					case SDLK_b:
						game->game_state = STATE_FIRST_MENU;
						break;
					case SDLK_s:
						game->game_state = STATE_GAME;
						break;
					case SDLK_c:
						if (!game->multiplayer && savefile_exist) {
							load_game(game);
							game->game_state = STATE_GAME;
						}
						break;
					default:
						break;
				}
			}

			else if (game->game_state == STATE_GAME_OVER) {

				switch (event.key.keysym.sym) {
					case SDLK_r:
						game->game_state = STATE_GAME;
						break;
					case SDLK_m:
						game->game_state = STATE_FIRST_MENU;
						break;
					case SDLK_q:
						return 1;
						break;	
					default:
						break;
				}
			}

			break;
		}
	}

	return 0;

}

short game_input_keyboard(struct game* game) {

	assert(game);

	SDL_Event event;
	struct player* player = game_get_player(game);
	struct player* player_2 = game->player_2;
	struct map* map = level_get_curr_map(game_get_curr_level(game));
	int move = 0;

	while (SDL_PollEvent(&event)) {

		// We look for all the inputs only if the game isn't stop
		if (game->pause == 1) {
			switch (event.type) {
				case SDL_QUIT:
					return 1;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_p) {
						game->pause = 0;
					}
					break;
				default:
					break;
			}
			return 0;
		} // Game is not stopped
		else {
			switch (event.type) {
			case SDL_QUIT:
				return 1;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
					case SDLK_ESCAPE:
						return 1;
					case SDLK_F6:
						if (!game->multiplayer) save_game(*game);
						break;
					case SDLK_F9:
						if (!game->multiplayer) load_game(game);
						break;
					case SDLK_p:
						game->pause = 1;
						return 0;
					case SDLK_UP:
						player_set_current_way(player, NORTH);
						move = player_move(player, map);
						break;
					case SDLK_DOWN:
						player_set_current_way(player, SOUTH);
						move = player_move(player, map);
						break;
					case SDLK_RIGHT:
						player_set_current_way(player, EAST);
						move = player_move(player, map);
						break;
					case SDLK_LEFT:
						player_set_current_way(player, WEST);
						move = player_move(player, map);
						break;
					case SDLK_SPACE:
						game->bomb = create_bomb(level_get_curr_map(game->curr_level), game->bomb, game->player);
						break;
					// Second player (Double switch ?)
					case SDLK_z:
					case SDLK_s:
					case SDLK_d:
					case SDLK_q:
						if (game->multiplayer) {	
							switch (event.key.keysym.sym) {
								case SDLK_z:
									player_set_current_way(player_2, NORTH);
									break;
								case SDLK_s:
									player_set_current_way(player_2, SOUTH);
									break;
								case SDLK_d:
									player_set_current_way(player_2, EAST);
									break;
								case SDLK_q:
									player_set_current_way(player_2, WEST);
									break;
								default:
									break;
							}
							move = player_move(player_2, map);
						}
						break;
					case SDLK_w:
						if (game->multiplayer) game->bomb = create_bomb(level_get_curr_map(game->curr_level), game->bomb, game->player_2);
						break;
					default:
						break;
				}

				break;
			}
		}
	}

	// If we moved, we need to check if player & monsters are in the same cell
	if (move > 0) {
		player_on_monster(player, game->monster, level_get_curr_map(game->curr_level));
	}

	return 0;
}

void next_map(struct game* game) {

	assert(game);

	if (level_continu(game->curr_level)) { // This condition also change the map

		player_from_map(game->player, level_get_curr_map(game->curr_level), 0);
		//player_set_next_lvl(game->player, 0);
	}
	else if (count_maps(next_level_number(game->curr_level), 1, game->multiplayer)) {

		// Changing the level
		level_free(game->curr_level);
		game->game_state = STATE_LEVEL_COMPLETED;
		game->state_timer = SPLASH_SCREEN;
		game->curr_level = level_get_level(next_level_number(game->curr_level), game->multiplayer);
	}
	else {

		game_reset(game, game->multiplayer);
		game->state_timer = 2 * SPLASH_SCREEN;
		game->game_state = STATE_VICTORY;
	}

	player_reset(game->player);

	kill_the_monsters(game->monster);
	delete_bombs(game->bomb);

	game->monster = monsters_from_map(level_get_curr_map(game->curr_level));
	game->bomb = NULL;
}

int state_game_update(struct game* game) {

	assert(game);

	if (game_input_keyboard(game))
		return 1; // exit game

	// Incrementing the counter for monster moves
	game->counter++;

	// Going to the next level
	if (player_next_level(game->player)) {
		next_map(game);
		return 0;
	}

	// We're not stopped
	if (game->pause != 1) {

		if (game->counter > 15) {

			game->counter = 0;

			game->monster = monsters_move(game->monster, level_get_curr_map(game->curr_level));

			// We also call this function in input_keyboard if the player moved
			player_on_monster(game->player, game->monster, level_get_curr_map(game->curr_level));

		}

		// Updating player (and displaying 'game over' if needed)
		if (player_update(level_get_curr_map(game->curr_level), game->player)) {
			game_reset(game, game->multiplayer);
			game->game_state = STATE_GAME_OVER;
		}

		if (game->multiplayer && game->player_2 != NULL) {
			player_update(level_get_curr_map(game->curr_level), game->player_2);
		}

		// Updating bombs
		if (bombs_update(level_get_curr_map(game->curr_level), game->bomb, game->player, game->player_2, game->monster)) {
			free(game->bomb);
			game->bomb = NULL;
		}
	}

	return 0;

}

void state_level_comp(struct game* game) {

	assert(game);

	if (game->game_state == STATE_LEVEL_COMPLETED) {

		if (game->state_timer > 0) {
			game->state_timer--;

			window_clear();
			window_display_image(sprite_get_completed(), 0, 0);
			window_refresh();
		}
		else {
			game->game_state = STATE_GAME;
			player_from_map(game->player, level_get_curr_map(game->curr_level), 0);
		}

	}
	else if (game->game_state == STATE_VICTORY) {

		if (game->state_timer > 0) {
			game->state_timer--;

			window_clear();
			window_display_image(sprite_get_victory(), 0, 0);
			window_refresh();
		}
		else
			game->game_state = STATE_FIRST_MENU;
	}

}

int state_menu(struct game* game) {

	assert(game);

	if (menu_input_keyboard(game))
		return 1; // exit game

	if (game->game_state == STATE_FIRST_MENU) {
		window_clear();
		window_display_image(sprite_get_menu(0), 0, 0);
		window_refresh();
	}

	else if(game->game_state == STATE_SECOND_MENU) {
		window_clear();
		window_display_image(sprite_get_menu(1), 0, 0);

		if (!savefile_exist || game->multiplayer) window_display_array(100, 70, 370, 123, sprite_get_menu_h());

		window_refresh();
	}

	else if(game->game_state == STATE_GAME_OVER) {
		window_clear();
		window_display_image(sprite_get_menu(2), 0, 0);
		window_refresh();
	}

	return 0;

}

int game_update(struct game* game) {

	assert(game);

	switch (game->game_state) {
		case STATE_GAME:
			return state_game_update(game);
			break;
		case STATE_LEVEL_COMPLETED:
		case STATE_VICTORY:
			state_level_comp(game);
			break;
		case STATE_FIRST_MENU:
		case STATE_SECOND_MENU:
		case STATE_GAME_OVER:
			return state_menu(game);
			break;
	}

	return 0;
}
