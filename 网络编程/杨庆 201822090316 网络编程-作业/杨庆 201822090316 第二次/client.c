#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define PORT 6789
#define MAXDATASIZE 1000
int main(int argc,char *argv[]){
  int connfd,numbytes,scan_state;
  char buffer[MAXDATASIZE];
  struct hostent *host;
  struct sockaddr_in server;
  printf("Client starting...\n");
  if(argc!=2){
    printf("Usage:%s <IP address>\n",argv[0]);
    exit(-1);
  }
  if((host=gethostbyname(argv[1]))==NULL){
    perror("Get host by name failed.\n");
    exit(1);
  }
  if((connfd=socket(AF_INET,SOCK_STREAM,0))==-1){
    perror("Create socket failed.\n");
    exit(1);
  }
  bzero(&server,sizeof(server));
  server.sin_family=AF_INET;
  server.sin_port=htons(PORT);
  server.sin_addr=*((struct in_addr *)host->h_addr);
  if(connect(connfd,(struct sockaddr *)&server,sizeof(struct sockaddr))==-1){
    perror("Connect failed.\n");
    exit(1);
  }
  	
  if(((numbytes=recv(connfd,buffer,MAXDATASIZE,0))==-1)){
    perror("Receive data failed.\n");
    exit(1);
  }
  buffer[numbytes]='\0';
  printf("server message:%s\n",buffer);
  while(1){
    printf("input your words:");
    
    if((scan_state=scanf("%s",buffer))==-1){
      printf("end\n");
      break;
    }
    numbytes=strlen(buffer);
    send(connfd,buffer,numbytes,0);
    if(((numbytes=recv(connfd,buffer,MAXDATASIZE,0))==-1)){
      perror("Receive data failed.\n");
      exit(1);
    }
    buffer[numbytes]='\0';
    printf("%s\n",buffer);
  }
  close(connfd);
}