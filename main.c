#include "GameElements.c"
#include "Game.c"
#include <pthread.h>


void* treatRequest();

int main() {
    initLobbies();

    int sock_listen_fd, sock_conn_fd;
    struct sockaddr_in servaddr;

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

    // listen to clients
    while(1)
    {
        sock_conn_fd = accept(sock_listen_fd, (struct sockaddr*) NULL, NULL);
        printf("socket id = %d\n", sock_conn_fd);
        pthread_t tid;
        pthread_create(&tid, NULL, &treatRequest, (void*) sock_conn_fd );
    }

    return 0;
}

void* treatRequest(void* arg) {
    int socket_fd = (int) arg;
    int nb_char = NB_ROOM * 6;
    char* lobbyInfoResponse;
    lobbyInfoResponse = malloc(sizeof(char) * nb_char);
    char chosenLobbyId[10];
    char recvline[10];
    int currentLobbyId = 0;

    int quit = 0;

    while (!quit) {
        bzero(recvline, 10);
        read(socket_fd, recvline, 10);

        if (strcmp(recvline, "get-rooms") == 0) {
            printf("get-rooms\n");
            read(socket_fd, recvline, 10);
            getLobbiesInfo(lobbyInfoResponse);
            write(socket_fd, lobbyInfoResponse, strlen(lobbyInfoResponse)+1);

        } else if (strcmp(recvline, "%") != 0) {
            int lobbyId = atoi(recvline);
            if ((currentLobbyId != 0) && (lobbyId != currentLobbyId)) {
                removeClientFromLobby(socket_fd, currentLobbyId);
            }
            int lobbyRes = putClientInLobby(socket_fd, lobbyId);
            currentLobbyId = lobbyId;
            if (!lobbyRes) {
                write(socket_fd, "NOK", 4);
            } else {
                write(socket_fd, "OK", 3);
                if (isLobbyReady(*getLobbyById(lobbyId))) {
                    game();
                }
            }
        }
    }

    free(lobbyInfoResponse);
    return NULL;
}
