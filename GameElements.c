
struct Wall {
} wall;

struct Bomb {
    int explodesIn;
} bomb;

struct Player {
    int nb;
    char color[4]; // rgb
} player;

struct Square {
    // wall | bomb | player | (bomb & player)
} square;

struct Playground {
    struct Square matrix[20]; // 10x10
} playground;

struct Room {
    int id;
    struct Playground playground;
    struct Player* players;
    int maxPlayers;
} room;
