/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: ClientSrc.cpp
--
-- PROGRAM: 	ChatterBox
--
-- FUNCTIONS:
--   void recvList()
--	 void *readMsg(void *sock)
--	 void clientStart()
--	 void sendPersonalInfo()
--	 void createMsg(MsgStr *mesgForm, int typeNum, const char *message)
--	 void writeMsg(int type, std::string message)
--
--
-- DATE		 : March 9, 2016
--
-- REVISIONS : March 10, 2016    - add receive thread
--			   March 13, 2016	 - add  message structure
--			   March 16, 2016	 - update List
--			   March 22, 2016	 - combine QT UI
--
--
-- DESIGNER	 : Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: Eunwon Moon
--
-- NOTES:
-- 	This program is Cilent side socket communication programs using TCP
--	Socket is created when user connect, and possible to chat
--	Using thread receive the message from server, and depending on the type tag
--	in message structure, display message, update client list, or show other
--	clients status.
--
----------------------------------------------------------------------------------------------------------------------*/


#include "clientsrc.h"
#include <QDebug>

//global variable for Client to communicate with UI
int sockNum = -1;
Clients list[MAXUSER] = {0};
Client *parentx;


clientSrc::clientSrc(QWidget *parent)
{
    parentx = qobject_cast<Client*>(parent);
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	recvList
--
-- DATE: 		March 16, 2016
--
-- REVISIONS: 	March 23, 2016   - add on GUI
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon, Gabriel Lee
--
-- INTERFACE: 	void recvList()
--
-- RETURNS: void.
--
-- NOTES:
--	This function is to receive clients list from Server.
--	It will be called when received MSG_CONN or MSG_QUIT type message
--	to update list of user name in the chat room.
----------------------------------------------------------------------------------------------------------------------*/
void recvList()
{
	Clients *listRcv = list;
	int n = 0, bytes_to_read;
	bytes_to_read = sizeof(list);
	qDebug("received new List");
	//receive client list
    while ((n = recv (sockNum, (MsgStr *)listRcv, bytes_to_read, 0)) < bytes_to_read)
    {
      listRcv += n;
      bytes_to_read -= n;
    }
	
	
	//check user list update
    qDebug("received new List OK");
    for(int i =0; i < MAXUSER; i++){
        if(list[i].sockNum <0)
            continue;
     qDebug()<<"client "<< list[i].name<<"\t";
    }
	
	
    parentx->update_list();
}





/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	recvList
--
-- DATE: 		March 10, 2016
--
-- REVISIONS: 	March 16, 2016   - seperate type of message from structure
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void *readMsg(void *sock)
--					void * sock : the socket number to communicate with server.
--
-- RETURNS: void *
--
-- NOTES:
--	This method is receive thread to get the message continueously.
--	After receiving message structure, check type of message.
--  If the type is MSG_QUIT or MSG_CONN, 
----------------------------------------------------------------------------------------------------------------------*/
void *readMsg(void *sock)
{
    int sockNum = *((int*)sock);
    int n, bytes_to_read;
    MsgStr *MsgRcv, rcvform;
    MsgRcv = &rcvform;

    qDebug("RcvThread");

    while(1){
        n = 0;
        bytes_to_read = sizeof(MsgStr);

        while ((n = recv (sockNum, (MsgStr *)MsgRcv, bytes_to_read, 0)) < bytes_to_read)
        {
            MsgRcv += n;
            bytes_to_read -= n;

        }
		
        qDebug()<<"THE RECEIVE INFO! " << MsgRcv->type << ", " << MsgRcv->name << ", ";
		//seperate work depending on message type
        switch(MsgRcv->type){
            case MSG_QUIT:
                qDebug("client %s leave the chat room", MsgRcv->name);
                recvList();
                break;
            case MSG_CONN:
                qDebug("%s enter the chat room\n", MsgRcv->name);
                recvList();
                break;
            case MSG_MESG:
                QString s(MsgRcv->msgTxt);
                parentx->print_msg(s);
                break;
        }

    }
    return (0);
}






/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	clientStart
--
-- DATE: 		March 9, 2016
--
-- REVISIONS: 	March 10, 2016    - add receive thread
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void clientStart()
--
-- RETURNS: void 
--
-- NOTES:
--	This method is client main method to connect to server and 
--	share basic information of client and other clients.
--	Also open receive thread.
----------------------------------------------------------------------------------------------------------------------*/
void clientSrc::clientStart()
{
    struct hostent	*hp;
    struct sockaddr_in server;
    pthread_t p_thread;

    // Create the socket
    if ((sockNum = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Cannot create socket");
        return;
    }

    bzero((char *)&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    if ((hp = gethostbyname(host.c_str())) == NULL)
    {
        qWarning("Unknown server address\n");
        return;
    }
    bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

    // Connecting to the server
    if (connect (sockNum, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        qDebug("Can't connect to server\n");
        perror("connect");
        return;
    }

	//send my information (name)
    sendPersonalInfo();
	
	//receive users information.
    recvList();

    pthread_create(&p_thread, NULL, readMsg, (void *)&sockNum);

    qDebug("\t - read thread created...\n");
}




/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	sendPersonalInfo
--
-- DATE: 		March 13, 2016
--
-- REVISIONS: 	
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon
--
-- INTERFACE: 	void sendPersonalInfo()
--
-- RETURNS: void 
--
-- NOTES:
--	This method is to send personal information of client.
--  When connect or client change nickname, get the name and send it to server
--	to update information
--
----------------------------------------------------------------------------------------------------------------------*/
void clientSrc::sendPersonalInfo()
{
    MsgStr *myInfo, infoTxt;
    myInfo = &infoTxt;

    myInfo->type = MSG_CONN;
    strcpy(myInfo->name, nickname.c_str());
    strcpy(myInfo->msgTxt, "");
    send (sockNum, (void *)myInfo, sizeof(MsgStr) ,0);
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
-- INTERFACE: 	void createMsg(MsgStr *mesgForm, int typeNum, const char *message)
--						MsgStr *mesgForm: the message structure to send
--						int		typeNum : the type of message
--						char * 	message : the message context to send
--
-- RETURNS: void 
--
-- NOTES:
--	This method is to make message easily just adding message and type.
--	Using pointer structure, just update value of message structure.
--	and add user name as well.
--
----------------------------------------------------------------------------------------------------------------------*/
void clientSrc::createMsg(MsgStr *mesgForm, int typeNum, const char *message)
{
    mesgForm->type = typeNum;
    strcpy(mesgForm->name, nickname.c_str());
    strcpy(mesgForm->msgTxt, message);
}





/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION:	writeMsg
--
-- DATE: 		March 13, 2016
--
-- REVISIONS: 	
--
-- DESIGNER	 : 	Eunwon Moon, Gabriel Lee
--
-- PROGRAMMER: 	Eunwon Moon, Gabriel Lee
--
-- INTERFACE: 	writeMsg(int type, std::string message)
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
void clientSrc::writeMsg(int type, std::string message)
{
    qDebug()<<"WRITE socket number : "<<sockNum<<"\n";
    MsgStr *msgPtr, msgTxt;
    msgPtr = &msgTxt;
    createMsg(msgPtr, type, message.c_str());
    send(sockNum, (void *)msgPtr, sizeof(MsgStr), 0);
	
    if(type == MSG_QUIT)
        close(sockNum);
}
