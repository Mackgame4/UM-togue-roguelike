#include <stdio.h>
#include <stdlib.h> // imports exit function
#include <ncurses.h>
//#include <unistd.h> // imports sleep function (to be used in the future)
#include <time.h> // imports time function (to be used in the future)
#include <string.h> // imports string functions
#include <stdbool.h> // imports bool type

// Import game modules
#include "types.h"
#include "menus.c"
#include "utils.c"
#include "map.c"
#include "enemies.c"

bool is_paused = false; // pause the game when a notification is displayed
char *notification = "The game is paused. Press P to continue.";
bool in_menu = true;
bool is_game_ready = false;
int killed_by_enemy = 0; // used to check if the player was killed by an enemy (lazy fix for the combat loop and dont use the is_paused variable as pointer, etc)

static int prev_pos[2]; // used to store the previous position of the player (to be used in the future)

void update(int ncols, int nrows, int map[ncols][nrows], PLAYER *player) {
    int key = getch();
    switch(key) {
		case 119: // 119 is the w key
        case KEY_UP: if (!is_paused && map[player->x][player->y-1] == 0) {
			map[player->x][player->y] = 0;
			player->y--;
			map[player->x][player->y] = 2; // add player x and y to map as a 2 (so we can calculate the vision path and intersect with walls)
			} else if (map[player->x][player->y-1] == DOOR_ID || map[player->x][player->y-1] == KNIFE_ID || map[player->x][player->y-1] == SWORD_ID || map[player->x][player->y-1] == MEDKIT_ID || map[player->x][player->y-1] == ENEMY_ID) { // if the player is on a door, dont replace it
				prev_pos[0] = player->x;
				prev_pos[1] = player->y;
				map[prev_pos[0]][prev_pos[1]] = 0; // always remove the ghost player from the map
				player->y--;
			} break;
		case 115: // 115 is the s key
        case KEY_DOWN: if (!is_paused && map[player->x][player->y+1] == 0) {
			map[player->x][player->y] = 0;
			player->y++;
			map[player->x][player->y] = 2;
			} else if (map[player->x][player->y+1] == DOOR_ID || map[player->x][player->y+1] == KNIFE_ID || map[player->x][player->y+1] == SWORD_ID || map[player->x][player->y+1] == MEDKIT_ID || map[player->x][player->y+1] == ENEMY_ID) {
				prev_pos[0] = player->x;
				prev_pos[1] = player->y;
				map[prev_pos[0]][prev_pos[1]] = 0;
				player->y++;
			} break;
		case 97: // 97 is the a key
        case KEY_LEFT: if (!is_paused && map[player->x-1][player->y] == 0) {
			map[player->x][player->y] = 0;
			player->x--;
			map[player->x][player->y] = 2;
			} else if (map[player->x-1][player->y] == DOOR_ID || map[player->x-1][player->y] == KNIFE_ID || map[player->x-1][player->y] == SWORD_ID || map[player->x-1][player->y] == MEDKIT_ID || map[player->x-1][player->y] == ENEMY_ID) {
				prev_pos[0] = player->x;
				prev_pos[1] = player->y;
				map[prev_pos[0]][prev_pos[1]] = 0;
				player->x--;
			} break;
		case 100: // 100 is the d key
        case KEY_RIGHT: if (!is_paused && map[player->x+1][player->y] == 0) {
			map[player->x][player->y] = 0;
			player->x++;
			map[player->x][player->y] = 2;
			} else if (map[player->x+1][player->y] == DOOR_ID || map[player->x+1][player->y] == KNIFE_ID || map[player->x+1][player->y] == SWORD_ID || map[player->x+1][player->y] == MEDKIT_ID || map[player->x+1][player->y] == ENEMY_ID) {
				prev_pos[0] = player->x;
				prev_pos[1] = player->y;
				map[prev_pos[0]][prev_pos[1]] = 0;
				player->x++;
			} break;
        case 27: in_menu = true; break; // 27 is the escape key
        case 113: in_menu = true; break; // 113 is the q key
		case 112: is_paused = !is_paused; notification = "The game is paused. Press P to continue."; break; // 112 is the p key
		case 32: is_paused = false; break; // 32 is the space key
		case 13: is_paused = false; break; // 13 is enter key
    }
}

