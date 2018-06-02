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