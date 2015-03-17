#include <SDL/SDL_image.h>
#include <assert.h>

#include <monster.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>
#include <time.h>

struct monster {
	int x, y;
	short life;
	enum direction current_direction;
	struct monster* next;
};

struct monster* monster_init(int x, int y, struct monster* next, short rand) {

	struct monster* monster = malloc(sizeof(*monster));

	if (!monster)
		error("Memory error");

	monster->x = x;
	monster->y = y;

	// TODO : Constante ?
	monster->life = 1;

	// The generation of the random first position is made outside this function
	monster->current_direction = rand; // From 0 to 3 for all the directions

	monster->next = next;

	return monster;
}

void monster_free(struct monster* monster) {
	assert(monster);
	free(monster);
}

int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

void monster_set_current_way(struct monster* monster, enum direction way) {
	assert(monster);
	monster->current_direction = way;
}


struct monster* monsters_from_map(struct map* map) {
	assert(map);

	/* Intializes random number generator */
	time_t t; srand((unsigned) time(&t));

	struct monster* previous_monster = NULL;

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_MONSTER) {
				previous_monster = monster_init(i, j, previous_monster, rand() % 4);
			}
		}
	}

	return previous_monster;
}

static int monster_move_aux(struct monster* monster, struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		// We deny the move
		return 0;
		break;

	case CELL_CASE:
		return 0;
		break;

	case CELL_BONUS:
		break;

	case CELL_GOAL:
		break;

	case CELL_MONSTER:
		return 0;
		break;

	case CELL_BOMB:
		return 0;
		break;

	case CELL_PLAYER:
		break;

	default:
		break;
	}

	// monster has moved
	return 1;
}

void monster_move(struct monster* monster, struct map* map, short force) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;

	switch (monster->current_direction) {
	case NORTH:
		if (monster_move_aux(monster, map, x, y - 1)) {
			monster->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1)) {
			monster->y++;
			move = 1;
		}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y)) {
			monster->x--;
			move = 1;
		}
		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y)) {
			monster->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
	}

	else if (force) {
		// If "force" is used, we keep moving
		monster->current_direction = (monster->current_direction + 1) % 4;
		monster_move(monster, map, 1);
	}
}

void monsters_move(struct monster* monster, struct map* map) {

	// For the random moves
	time_t t; srand((unsigned) time(&t));

	while(monster != NULL) {
		monster->current_direction = rand() % 4;
		monster_move(monster, map, 0);
		monster = monster->next;
	}

}

void kill_the_monster_here(struct monster* monster, int x, int y) {

	while(monster != NULL) {
		if (monster->x == x && monster->y == y) {
			//kill_monster();
			return;
		}
		monster = monster->next;
	}

}

short is_there_a_monster_here(struct monster* monster, int x, int y, struct map* map) {

	while(monster != NULL) {
		if (monster->x == x && monster->y == y) {
			monster_move(monster, map, 1);
			return 1;
		}
		monster = monster->next;
	}

	return 0;
}

void monster_display(struct monster* monster) {
	assert(monster);
	window_display_image(sprite_get_monster(monster->current_direction),
			monster->x * SIZE_BLOC, monster->y * SIZE_BLOC);
}

void monsters_display(struct monster* monster) {

	if (monster == NULL)
		return;

	while(monster != NULL) {
		monster_display(monster);
		monster = monster->next;
	}

}
