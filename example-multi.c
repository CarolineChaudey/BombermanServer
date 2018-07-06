#include<stdio.h>
#include<sys/types.h>//socket
#include<sys/socket.h>//socket
#include<string.h>//memset
#include<stdlib.h>//sizeof
#include<netinet/in.h>//INADDR_ANY

#define PORT 8000
#define MAXSZ 100

// SOURCE https://clinuxcode.blogspot.com/2014/02/concurrent-server-handling-multiple.html

int main()
{
 int sockfd;//to create socket
 int newsockfd;//to accept connection

 struct sockaddr_in serverAddress;//server receive on this address
 struct sockaddr_in clientAddress;//server sends to client on this address

 int n;
 char msg[MAXSZ];
 int clientAddressLength;
 int pid;

 //create socket
 sockfd=socket(AF_INET,SOCK_STREAM,0);
 //initialize the socket addresses
 memset(&serverAddress,0,sizeof(serverAddress));
 serverAddress.sin_family=AF_INET;
 serverAddress.sin_addr.s_addr=htonl(INADDR_ANY);
 serverAddress.sin_port=htons(PORT);

 //bind the socket with the server address and port
 bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress));

 //listen for connection from client
 listen(sockfd,5);

// RECUP CLIENTS
 while(1)
 {
    //parent process waiting to accept a new connection
    printf("\n*****server waiting for new client connection:*****\n");
    clientAddressLength=sizeof(clientAddress);
    newsockfd=accept(sockfd,(struct sockaddr*)&clientAddress,&clientAddressLength);
    printf("connected to client: %s\n",inet_ntoa(clientAddress.sin_addr));

    // UN CLIENT = UN PROCESSUS
    pid=fork();
    // REUSSITE
    if(pid==0)//child process rec and send
    {
        // ECOUTE EN BOUCLE LE CLIENT
        while(1)
        {
            n=recv(newsockfd,msg,MAXSZ,0);
            // DECONNEXION
            if(n==0) {
                close(newsockfd);
                break;
            }
            // instructions
            msg[n]=0;
            send(newsockfd,msg,n,0);
            printf("Receive and set:%s\n",msg);
        }
        exit(0);
    }
    // ECHEC
    else {
        close(newsockfd);//sock is closed BY PARENT
    }
 }

 return 0;
}