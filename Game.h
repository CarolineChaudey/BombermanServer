#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define NB_ROOM         2
#define MAX_PLAYERS_PER_ROOM     4


struct Room rooms[2];
struct Lobby lobbies[2];

int getNbPlayers(struct Lobby* lobby);
void getLobbiesInfo(char* response);
void initLobbies();
int isLobbyReady(struct Lobby lobby);
