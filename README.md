# Bombeirman

## Modifications

### 17.03.15

* *include/bomb.h*
* *src/bomb.c*
	* Externalize the deletion of a bomb when it's the last of the map
* *src/game.c*

### 06.03.15

* *include/bomb.h*
* *include/constant.h*
* *include/monster.h*
* *include/player.h*
* *src/sprite.c*
* *src/game.c*
	* Blinking of the player when hit (`game_display`)
	* Calling function `player_on_monster` for collision with monster
* *src/monster.c*
	* 'Force' functionnality for moving a monster in any case
	* Adding a function for checking if a case is occupied by a monster, and moving it
* *src/player.c*
	* Support for invincibility & visibility (blinking)
	* Adding a fuction for updating part of the player every frame
	* Putting a bomb on the map
* *src/bomb.c*
	* Displaying, exploding, updating bombs
	

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
