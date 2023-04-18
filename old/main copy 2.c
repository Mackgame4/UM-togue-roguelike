#include <ncurses.h>

int main()
{
    // Initialize ncurses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

	// Set the cursor to invisible
	curs_set(0);

    // Define menu items
    char *menu_items[] = {
        "Jogar",
        "Opções",
        "Sair"
    };

    int num_items = sizeof(menu_items) / sizeof(char*);

    // Create menu window
    int win_height = num_items + 2;
    int win_width = 20;
    int win_y = (LINES - win_height) / 2;
    int win_x = (COLS - win_width) / 2;

    WINDOW *menu_win = newwin(win_height, win_width, win_y, win_x);
    box(menu_win, 0, 0);

    // Display menu items
    int highlight = 0;

    // Redraw menu window and refresh screen
    wrefresh(menu_win);
    refresh();

    while (1) {
        // Highlight current item
        wclear(menu_win); // Clear window before redrawing
        for (int i = 0; i < num_items; i++) {
            if (i == highlight) {
                wattron(menu_win, A_REVERSE);
            }
            mvwprintw(menu_win, i+1, 1, "%s", menu_items[i]);
            wattroff(menu_win, A_REVERSE);
        }

        // Refresh menu window
        wrefresh(menu_win);

        // Wait for user input
        int ch = getch();

        // Process user input
        switch (ch) {
            case KEY_UP:
                highlight--;
                if (highlight < 0) {
                    highlight = num_items - 1;
                }
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight >= num_items) {
                    highlight = 0;
                }
                break;
            case 10: // Enter key
                if (highlight == num_items - 1) {
                    goto end;
                } else {
                    // Do something with selected item
                }
                break;
        }
    }

end:
    // Clean up and exit
    delwin(menu_win);
    endwin();
    return 0;
}
