#include <SDL/SDL_image.h>
#include <assert.h>

#include <player.h>
#include <sprite.h>
#include <window.h>
#include <misc.h>
#include <constant.h>

struct player {
	int x, y;
	enum direction current_direction;
	int nb_bomb;
	int nb_bomb_now;
	short life;
	int invincibility;
	short visible; // Used for making the player blinked
	short portee;
	short clee;
	short next_level;
};

struct player* player_init(int bomb_number) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
		error("Memory error");

	player->current_direction = SOUTH;
	player->nb_bomb = bomb_number;
	player->nb_bomb_now = bomb_number;
	player->life = LIFE;
	player->portee = PORTEE;
	player->invincibility = 0;
	player->visible = 1;
	player->clee = 0;
	player->next_level = 0;

	return player;
}

int player_sizeof() {
	return sizeof(struct player);
}

void player_free(struct player* player) {
	assert(player);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_current_way(struct player* player, enum direction way) {
	assert(player);
	player->current_direction = way;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	player->nb_bomb_now += 1;
	player->nb_bomb += 1;
}

void player_dec_nb_bomb(struct player* player) {
	assert(player);
	if (player->nb_bomb > 1) {

		player->nb_bomb -= 1;

		if (player->nb_bomb_now >= 1)
			player->nb_bomb_now -= 1;

	}
}

int player_get_nb_bomb_now(struct player* player) {
	assert(player);
	return player->nb_bomb_now;
}

void player_inc_nb_bomb_now(struct player* player) {
	assert(player);
	if (player->nb_bomb_now + 1 <= player->nb_bomb)
		player->nb_bomb_now += 1;
}

void player_dec_nb_bomb_now(struct player* player) {
	assert(player);
	player->nb_bomb_now -= 1;
}

int  player_get_scope(struct player * player) {
	assert(player);
	return player->portee;
}

void player_inc_scope(struct player * player) {
	assert(player);
	player->portee += 1;
}

void player_dec_scope(struct player * player) {
	assert(player);
	if (player->portee > 1)
		player->portee -= 1;
}

short player_get_life(struct player* player) {
	assert(player);
	return player->life;
}

void fire_in_the_hole(struct player* player, int x, int y) {
	assert(player);

	if (player->x == x && player->y == y) {
		player_dec_life(player);
	}

}

void player_dec_life(struct player* player) {
	assert(player);
	if (player->invincibility <= 0) {
		player->life -= 1;
		player->invincibility = INVINCIBILITY;
	}
}

void player_inc_life(struct player * player) {
	assert(player);
	if (player->life <9)
		player->life ++;
}

short player_has_key(struct player* player) {
	assert(player);
	return player->clee;
}

short player_next_level(struct player* player) {
	assert(player);

	return player->next_level;
}

void player_reset(struct player* player) {
	assert(player);

	player->clee = 0;
	player->invincibility = 0;
	player->nb_bomb_now = player->nb_bomb;
	player->next_level = 0;
	player->visible = 1;

}

int player_is_inv(struct player* player) {
	assert(player);
	return player->invincibility;
}

short player_is_vis(struct player* player) {
	assert(player);
	return player->visible;
}

struct bomb* create_bomb(struct map* map, struct bomb* previous, struct player* player) {
	assert(player);
	assert(map);

	if (player->nb_bomb_now > 0) {
		player_dec_nb_bomb_now(player);
		map_set_cell_type(map, player->x, player->y, CELL_BOMB);
		return bomb_init(player->x, player->y, previous, player->portee, player);
	}
	else
		return previous;
}

void player_on_monster(struct player* player, struct monster* monster, struct map* map) {
	assert(player);
	assert(map);

	if (is_there_a_monster_here(monster, player_get_x(player), player_get_y(player), map) != 0)
		player_dec_life(player);

}

void player_from_map(struct player* player, struct map* map) {
	assert(player);
	assert(map);

	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
				player->x = i;
				player->y = j;
			}
		}
	}
}

void player_on_bonus(struct player* player, struct map* map, int x, int y) {
	assert(player);
	assert(map);

	switch(map_get_bonus_type(map, x, y)) {
		case BONUS_BOMB_RANGE_INC:
			player_inc_scope(player);
			return;	break;
		case BONUS_BOMB_RANGE_DEC:
			player_dec_scope(player);
			return;	break;
		case BONUS_BOMB_NB_INC:
			player_inc_nb_bomb(player);
			return;	break;
		case BONUS_BOMB_NB_DEC:
			player_dec_nb_bomb(player);
			return;	break;
		case BONUS_LIFE:
			player_inc_life(player);
			return;	break;
		case BONUS_MONSTER:
			return; break;
	}

}

static int player_move_aux(struct player* player, struct map* map, int x, int y) {
	assert(player);
	assert(map);

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
		case CELL_SCENERY:
			// We deny the move
			return 0;
			break;

		case CELL_CASE:

			if (case_move(player->current_direction, x, y, map)) {
				return 1;
			}
			else {
				return 0;
			}

			break;

		case CELL_GOAL:
			break;

		case CELL_MONSTER:
			// When the player can't die, we're not letting him going through monsters
			if (player->invincibility > 0)
				return 0;
			break;

		case CELL_PLAYER:
			break;

		case CELL_CLOSED_DOOR:
			return 0;
			break;

		case CELL_KEY:
			player->clee = 1;
			map_set_cell_type(map, x, y, CELL_EMPTY);
			// We open the door :
			map_open_door(map);
			break;

		case CELL_DOOR:
			player->next_level = 1;
			break;

		case CELL_BOMB:
			return 0;
			break;

		case CELL_BONUS:
			player_on_bonus(player, map, x, y);
			break;

		default:
			break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	assert(player);

	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->current_direction) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1)) {
			player->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1)) {
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y)) {
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y)) {
			player->x++;
			move = 1;
		}
		break;
	}

	if (move) {

		if (map_get_cell_type(map, x, y) == CELL_PLAYER)
			map_set_cell_type(map, x, y, CELL_EMPTY);

		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
	}

	return move;
}

void player_display(struct player* player) {
	assert(player);
	window_display_image(sprite_get_player(player->current_direction),
			player->x * SIZE_BLOC, player->y * SIZE_BLOC);
}

short player_update(struct map* map, struct player* player) {

	assert(map);
	assert(player);

	if (player_get_life(player) < 1)
		return 1;

	if (player->visible == 0) {
		player->visible = 1;
	}
	else if (player->invincibility > 0) {
		player->visible = 0;
	}

	if (player->invincibility > 0) {
		player->invincibility--;
	}

	return 0;
}
