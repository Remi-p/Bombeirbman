# Bombeirman

## Modifications

### 24/05/15

* *include/level.h*
* *include/map.h*
* *include/sprite.h*
* *src/level.c*
* *src/player.c*
* *src/sprite.c*
* *src/map.c*
	* Modifying a door into a princess
* *src/game.c*
	* Displaying a princess if the map is final

### 30/04/15

(various bugs correction)

* *data/*
* *src/level.c*
* *src/monster.c*
* *src/sprite.c*
* *include/constant.h*
* *src/game.c*
	* Game debugger
* *src/map.c*
	* Fire debugger

### Before 28/04/15

* *data/*
* *include/bomb.h*
* *include/constant.h*
* *include/game.h*
* *include/map.h*
* *include/player.h*
* *include/sprite.h*
* *include/window.h*
* *src/level.*
* *src/sprite.c*
* *src/bomb.c*
	* Support for explosion hitting one or two player(s)
* *src/game.c*
	* Constant for save filename
	* Conditional menu if savefile exists
* *src/main.c*
	* File testin
* *src/map.c*
	* Map debugger (print info on map : `map_debug`)
* *src/monster.c*
	* Updating monster debugger
* *src/player.c*
	* Player debugger (`player_debug`)
	* Optimization for two players loading
* *src/windowc.*
	* Support for rectangular hidding pannel

### 15.04.15

* *data/*
* *include/level.h*
* *include/map.h*
* *include/player.h*
* *src/game.c*
	* Starting the multiplayer mode
	* Keyboard input (& other functions) for second player
* *src/level.c*
	* Conditional level loading (for solo/multi)
* *src/map.c*
	* Conditional map loading
* *src/player.c*
	* Support for loading two players from map

### 11.04.15

* *src/level.c*
* *src/map.c*
* *src/player.c*
* *src/bomb.c*
	* Adding all the `assert`
	* Bugs correction
* *src/game.c*
	* Bugs correction
	* Finalizing quick save

### 10.04.15

* *data/*
* *include/bomb.h*
* *include/level.h*
* *include/map.h*
* *include/monster.h*
* *include/player.h*
* *src/sprite.c*
* *src/bomb.c*
	* Saving an approximation of bomb time remaining in map
	* `bomb_from_map` for loading map-coded bombs
* *src/game.c*
	* Function for default value
	* `save_game` and `load_game`
* *src/level.c*
	* Saving and loading level
* *src/map.c*
	* Saving and loading map
* *src/monster.c*
	* Miscellaneous functions
* *src/player.c*
	* `player_sizeof` for the saving part (game.c doesn't know the structure size)

### 04.04.15

* *include/constant.h*
* *include/game.h*
* *include/player.h*
* *include/sprite.h*
* *src/map.c*
* *src/bomb.c*
	* Rewriting the freeing of bombs
	* Adding all the `assert`
* *src/game.c*
	* Support for game state
	* Functions for reseting the game
	* Separating all the states (menu, splashscreen, game) with
	* Separating functionnalities (input keyboard, displaying)
* *src/level.c*
	* Automatic count for maps
* *src/player.c*
	* Adding all the `assert`
	* Support for death splashscreen
* *src/sprite.c*
	* Displaying menus & splashscreens

### 03.04.15

* *include/bomb.h*
* *include/level.h*
* *include/map.h*
* *include/monster.h*
* *include/player.h*
* *src/sprite.c*
* *include/bomb.c*
	* Free/delete the bombs at a map change
* *src/game.c*
	* Opening doors with a key
	* Updating the get_map
	* Support of going to the next map / level
* *src/level.c*
	* Support for loading multiple maps level
	* Support for auto detection of next level/map files
		* Count the number of maps per level
		* Tell when there is no more level
* *src/map.c*
	* Now it opens a door when the player got a key
	* Animation of the fire
* *src/monster.c*
	* Free/delete the monsters at a map change
* *src/player.c*
	* Support for the key-door interaction
	* Variable for telling when we need to change the level (`next_level`)
* *src/game.c*
	* Displaying the actual level number

### 02.04.15

* Adding test maps
* *include/constant.h*
* *include/map.h*
* *src/map.c*
	* Support for loading a map from a file (`map_from_file`)

### 01.04.15

* *include/constant.h*
* *include/map.h*
* *include/monster.h*
* *include/player.h*
* *src/sprite.c*
* *src/bomb.c*
	* Deletion of bonus when explode
	* Adding a bonus or a monster when a case exploded (according to the type of the case)
* *src/game.c*
	* Correction of the pause
* *src/map.c*
	* Support for obtaining the bonus type of a cell (`map_get_bonus_type`)
	* Support for setting a complete type cell (basic type + bonus)
	* Moving a case keep it type
* *src/monster.c*
	* Adding a monster (`add_monster`) outside the main function of creation
* *src/player.c*
	* We now got two types of `short` : nb_bomb & nb_bomb_now (for the number of bomb possible, and the actual number of bombs owned by the player)
	* Managing the numbers for the bombs
	* Support for the bonus (when the player is in a case)

### 31.03.15

* *include/bomb.h*
* *include/constant.h*
* *include/map.h*
* *include/monster.h*
* *include/sprite.h*
* *src/player.c*
* *src/sprite.c*
* *src/bomb.c*
	* Adding the player in the structure (let us play with the bomb capacity)
	* Support for the fire generated by the bomb
	* Chain explosion
* *src/game.c*
	* Taking charge of the fire displaying
	* The first monster of the list is now always sent back to the call of the function `monsters_move`
* *src/map.c*
	* Support for fire creation, management (`struct`), deletion, displaying, etc.
* *src/monster.c*
	* Making the monster structure like the fire (with previous and next pointeurs)
	* Externalize the deletion of monsters (with the variable `to_kill`)
	* As said before, `monsters_move` now return the first monster of the list

	

### 17.03.15

* *include/bomb.h*
* *include/constant.h*
* *include/monster.h*
* *include/player.h*
* *src/bomb.c*
	* Externalize the deletion of a bomb when it's the last of the map
	* Take charge of the explosion (`explosion`)
	* Start of supporting the fire from a bomb
	* Range of bombs
* *src/game.c*
* *src/monster.c*
	* life of monsters
	* decreasing monster's life when on a bomb impact
* *src/player.c*
	* decreasing player's life when on a bomb

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
