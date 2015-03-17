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
	struct bomb* next;
};

struct bomb* bomb_init(int x, int y, struct bomb* next) {
	struct bomb* bomb = malloc(sizeof(*bomb));
	if (!bomb)
		error("Memory error");

	bomb->x = x;
	bomb->y = y;
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

short bombs_update(struct map* map, struct bomb* bomb) {

	struct bomb* previous = NULL;

	while(bomb != NULL) {

		if (bomb->time > 0)
			bomb->time--;
		else {
			map_set_cell_type(map, bomb->x, bomb->y, CELL_EMPTY);
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
