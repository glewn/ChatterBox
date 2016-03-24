/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: ClientSrc.cpp
--
-- PROGRAM: 	ChatterBox
--
-- FUNCTIONS:
-- void daemonize (void)
--	
--
--
-- DATE		 : March 9, 2016
--
-- REVISIONS : March 10, 2016    - add send thread
--			   March 13, 2016	 - add  message structure
--			   March 16, 2016	 - update List
--
--
-- DESIGNER	 : Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: Eunwon Moon
--
-- NOTES:
-- 	This program is Serverside code to connect users to chat.
--	It will be working using multiplexing to detect signal from client.
--	depending on the what type of message the client sent, it will do different working
--	to notify other users; for example, echo message, connection and quit notiication,
--	or update user list. 
----------------------------------------------------------------------------------------------------------------------*/

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
#include<fstream>

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
using namespace std;


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


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	SystemFatal
--
-- DATE: 		March 13, 2016
--
-- REVISIONS: 	
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	static void SystemFatal(const char* message)
--
-- RETURNS: void 
--
-- NOTES:
--	This method is to send message depending on message type and message
--	Using createMsg function, generate message and send it
--	if the message type is QUIT, close socket to block sending garbage character
--	or signal.
--
----------------------------------------------------------------------------------------------------------------------*/
static void SystemFatal(const char* message)
{
    perror (message);
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	initClientInfo
--
-- DATE: 		March 13, 2016
--
-- REVISIONS: 	
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void initClientInfo(int i)
--
-- RETURNS: void 
--
-- NOTES:
--	This method is to initialize clientList.
--	socketNum will be -1, and the user name will be user + index name.
--
----------------------------------------------------------------------------------------------------------------------*/
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


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	createMsg
--
-- DATE: 		March 13, 2016
--
-- REVISIONS: 	
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void createMsg(MsgStr* fullMsg, int type, int i, const char *msgText)
--						MsgStr* fullMsg : message structure point to send
--						int type		: type of the message
--						int i			: index number of client from select
--						const char *msgText : message to send
--
-- RETURNS: void 
--
-- NOTES:
--	This method is to create message easily.
--	Depending on the index number of multiplex, find user name and fill the form of message
--
----------------------------------------------------------------------------------------------------------------------*/
void createMsg(MsgStr* fullMsg, int type, int i, const char *msgText){
	fullMsg->type = type;
	if(i < MAXUSER)
		strcpy(fullMsg->name, clientList[i].name);
	else
		strcpy(fullMsg->name, "ERRORMESSAGE");
	strcpy(fullMsg->msgTxt, msgText);
	printf("CreateMsg - type: %d, client: %s, txt: %s\n", fullMsg->type, fullMsg->name, fullMsg->msgTxt);
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	sendList
--
-- DATE: 		March 13, 2016
--
-- REVISIONS: 	
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void sendList(int sockNum)
--
-- RETURNS: void 
--
-- NOTES:
--	This method is to send clients list(user list) to client.
--  receive socketnumber, which is clients socket number, and send it to a certain client.
--
----------------------------------------------------------------------------------------------------------------------*/
void sendList(int sockNum){
	Clients *list = clientList;
	write(sockNum, (void *)list, sizeof(clientList));
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	writeMsg(void * messageStr)
--
-- DATE: 		March 13, 2016
--
-- REVISIONS: 	
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void *writeMsg(void * messageStr)
--						void * messageStr:  message string structure to send
--
-- RETURNS: void 
--
-- NOTES:
--	This method is write thread to send message to clients.
--	receive message structure and send to all client in the list except the message sender
--	If the message type is MSG_CONN or MSG_QUIT, which means someone is entered or left,
--	call sendList function to update list of users.
--
----------------------------------------------------------------------------------------------------------------------*/
void *writeMsg(void * messageStr){

	struct message *msge = (struct message *)messageStr;
	printf("\n\nMsgType :%d, writeMsg Thread: %s\n",msge->type, msge->msgTxt);
	if(strcmp(msge->msgTxt, "\n")==0){
		printf("[%s]EMPTY MESSAGE ", msge->name);
		return 0;
	}
	for(int i = 0; i <=maxi; i++){
		if(clientList[i].sockNum < 0){
			printf("noperson");
			continue;
		}
		else if(strcmp(clientList[i].name, msge->name)==0)
		{
			printf("SENDER\n");
			continue;
		}
		write(clientList[i].sockNum, (void *)msge, sizeof(MsgStr));
		printf("write to client[%d]: %d: %s\n",i, clientList[i].sockNum, clientList[i].name);

		if(msge->type == MSG_QUIT || msge->type == MSG_CONN){
			sendList(clientList[i].sockNum);
			printf("send LIST   ");
		}
	}
	printf("Success writeMsg\n");
	return 0;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	writeOne
--
-- DATE: 		March 13, 2016
--
-- REVISIONS: 	
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void writeOne(int clientIdx, MsgStr *fullMsg)
--						void * messageStr:  message string structure to send
--
-- RETURNS: void 
--
-- NOTES:
--	This method is write thread to send message to clients.
--	receive message structure and send to all client in the list except the message sender
--	If the message type is MSG_CONN or MSG_QUIT, which means someone is entered or left,
--	call sendList function to update list of users.
--
----------------------------------------------------------------------------------------------------------------------*/
void writeOne(int clientIdx, MsgStr *fullMsg){
		write((clientIdx<MAXUSER)?clientList[clientIdx].sockNum:clientIdx, (void *)fullMsg, sizeof(MsgStr));
}



/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	client_display
--
-- DATE: 		March 13, 2016
--
-- REVISIONS: 	
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void client_display(int maxi)
--						int maxi : the max index number in the client list.
--
-- RETURNS: void 
--
-- NOTES:
--	This method is to display client list in server consol.
--
----------------------------------------------------------------------------------------------------------------------*/
void client_display(int maxi){
	fprintf(stdout, "************* client List *************\n");
	for(int i = 0; i <= maxi; i++){
			if(clientList[i].sockNum > 0){
				printf("socket number %d - name: %s\n", clientList[i].sockNum, clientList[i].name);
			}
	}
	fprintf(stdout, "**************************************\n\n");
}



/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	main
--
-- DATE: 		March 13, 2016
--
-- REVISIONS: 	
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	int main(int argc, char **argv)
--						argc: the number of input argument
--						argv: the pointer of array which is input arguments
--
-- RETURNS: int
--
-- NOTES:
--	This method is main method of server side. Using console, read port number or use default one
--	Using input information, open server socket and start to write log finle.
--	using select, multiplx clients and detect who send the message. 
--	receive the message and generate and echo back to others.
--	if the user sent quit message close the socket and clear that users infom 
--  on the client list 
--
----------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char **argv){
	int i, nready;
	int listen_sd, new_sd, sockfd, port, maxfd ;//client array
	socklen_t  client_len;
	struct sockaddr_in client_addr;

	fd_set rset, allset;
	pthread_t threadid;
	int thr_id =0;

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
	
	//define saving file.
	ofstream log("logfile.txt", ios_base::trunc | ios_base::out);
	log << "socket connected" << endl;
	
	
	
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


			for (i = 0; i < MAXUSER; i++){
				fprintf(stdout, "%d ", i);
				if (clientList[i].sockNum < 0)
				{
					clientList[i].sockNum = new_sd;
					break;
				}

			}

			//tempolarly use MAXUSER
			if (i == MAXUSER)//FD_SETSIZE)
			{
				 MsgStr errMsg;
				 createMsg(&errMsg, MSG_QUIT,i,"sorry no space");
				 writeOne(new_sd, &errMsg);
				 log <<"CHAT ROOM IS FULL." <<endl;
			}
			else{
				log << "client [" << inet_ntoa(client_addr.sin_addr)
				    << "] connected - socket" <<clientList[i].sockNum<<endl;
			}

			FD_SET (new_sd, &allset);     // add new descriptor to set
			if (new_sd > maxfd)
				maxfd = new_sd;	// for select

			if (i > maxi)
				maxi = i;
			
			if (--nready <= 0)
				continue;	// no more readable descriptors

		}

		for (i = 0; i <= maxi; i++)	// check all clients for data
		{
			//make temporal value in for loop
			MsgStr *sendForm, msgForm, *recvForm, recvMsg;
			sendForm = &msgForm;
			recvForm = &recvMsg;
			if ((sockfd = clientList[i].sockNum) < 0)
				continue;

			if (FD_ISSET(sockfd, &rset))
			{
				receiveMsg(i, sendForm, recvForm);


				switch(recvForm->type){
					case MSG_CONN:
					case MSG_MESG:
						log <<"Receive From ["<< recvForm->name <<"]: " << recvForm->msgTxt<<endl;
						log <<"echo to all client" << endl;
						if ((thr_id = pthread_create(&threadid, NULL, writeMsg,(void *)sendForm)) < 0)
						{
							perror("thread create error : ");
							exit(0);
						}
						break;

					case MSG_QUIT:
						log<< "["<< recvForm->name <<"] Left the chat room" <<endl;
						if ((thr_id = pthread_create(&threadid, NULL, writeMsg,(void *)sendForm)) < 0)
						{
							perror("thread create error : ");
							exit(0);
						}
						close(sockfd);
						FD_CLR(sockfd, &allset);
						initClientInfo(i);
						break;
				}

				if (--nready > 0){
							continue;        // no more readable descriptors
				}
				client_display(maxi);
			}

		}
	}
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	receiveMsg
--
-- DATE: 		March 13, 2016
--
-- REVISIONS: 	
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void receiveMsg(int i, MsgStr* sendMsg, MsgStr* rcvMsg)
--						argc: the number of input argument
--						argv: the pointer of array which is input arguments
--
-- RETURNS: int
--
-- NOTES:
--	This method is main method of server side. Using console, read port number or use default one
--	Using input information, open server socket and start to write log finle.
--	using select, multiplx clients and detect who send the message. 
--	receive the message and generate and echo back to others.
--	if the user sent quit message close the socket and clear that users infom 
--  on the client list 
--
----------------------------------------------------------------------------------------------------------------------*/
void receiveMsg(int i, MsgStr* sendMsg, MsgStr* rcvMsg){
	int n = 0;
	int bytes_to_read = sizeof(MsgStr);
	while ((n = recv(clientList[i].sockNum, (MsgStr *)rcvMsg, bytes_to_read, 0)) > bytes_to_read)
	{
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
		case MSG_MESG:
			printf("the recv message type: %d  context%s\n", rcvMsg->type, rcvMsg->msgTxt);
			createMsg(sendMsg, MSG_MESG, i, rcvMsg->msgTxt);
				break;
		case MSG_QUIT:
			printf("CLIENT %s LEAVE", rcvMsg->name);
			createMsg(sendMsg, MSG_QUIT, i, "client leave");
			break;
	}
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	open_socket
--
-- DATE: 		March 13, 2016
--
-- REVISIONS: 	
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	int open_socket(int port = DEFAULT_PORT)
--						port : the port number to open server socket,
--							   if EMPTY use DEFAULT_PORT, which is 7000
--
-- RETURNS: int
--
-- NOTES:
--	This method is to open socket using port information.
--	It will do all things related to open server: such as opening socket,
--	setting socket option, binding and listening.
--
----------------------------------------------------------------------------------------------------------------------*/
int open_socket(int port = DEFAULT_PORT){
	int arg;
	int listen_sd =0;
	struct sockaddr_in server;

	//Create a stream socket
	if ((listen_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		SystemFatal("Cannot Create Socket!");

	//set SO_REUSEADDR so port can be resused imemediately after exit, i.e., after CTRL-c
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
