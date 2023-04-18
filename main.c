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

//bool is_paused = false; // pause the game when a notification is displayed
//bool is_game_over = false;
bool in_menu = true;
bool is_game_ready = false;

void update(int ncols, int nrows, int map[ncols][nrows], PLAYER *player) {
    int key = getch();
    switch(key) {
        case KEY_UP: if (map[player->x][player->y-1] != 1) { player->y--; } break;
        case KEY_DOWN: if (map[player->x][player->y+1] != 1) { player->y++; } break;
        case KEY_LEFT: if (map[player->x-1][player->y] != 1) { player->x--; } break;
        case KEY_RIGHT: if (map[player->x+1][player->y] != 1) { player->x++; } break;
        case 27: in_menu = true; break; // 27 is the escape key
        case 113: in_menu = true; break; // 113 is the q key
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
	int map[ncols][nrows]; // Create a map with the same size as the window
	PLAYER player;
	player.x = 1;
	player.y = 1;
	player.health = 100;

	// Game loop
	while(1) {
		//clear(); // Clear the screen every frame (to avoid drawing on top of the previous frame, aka update)
		erase(); // Erase the contents of the window (leaving the background color intact)
		if (in_menu) {
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
				generate_map(ncols, nrows, map); // TODO: generate a new map every time the player dies/goes to the menu
				player.x = get_random_free_space(ncols, nrows, map)[0];
				player.y = get_random_free_space(ncols, nrows, map)[1];
				is_game_ready = true;
			}
			// Draw map and player every frame and listen for input
			draw_map(ncols, nrows, map);
			attron(COLOR_PAIR(COLOR_BLUE));
			attron(A_BOLD);
			mvprintw(player.y, player.x, "@");
			attroff(A_BOLD);
			attroff(COLOR_PAIR(COLOR_BLUE));
			// add player x and y to map as a 2 (so we can calculate the vision path and intersect with walls)
			map[player.x][player.y] = 2; // TODO: fix, remove 2 from the last position
			draw_debug_window(ncols, nrows, &player);
			update(ncols, nrows, map, &player);
		}
		refresh(); // Update the screen
	}
	return 0;
}