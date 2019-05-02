#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define PORT 4321
#define BACKLOG 1
int main(void){
  int listenfd,connectfd;
  struct sockaddr_in server,client;
  socklen_t sin_size;

  if((listenfd=socket(AF_INET,SOCK_STREAM,0))==-1){
    perror("Create socket failed.");
    exit(-1);
  }
  int opt=SO_REUSEADDR;
  setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  bzero(&server,sizeof(server));
  server.sin_family=AF_INET;
  server.sin_port=htons(PORT);
  server.sin_addr.s_addr=htonl(INADDR_ANY);
  if(bind(listenfd,(struct sockaddr *)&server,sizeof(struct sockaddr))==-1){
    perror("Bind error.");
    exit(-1);
  }
  if(listen(listenfd,BACKLOG)==-1){
    perror("listen orror.");
    exit(-1);
  }
  sin_size=sizeof(struct sockaddr_in);
  while(1){
    if((connectfd=accept(listenfd,(struct sockaddr *)&client,&sin_size))==-1){
      perror("accept error.");
      exit(-1);
    }
    printf("client IP : %s port is:%d.\n",inet_ntoa(client.sin_addr),client.sin_port);
    close(connectfd);
  }
}