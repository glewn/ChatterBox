
//#server
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#define DEFAULT_PORT 7000
#define MSGSIZE 255

#define TRUE	1
#define LISTENQ	5
#define MAXLINE 4096
#define NAMESIZE 20
#define MAXUSER 10
//message type initializew
#define MSG_CONN 10000
#define MSG_NAME 10001
#define MSG_MESG 10002
#define MSG_QUIT 10003

typedef struct message{
	int type;
	char name[NAMESIZE];
	char msgTxt[MSGSIZE];
} MsgStr;

typedef struct clientStr{
	int sockNum;
	char name[NAMESIZE];
} Clients;

int client[MAXUSER];
int maxi;
Clients clientList[MAXUSER];

void receiveMsg(int, MsgStr*, MsgStr*);


// Prints the error stored in errno and aborts the program.
static void SystemFatal(const char* message)
{
    perror (message);
    exit (EXIT_FAILURE);
}
void initClientInfo(int i){
	char temp[NAMESIZE];
	if(i >=0 && i < MAXUSER){
		clientList[i].sockNum = -1;
		sprintf(temp, "USER %d",i);
		strcpy(clientList[i].name, temp);
	}
	else
		printf("INVALID\n");

}

void createMsg(MsgStr* fullMsg, int type, int i, const char *msgText){
	fullMsg->type = type;
	strcpy(fullMsg->name, clientList[i].name);
	strcpy(fullMsg->msgTxt, msgText);
	printf("CreateMsg - type: %d, client: %s, txt: %s\n", fullMsg->type, fullMsg->name, fullMsg->msgTxt);
}

void sendList(int sockNum){
	Clients *list = clientList;
	write(sockNum, (void *)list, sizeof(clientList));
}
void *writeMsg(void * messageStr){

	struct message *msge = (struct message *)messageStr;
	printf("\n\nMsgType :%d, writeMsg Thread: %s\n",msge->type, msge->msgTxt);
	for(int i = 0; i <=maxi; i++){
		if(client[i] < 0)
			continue;
		write(clientList[i].sockNum, (void *)msge, sizeof(MsgStr));
		printf("write to client[%d]: %d\n",i, clientList[i].sockNum);
	}
	printf("Success writeMsg\n");
	return 0;
}

void writeOne(int clientIdx, MsgStr *fullMsg){
		write(clientList[clientIdx].sockNum, (void *)fullMsg, sizeof(MsgStr));
}

void write_to_all(MsgStr *fullMsg, int maxi){
//	struct message *msg = fullMsg;
	for(int i = 0; i <maxi; i++){
		if(client[i] < 0)
			continue;
			write(clientList[i].sockNum, (void *)fullMsg, sizeof(MsgStr));
	}

}



void client_display(int maxi){
	fprintf(stdout, "************* client List *************\n");
	for(int i = 0; i <= maxi; i++){
			if(clientList[i].sockNum!=0){
				printf("socket number %d - name: %s\n", clientList[i].sockNum, clientList[i].name);
			}
	}
	fprintf(stdout, "**************************************\n\n");
}




