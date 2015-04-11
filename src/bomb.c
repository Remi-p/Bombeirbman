#include <SDL/SDL_image.h>
#include <assert.h>

#include <bomb.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

struct bomb {
	int x, y;
	int time;
	short portee;
	struct bomb* next;
	struct player* player;
};

struct bomb* bomb_init(int x, int y, struct bomb* next, short portee, struct player* player) {
	assert(player);

	struct bomb* bomb = malloc(sizeof(*bomb));
	if (!bomb)
		error("Memory error");

	bomb->x = x;
	bomb->y = y;
	bomb->portee = portee;
	bomb->next = next;
	bomb->time = BOMB;
	// We save the player, so that we could increase his number of bomb we the bomb explode
	bomb->player = player;

	return bomb;
}

struct bomb* bomb_from_map(struct map* map, struct player* player) {

	assert(map);
	assert(player);

	struct bomb* bomb_previous = NULL;

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_BOMB) {

				// If a loading is made, it's in one player mode
				// So we give it the addresse of the only available player
				bomb_previous = bomb_init(i, j, bomb_previous, player_get_scope(player), player);

				// And also we decode the approximation of time left
				bomb_previous->time = (BOMB / 15) * map_get_bonus_type(map, i, j);

			}
		}
	}

	return bomb_previous;
}

void bomb_free(struct bomb* bomb) {
	assert(bomb);
	free(bomb);
}

int bomb_get_x(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->x;
}

int bomb_get_y(struct bomb* bomb) {
	assert(bomb != NULL);
	return bomb->y;
}

void delete_bombs(struct bomb* bomb) {

	struct bomb* previous = NULL;

	while(bomb != NULL) {

		previous = bomb;

		if (previous != NULL)
			free(previous);

		bomb = bomb->next;
	}


}

short bomb_explode(struct bomb* previous, struct bomb* bomb, struct player* player) {

	assert(player);
	assert(bomb);

	//printf("Adresse de la bomb précédente : %p\nAdresse de la bomb actuelle : %p\nAdresse de la bomb suivante : %p\n\n", previous, bomb, bomb->next);

	player_inc_nb_bomb_now(player);

	// It's not the first of the list
	if (previous != NULL) {
		previous->next = bomb->next;
		free(bomb);
	}
	// It is alone
	else if (bomb->next == NULL) {
		return 1;
	}
	else {
		free(bomb);
	}

	return 0;

}

short exp_fire(struct map* map, int x, int y, struct player* player, struct monster* monster) {

	assert(player);
	assert(map);

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
		case CELL_SCENERY:
			return 0;
			break;

		case CELL_BOMB:
			add_fire_to_map(map, x, y, FIRE);
			return 0;
			break;

		case CELL_DOOR:
		case CELL_CLOSED_DOOR:
			return 0;
			break;

		case CELL_BONUS:
			map_set_cell_type(map, x, y, CELL_EMPTY);
			break;

		case CELL_CASE:
			// Monster apparition
			if (map_get_bonus_type(map, x, y) == BONUS_MONSTER) {
				add_monster(monster, x, y);
				map_set_cell_type(map, x, y, CELL_MONSTER);
			}
			map_set_compose_type(map, x, y, CELL_BONUS, map_get_bonus_type(map, x, y));
			return 0;
			break;

		default:
			break;
	}

	fire_in_the_hole(player, x, y);
	kill_the_monster_here(monster, x, y);

	add_fire_to_map(map, x, y, FIRE);

	return 1;

}

void explosion(struct map* map, int x, int y, short portee, struct player* player, struct monster* monster) {

	assert(map);
	assert(player);

	int x_dir, y_dir;
	int x_move, y_move;

	exp_fire(map, x, y, player, monster);

	y_move = 0;
	for (x_dir = -1 ; x_dir <= 1 ; x_dir = x_dir + 2) { // Loop for x-direction

		for (x_move = x_dir ; x_move*x_dir <= x_dir*portee*x_dir ; x_move = x_move + x_dir) {

			if (!exp_fire(map, x + x_move, y + y_move, player, monster))
				break;

		}

	}

	x_move = 0;
	for (y_dir = -1 ; y_dir <= 1; y_dir = y_dir + 2) { // And for y

		for (y_move = y_dir ; y_move*y_dir <= y_dir*y_dir*portee ; y_move = y_move + y_dir) {

			if (!exp_fire(map, x + x_move, y + y_move, player, monster))
				break;

		}

	}

}

short bombs_update(struct map* map, struct bomb* bomb, struct player* player, struct monster* monster) {

	assert(map);
	assert(player);

	struct bomb* previous = NULL;
	struct bomb* next = NULL;

	while(bomb != NULL) {

		if (is_there_fire(map, bomb->x, bomb->y))
			bomb->time = 0;

		if (bomb->time > 0) {
			bomb->time--;

			// For some reasons we have to typecast float for the denominator
			int time = (15 / (float) BOMB) * (bomb->time); // By using int, time is automatically rounded

			// We use compose_type for saving an approximation of the remaining time (in case of saving)
			map_set_compose_type(map, bomb->x, bomb->y, CELL_BOMB, time);

			previous = bomb;
			bomb = bomb->next;
		}
		else {
			map_set_cell_type(map, bomb->x, bomb->y, CELL_EMPTY);
			explosion(map, bomb->x, bomb->y, bomb->portee, player, monster);

			// 'Incrementing' the loop
			next = bomb->next;

			if (bomb_explode(previous, bomb, player)) return 1;

			bomb = next;
		}

	}

	return 0;

}

void bomb_display(struct bomb* bomb) {

	assert(bomb);

	short type;
	if (bomb->time <= BOMB && bomb->time > (BOMB/4)*3)
		type = 4;
	else if (bomb->time <= (BOMB/4)*3 && bomb->time > (BOMB/4)*2)
		type = 3;
	else if (bomb->time <= (BOMB/4)*2 && bomb->time > (BOMB/4)*1)
		type = 2;
	else
		type = 1;

	window_display_image(sprite_get_bomb(type),
			bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
}

void bombs_display(struct bomb* bomb) {

	if (bomb == NULL)
		return;

	while(bomb != NULL) {
		bomb_display(bomb);
		bomb = bomb->next;
	}

}
