void generate_enemies(int ncols, int nrows, int map[ncols][nrows], ENEMY enemies[], int max_enemies) {
    for (int i = 0; i < max_enemies; i++) {
        enemies[i].x = rand() % ncols;
        enemies[i].y = rand() % nrows;
        enemies[i].health = 100;
        enemies[i].type = rand() % 3;
        map[enemies[i].x][enemies[i].y] = 3;
    }
}

// if type is 0, it's a zombien "z"
// if type is 1, it's a skeleton "s"
// if type is 2, it's a ghost "g"
void draw_enemies(ENEMY enemies[], int max_enemies) {
    attron(COLOR_PAIR(COLOR_RED));
    attron(A_BOLD);
    for (int i = 0; i < max_enemies; i++) {
        if (enemies[i].type == 0) {
            mvprintw(enemies[i].y, enemies[i].x, "z");
        } else if (enemies[i].type == 1) {
            mvprintw(enemies[i].y, enemies[i].x, "s");
        } else if (enemies[i].type == 2) {
            mvprintw(enemies[i].y, enemies[i].x, "g");
        }
    }
    attroff(A_BOLD);
    attroff(COLOR_PAIR(COLOR_RED));
}