int main(int argc, char **argv){
	int i, nready;//, bytes_to_read, n;
	int listen_sd, new_sd, sockfd, port, maxfd ;//client array
	socklen_t  client_len;
	struct sockaddr_in client_addr;

	fd_set rset, allset;
	pthread_t threadid;
	int thr_id =0;

	int open_socket(int);

	switch(argc)
	{
		case 1:
			port = DEFAULT_PORT;	// Use the default port
		break;
		case 2:
			port = atoi(argv[1]);	// Get user specified port
		break;
		default:
			fprintf(stderr, "Usage: %s [port]\n", argv[0]);
			exit(1);
	}


	listen_sd = open_socket(port);

	bzero(clientList, sizeof(clientList));



	maxfd	= listen_sd;	// initialize
	maxi	= -1;		// index into client[] array

	for (i = 0; i < MAXUSER; i++){
      initClientInfo(i);
	}

 	FD_ZERO(&allset);
	FD_SET(listen_sd, &allset);  //all my listenling socet set to allset



	while(1)
	{

		rset = allset;               // structure assignment
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);


		if (FD_ISSET(listen_sd, &rset)) // new client connection
		{
			client_len = sizeof(client_addr);
			//accept is blocking call go straihgt throw
			if ((new_sd = accept(listen_sd, (struct sockaddr *) &client_addr, &client_len)) == -1)
				SystemFatal("accept error");

			printf(" Remote Address:  %s\n", inet_ntoa(client_addr.sin_addr));


			for (i = 0; i < MAXUSER; i++){
				fprintf(stdout, "%d ", i);
				if (clientList[i].sockNum < 0)
				{
					clientList[i].sockNum = new_sd;


					break;
				}
			}
			printf("\nbreak client declare\n");
			//**** designing scaling server :
			//dont realize why it is working ***ALWAYS CHECKING

			//tempolarly use MAXUSER
			if (i == MAXUSER)//FD_SETSIZE)
			{
				 SystemFatal("Too many clients\n");
					exit(1);
			}

			FD_SET (new_sd, &allset);     // add new descriptor to set
			if (new_sd > maxfd)
				maxfd = new_sd;	// for select

			if (i > maxi)
				maxi = i;	// new max index in client[] array
			printf("check nready :%d\tmaxi: %d\n", nready,maxi);
			if (--nready <= 0)
				continue;	// no more readable descriptors
			printf("more nready\n");
		}

		for (i = 0; i <= maxi; i++)	// check all clients for data
		{
			//make temporal value in for loop
			MsgStr *sendForm, msgForm, *recvForm, recvMsg;
			sendForm = &msgForm;
			recvForm = &recvMsg;
			if ((sockfd = clientList[i].sockNum) < 0)
				continue;

		  printf("FD_ISSET to recv \n");
			if (FD_ISSET(sockfd, &rset))
			{
				receiveMsg(i, sendForm, recvForm);

				printf("RESULT Of SEND: Type %d, Name %s\n\tText: %s\n",
				 			sendForm->type, sendForm->name, sendForm->msgTxt);
				printf("RESULT Of SEND: Type %d, Name %s\n\tText: %s\n",
							recvForm->type, recvForm->name, recvForm->msgTxt);


				switch(recvForm->type){
					case MSG_CONN:
					case MSG_MESG:
							printf("echo all\n");
							if ((thr_id = pthread_create(&threadid, NULL, writeMsg,(void *)sendForm)) < 0)
							{
								perror("thread create error : ");
								exit(0);
							}
							break;

					case MSG_QUIT:
							printf("\t Remote Address:  %s closed connection\n", inet_ntoa(client_addr.sin_addr));
							close(sockfd);
							FD_CLR(sockfd, &allset);
							initClientInfo(i);
							break;
				}



				if (--nready > 0){
							continue;        // no more readable descriptors
							printf("nready  left\n");
				}
				client_display(maxi);
			}

		}
	}
}


void receiveMsg(int i, MsgStr* sendMsg, MsgStr* rcvMsg){
	int n = 0;
	int bytes_to_read = sizeof(MsgStr);
	while ((n = recv(clientList[i].sockNum, (MsgStr *)rcvMsg, bytes_to_read, 0)) <bytes_to_read)
	{
		if(n<0){
			rcvMsg->type = 0;
			strcpy(rcvMsg->name, "");
			strcpy(rcvMsg->msgTxt,"");
			return;
		}
		printf("n is %d\n", n);
		rcvMsg+=n;
		bytes_to_read -= n;
	}

	printf("RECV SUCCESS\n");
	printf("TEST type %d : name - %s   : update %s\n",rcvMsg->type, rcvMsg->name, rcvMsg->msgTxt);

	switch(rcvMsg->type){
		case MSG_CONN:
			strcpy(clientList[i].name, rcvMsg->name);
			sendList(clientList[i].sockNum);
			createMsg(sendMsg, MSG_CONN, i, rcvMsg->msgTxt);

			printf("TEST MSG_CONN : name - %s   : update %s\n", rcvMsg->name, clientList[i].name);
			break;
		case MSG_NAME:
			strcpy(clientList[i].name, rcvMsg->name);
			break;
		case MSG_MESG:
			printf("the recv message type: %d  context%s\n", rcvMsg->type, rcvMsg->msgTxt);
			createMsg(sendMsg, MSG_MESG, i, rcvMsg->msgTxt);

			break;
	}
	printf("RESULT Of SEND: Type %d, Name %s\n\tText: %s\n", sendMsg->type, sendMsg->name, sendMsg->msgTxt);
}



int open_socket(int port = DEFAULT_PORT){
	int arg;
	int listen_sd =0;
	struct sockaddr_in server;

	//Create a stream socket
	if ((listen_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		SystemFatal("Cannot Create Socket!");

    arg = 1;
    if (setsockopt (listen_sd, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg)) == -1)
        SystemFatal("setsockopt");

	// Bind an address to the socket
	bzero((char *)&server, sizeof(struct sockaddr_in));
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client


	if (bind(listen_sd, (struct sockaddr *)&server, sizeof(server)) == -1)
		SystemFatal("bind error");

	// Listen for connections
	// queue up to LISTENQ connect requests
	printf("Ready for listen\n");

	listen(listen_sd, LISTENQ);


	return listen_sd;

}
