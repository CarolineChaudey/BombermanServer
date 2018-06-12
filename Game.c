#include "Game.h"

int getNbPlayers(struct Room* room) {
    int res = 0;
    for (int i = 0; i < room->maxPlayers; i++) {
        if (room->players[i].nb != 0) {
            res++;
        }
    }
    return res;
}

void getRoomsAnswer(char* response) {
    // format : "id:nbPlayers/nbPlayerMax"
    strcpy(response, "");
    for (int i = 0; i < NB_ROOM; i++) {
        char room[6];
        room[0] = rooms[i].id + '0';
        room[1] = ':';
        room[2] = getNbPlayers(&rooms[i]) + '0';
        room[3] = '/';
        room[4] = rooms[i].maxPlayers + '0';
        room[5] = ';';
        strcat(response, room);
    }
}

void initRooms() {
    for (int i = 0; i < NB_ROOM; i++) {
        struct Room room;
        struct Player* playersTab = malloc(sizeof(struct Player) * 4);
        for(int i = 0; i < 4; i++){
            playersTab[i].nb = 0;
        }
        struct Playground playground;
        room.id = i;
        room.playground = playground;
        room.players = playersTab;
        room.maxPlayers = MAX_PLAYERS_PER_ROOM;

        rooms[i] = room;
    }
}
