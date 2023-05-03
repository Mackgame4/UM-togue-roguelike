#include <stdio.h>
#include <stdlib.h> // imports exit function
#include <ncurses.h>
//#include <unistd.h> // imports sleep function (to be used in the future)
//#include <time.h> // imports time function (to be used in the future)
#include <string.h> // imports string functions
#include <stdbool.h> // imports bool type

// Import game modules
#include "types.h"
#include "menus.c"
#include "map.c"
#include "utils.c"
#include "enemies.c"

bool is_paused = false; // pause the game when a notification is displayed
char *notification = "The game is paused. Press P to continue.";
bool in_menu = true;
bool is_game_ready = false;

void update(int ncols, int nrows, int map[ncols][nrows], PLAYER *player) {
    int key = getch();
    switch(key) {
        case KEY_UP: if (map[player->x][player->y-1] == 0) {
			map[player->x][player->y] = 0;
			player->y--;
			map[player->x][player->y] = 2; // add player x and y to map as a 2 (so we can calculate the vision path and intersect with walls)
			} else if (map[player->x][player->y-1] == DOOR_ID || map[player->x][player->y-1] == KNIFE_ID || map[player->x][player->y-1] == SWORD_ID) { // if the player is on a door, dont replace it
				map[player->x][player->y] = 0; // always remove the ghost player from the map
				player->y--;
			} break;
        case KEY_DOWN: if (map[player->x][player->y+1] == 0) {
			map[player->x][player->y] = 0;
			player->y++;
			map[player->x][player->y] = 2;
			} else if (map[player->x][player->y+1] == DOOR_ID || map[player->x][player->y+1] == KNIFE_ID || map[player->x][player->y+1] == SWORD_ID) {
				map[player->x][player->y] = 0;
				player->y++;
			} break;
        case KEY_LEFT: if (map[player->x-1][player->y] == 0) {
			map[player->x][player->y] = 0;
			player->x--;
			map[player->x][player->y] = 2;
			} else if (map[player->x-1][player->y] == DOOR_ID || map[player->x-1][player->y] == KNIFE_ID || map[player->x-1][player->y] == SWORD_ID) {
				map[player->x][player->y] = 0;
				player->x--;
			} break;
        case KEY_RIGHT: if (map[player->x+1][player->y] == 0) {
			map[player->x][player->y] = 0;
			player->x++;
			map[player->x][player->y] = 2;
			} else if (map[player->x+1][player->y] == DOOR_ID || map[player->x+1][player->y] == KNIFE_ID || map[player->x+1][player->y] == SWORD_ID) {
				map[player->x][player->y] = 0;
				player->x++;
			} break;
        case 27: in_menu = true; break; // 27 is the escape key
        case 113: in_menu = true; break; // 113 is the q key
		case 112: is_paused = !is_paused; break; // 112 is the p key
		case 32: is_paused = false; break; // 32 is the space key
    }
}

void draw_player(PLAYER player) {
	attron(COLOR_PAIR(COLOR_BLUE));
	attron(A_BOLD);
	mvprintw(player.y, player.x, "@");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(COLOR_BLUE));
}

char* get_weapon_name(int weapon) {
	switch(weapon) {
		case 0: return "Hand";
		case 1: return "Knife";
		case 2: return "Sword";
		default: return "Hand";
	}
}

// TODO:
int get_weapon_damage(int weapon) {
	switch(weapon) {
		case 0: return 10;
		case 1: return 15;
		case 2: return 20;
		default: return 10;
	}
}

char* get_enemy_name(int enemy_type) {
	switch(enemy_type) {
		case 0: return "Zombie";
		case 1: return "Skeleton";
		case 2: return "Ghost";
		default: return "Zombie";
	}
}

// TODO:
int get_enemy_damage(int enemy_type) {
	switch(enemy_type) {
		case 0: return 5;
		case 1: return 15;
		case 2: return 20;
		default: return 5;
	}
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
	mvprintw(nrows, health_bar_max+2+string_offset+2-4, "%d Enemies: ", enemy_count);
	for (int i = 0; i < enemy_count; i++) {
		int enemy_health_bar_width = enemies[i].health * ncols / 2 / 100 / enemy_count;
		int enemy_health_bar_max = 100 * ncols / 2 / 100 / enemy_count;
		draw_bar(nrows, health_bar_max+2+string_offset+2+strlen("Enemies:  ")+i*enemy_health_bar_max, enemy_health_bar_width, enemy_health_bar_max, get_enemy_name(enemies[i].type));
	}
	attroff(A_BOLD);
	attroff(COLOR_PAIR(COLOR_BLUE));
}

// function that will check if the player is on top of an enemy (give damage) or on top of a weapon (pick it up) or on top of the exit (generate a new map)
void check_player_collision(int ncols, int nrows, PLAYER *player, ENEMY enemies[], int *enemy_count, int map[ncols][nrows]) {
	for (int i = 0; i < ncols; i++) {
		for (int j = 0; j < nrows; j++) {
			if (player->x == i && player->y == j) {
				if (map[i][j] == 4) {
					is_game_ready = false; // by changing this the main loop will generate a new map and set back to true
				} else if (map[i][j] == 7) {
					player->weapon = 1;
					map[i][j] = 2; // set the player to be on top of the weapon (replacing it)
				} else if (map[i][j] == 8) {
					player->weapon = 2;
					map[i][j] = 2;
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
	int max_enemies = 5; // Maximum number of enemies
	ENEMY enemies[max_enemies];

	// Game Variables
	int enemy_count = 0;

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
				endwin();
				exit(0); // or return 0;
			}
		} else {
			// Only execute this code once (setup the game)
			if (!is_game_ready) {
				generate_map(ncols, nrows, map);
				player.x = get_random_free_space_with_min_distance_from_wall(ncols, nrows, map)[0];
				player.y = get_random_free_space_with_min_distance_from_wall(ncols, nrows, map)[1];
				player.health = 100;
				player.weapon = 0;
				enemy_count = generate_enemies(ncols, nrows, map, enemies, max_enemies);
				is_game_ready = true;
			}
			// Draw map and player every frame and listen for input
			draw_map(ncols, nrows, map);
			draw_player(player);
			draw_enemies(enemies, enemy_count);
			draw_hud(ncols, nrows, player, enemies, enemy_count);
			// function that will check if the player is on top of an enemy (give damage) or on top of a weapon (pick it up) or on top of the exit (generate a new map)
			// TODO: check_player_collision(ncols, nrows, &player, enemies, &enemy_count, map);
			check_player_collision(ncols, nrows, &player, enemies, &enemy_count, map);
			if (is_paused) {
				draw_notification(ncols, nrows, notification);
			}
			draw_debug_window(ncols, nrows, map, &player);
			update(ncols, nrows, map, &player);
		}
		refresh(); // Update the screen
	}
	return 0;
}