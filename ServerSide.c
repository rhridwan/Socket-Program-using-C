
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg); 	//invalid function interprete error number output 				  error discription using stdrr
    exit(1); 		//return true to terminate our program
}

int main(int argc, char *argv[]) //argc is num of parameters we r passing 					   which are two,one is filename & other 					   is portnumber
{
     int sockfd, newsockfd, portno;
     socklen_t clilen; //soklen_t is 32bit datatype in sockst.h
     char buffer[255]; 	//store msg in buffer and transfer in datastream
     struct sockaddr_in serv_addr, cli_addr; //socaddr_in contains 					              internet address contains in 						      netinet/in.h
     int n;
     if (argc < 2) { //arc is less than 2,total num of parameter
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1); 
     }


     sockfd = socket(AF_INET, SOCK_STREAM, 0); //create socket and ini tcp
     if (sockfd < 0) //if no client then throw error
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));   //to take right dat
     portno = atoi(argv[1]);         //atoi to convet string into integer

     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno); // htons-host to network along

     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");
						//fd-file discripter
     listen(sockfd,5); 		//connect and limits client(5)
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd, //accepted aonnection and create socket
                 (struct sockaddr *) &cli_addr, 
                 &clilen);
     if (newsockfd < 0) //if no connection then throw error
          error("ERROR on accept");
     while(1) 		//while true we communicate
     {
           bzero(buffer,256);
           n = read(newsockfd,buffer,255);
           if (n < 0) error("ERROR reading from socket");//if no read then 								   throw error
           printf("Client: %s\n",buffer);//print clients statement in the 						  server side by buffer
          bzero(buffer,256); //clear buffer
          fgets(buffer,255,stdin); //fun reads from buffer stream-stdio.h
          n = write(newsockfd,buffer,strlen(buffer)); //write in buffer

           if (n < 0) error("ERROR writing to socket"); //if no write 								  throw error
           int i=strncmp("Bye" , buffer, 3); //ttrcmp-built in func to compare string. if bye we terminate server hosting
           if(i == 0)
               break;
     }
     close(newsockfd); //we close new socket and old fd
     close(sockfd);
     return 0; 
}
