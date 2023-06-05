/**
 * a100536 - Pedro Rosário
 * Gera inimigos.
 */
int generate_enemies(int ncols, int nrows, int map[ncols][nrows], ENEMY enemies[], int max_enemies) {
    int enemy_count = 0;
    //int random_enemy_count = rand() % max_enemies + 1; // 1 to max_enemies
    int random_enemy_count = rand() % (max_enemies - 2) + 2; // random between 2 and max_enemies
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

/**
 * a104168 - Jorge Rafael Fernandes
 * Calcula o movimento/ai dos inimigos e o combate.
 */
// move enemies inside player vision towards the player
void enemies_ai(int *killed_by_enemy, int ncols, int nrows, int map[ncols][nrows], PLAYER *player, ENEMY enemies[], int enemy_count) {
    // move the "3"s in the map array
    for (int i = 0; i < enemy_count; i++) {
        int x = enemies[i].x;
        int y = enemies[i].y;
        int player_x = player->x;
        int player_y = player->y;
        int x_diff = player_x - x;
        int y_diff = player_y - y;
        int x_diff_abs = abs(x_diff);
        int y_diff_abs = abs(y_diff);
        int x_diff_sign = x_diff / x_diff_abs;
        int y_diff_sign = y_diff / y_diff_abs;
        if (x_diff_abs > y_diff_abs) {
            if (map[x + x_diff_sign][y] == 0) {
                map[x][y] = 0;
                enemies[i].x += x_diff_sign;
                map[x + x_diff_sign][y] = 3;
            }
        } else {
            if (map[x][y + y_diff_sign] == 0) {
                map[x][y] = 0;
                enemies[i].y += y_diff_sign;
                map[x][y + y_diff_sign] = 3;
            }
        }
    }

    // if enemies is side by side with player, attack
    for (int i = 0; i < enemy_count; i++) {
        int x = enemies[i].x;
        int y = enemies[i].y;
        int player_x = player->x;
        int player_y = player->y;
        int x_diff = player_x - x;
        int y_diff = player_y - y;
        int x_diff_abs = abs(x_diff);
        int y_diff_abs = abs(y_diff);
        if (x_diff_abs == 1 && y_diff_abs == 0) {
            player->health -= get_enemy_damage(enemies[i].type);
        } else if (x_diff_abs == 0 && y_diff_abs == 1) {
            player->health -= get_enemy_damage(enemies[i].type);
        }
        if (player->health <= 0) {
            player->health = 0;
            *killed_by_enemy = 1;
        }
    }
}