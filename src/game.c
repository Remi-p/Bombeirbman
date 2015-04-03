#include <assert.h>
#include <time.h>

#include <game.h>
#include <misc.h>
#include <window.h>
#include <sprite.h>

struct game {
	struct level* curr_level; // current level
	struct player* player;
	struct monster* monster;
	struct bomb* bomb;
	short pause;
	short counter;
};

struct game* game_new(void) {
	sprite_load(); // load sprites into process memory

	struct game* game = malloc(sizeof(*game));
	game->curr_level = level_get_level(1); // get maps of the first level

	game->player = player_init(2);
	game->monster = monsters_from_map(level_get_map(game->curr_level, 0));
	game->bomb = NULL;
	player_from_map(game->player, level_get_map(game->curr_level, 0)); // get x,y of the player on the first map

	game->pause = 0;

	return game;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	level_free(game->curr_level);
}

struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

struct level* game_get_curr_level(struct game* game) {
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

	window_clear();

	game_banner_display(game);
	level_display(game_get_curr_level(game));
	bombs_display(game->bomb);
	if (player_is_vis(game->player))
		player_display(game->player);
	monsters_display(game->monster);
	display_fire(level_get_curr_map(game_get_curr_level(game)));

	window_refresh();
}

short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
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

void next_level(struct game* game) {

	assert(game);

	if (level_continu(game->curr_level)) { // This condition also change the map

		player_from_map(game->player, level_get_curr_map(game->curr_level));
	}
	else if (count_maps(next_level_number(game->curr_level), 1)) {

		// Changing the level
		level_free(game->curr_level);
		game->curr_level = level_get_level(next_level_number(game->curr_level));
		player_from_map(game->player, level_get_curr_map(game->curr_level));
	}
	else
		exit(0);

	player_reset(game->player);

	kill_the_monsters(game->monster);
	game->monster = monsters_from_map(level_get_curr_map(game->curr_level));


	game->bomb = NULL;

}

int game_update(struct game* game) {

	if (input_keyboard(game))
		return 1; // exit game

	// Incrementing the counter for monster moves
	game->counter++;

	// Going to the next level
	if (player_next_level(game->player))
		next_level(game);

	// We're not stopped
	if (game->pause != 1) {

		if (game->counter > 15) {

			game->counter = 0;

			game->monster = monsters_move(game->monster, level_get_curr_map(game->curr_level));

			// We also call this function in input_keyboard if the player moved
			player_on_monster(game->player, game->monster, level_get_curr_map(game->curr_level));

		}

		// Updating player
		player_update(level_get_curr_map(game->curr_level), game->player);

		// Updating bombs
		if (bombs_update(level_get_curr_map(game->curr_level), game->bomb, game->player, game->monster)) {
			free(game->bomb);
			game->bomb = NULL;
		}
	}

	return 0;
}
