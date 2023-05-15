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