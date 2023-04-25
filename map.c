#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

/* entitie numbers:
0 - empty space/ground
1 - wall
2 - player
3 - enemy
4 - chest
5 - door
*/

// values that can be changed to make the map more or less random
int agglutination = 38; // chance of a rock to spawn
int min_empty_space = 15; // minimum empty space between clusters to player to pass through

// TODO: make sure theres a path from one side of the map to the other
// add an "x" as exit of the map (go to next level) and make sure theres a path to it
void generate_map(int ncols, int nrows, int map[ncols][nrows]) {
    // Set the random seed
    srand(time(NULL));

    // Fill the map with random rocks
    for (int i = 0; i < ncols; i++) {
        for (int j = 0; j < nrows; j++) {
            if ((rand() % 100) < 40) {
                // Set a random chance for a rock to spawn
                map[i][j] = 1;
            } else {
                map[i][j] = 0;
            }
        }
    }

    // Smooth out the cave
    for (int i = 0; i < 5; i++) {
        for (int x = 1; x < ncols - 1; x++) {
            for (int y = 1; y < nrows - 1; y++) {
                int count = 0;
                for (int dx = -1; dx <= 1; dx++) {
                    for (int dy = -1; dy <= 1; dy++) {
                        if (map[x + dx][y + dy] == 1) {
                            count++;
                        }
                    }
                }
                if (count > 4) {
                    map[x][y] = 1;
                } else if (count < 3) {
                    map[x][y] = 0;
                }
            }
        }
    }

    // Revert all the map, where 1 put 0, and 0 put 1
    for (int i = 0; i < ncols; i++) {
        for (int j = 0; j < nrows; j++) {
            if (map[i][j] == 1) {
                map[i][j] = 0;
            } else {
                map[i][j] = 1;
            }
        }
    }

    // Draw boundaries
    for (int i = 0; i < ncols; i++) {
        for (int j = 0; j < nrows; j++) {
            if (j == 0 || j == nrows - 1 || i == 0 || i == ncols - 1) {
                map[i][j] = 1;
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

// get random free space with a minimum distance from a wall (to avoid spawning player/enemies inside walls)
int min_distance = 8;
int* get_random_free_space_with_min_distance_from_wall(int ncols, int nrows, int map[ncols][nrows]) {
    int x, y;
    do {
        x = rand() % (ncols - 2 * min_distance) + min_distance;
        y = rand() % (nrows - 2 * min_distance) + min_distance;
    } while (map[x][y] != 0);
    static int coords[2];
    coords[0] = x;
    coords[1] = y;
    return coords;
}