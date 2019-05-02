#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#define PORT 4321
#define MAXDATASIZE 100
int main(int argc,char *argv[]){
  int fd,numbytes;
  char buf[MAXDATASIZE];
  struct hostent *he;
  struct sockaddr_in server;
  if(argc!=2){
    printf("Usage:%s <IP address>\n",argv[0]);
    exit(-1);
  }
  if((he=gethostbyname(argv[1]))==NULL){
    perror("gethostbyname error.");
    exit(-1);
  }
  if((fd=socket(AF_INET,SOCK_STREAM,0))==-1){
    perror("Create socket failed.");
    exit(-1);
  }
  bzero(&server,sizeof(server));
  server.sin_family=AF_INET;
  server.sin_port=htons(PORT);
  server.sin_addr=*((struct in_addr *)he->h_addr);
  if(connect(fd,(struct sockaddr *)&server,sizeof(struct sockaddr))==-1){
    perror("connect failed.\n");
    exit(1);
  }
  printf("connection success.\n");
  close(fd);
}