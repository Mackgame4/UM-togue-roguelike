// declares "PLAYER" AND ENEMY" classes

// define numbers to represent different types of tiles in the map (prevents errors later)
#define EMPTY_ID 0
#define WALL_ID 1
#define PLAYER_ID 2
#define ENEMY_ID 3
#define CHEST_ID 4
#define DOOR_ID 5

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