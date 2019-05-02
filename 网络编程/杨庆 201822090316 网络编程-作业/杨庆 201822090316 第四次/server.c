#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#define PORT 4321
#define BACKLOG 5
#define MAXDATASIZE 1000
typedef struct{
  int fd;
  char *name;
  struct sockaddr_in addr;
  char *data;
  int data_len;
}CLIENT;
void process_cli(CLIENT *client, char *recvbuf,int len);
void save_data(char* cli_buf,char * recv_buf,int recv_len,int * cli_len);

void inverse_str(char* recv_str,int str_length){
  char temp;
  for(int i=0;i<(str_length/2+str_length%2);i++){
    temp=recv_str[i];
    recv_str[i]=recv_str[str_length-1-i];
    recv_str[str_length-1-i]=temp;
  }
}

int main(void){
  int i,maxi,maxfd,sockfd;
  int nready;
  ssize_t n;
  fd_set rset,allset;
  int listenfd,connectfd;
  struct sockaddr_in server;
  CLIENT client[FD_SETSIZE];
  char recvbuf[MAXDATASIZE];
  int sin_size;
  
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
  maxfd=listenfd;
  maxi=-1;

  for(i=0;i<FD_SETSIZE;i++)
    client[i].fd=-1;
  FD_ZERO(&allset);
  FD_SET(listenfd,&allset);
  while(1){
    struct sockaddr_in addr;
    rset=allset;
        nready=select(maxfd+1,&rset,NULL,NULL,NULL);
       if(FD_ISSET(listenfd,&rset)){
      if((connectfd=accept(listenfd,(struct sockaddr *)&addr,&sin_size))==-1){
        perror("accept error");
        continue;
      }
      for(i=0;i<FD_SETSIZE;i++){
        if(client[i].fd<0){
          client[i].fd=connectfd;
          client[i].name=(char*)calloc(20,sizeof(char));
          client[i].addr=addr;
          client[i].data=(char*)calloc(MAXDATASIZE,sizeof(char));
          client[i].name[0]='\0';
          client[i].data[0]='\0';
          client[i].data_len=0;
          printf("You got a connect from %s:%d\n",inet_ntoa(client[i].addr.sin_addr),client[i].addr.sin_port);
          send(client[i].fd,"welcome .\n",22,0);
          break;
        }
      }
      if(i==FD_SETSIZE) printf("too many clients");
      FD_SET(connectfd,&allset);
      if(connectfd>maxfd) maxfd=connectfd;
      if(i>maxi) maxi=i;
      if(--nready<=0) continue;
    }
    // printf("3\n");
    for(i=0;i<=maxi;i++){
      if((sockfd=client[i].fd)<0) continue;
      if(FD_ISSET(sockfd,&rset)){

        if((n=recv(sockfd,recvbuf,MAXDATASIZE,0))==0){
          close(sockfd);
          printf("Client(%s:%d) closed. Message is %s\n",inet_ntoa(client[i].addr.sin_addr), client[i].addr.sin_port,client[i].data);
          FD_CLR(sockfd,&allset);
          for(int i=0;i<client[i].data_len;i++){
            printf("%c",client[i].data[i]);
          }
          client[i].fd=-1;
          free(client[i].name);
          free(client[i].data);
          client[i].data_len=0;
        }else{
          process_cli(&client[i],recvbuf,n);
        }
        if(--nready<=0) break;
      }
    }
  }
  close(listenfd);
}

void process_cli(CLIENT *client, char *recvbuf,int len){
    save_data(client->data,recvbuf,len,&client->data_len);
    send(client->fd,recvbuf,len,0);
    printf("Message from client(%s:%d):",inet_ntoa(client->addr.sin_addr), client->addr.sin_port);
    for(int i=0;i<len;i++){
      printf("%c",recvbuf[i]);
    }
    printf("\n");
}


void save_data(char* cli_buf,char * recv_buf,int recv_len,int * cli_len){
  if((*cli_len+recv_len)<5000){
    for(int i=0;i<recv_len;i++){
      cli_buf[i+*cli_len]=recv_buf[i];
    }
    *cli_len+=recv_len;
  }else{
    for(int i=0;i<5000-*cli_len;i++){
      cli_buf[i+*cli_len]=recv_buf[i];
    }
    *cli_len=5000;
  }
} 