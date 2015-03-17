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
};

struct bomb* bomb_init(int x, int y, struct bomb* next, short portee) {
	struct bomb* bomb = malloc(sizeof(*bomb));
	if (!bomb)
		error("Memory error");

	bomb->x = x;
	bomb->y = y;
	bomb->portee = portee;
	bomb->next = next;
	bomb->time = BOMB;

	return bomb;
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

short bomb_explode(struct bomb* previous, struct bomb* bomb) {

	assert(bomb);

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

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
		case CELL_SCENERY:
			return 0;
			break;

		case CELL_BOMB:
			return 0;
			break;

		default:
			break;
	}

	fire_in_the_hole(player, x, y);
	kill_the_monster_here(monster, x, y);
	map_set_cell_type(map, x, y, CELL_TREE);

	return 1;

}

void explosion(struct map* map, int x, int y, short portee, struct player* player, struct monster* monster) {

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

	struct bomb* previous = NULL;

	while(bomb != NULL) {

		if (bomb->time > 0)
			bomb->time--;
		else {
			map_set_cell_type(map, bomb->x, bomb->y, CELL_EMPTY);
			explosion(map, bomb->x, bomb->y, bomb->portee, player, monster);
			if (bomb_explode(previous, bomb)) return 1;
		}

		previous = bomb;
		bomb = bomb->next;

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
