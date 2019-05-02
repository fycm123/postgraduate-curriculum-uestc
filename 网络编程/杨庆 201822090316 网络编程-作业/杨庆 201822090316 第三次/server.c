#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define PORT 4321
#define BACKLOG 2
#define MAXDATASIZE 1000
void process_client(int connectfd,struct sockaddr_in client);
void save_client(char* cli_buf,char * recv_buf,int recv_len,int * cli_len);
void *start_routine(void *arg);
struct ARG{
  int connfd;
  struct sockaddr_in client;
};

void inverse_str(char* str,int len)
{
  char temp;
  char *strLeft = str;
  char *strRight = str + len - 1;
  while(strRight > strLeft)
	{
		temp = *strLeft;
		*strLeft = *strRight;
		*strRight = temp;
		++strLeft;
		--strRight;
     }
}


int main(void)
{
  int listenfd,connectfd;
  struct sockaddr_in server,client;
  pthread_t tid;
  struct ARG *arg;
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
      perror("listen error.");
      exit(-1);
    }
    sin_size=sizeof(struct sockaddr_in);
    while(1){
      if((connectfd=accept(listenfd,(struct sockaddr *)&client,&sin_size))==-1){
        perror("accept error");
        exit(-1);
      }
      arg=(struct ARG *)malloc(sizeof(struct ARG));
      arg->connfd=connectfd;
      memcpy((void*)&arg->client,&client,sizeof(client));
      if(pthread_create(&tid,NULL,start_routine,(void*)arg)){
        perror("thread create failed\n");
        exit(1);
      }
    }
    close(listenfd);
}
void *start_routine(void *arg){
  struct ARG *info;
  info=(struct ARG *)arg;
  process_client(info->connfd,info->client);
  free(arg);
  pthread_exit(NULL);
}
void process_client(int connectfd,struct sockaddr_in client){
  int numbytes,cli_len;
  char cli_data[5000],recv_buf[MAXDATASIZE],name[20];
  printf("you get a connection from %s\n",inet_ntoa(client.sin_addr));
  send(connectfd,"welcome to my server.\n",22,0);
  if(((numbytes=recv(connectfd,recv_buf,MAXDATASIZE,0))==-1)){
      perror("recv error.");
      exit(1);
  }
  if(numbytes>=20){
    for(int i=0;i<20;i++){
      name[i]=recv_buf[i];
    }
    name[19]='\0';
  }else{
	for(int i=0;i<numbytes;i++){
      name[i]=recv_buf[i];
    }
    name[numbytes]='\0';
  }
  printf("%s connected.\n",name);
  sprintf(recv_buf,"welcome %s.\n",name);
  send(connectfd,recv_buf,strlen(recv_buf),0);
  while(1){
    if(((numbytes=recv(connectfd,recv_buf,MAXDATASIZE,0))==-1)){
      perror("recv error.");
      exit(1);
    }else if(numbytes==0){
      printf("connection end\n");
      break;
    }
    save_client(cli_data,recv_buf,numbytes,&cli_len);
    send(connectfd,recv_buf,numbytes,0);
      }
  printf("The client Name is:  %s\n",name);
   	inverse_str(cli_data,cli_len);
    for(int i=0;i<cli_len;i++){
    printf("%c",cli_data[i]);
  }
  printf("\n");
  
  close(connectfd);
}

void save_client(char* cli_buf,char * recv_buf,int recv_len,int * cli_len){
  inverse_str(recv_buf,recv_len);
	for(int i=*cli_len-1;i>=0; i--) {
	  cli_buf[i+recv_len]=cli_buf[i];
	}
	for(int i=0;i<recv_len;i++){
	  cli_buf[i]=recv_buf[i];
	}
	*cli_len+=recv_len;
}