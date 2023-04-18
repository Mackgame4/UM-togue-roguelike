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
// generate a map with walls 1 and empty space 0
// nrows and ncols should be 1 to close the map

// values that can be changed to make the map more or less random
int agglutination = 63; // the bigger the more agglutinated the clusters will be and more space will be left between them so the player can move
int cluster_size = 37; // the bigger the clusters will be
int min_empty_space = 85; // minimum empty space between clusters to player to pass through

// TODO: make sure theres a path from one side of the map to the other
// add an "x" as exit of the map (go to next level) and make sure theres a path to it
// fix the player spawning in middle of clusters (fill them with walls)
// fix the random algorithm that is moving the boundaries of the map and making holes in it (map must be closed) -> maybe try putting the boundaries in the end of the map generation
void generate_map(int ncols, int nrows, int map[ncols][nrows]) {
    // create map boundaries
    for (int i = 0; i < ncols; i++) {
        for (int j = 0; j < nrows; j++) {
            if (i == 0 || i == ncols - 1 || j == 0 || j == nrows - 1) {
                map[i][j] = 1;
            } else {
                map[i][j] = 0;
            }
        }
    }

    srand(time(NULL));
    // create clusters of "#" tiles
    for (int i = 0; i < ncols; i++) {
        for (int j = 0; j < nrows; j++) {
            if (map[i][j] == 0) {
                if (rand() % agglutination == 0) {
                    // generate a cluster of "#" tiles
                    int cluster_x = i;
                    int cluster_y = j;
                    for (int k = 0; k < cluster_size; k++) {
                        int dx = rand() % 3 - 1;
                        int dy = rand() % 3 - 1;
                        if (dx == 0 && dy == 0) {
                            // make sure the center of the cluster is empty
                            map[cluster_x][cluster_y] = 0;
                        } else if (cluster_x + dx >= 0 && cluster_x + dx < ncols && 
                                   cluster_y + dy >= 0 && cluster_y + dy < nrows) {
                            // add a "#" tile to the cluster
                            cluster_x += dx;
                            cluster_y += dy;
                            map[cluster_x][cluster_y] = 1;
                        }
                    }
                    // add empty space around the cluster
                    for (int k = 0; k < min_empty_space; k++) {
                        int dx = rand() % 3 - 1;
                        int dy = rand() % 3 - 1;
                        if (cluster_x + dx >= 0 && cluster_x + dx < ncols && 
                                   cluster_y + dy >= 0 && cluster_y + dy < nrows &&
                                   map[cluster_x + dx][cluster_y + dy] == 0) {
                            // add an empty tile
                            map[cluster_x + dx][cluster_y + dy] = -1;
                        }
                    }
                }
            }
        }
    }

    // replace empty tiles with 0
    for (int i = 0; i < ncols; i++) {
        for (int j = 0; j < nrows; j++) {
            if (map[i][j] == -1) {
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