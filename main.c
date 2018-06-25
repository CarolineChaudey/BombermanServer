#include "GameElements.c"
#include "Game.c"


void treatRequest();

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
        treatRequest(sock_conn_fd);
    }

    return 0;
}

void treatRequest(int socket_fd/*, char* req*/) {
    int nb_char = NB_ROOM * 6;
    char* lobbyInfoResponse;
    lobbyInfoResponse = malloc(sizeof(char) * nb_char);

    // give lobbies info to client
    getLobbiesInfo(lobbyInfoResponse);
    write(socket_fd, lobbyInfoResponse, strlen(lobbyInfoResponse)+1);
    free(lobbyInfoResponse);
}
