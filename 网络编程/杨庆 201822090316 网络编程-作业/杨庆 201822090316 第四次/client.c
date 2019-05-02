#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define PORT 4321
#define MAXDATASIZE 1000
int main(int argc,char *argv[]){
  int fd,numbytes,scan_state;
  char buf[MAXDATASIZE];
  struct hostent *he;
  struct sockaddr_in server;
  if(argc!=2){
    printf("Usage:%s <IP address>\n",argv[0]);
    exit(-1);
  }
  if((he=gethostbyname(argv[1]))==NULL){
    perror("gethostbyname error.");
    exit(1);
  }
  if((fd=socket(AF_INET,SOCK_STREAM,0))==-1){
    perror("Create socket failed.");
    exit(1);
  }
  bzero(&server,sizeof(server));
  server.sin_family=AF_INET;
  server.sin_port=htons(PORT);
  server.sin_addr=*((struct in_addr *)he->h_addr);
  if(connect(fd,(struct sockaddr *)&server,sizeof(struct sockaddr))==-1){
    perror("connect failed.");
    exit(1);
  }
  if(((numbytes=recv(fd,buf,MAXDATASIZE,0))==-1)){
    perror("recv error.");
    exit(1);
  }
  buf[numbytes]='\0';
  printf("server message:%s\n",buf);
  while(1){
    printf("input your words:");
    
    if((scan_state=scanf("%s",buf))==-1){
      printf("end\n");
      break;
    }
    numbytes=strlen(buf);
    send(fd,buf,numbytes,0);
    if(((numbytes=recv(fd,buf,MAXDATASIZE,0))==-1)){
      perror("recv error.");
      exit(1);
    }
    buf[numbytes]='\0';
    printf("%s\n",buf);
  }
  close(fd);
}