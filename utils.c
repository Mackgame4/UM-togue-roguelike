#include <ncurses.h>
#include <stdbool.h>
#include <string.h>

bool DEBUG = false;

void draw_rectangle(int y1, int x1, int y2, int x2) {
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);
    mvvline(y1, x1, 0, y2-y1);
    mvvline(y1, x2, 0, y2-y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}

void draw_debug_window(int ncols, int nrows, int map[ncols][nrows], PLAYER *player) {
    if (DEBUG) {
        draw_rectangle(0, 0, 10, 30);
        attron(COLOR_PAIR(COLOR_BLUE));
        attron(A_BOLD);
        mvprintw(1, 1, "Debug window:");
        attroff(A_BOLD);
        attroff(COLOR_PAIR(COLOR_BLUE));
        // Information to be displayed
        mvprintw(2, 1, "ncols: %d", ncols);
        mvprintw(3, 1, "nrows: %d", nrows);
        mvprintw(4, 1, "player.x: %d", player->x);
        mvprintw(5, 1, "player.y: %d", player->y);
        mvprintw(6, 1, "player.health: %d", player->health);
        // draw map
        mvprintw(7, 1, "map ready: %d", map[0][0]);
        /*for (int i = 0; i < ncols; i++) {
            for (int j = 0; j < nrows; j++) {
                mvprintw(8+j, 1+i, "%d", map[i][j]);
            }
        }*/
    }
}

int notify = 0;
void draw_notification(int ncols, int nrows, char *message) {
    int text_length = strlen(message);
    draw_rectangle(nrows/2-2, ncols/2-text_length/2-1, nrows/2, ncols/2+text_length/2+1);
    attron(A_BOLD);
    attron(COLOR_PAIR(COLOR_RED));
    mvprintw(nrows/2-1, ncols/2-text_length/2, "%s", message);
    attroff(COLOR_PAIR(COLOR_RED));
    attron(A_REVERSE);
    mvprintw(nrows/2, ncols/2, "OK");
    attroff(A_REVERSE);
    attroff(A_BOLD);
}

void initialize_colors() {
	init_pair(COLOR_BLACK, COLOR_BLACK, COLOR_BLACK); // Initialize color pairs to black
	init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK); // White
	init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK); // Red
	init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK); // Green
	init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK); // Yellow
	init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK); // Blue
	init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK); // Magenta
	init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK); // Cyan
}