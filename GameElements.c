enum LAYERS { LAYER1, LAYER2, LAYER3 };

enum TILES { EMPTY = 0, TOP_WALL = 1, GROUND = 2, LEFT_WALL = 3,
                   RIGHT_WALL = 4, BOTTOM_WALL = 5, HAY = 6, TOP_LEFT_CORNER = 7,
                   TOP_RIGHT_CORNER = 8, BOTTOM_LEFT_CORNER = 9, BOTTOM_RIGHT_CORNER = 10,
                   BUTTER = 11, PILLAR = 12
};

struct Wall {
} wall;

struct Bomb {
    int explodesIn;
} bomb;

struct Player {
    int nb;
    int socket;
    int x;
    int y;
} player;

struct Square {
    // wall | bomb | player | (bomb & player)
} square;

struct Playground {
    char *mapInfo;
    int **layer1;
    int **layer2;
    int **layer3;
} playground;

struct Lobby {
    int client_1_socket_fd;
    int client_2_socket_fd;
    int client_3_socket_fd;
    int client_4_socket_fd;
} lobby;
