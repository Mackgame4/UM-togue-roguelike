#include <ncurses.h>

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
}

int draw_menu(int ncols, int nrows) {
    int selected_option = 0;
    int key;
    while(1) {
        erase();
		draw_logo(ncols, nrows);
        if (selected_option == 0) {
            attron(COLOR_PAIR(COLOR_GREEN));
			wattron(stdscr, A_BOLD);
			wattron(stdscr, A_REVERSE);
			mvprintw(nrows/2 + 1, ncols/2 - strlen("Play")/2, "Play");
			wattroff(stdscr, A_REVERSE);
			wattroff(stdscr, A_BOLD);
            attroff(COLOR_PAIR(COLOR_GREEN));
        } else {
			wattron(stdscr, A_BOLD);
            mvprintw(nrows/2 + 1, ncols/2 - strlen("Play")/2, "Play");
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