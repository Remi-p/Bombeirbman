# Bombeirman

## Modifications

### Before 05.03.15

* *include/constant.h*
* *include/game.h*
* *include/map.h*
* *include/monster.h*
* *include/player.h*
* *include/sprite.h*
* *src/game.c*
	* Adding in the `struct game` the monsters, a pause boolean, and a counter
	* Supporting the monsters
	* Adding pause functionnality
	* Adding the movements of monsters in `game_update`
* *src/map.c*
	* Filling `map_is_inside` function
	* Adding functions for moving cases
* *src/monster.c*
	* From the file `player.c`
* *src/player.c*
	* Initializing life from the constant `LIFE`
	* Functions for playing with life
	* Case movement
* *src/sprite.c*
	* Support for monster sprite
