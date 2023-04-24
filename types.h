// declares "PLAYER" AND ENEMY" classes

//#define PLAYER_DAMAGE 10

// define numbers to represent different types of tiles in the map (prevents errors later)
/*#define EMPTY 0
#define WALL 1
#define VISIBLE_WALL 2
#define PLAYER 3
#define ENEMY 4
#define VISIBLE_ENEMY 5*/

// player state
struct player {
	int x;
	int y;
	int health;
	// TODO: add inventory
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