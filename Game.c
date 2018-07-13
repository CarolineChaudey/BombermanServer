#include "Game.h"

void initMap(char *res, struct Playground pground) {
    char c;
    FILE *file;
    file = fopen("BonBeurreMap1.txt", "r");
    if (file) {
        // get map data
        char mapData[100];
        int i = 0;
        while ((c = getc(file)) != '\n') {
            mapData[i] = c;
            i++;
        }
        pground.mapInfo = mapData;
        printf("Map data : %s\n", pground.mapInfo);
        // get layer 1
        char layer1[350];
        i = 0;
        int nbLines = 0;
        while (nbLines < 10) {
            c = getc(file);
            if (c == '\n') {
                nbLines++;
            }
            layer1[i] = c;
            i++;
        }
        pground.layer1 = layer1;
        printf("Layer 1 : %s\n", layer1);
        // get layer 2
        char layer2[500];
        i = 0;
        nbLines = 0;
        while (nbLines < 10) {
            c = getc(file);
            if (c == '\n') {
                nbLines++;
            }
            layer2[i] = c;
            i++;
        }
        pground.layer2 = layer2;
        printf("Layer 2 : %s\n", layer2);
        // get layer 3
        char layer3[200];
        i = 0;
        nbLines = 0;
        while (nbLines < 10) {
            c = getc(file);
            if (c == '\n') {
                nbLines++;
            }
            layer3[i] = c;
            i++;
        }
        pground.layer3 = layer3;
        printf("Layer 3 : %s\n", layer3);

        fclose(file);
    }
}

void game(struct Lobby *lobby) {
    printf("Game begins.\n");
    struct Playground pground;

    char rawData[1100];
    initMap(rawData, pground);
    printf("%s", rawData);
    
}

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
    if ((chosenLobby->client_1_socket_fd == -1) || (chosenLobby->client_1_socket_fd == client_socket_fd)) {
        chosenLobby->client_1_socket_fd = client_socket_fd;
        return 1;
    } else if ((chosenLobby->client_2_socket_fd == -1) || (chosenLobby->client_2_socket_fd == client_socket_fd)) {
        chosenLobby->client_2_socket_fd = client_socket_fd;
        return 1;
    } else if ((chosenLobby->client_3_socket_fd == -1) || (chosenLobby->client_3_socket_fd == client_socket_fd)) {
        chosenLobby->client_3_socket_fd = client_socket_fd;
        return 1;
    } else if ((chosenLobby->client_4_socket_fd == -1) || (chosenLobby->client_4_socket_fd == client_socket_fd)) {
        chosenLobby->client_4_socket_fd = client_socket_fd;
        return 1;
    }
    return 0;
}

int removeClientFromLobby(int client_socket_fd, int lobbyId) {
    struct Lobby* chosenLobby = getLobbyById(lobbyId);
    if (chosenLobby->client_1_socket_fd == client_socket_fd) {
        chosenLobby->client_1_socket_fd = -1;
        return 1;
    } else if (chosenLobby->client_2_socket_fd == client_socket_fd) {
        chosenLobby->client_2_socket_fd = -1;
        return 1;
    } else if (chosenLobby->client_3_socket_fd == client_socket_fd) {
        chosenLobby->client_3_socket_fd = -1;
        return 1;
    } else if (chosenLobby->client_4_socket_fd == client_socket_fd) {
        chosenLobby->client_4_socket_fd = -1;
        return 1;
    }
    return 0;
}
