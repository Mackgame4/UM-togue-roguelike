#include <stdio.h>
#include <stdlib.h> // imports exit function
#include <ncurses.h>
//#include <unistd.h> // imports sleep function (to be used in the future)
//#include <time.h> // imports time function (to be used in the future)
#include <string.h> // imports string functions (to be used in the future)
#include <stdbool.h> // imports bool type

//bool is_paused = false; // pause the game when a notification is displayed
//bool is_game_over = false;
bool in_menu = true;

/*void update() {
	int key = getch();
	switch(key) {
		case 'q': endwin(); exit(0); break;
	}
}*/

void initialize_colors() {
	// init_pair(nome, cor1, cor2)
	init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK); // Initialize color pairs to black
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK); // White
	init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK); // Red
	init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK); // Green
	init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK); // Yellow
	init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK); // Blue
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK); // Magenta
	init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK); // Cyan
}

void draw_logo(int ncols, int nrows) {
	char* text_lines[] = {
    " _____              ",
    "|_   _|_______ _ ___",
    "  | | . | . | | | -_|",
    "  |_|___|_  |___|___|",
    "        |___|        "
	};
	int num_lines = sizeof(text_lines) / sizeof(char*);
	for (int i = 0; i < num_lines; i++) {
		attron(COLOR_PAIR(COLOR_RED));
		mvprintw(nrows/2 - num_lines/2 + i - 3, ncols/2 - strlen(text_lines[i])/2, "%s", text_lines[i]); // 3 is the offset
		attroff(COLOR_PAIR(COLOR_RED));
	}
	/*attron(COLOR_PAIR(COLOR_RED));
	char text[] = "Your text-based rogue-like game"; // or "Press ENTER to select"
	mvprintw(nrows/2 + 0, ncols/2 - strlen(text)/2, "%s", text);
	attroff(COLOR_PAIR(COLOR_RED));*/
}

int draw_menu(int ncols, int nrows) {
    int selected_option = 0;
    int key;
    while(1) {
        erase();
		//clear();
		draw_logo(ncols, nrows);
        if (selected_option == 0) {
            attron(COLOR_PAIR(COLOR_GREEN));
			wattron(stdscr, A_BOLD); // A_BOLD is a macro that enables bold text
			wattron(stdscr, A_REVERSE); // A_REVERSE is a macro that enables reverse text colors
			mvprintw(nrows/2 + 1, ncols/2 - strlen("Play")/2, "Play");
			wattroff(stdscr, A_REVERSE);
			wattroff(stdscr, A_BOLD);
            attroff(COLOR_PAIR(COLOR_GREEN));
        } else {
			wattron(stdscr, A_BOLD);
            mvprintw(nrows/2 + 1, ncols/2 - strlen("Play")/2, "Play"); // 1 is the offset
			wattroff(stdscr, A_BOLD);
        }
        if (selected_option == 1) {
            attron(COLOR_PAIR(COLOR_RED));
			wattron(stdscr, A_BOLD);
			wattron(stdscr, A_REVERSE);
            mvprintw(nrows/2 + 2, ncols/2 - strlen("Exit")/2, "Exit");
			wattroff(stdscr, A_REVERSE);
			wattroff(stdscr, A_BOLD);
            attroff(COLOR_PAIR(COLOR_RED));
        } else {
			wattron(stdscr, A_BOLD);
            mvprintw(nrows/2 + 2, ncols/2 - strlen("Exit")/2, "Exit");
			wattroff(stdscr, A_BOLD);
        }
        refresh();
        key = getch();
        switch(key) {
            case KEY_UP: selected_option--; break;
            case KEY_DOWN: selected_option++; break;
            case 13: return selected_option; break;
        }
        if (selected_option < 0) {
            selected_option = 1;
        } else if (selected_option > 1) {
            selected_option = 0;
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
	keypad(stdscr, true); // Enable keypad
	curs_set(0); // Set the cursor to invisible
	// Initialize colors
	start_color(); // start color mode in the terminal and put the background color to black
	initialize_colors();

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
			attron(COLOR_PAIR(COLOR_WHITE));
			mvprintw(0,0,"Playing");
			attroff(COLOR_PAIR(COLOR_WHITE));
		}
		refresh(); // Update the screen
	}
	return 0;
}