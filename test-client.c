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
 
    printf("connect\n");
    connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
 
    bzero( sendline, 100);
    bzero( recvline, 100);

    printf("write\n");
    write(sockfd, "get rooms", 11);
    printf("read\n");
    read(sockfd, recvline, 100);
    printf("%s\n", recvline);
 
}
