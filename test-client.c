#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

 
int main(int argc,char **argv)
{
    int sockfd,n;
    char sendline[100];
    char recvline[100];
    struct sockaddr_in servaddr;
 
    sockfd=socket(AF_INET,SOCK_STREAM,0);
    bzero(&servaddr,sizeof servaddr);
 
    servaddr.sin_family=AF_INET;
    servaddr.sin_port=htons(8081);
 
    inet_pton(AF_INET,"127.0.0.1",&(servaddr.sin_addr));
 
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
 
    bzero( sendline, 100);
    bzero( recvline, 100);

    write(sockfd, "get-rooms", 11);
    read(sockfd, recvline, 100);
    printf("%s\n", recvline);

    write(sockfd, "1", 1);
    printf("Choosen lobby sent.\n");
    read(sockfd, recvline, 100);
    printf("%s\n", recvline);

    write(sockfd, "get-rooms", 11);
    read(sockfd, recvline, 100);
    printf("%s\n", recvline);
/*
    int roomId = 2;
    char str[10];
    sprintf(str, "%d", roomId);
    write(sockfd, (void*) str, 10);
    printf("Choosen lobby sent.\n");
    read(sockfd, recvline, 100);
    printf("%s\n", recvline);

    write(sockfd, "get-rooms", 11);
    read(sockfd, recvline, 100);
    printf("%s\n", recvline);
*/
    //close(sockfd);
    read(sockfd, recvline, 100);
    printf("%s\n", recvline);
    while (1) {};
}
