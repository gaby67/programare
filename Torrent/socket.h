#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

typedef struct msgreg_t {
    char mtype;
    short rvport;
} NodeMsg;

typedef struct msggetfile_t {
    char mtype;
    char filename[10];
    long long from;
} GetFile;

typedef struct msggetnod_t {
    char mtype;
} GetNode;

typedef struct msgnodlst_t {
    char mtype;
    struct sockaddr_in addr[10];
    int length;
} NodList;

typedef struct msggettext_t {
        char mtype;
        char data[100];
        int length;
        long long complition;
} GetText;

#define CREATE 0
#define CONNECT 1

int SockCreate(char *ip, int port){
	int server;
	struct sockaddr_in s;
	server = socket(AF_INET, SOCK_STREAM, 0);
   	memset(&s, 0, sizeof(s));
	s.sin_port = htons(port);
	s.sin_family = AF_INET;
	s.sin_addr.s_addr = inet_addr(ip);
	if(bind(server, (struct sockaddr *) &s, sizeof(s))< 0)
		return -1;
	listen(server,5);
	return server;
}
int SockConnect(char * ip,int port){
	struct sockaddr_in client;
	memset(&client, 0, sizeof(client));
	int server = socket(AF_INET, SOCK_STREAM, 0);
	client.sin_port = htons(port);
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr(ip);
	if (connect(server, (struct sockaddr*)&client, sizeof(client)) < 0)
		return -1;
	return server;
}

void SockRead(int client,char* msg){
	recv(client,msg,sizeof(msg),0);
}

void SockWrite(int client,char* msg){
	send(client,msg,sizeof(msg),0);
}