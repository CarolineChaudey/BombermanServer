#include "Game.h"

void getMapDimensions(FILE *file, int *dimensions) {
    char line[15];
    char *chunk;
    fgets(line, 15, file);
    // jump to first data
    chunk = strtok(line, " ");
    while(strcmp(chunk, "0") == 0) {
        chunk = strtok(NULL, " ");
    }
    dimensions[0] = atoi(chunk);
    strtok(line, " ");
    dimensions[1] = atoi(chunk);
}

void initLayer(int ***layer, int *dimensions) {
    *layer = malloc(sizeof(int *) * dimensions[0]);

    for (int i = 0; i < dimensions[0]; i++) {
        *(*layer + i) = malloc(sizeof(int) * dimensions[1]);
        for (int j = 0; j < dimensions[1]; j++) {
            *(*(*layer + i) + j) = 0;
        }
    }
}


void displayLayer(int **layer, int *dimensions) {
    printf("displayLayer\n");
    for (int i = 0; i < dimensions[0]; i++) {
        for (int j = 0; j < dimensions[1]; j++) {
            printf("%d-", layer[i][j]);
        }
        printf("\n");
    }
}

void initMap(struct Playground pground) {
    char c;
    int dimensions[2];
    FILE *file;
    file = fopen("BonBeurreMap1.txt", "r");
    if (file) {
        getMapDimensions(file, dimensions);
        printf("%dx%d\n", dimensions[0], dimensions[1]);

        initLayer(&pground.layer1, dimensions);
        initLayer(&pground.layer2, dimensions);
        initLayer(&pground.layer3, dimensions);
        
        char line[60];
        fgets(line, 60, file); // passe la ligne des infos
        // layer 1
        for (int i = 0; i < dimensions[0]; i++) {
            fgets(line, 60, file);
            // retirer le \n
            line[strlen(line) - 1] = 0;
            // prendre chaque "case"
            char *chunk;
            chunk = strtok(line, " ");
            int j = 0;
            while (chunk != NULL) {
                int nb = atoi(chunk);
                pground.layer1[i][j] = nb;
                chunk = strtok(NULL, " ");
                j++;
            }
        }

        // layer 2
        for (int i = 0; i < dimensions[0]; i++) {
            fgets(line, 60, file);
            // retirer le \n
            line[strlen(line) - 1] = 0;
            // prendre chaque "case"
            char *chunk;
            chunk = strtok(line, " ");
            int j = 0;
            while (chunk != NULL) {
                int nb = atoi(chunk);
                pground.layer2[i][j] = nb;
                chunk = strtok(NULL, " ");
                j++;
            }
        }

        // layer 3
        for (int i = 0; i < dimensions[0]; i++) {
            fgets(line, 60, file);
            // retirer le \n
            line[strlen(line) - 1] = 0;
            // prendre chaque "case"
            char *chunk;
            chunk = strtok(line, " ");
            int j = 0;
            while (chunk != NULL) {
                int nb = atoi(chunk);
                pground.layer3[i][j] = nb;
                chunk = strtok(NULL, " ");
                j++;
            }
        }
        
        fclose(file);
    }
}

void sendToAllPlayers(struct Lobby *lobby, char *msg) {
    write(lobby->client_1_socket_fd, msg, sizeof(msg) + 1);
    write(lobby->client_2_socket_fd, msg, sizeof(msg) + 1);
    write(lobby->client_3_socket_fd, msg, sizeof(msg) + 1);
    write(lobby->client_4_socket_fd, msg, sizeof(msg) + 1);
}

void game(struct Lobby *lobby) {
    printf("Game begins.\n");
    struct Playground pground;

    char rawData[1100];
    initMap(pground);
    
    sendToAllPlayers(lobby, "GO");
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
        pthread_mutex_lock(&mutex);
        struct Lobby *result = &lobbies[id - 1];
        pthread_mutex_unlock(&mutex);
        return result;
    }
    return NULL;
}

void getLobbiesInfo(char* response) {
    strcpy(response, "");
    for (int i = 0; i < NB_ROOM; i++) {
        char lobby[6];
        lobby[0] = i + 1 + '0'; // lobby nb begins at 1
        lobby[1] = ':';
        pthread_mutex_lock(&mutex);
        lobby[2] = getNbPlayers(&lobbies[i]) + '0';
        pthread_mutex_unlock(&mutex);
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
        pthread_mutex_lock(&mutex);
        lobbies[i] = lobby;
        pthread_mutex_unlock(&mutex);
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
    int addedPlayer = 0;
    if ((chosenLobby->client_1_socket_fd == -1) || (chosenLobby->client_1_socket_fd == client_socket_fd)) {
        chosenLobby->client_1_socket_fd = client_socket_fd;
        addedPlayer = 1;
    } else if ((chosenLobby->client_2_socket_fd == -1) || (chosenLobby->client_2_socket_fd == client_socket_fd)) {
        chosenLobby->client_2_socket_fd = client_socket_fd;
        addedPlayer = 2;
    } else if ((chosenLobby->client_3_socket_fd == -1) || (chosenLobby->client_3_socket_fd == client_socket_fd)) {
        chosenLobby->client_3_socket_fd = client_socket_fd;
        addedPlayer = 3;
    } else if ((chosenLobby->client_4_socket_fd == -1) || (chosenLobby->client_4_socket_fd == client_socket_fd)) {
        chosenLobby->client_4_socket_fd = client_socket_fd;
        addedPlayer = 4;
    }
    if (addedPlayer) {
        char res[10];
        sprintf(res, "joined:%d", addedPlayer);
        sendToAllPlayers(chosenLobby, res);
    }
    return addedPlayer;
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
