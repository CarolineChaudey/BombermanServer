#include "Game.h"

int getNbPlayers(struct Lobby* lobby) {
    int res = 0;
    int clients[4] = {lobby->client_1_socket_fd, lobby->client_2_socket_fd, 
        lobby->client_3_socket_fd, lobby->client_4_socket_fd};
    for (int i = 0; i < 4; i++) {
        if (clients[i] >= 0) {
            res++;
        }
    }
    return res;
}

struct Lobby* getLobbyById(int id) {
    if ((id > 0) && (id <= NB_ROOM)) {
        return &lobbies[id - 1];
    }
    return NULL;
}

void getLobbiesInfo(char* response) {
    strcpy(response, "");
    for (int i = 0; i < NB_ROOM; i++) {
        char lobby[6];
        lobby[0] = i + 1 + '0'; // lobby nb begins at 1
        lobby[1] = ':';
        lobby[2] = getNbPlayers(&lobbies[i]) + '0';
        lobby[3] = '/';
        lobby[4] = '4';
        lobby[5] = ';';
        strcat(response, lobby);
    }
}

void initLobbies() {
    for (int i = 0; i < NB_ROOM; i++) {
        struct Lobby lobby;
        lobby.client_1_socket_fd = -1;
        lobby.client_2_socket_fd = -1;
        lobby.client_3_socket_fd = -1;
        lobby.client_4_socket_fd = -1;
        lobbies[i] = lobby;
    }
}

int isLobbyReady(struct Lobby lobby) {
    if (getNbPlayers(&lobby) == 4) {
        return 1;
    }
    return 0;
}

int putClientInLobby(int client_socket_fd, int lobbyId) {
    struct Lobby* chosenLobby = getLobbyById(lobbyId);
    if (chosenLobby->client_1_socket_fd == -1) {
        chosenLobby->client_1_socket_fd = client_socket_fd;
        printf("as client 1\n");
        return 1;
    } else if (chosenLobby->client_2_socket_fd == -1) {
        chosenLobby->client_2_socket_fd = client_socket_fd;
        return 1;
    } else if (chosenLobby->client_3_socket_fd == -1) {
        chosenLobby->client_3_socket_fd = client_socket_fd;
        return 1;
    } else if (chosenLobby->client_4_socket_fd == -1) {
        chosenLobby->client_4_socket_fd = client_socket_fd;
        return 1;
    }
    return 0;
}
