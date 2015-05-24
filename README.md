# Liste des fonctionnalités

Rémi Perrot - T1-G2 - 2015

## Gestion des collisions

* Mise en place de la fonction `map_is_inside` pour vérifier la véracité de coordonnées.
* `case_player_aux` vérifie avant chaque mouvement la nature de la case de destination, et gère en fonction :
	* Refus de déplacement (pierre, arbre, bord ...)
	* Perte d'une vie (monstre)
	* Augmentation/diminution de caractéristiques (bonus)
* `case_move_aux` basé sur la fonction `player_move_aux` pour gérer les collisions des caisses

## Bonus, Malus

* `map_get_bonus_type` et ̀`map_set_compose_type` permettent de récupérer et assigner des bonus aux cellules de la carte (basées sur `map_set_cell_type` et consors)
* Plusieurs fonctions de `player.c` permettent de gérer les caractéristiques, suite à une rencontre avec un bonus
* Quand une bombe explose une caisse, le type de la case est remplacé par le bonus

## Bombes (src/bomb.c)

* Une liste chaînée est créée dans la structure `game`.
	* `game->bomb` est un pointeur égal à *NULL* quand aucune bombe n'existe
	* Sinon, c'est la structure d'une bombe, avec l'adresse de la bombe suivante (ou NULL le cas échéant)
* `bomb_init` crée une nouvelle bomb en enregistrant dans sa variable `next` l'adresse d'une autre bombe
* `bombs_update` décrémente le timer des bombs, enregistre le temps restant des bombs dans la carte (pour la sauvegarde), lance les explosions
* `explosion` crée des explosions en croix, en appellant la fonction `exp_fire`, qui s'arrête si elle rencontre un obstacle
	* la fonction `exp_fire` permet également de gérer la destruction de caisse, de monstre (`kill_the_monster_here`) et la décrémentation de vie du joueur (`fire_in_the_hole`)
* `bomb_explode` supprime la bombe en '/raccordant/' la liste des bombes restantes, et crée le feu de l'explosion avec

## Gestion des vies

* La foncion `player_dec_life`(/`player_inc_life`) permet de décrémenter(/incrémenter) la vie du joueur
* Dans `player_update`, appelé depuis `game.c`, si la vie du joueur est inférieur à 1, le jeu s'arrête (depuis /game.c/)

## Panneau d'informations

* `game_banner_display` se charge de diffuser toutes les informations nécessaire

## Monstres (src/monster.c)

* La structure `monster` est une liste chaînée avec les adresses des monstres précédent et suivant (plus simple à supprimer qu'avec les bombes)
* Les déplacements, comme pour les caisses ou le joueur, sont gérés par `monster_move_aux` et `monster_move`. `monsters_move` s'occupe de parcourir la liste des monstres et de supprimer ceux marqué comme `to_kill=1`.

## Fichiers carte et chargement

* `level_get_level` s'occupe de charger les cartes en appellant `map_from_file` (qui s'occupe d'analyser le fichier pour retourner une carte) pour chacune d'elle
	* `count_maps` est utilisé pour savoir combien de cartes un tel niveau possède (en regardant les fichiers dans data/)

## Pause

* La pause est gérée lorsque le joueur est en jeu : dans `game_input_keyboard`, appelé de `state_game_update`
* Une condition sur la variable pause de la structure `game` permet d'appeller ou non toutes les fonctions de mise à jour

## Sauvegarde / Chargement partie

* `load_game` et `save_game` de /game.c/ s'occupe de charger et d'enregistrer un fichier suite à l'appuie sur F9 / F6 (sauvegarde)
	* Les données sont directement sauvegardées en binaire
	* Par une suite d'appel de fonction, level.c et map.c prennent part au chargement
	* `load_map` notamment parcourt toutes les cellules pour renvoyer une carte
	
## Fonctionnalitées bonus

* Splashscreen entre les niveaux / victoire / défaite
* Menu pour choisir différentes options (cf `state_menu`)
* Mini-mode multijoueur
* Sprite non-originel
* Animation for the fires
* Plusieurs debuggeurs
* ...
