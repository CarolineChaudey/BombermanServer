#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    int sock_listen_fd, sock_conn_fd;
    struct sockaddr_in servaddr;
    const int NB_ROOM = 2;

    sock_listen_fd = socket(AF_INET, SOCK_STREAM, 0); // (ipv4, tcp, 0)

    // initialize server params
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET; // ipv4
    servaddr.sin_addr.s_addr = htons(INADDR_ANY); // any ip
    servaddr.sin_port = htons(8081);

    // bind listen socket and the server params
    printf("Binding socket ...\n");
    bind(sock_listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));

    // total players = rooms * players_per_rooms = 2 * 4 = 8 so 10 connections in the queue is ok
    printf("Listening ...\n");
    listen(sock_listen_fd, 10);

    // taking first connection request
    printf("Taking first connection...\n");
    sock_conn_fd = accept(sock_listen_fd, (struct sockaddr*) NULL, NULL);

    printf("Begining.\n");
    while(1)
    {
        //bzero( str, 100);
        //read(sock_conn_fd, str, 100);
        printf("Connection received");
        //write(conn_fd, str, strlen(str)+1);
    }

    return 0;
}