void draw_player(PLAYER player) {
	attron(COLOR_PAIR(COLOR_BLUE));
	attron(A_BOLD);
	mvprintw(player.y, player.x, "@");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(COLOR_BLUE));
}

// Shows player health, weapon, and enemies health in combat
void draw_hud(int ncols, int nrows, PLAYER player, ENEMY enemies[], int enemy_count) {
	// Draw player health
	attron(COLOR_PAIR(COLOR_RED));
	attron(A_BOLD);
	// make the player bar fit the screen width
	int health_bar_width = player.health * ncols / 3 / 100;
	int health_bar_max = 100 * ncols / 3 / 100;
	draw_bar(nrows, 0, health_bar_width, health_bar_max, "Health");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(COLOR_RED));
	// Draw player weapon
	attron(COLOR_PAIR(COLOR_YELLOW));
	attron(A_BOLD);
	mvprintw(nrows, health_bar_max-4, "Equipped: %s", get_weapon_name(player.weapon));
	attroff(A_BOLD);
	attroff(COLOR_PAIR(COLOR_YELLOW));
	// Draw enemies health
	int string_offset = strlen("Equipped: ") + strlen(get_weapon_name(player.weapon));
	attron(COLOR_PAIR(COLOR_BLUE));
	attron(A_BOLD);
	mvprintw(nrows, health_bar_max+2+string_offset-2, "%d Enemies: ", enemy_count);
	int string_offset2 = strlen("Enemies: ");
	int free_space = ncols - (health_bar_max+2+string_offset+string_offset2); // Calculates the free space left till the end of the screen
	int free_space_per_enemy = free_space / enemy_count;
	for (int i = 0; i < enemy_count; i++) {
		int enemy_health_bar_width = enemies[i].health * free_space_per_enemy / 100;
		int enemy_health_bar_max = 100 * free_space_per_enemy / 100;
		draw_bar(nrows, health_bar_max+2+string_offset+2+string_offset2+i*free_space_per_enemy, enemy_health_bar_width, enemy_health_bar_max, get_enemy_name(enemies[i].type));
	}
	attroff(A_BOLD);
	attroff(COLOR_PAIR(COLOR_BLUE));
}

// function that will check if the player is on top of an enemy (give damage) or on top of a weapon (pick it up) or on top of the exit (generate a new map)
void check_player_collision(int ncols, int nrows, PLAYER *player, ENEMY enemies[], int *enemy_count, int map[ncols][nrows]) {
	// check if player collided with a special item
	for (int i = 0; i < ncols; i++) {
		for (int j = 0; j < nrows; j++) {
			if (player->x == i && player->y == j) {
				if (map[i][j] == 4) {
					is_game_ready = false; // by changing this the main loop will generate a new map and set back to true
				} else if (map[i][j] == KNIFE_ID) {
					player->weapon = 1;
					map[i][j] = 2; // set the player to be on top of the weapon (replacing it)
					is_paused = true;
					notification = "You picked up a Knife!";
				} else if (map[i][j] == SWORD_ID) {
					player->weapon = 2;
					map[i][j] = 2;
					is_paused = true;
					notification = "You picked up a Sword!";
				} else if (map[i][j] == MEDKIT_ID) {
					player->health = 100;
					map[i][j] = 2;
					is_paused = true;
					notification = "You picked up a Medkit!";
				} else if (map[i][j] == ENEMY_ID) {
					// combat logic
					for (int k = 0; k < *enemy_count; k++) {
						if (enemies[k].x == i && enemies[k].y == j) {
							// player attacks enemy
							enemies[k].health -= get_weapon_damage(player->weapon);
							if (enemies[k].health <= 0) {
								enemies[k].health = 0;
								// enemy is dead
								*enemy_count -= 1;
								// remove enemy from the array
								for (int l = k; l < *enemy_count; l++) {
									enemies[l] = enemies[l+1];
								}
								// remove enemy from the map
								// TODO: fix when player picks weapon/combat
								map[i][j] = 2;
								// add a new enemy to the map
								int enemy_x = rand() % ncols;
								int enemy_y = rand() % nrows;
								while (map[enemy_x][enemy_y] != 2) {
									enemy_x = rand() % ncols;
									enemy_y = rand() % nrows;
								}
								map[enemy_x][enemy_y] = ENEMY_ID;
								// show notification
								is_paused = true;
								notification = "You killed an enemy!";
							} else {
								// enemy is still alive
								// enemy attacks player
								player->health -= get_enemy_damage(enemies[k].type);
								if (player->health <= 0) {
									// player is dead during combat
									is_paused = true;
									notification = "You died!";
									is_game_ready = false;
								}
							}
							// return player to the previous position so it doent loop the combat
							player->x = prev_pos[0];
							player->y = prev_pos[1];
							map[prev_pos[0]][prev_pos[1]] = 2;
						}
					}
				}
			}
		}
	}
}

