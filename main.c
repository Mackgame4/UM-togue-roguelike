#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <stdbool.h>

// Import game modules
#include "types.h"
#include "menus.c"
#include "map.c"
#include "utils.c"
#include "enemies.c"

bool is_paused = false;
char *notification = "The game is paused. Press P to continue.";
bool in_menu = true;
bool is_game_ready = false;

void update(int ncols, int nrows, int map[ncols][nrows], PLAYER *player) {
    int key = getch();
    switch(key) {
        case KEY_UP: if (map[player->x][player->y-1] != 1) {
			map[player->x][player->y] = 0;
			player->y--;
			map[player->x][player->y] = 2;
			} break;
        case KEY_DOWN: if (map[player->x][player->y+1] != 1) {
			map[player->x][player->y] = 0;
			player->y++;
			map[player->x][player->y] = 2;
			} break;
        case KEY_LEFT: if (map[player->x-1][player->y] != 1) {
			map[player->x][player->y] = 0;
			player->x--;
			map[player->x][player->y] = 2;
			} break;
        case KEY_RIGHT: if (map[player->x+1][player->y] != 1) {
			map[player->x][player->y] = 0;
			player->x++;
			map[player->x][player->y] = 2;
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

int main() {
	// Initialize ncurses (game window)
	WINDOW *win = initscr();
	int ncols, nrows;
	getmaxyx(win,nrows,ncols);
	// Settings for ncurses
	cbreak(); // Disable line buffering
	noecho(); // Disable echoing of typed characters
	nonl(); // Disable newline translation
	intrflush(stdscr, false); // Disable flushing of input
	nodelay(stdscr, true); // Disable blocking of getch()
	keypad(stdscr, true); // Enable keypad
	curs_set(0); // Set the cursor to invisible
	start_color();
	initialize_colors();

	// Initialize game
	int map[ncols][nrows];
	PLAYER player;
	player.x = 1;
	player.y = 1;
	player.health = 100;
	int max_enemies = 5;
	ENEMY enemies[max_enemies];

	// Game Variables
	int enemy_count = 0;

	// Game loop
	while(1) {
		erase();
		if (in_menu) {
			is_game_ready = false;
			int selected_option = draw_menu(ncols, nrows);
			if (selected_option == 0) {
				in_menu = false;
			} else if (selected_option == 1) {
				endwin();
				exit(0);
			}
		} else {
			if (!is_game_ready) {
				generate_map(ncols, nrows, map);
				player.x = get_random_free_space_with_min_distance_from_wall(ncols, nrows, map)[0];
				player.y = get_random_free_space_with_min_distance_from_wall(ncols, nrows, map)[1];
				enemy_count = generate_enemies(ncols, nrows, map, enemies, max_enemies);
				is_game_ready = true;
			}
			draw_map(ncols, nrows, map);
			draw_player(player);
			draw_enemies(enemies, enemy_count);
			if (is_paused) {
				draw_notification(ncols, nrows, notification);
			}
			draw_debug_window(ncols, nrows, map, &player);
			update(ncols, nrows, map, &player);
		}
		refresh();
	}
	return 0;
}