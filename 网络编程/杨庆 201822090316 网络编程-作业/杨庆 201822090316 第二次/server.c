#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#define PORT 6789
#define BACKLOG 1
#define MAXDATASIZE 1000

void inverse_str(char* str,int len){
  char temp;
  char *strLeft = str;
	char *strRight = str + len - 1;

	while(strRight > strLeft)
	{
		char temp = *strLeft;
		*strLeft = *strRight;
		*strRight = temp;
		--strRight;
		++strLeft;
    	}
 }

int main(void){
  int listenfd,connectfd,numbytes;
  struct sockaddr_in server,client;
  socklen_t sin_size;
  char buffer[MAXDATASIZE];
  char name[20];
  pid_t pid;
  if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1){
    perror("Create socket failed.\n");
    exit(-1);
  }
  int opt=SO_REUSEADDR;
  setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  bzero(&server,sizeof(server));
  server.sin_family=AF_INET;
  server.sin_addr.s_addr=htonl(INADDR_ANY);
  server.sin_port=htons(PORT);
 
  if(bind(listenfd,(struct sockaddr *)&server,sizeof(struct sockaddr))==-1){
    perror("Bind socket failed.\n");
    exit(-1);
  }
  if(listen(listenfd,BACKLOG)==-1){
    perror("Listen socket failed.\n");
    exit(-1);
  }
 
  sin_size=sizeof(struct sockaddr_in);
  while(1){
    if((connectfd=accept(listenfd,(struct sockaddr *)&client,&sin_size))==-1){
      perror("Accept failed.\n");
      exit(-1);
    }
    printf("you get a connection from %s:%d\n",inet_ntoa(client.sin_addr),client.sin_port);
    send(connectfd,"welcome to my server.\n",22,0);
    while(1){
      if(((numbytes=recv(connectfd,buffer,MAXDATASIZE,0))==-1)){
        perror("Receive data of name of client error.\n");
        exit(1);
      }else if(numbytes==0){
        printf("connection end\n");
        break;
      }
	   buffer[numbytes]='\0';
	   printf("Get data from client: %s\n", buffer);
      inverse_str(buffer,numbytes);
      send(connectfd,buffer,numbytes,0);
      
     
    }
    close(connectfd);

  }
  close(listenfd);
}