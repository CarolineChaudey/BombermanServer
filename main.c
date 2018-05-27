#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

void treatRequest();

int main() {
    int sock_listen_fd, sock_conn_fd;
    struct sockaddr_in servaddr;
    const int NB_ROOM = 2;
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
    printf("Binding socket ...\n");
    if (bind(sock_listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // total players = rooms * players_per_rooms = 2 * 4 = 8
    // so 10 connections in the queue is ok
    printf("Listening ...\n");
    if (listen(sock_listen_fd, 10) < 0) {
        perror("listening to socket failed");
        exit(EXIT_FAILURE);
    }

    // taking first connection request
    printf("Taking first connection...\n");
    sock_conn_fd = accept(sock_listen_fd, (struct sockaddr*) NULL, NULL);

    bzero(client_query, 100);
    printf("Reading request\n");
    read(sock_conn_fd, client_query, 100);
    printf("%s\n", client_query);
    //write(sock_conn_fd, "test", strlen(client_query)+1);

/*
    while(1) {
        printf("loop");
        bzero(client_query, 100);
        read(sock_conn_fd, client_query, 100);
        //printf("Echoing back - %s",str);
        write(sock_conn_fd, client_query, strlen(client_query)+1);
    }
*/
    return 0;
}

void treatRequest() {

}