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
            attron(COLOR_PAIR(COLOR_YELLOW));
			wattron(stdscr, A_BOLD);
			wattron(stdscr, A_REVERSE);
            mvprintw(nrows/2 + 2, ncols/2 - strlen("Options")/2, "Options");
			wattroff(stdscr, A_REVERSE);
			wattroff(stdscr, A_BOLD);
            attroff(COLOR_PAIR(COLOR_YELLOW));
        } else {
			wattron(stdscr, A_BOLD);
            mvprintw(nrows/2 + 2, ncols/2 - strlen("Options")/2, "Options");
			wattroff(stdscr, A_BOLD);
        }
        if (selected_option == 2) {
            attron(COLOR_PAIR(COLOR_RED));
			wattron(stdscr, A_BOLD);
			wattron(stdscr, A_REVERSE);
            mvprintw(nrows/2 + 3, ncols/2 - strlen("Exit")/2, "Exit");
			wattroff(stdscr, A_REVERSE);
			wattroff(stdscr, A_BOLD);
            attroff(COLOR_PAIR(COLOR_RED));
        } else {
			wattron(stdscr, A_BOLD);
            mvprintw(nrows/2 + 3, ncols/2 - strlen("Exit")/2, "Exit");
			wattroff(stdscr, A_BOLD);
        }
        refresh();
        key = getch();
        switch(key) {
            case KEY_UP: selected_option--; break;
            case KEY_DOWN: selected_option++; break;
            case 13: return selected_option; break;
        }
        if (selected_option < 0) selected_option = 2;
        if (selected_option > 2) selected_option = 0;
    }
}

#define N_OPTIONS 4
int enabled_options[N_OPTIONS] = {0};
int draw_options_menu(int ncols, int nrows) {
    int selected_option = 0;
    while(1) {
        erase();
		draw_logo(ncols, nrows);
        /*attron(COLOR_PAIR(COLOR_YELLOW));
        wattron(stdscr, A_BOLD);
        mvprintw(nrows/2 + 1, ncols/2 - strlen("Options")/2, "Options");
        wattroff(stdscr, A_BOLD);
        attroff(COLOR_PAIR(COLOR_YELLOW));*/
        if (selected_option == 0) {
            char *option_text = "Circle light";
            if (enabled_options[0]) {
                attron(COLOR_PAIR(COLOR_GREEN));
                option_text = "Circle light - ON";
            } else {
                attron(COLOR_PAIR(COLOR_RED));
                option_text = "Circle light - OFF";
            }
            wattron(stdscr, A_BOLD);
            wattron(stdscr, A_REVERSE);
            mvprintw(nrows/2 + 1, ncols/2 - strlen(option_text)/2, "%s", option_text);
            wattroff(stdscr, A_REVERSE);
            wattroff(stdscr, A_BOLD);
            if (enabled_options[0]) {
                attroff(COLOR_PAIR(COLOR_GREEN));
            } else {
                attroff(COLOR_PAIR(COLOR_RED));
            }
        } else {
            wattron(stdscr, A_BOLD);
            mvprintw(nrows/2 + 1, ncols/2 - strlen("Circle light")/2, "Circle light");
            wattroff(stdscr, A_BOLD);
        }
        if (selected_option == 1) {
            char *option_text = "Draw all map";
            if (enabled_options[1]) {
                attron(COLOR_PAIR(COLOR_GREEN));
                option_text = "Draw all map - ON";
            } else {
                attron(COLOR_PAIR(COLOR_RED));
                option_text = "Draw all map - OFF";
            }
            wattron(stdscr, A_BOLD);
            wattron(stdscr, A_REVERSE);
            mvprintw(nrows/2 + 2, ncols/2 - strlen(option_text)/2, "%s", option_text);
            wattroff(stdscr, A_REVERSE);
            wattroff(stdscr, A_BOLD);
            if (enabled_options[1]) {
                attroff(COLOR_PAIR(COLOR_GREEN));
            } else {
                attroff(COLOR_PAIR(COLOR_RED));
            }
        } else {
            wattron(stdscr, A_BOLD);
            mvprintw(nrows/2 + 2, ncols/2 - strlen("Draw all map")/2, "Draw all map");
            wattroff(stdscr, A_BOLD);
        }
        if (selected_option == 2) {
            char *option_text = "Debug light";
            if (enabled_options[2]) {
                attron(COLOR_PAIR(COLOR_GREEN));
                option_text = "Debug light - ON";
            } else {
                attron(COLOR_PAIR(COLOR_RED));
                option_text = "Debug light - OFF";
            }
            wattron(stdscr, A_BOLD);
            wattron(stdscr, A_REVERSE);
            mvprintw(nrows/2 + 3, ncols/2 - strlen(option_text)/2, "%s", option_text);
            wattroff(stdscr, A_REVERSE);
            wattroff(stdscr, A_BOLD);
            if (enabled_options[2]) {
                attroff(COLOR_PAIR(COLOR_GREEN));
            } else {
                attroff(COLOR_PAIR(COLOR_RED));
            }
        } else {
            wattron(stdscr, A_BOLD);
            mvprintw(nrows/2 + 3, ncols/2 - strlen("Debug light")/2, "Debug light");
            wattroff(stdscr, A_BOLD);
        }
        if (selected_option == 3) {
            char *option_text = "Debug mode";
            if (enabled_options[3]) {
                attron(COLOR_PAIR(COLOR_GREEN));
                option_text = "Debug mode - ON";
            } else {
                attron(COLOR_PAIR(COLOR_RED));
                option_text = "Debug mode - OFF";
            }
            wattron(stdscr, A_BOLD);
            wattron(stdscr, A_REVERSE);
            mvprintw(nrows/2 + 4, ncols/2 - strlen(option_text)/2, "%s", option_text);
            wattroff(stdscr, A_REVERSE);
            wattroff(stdscr, A_BOLD);
            if (enabled_options[3]) {
                attroff(COLOR_PAIR(COLOR_GREEN));
            } else {
                attroff(COLOR_PAIR(COLOR_RED));
            }
        } else {
            wattron(stdscr, A_BOLD);
            mvprintw(nrows/2 + 4, ncols/2 - strlen("Debug mode")/2, "Debug mode");
            wattroff(stdscr, A_BOLD);
        }
        if (selected_option == N_OPTIONS) {
            attron(COLOR_PAIR(COLOR_RED));
            wattron(stdscr, A_BOLD);
            wattron(stdscr, A_REVERSE);
            mvprintw(nrows/2 + 6, ncols/2 - strlen("Back")/2, "Back");
            wattroff(stdscr, A_REVERSE);
            wattroff(stdscr, A_BOLD);
            attroff(COLOR_PAIR(COLOR_RED));
        } else {
            wattron(stdscr, A_BOLD);
            mvprintw(nrows/2 + 6, ncols/2 - strlen("Back")/2, "Back");
            wattroff(stdscr, A_BOLD);
        }
        refresh();
        int key = getch();
        switch(key) {
            case KEY_UP: selected_option--; break;
            case KEY_DOWN: selected_option++; break;
            case 13: if (selected_option == N_OPTIONS) {
                return selected_option;
            } else {
                enabled_options[selected_option] = !enabled_options[selected_option];
            } break;
        }
        if (selected_option < 0) selected_option = N_OPTIONS;
        if (selected_option > N_OPTIONS) selected_option = 0;
    }
}

int get_menu_option_state(int option) {
    return enabled_options[option];
}