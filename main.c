#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "GameElements.c"

const int NB_ROOM = 2;
const int MAX_PLAYERS = 4;
struct Room rooms[2];

void treatRequest();

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
        //struct Player playersTab[4] = {NULL};
        struct Player* playersTab = malloc(sizeof(struct Player) * 4);
        for(int i = 0; i < 4; i++){
            playersTab[i].nb = 0;
        }
        struct Playground playground;
        room.id = i;
        room.playground = playground;
        room.players = playersTab;
        room.maxPlayers = MAX_PLAYERS;

        rooms[i] = room;
    }
}

int main() {
    initRooms();

    int sock_listen_fd, sock_conn_fd;
    struct sockaddr_in servaddr;
    char client_query[100];

    if ((sock_listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    // initialize server params
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // ipv4
    servaddr.sin_addr.s_addr = htons(INADDR_ANY); // any ip
    servaddr.sin_port = htons(8081);

    // bind listen socket and the server params
    if (bind(sock_listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // total players = rooms * players_per_rooms = 2 * 4 = 8
    // so 10 connections in the queue is ok
    if (listen(sock_listen_fd, 10) < 0) {
        perror("listening to socket failed");
        exit(EXIT_FAILURE);
    }

    // taking first connection request
    sock_conn_fd = accept(sock_listen_fd, (struct sockaddr*) NULL, NULL);

    bzero(client_query, 100);
    read(sock_conn_fd, client_query, 100);
    printf("Received %s\n", client_query);
    treatRequest(sock_conn_fd, client_query);

    return 0;
}

void treatRequest(int socket_fd, char* req) {
    const char* ROOMS_DATA = "get-rooms";
    if (strcmp(req, ROOMS_DATA) == 0) {
       // char response[20];
        //sprintf(response, "there is %d rooms", NB_ROOM);
        int nb_char = NB_ROOM * 6;

        char* response;
        response = malloc(sizeof(char) * nb_char);
        
        getRoomsAnswer(response);
        write(socket_fd, response, strlen(response)+1);
        free(response);
    } else {
        char response[20] = "Unknown instruction";
        write(socket_fd, response, strlen(response)+1);
    }
}
