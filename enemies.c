int generate_enemies(int ncols, int nrows, int map[ncols][nrows], ENEMY enemies[], int max_enemies) {
    int enemy_count = 0;
    int random_enemy_count = rand() % max_enemies + 1; // 1 - max_enemies
    for (int i = 0; i < random_enemy_count; i++) {
        enemies[i].x = get_random_free_space_with_min_distance_from_wall(ncols, nrows, map)[0];
        enemies[i].y = get_random_free_space_with_min_distance_from_wall(ncols, nrows, map)[1];
        enemies[i].health = 100;
        enemies[i].type = rand() % 3;
        map[enemies[i].x][enemies[i].y] = 3;
        enemy_count++;
    }
    return enemy_count;
}

void draw_enemies(ENEMY enemies[], int enemy_count) {
    attron(COLOR_PAIR(COLOR_RED));
    attron(A_BOLD);
    for (int i = 0; i < enemy_count; i++) {
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