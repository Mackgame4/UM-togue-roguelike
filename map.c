#include <ncurses.h>

/* entitie numbers:
0 - empty space/ground
1 - wall
2 - player
3 - enemy
4 - chest
5 - door
*/
// generate a map with walls 1 and empty space 0
// nrows and ncols should be 1 to close the map
void generate_map(int ncols, int nrows, int map[ncols][nrows]) {
    for (int i = 0; i < ncols; i++) {
        for (int j = 0; j < nrows; j++) {
            if (i == 0 || i == ncols - 1 || j == 0 || j == nrows - 1) {
                map[i][j] = 1;
            } else {
                map[i][j] = 0;
            }
        }
    }
}

// function that draws "#" as walls 1, and " " as empty space 0
void draw_map(int ncols, int nrows, int map[ncols][nrows]) {
    for (int i = 0; i < ncols; i++) {
        for (int j = 0; j < nrows; j++) {
            if (map[i][j] == 1) {
                mvprintw(j, i, "#");
            } else {
                mvprintw(j, i, " ");
            }
        }
    }
}

// returns vector with 2 elements: x and y
int* get_random_free_space(int ncols, int nrows, int map[ncols][nrows]) {
    int x, y;
    do {
        x = rand() % ncols;
        y = rand() % nrows;
    } while (map[x][y] != 0);
    static int coords[2]; // static so that the array is not destroyed after the function ends (it is destroyed after the function ends if it is not static)
    // or: int* coords = malloc(2 * sizeof(int));
    coords[0] = x;
    coords[1] = y;
    return coords;
}