/*
filename, server_ipaddress, portno
argv[0]  filename
argv[10] server_ipaddress
argv[2]  portno
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //to write- read,write & close func
#include <string.h>
#include <sys/types.h> //helps socket
#include <sys/socket.h> //stucture for socket
#include <netinet/in.h> //for internet domain address
#include <netdb.h> //define host structure

void error(const char *msg) //error function to throw error
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(1);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);   //get ip address of server
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,     //copies server to server addrss
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

    serv_addr.sin_port = htons(portno); // htons-host to network along
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");   //connected

    printf("Client: ");
    while(1)
    {
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) 
             error("ERROR writing to socket");
        bzero(buffer,256); 		//again clear the buffer
        n = read(sockfd,buffer,255);
        if (n < 0) 
             error("ERROR reading from socket");
        printf("Server : %s\n",buffer);
        int i = strncmp("Bye" , buffer , 3); 	//if bye then close
        if(i == 0)
               break;
    }
    close(sockfd);
    return 0;
}
