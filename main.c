#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

const int NB_ROOM = 2;

void treatRequest();

int main() {
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
        char response[20];
        sprintf(response, "there is %d rooms", NB_ROOM);
        write(socket_fd, response, strlen(response)+1);
    } else {
        char response[20] = "Unknown instruction";
        write(socket_fd, response, strlen(response)+1);
    }

}