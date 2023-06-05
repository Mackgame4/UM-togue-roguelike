// declares "PLAYER" AND ENEMY" classes

/**
 * a104365 - Fábio Magalhães
 * Structs/types a serem usados durante as funções do jogo.
 */

// define numbers to represent different types of tiles in the map (prevents errors later)
#define EMPTY_ID 0
#define WALL_ID 1
#define PLAYER_ID 2
#define ENEMY_ID 3
#define DOOR_ID 4
//#define LIGHT_ID 5
//#define SOUND_ID 6
// items
#define KNIFE_ID 7
#define SWORD_ID 8
#define MEDKIT_ID 9

// player state
struct player {
	int x;
	int y;
	int health;
	int weapon; // TODO: add inventory
};
typedef struct player PLAYER;

// enemy state
struct enemy {
	int type;
	int x;
	int y;
	int health;
};
typedef struct enemy ENEMY;