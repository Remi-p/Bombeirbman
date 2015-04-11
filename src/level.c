#include <assert.h>
#include <stdlib.h>
#include <stddef.h> // NULL (macos)
#include <level.h>
#include <game.h>
#include <map.h>

// For the isset on filenames :
#include <unistd.h>
#include <stdio.h>
#include <string.h>

struct level {
	struct map** maps; // array of the level's maps
	short nb_maps; // nb maps of the level
	short cur_map; // the current map
	int number; // which number corresponds to this level
};

struct level* level_get_level(int num) {
	struct level* level = malloc(sizeof(*level));

	/*
	switch (num) {
	case 0:
		level->nb_maps = 1;
		level->cur_map = 0;
		level->maps = malloc(sizeof(*level->maps));
		level->maps[0] = map_get_default();
		return level;
		break;
	}*/

	level->number = num;
	level->nb_maps = count_maps(num, 0);
	level->cur_map = 0;
	// We allocate the first layer of the array
	level->maps = malloc(sizeof(*level->maps));

	for	(int i = 0; i <= (level->nb_maps-1); i++) {

		// Second allocation
		level->maps[i] = map_from_file(num, (i+1));

	}

	return level;
}

short level_continu(struct level* level) {
	assert(level);

	if (level->cur_map < (level->nb_maps-1)) {
		level->cur_map++;
		return 1;
	}
	else {
		return 0;
	}
}

void level_load(struct level* level, FILE* fLoadFile) {

	assert(level);

	int cur_map;

	fread(&cur_map, sizeof(int), 1, fLoadFile);

	level->cur_map = cur_map;

	printf("Level nb : %i // Map nb : %i", level->number, cur_map);

	load_map(level_get_curr_map(level), fLoadFile);

}

void level_save(struct level* level, FILE* fSaveFile) {

	assert(level);

	int level_nb = level->number;
	int map_nb = level->cur_map;

	fwrite(&level_nb, sizeof(int), 1, fSaveFile);
	fwrite(&map_nb, sizeof(int), 1, fSaveFile);

	save_map(level_get_curr_map(level), fSaveFile);

}

int next_level_number(struct level* level) {
	assert(level);
	return level->number + 1;
}

short count_maps(int level, int map) {

	// Check how many maps there is on the level
	char *filename = malloc(strlen("data/map_00_00"));

	if (map != 0) { // A specific map has been given
		sprintf(filename, "data/map_%i_%i", level, map);

		if( access( filename, F_OK ) != -1 ) {
			// Exist
			return 1;
		} else {
			// Does not exist
			return 0;
		}
	}
	else { // We want the number of available maps
		short i = 1;
		sprintf(filename, "data/map_%i_%i", level, i);

		// As long as we have a file, we continue
		while (access( filename, F_OK ) != -1) {
			i++;
			sprintf(filename, "data/map_%i_%i", level, i);
		}
		return (i-1);
	}

}

struct map* level_get_curr_map(struct level* level) {
	return level->maps[level->cur_map];
}

struct map* level_get_map(struct level* level, int num) {
	assert(num <= level->nb_maps);
	return level->maps[num];
}

void level_free(struct level* level) {
	for (int i = 0; i < level->nb_maps; i++)
		map_free(level->maps[i]);

	free(level->maps);
}

void level_display(struct level* level) {
	map_display(level->maps[level->cur_map]);
}