int main() {
	// Initialize ncurses (game window)
	WINDOW *win = initscr();
	int ncols, nrows;
	getmaxyx(win,nrows,ncols); // return window size (so we can generate a map based on it)
	// Settings for ncurses
	cbreak(); // Disable line buffering
	noecho(); // Disable echoing of typed characters
	nonl(); // Disable newline translation
	intrflush(stdscr, false); // Disable flushing of input
	nodelay(stdscr, true); // Disable blocking of getch()
	keypad(stdscr, true); // Enable keypad
	curs_set(0); // Set the cursor to invisible
	// Initialize colors
	start_color(); // start color mode in the terminal and put the background color to black
	initialize_colors();

	// Initialize game
	nrows = nrows-1; // Reserve the last row for the HUD
	int map[ncols][nrows]; // Create a map with the same size as the window
	PLAYER player;
	int max_enemies; // Maximum number of enemies
	if (ncols < 100) { // if ncols is little max_enemies = 3, else max_enemies = 5
		max_enemies = 3;
	} else {
		max_enemies = 4; // 5
	}
	ENEMY enemies[max_enemies];

	// Game Variables
	int enemy_count = 0;
	time_t last_enemy_movement_time = time(NULL); // Initialize the last enemy movement time to the current time

	// Game loop
	while(1) {
		//clear(); // Clear the screen every frame (to avoid drawing on top of the previous frame, aka update)
		erase(); // Erase the contents of the window (leaving the background color intact)
		if (in_menu) {
			is_game_ready = false;
			int selected_option = draw_menu(ncols, nrows);
			if (selected_option == 0) {
				in_menu = false;
			} else if (selected_option == 1) {
				int option = draw_options_menu(ncols, nrows);
				if (option == 0) {
					in_menu = true;
				}
			} else if (selected_option == 2) {
				endwin();
				exit(0); // or return 0;
			}
		} else {
			// Only execute this code once (setup the game)
			if (!is_game_ready) {
				generate_map(ncols, nrows, map);
				player.x = get_random_free_space_with_min_distance_from_wall(ncols, nrows, map)[0];
				player.y = get_random_free_space_with_min_distance_from_wall(ncols, nrows, map)[1];
				map[player.x][player.y] = 2;
				player.health = 100;
				player.weapon = 0;
				enemy_count = generate_enemies(ncols, nrows, map, enemies, max_enemies);
				is_game_ready = true;
			}
			// Draw map and player every frame and listen for input
			draw_map(ncols, nrows, map, &player, enemies, enemy_count);
			draw_player(player);
			draw_hud(ncols, nrows, player, enemies, enemy_count);
			if (is_paused) {
				draw_notification(ncols, nrows, notification);
			} else {
				check_player_collision(ncols, nrows, &player, enemies, &enemy_count, map);
				// Check if enough time has passed since the last enemy movement
				time_t current_time = time(NULL);
				double time_diff = difftime(current_time, last_enemy_movement_time);
				if (time_diff >= 0.8) { // moves every 0.8 seconds
					enemies_ai(&killed_by_enemy, ncols, nrows, map, &player, enemies, enemy_count);
					last_enemy_movement_time = current_time;
				}
				if (killed_by_enemy == 1) {
					// player is dead by enemy
					is_paused = true;
					notification = "You died!";
					is_game_ready = false;
					killed_by_enemy = 0;
				}
			}
			draw_debug_window(ncols, nrows, map, &player);
			update(ncols, nrows, map, &player);
		}
		refresh(); // Update the screen
	}
	endwin();
	return 0;
}