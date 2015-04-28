#ifndef CONSTANT_H_
#define CONSTANT_H_

#define WINDOW_NAME "[PG110] Bombeirman 2015"

// Size (# of pixels) of a cell of the map
#define SIZE_BLOC       40

// Size (# of pixels) of banner
#define LINE_HEIGHT		4
#define BANNER_HEIGHT	40

// Width of the first line of the map files
#define FIRST_LINE_WIDTH 7
// (2 more characters for the EOL)

// Time management
#define DEFAULT_GAME_FPS 30

// map size
#define MAP_WIDTH  12
#define MAP_HEIGHT 12

// Life of the player
#define LIFE 2
// Number of bombs
#define NB_BOMBS 2
// Default range for the player
#define PORTEE 4
// Invincibility time
#define INVINCIBILITY 50
// Bomb time
#define BOMB 100
// Fire time
#define FIRE 5

// Splash screen time (like level completed)
#define SPLASH_SCREEN 50

#define SAVEGAME "data/last_save.bin"

enum direction {
	NORTH = 0,
	SOUTH, // 1
	WEST, // 2
	EAST //3
};

#endif /* CONSTANT */
