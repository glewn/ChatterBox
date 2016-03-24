#include "serversrc.h"
clientStr clientList[MAXUSER];

ServerSrc::ServerSrc()
{
  // = portInput;
 //  bzero(clientList, sizeof(clientList));
  //clientList = {0};
  //clientList = new clientStr[MAXUSER];

 // memset(clientList, 0, sizeof(clientList));
//  readBuffer[BUFLEN] = NULL;
 // writeBuf = 0;
 // readBuf = 0;
}

void *writeMsg(void * messageStr){

    struct message *msge = (struct message *)messageStr;
    printf("\n\nMsgType :%d, writeMsg Thread: %s\n",msge->type, msge->msgTxt);
    for(int i = 0; i <= MAXUSER; i++){
        if(clientList[i].sockNum < 0)
            continue;
        write(clientList[i].sockNum, (void *)msge, sizeof(MsgStr));
        qDebug()<<"write to client["<<i<<"]: "<<clientList[i].sockNum<<"\n";
    }
    printf("Success writeMsg\n");
    return 0;
}

int ServerSrc::start(){
    int i, nready;
    int listen_sd, new_sd, sockfd, port, maxfd ;//client array
    socklen_t  client_len;
    struct sockaddr_in client_addr;
    pthread_t threadid;
    fd_set rset, allset;


  //  int open_socket(int);


    listen_sd = open_socket(port);

    bzero(clientList, sizeof(clientList));
    qDebug("Initialize ClientList\n");

    maxfd	= listen_sd;	// initialize
    maxi	= -1;		// index into client[] array
/*
    for (i = 0; i < MAXUSER; i++){
      initClientInfo(i);
    }
*/
    FD_ZERO(&allset);
    FD_SET(listen_sd, &allset);  //all my listenling socet set to allset



    while(1)
    {


        qDebug("While, ");
        rset = allset;               // structure assignment
        nready = select(maxfd + 1, &rset, NULL, NULL, NULL);


        if (FD_ISSET(listen_sd, &rset)) // new client connection
        {
            client_len = sizeof(client_addr);
            //accept is blocking call go straihgt throw
            if ((new_sd = accept(listen_sd, (struct sockaddr *) &client_addr, &client_len)) == -1)
                qWarning("accept error");

            qDebug()<<" Remote Address:  %s\n"<<inet_ntoa(client_addr.sin_addr);


            for (i = 0; i < MAXUSER; i++){
                qDebug("%d ", i);
                if (clientList[i].sockNum < 0)
                {
                    clientList[i].sockNum = new_sd;
                    break;
                }
            }
            qDebug("\nbreak client declare\n");

            if (i == MAXUSER)//FD_SETSIZE)
            {
                 //SystemFatal("Too many clients\n");
                 return -1;
            }

            FD_SET (new_sd, &allset);     // add new descriptor to set
            if (new_sd > maxfd)
                maxfd = new_sd;	// for select

            if (i > maxi)
                maxi = i;	// new max index in client[] array
            qDebug("check nready :%d\tmaxi: %d\n", nready,maxi);
            if (--nready <= 0)
                continue;	// no more readable descriptors
            qDebug("more nready\n");
        }

        for (i = 0; i <= maxi; i++)	// check all clients for data
        {
            //make temporal value in for loop
            MsgStr *sendForm, msgForm, *recvForm, recvMsg;
            sendForm = &msgForm;
            recvForm = &recvMsg;
            if ((sockfd = clientList[i].sockNum) < 0)
                continue;

          qDebug("FD_ISSET to recv \n");
            if (FD_ISSET(sockfd, &rset))
            {
                /*
                if(writeBuf < readBuf){
                    qDebug("ERROR READ IS LARGER");
                }
                if(writeBuf < BUFLEN)
                    readBuffer[writeBuf] = i;
                else if(writeBuf < 2*BUFLEN)
                    readBuffer[writeBuf%BUFLEN] =i;
                else{
                    qWarning("OUT OF BUF SIZE");
                    return -1;
                }
                writeBuf++;
                */

                receiveMsg(i, sendForm, recvForm);

                qDebug("RESULT Of SEND: Type %d, Name %s\n\tText: %s\n",
                            sendForm->type, sendForm->name, sendForm->msgTxt);
                qDebug("RESULT Of SEND: Type %d, Name %s\n\tText: %s\n",
                            recvForm->type, recvForm->name, recvForm->msgTxt);


                switch(recvForm->type){
                    case MSG_CONN:
                    case MSG_MESG:
                            //writeMessage(sendForm);

                            printf("echo all\n");
                            if (pthread_create(&threadid, NULL, writeMsg,(void *)sendForm) < 0)
                            {
                                perror("thread create error : ");
                                exit(0);
                            }
                            break;

                    case MSG_QUIT:
                            qDebug("\t Remote Address:  %s closed connection\n", inet_ntoa(client_addr.sin_addr));
                            close(sockfd);
                            FD_CLR(sockfd, &allset);
                            //initClientInfo(i);
                            break;
                }


                if (--nready > 0){
                            continue;        // no more readable descriptors
                            qDebug("nready  left\n");
                }
                //client_display(maxi);

            }

        }

    }

}


int ServerSrc::open_socket(int port = DEFAULT_PORT){
    int arg;
    int listen_sd =0;
    struct sockaddr_in server;

    //Create a stream socket
    if ((listen_sd = socket(AF_INET, SOCK_STREAM, 0)) == -1){
        qWarning("Cannot Create Socket!");
        return -1;
    }
    arg = 1;
    if (setsockopt (listen_sd, SOL_SOCKET, SO_REUSEADDR, &arg, sizeof(arg)) == -1){
        qWarning("setsockopt");
        return-1;
    }

    // Bind an address to the socket
    bzero((char *)&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY); // Accept connections from any client


    if (bind(listen_sd, (struct sockaddr *)&server, sizeof(server)) == -1){
        qWarning("bind error");
        return-1;
    }

    // Listen for connections
    // queue up to LISTENQ connect requests
    qDebug("Ready for listen\n");

    listen(listen_sd, LISTENQ);
    qDebug("Finish  listen\n");

    return listen_sd;

}



void ServerSrc::receiveMsg(int i, MsgStr* sendMsg, MsgStr* rcvMsg){
    int n = 0;
    int bytes_to_read = sizeof(MsgStr);
    while ((n = recv(clientList[i].sockNum, (MsgStr *)rcvMsg, bytes_to_read, 0)) < bytes_to_read)
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
 //           sendList(clientList[i].sockNum);
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


/*
void* ServerSrc::rcvWriteMsg(void *param){

    MsgStr *sendForm, msgForm, *recvForm, recvMsg;
    sendForm = &msgForm;
    recvForm = &recvMsg;

    int i = ((ServerSrc*)param)->socketBuffer[readBuf++];
    if(readBuf==2*BUFLEN-1){
        readBuf = readBuf-BUFLEN;
        writeBuf = writeBuf - BUFLEN;
    }
    else if(readBuf>BUFLEN)
        readBuf = readBuf%BUFLEN;


    receiveMsg(i, sendForm, recvForm);

    qDebug("RESULT Of SEND: Type %d, Name %s\n\tText: %s\n",
                sendForm->type, sendForm->name, sendForm->msgTxt);
    qDebug("RESULT Of SEND: Type %d, Name %s\n\tText: %s\n",
                recvForm->type, recvForm->name, recvForm->msgTxt);


    switch(recvForm->type){
        case MSG_CONN:
        case MSG_MESG:
                //writeMessage(sendForm);

                printf("echo all\n");
                if (pthread_create(&threadid, NULL, writeMsg,(void *)sendForm) < 0)
                {
                    perror("thread create error : ");
                    exit(0);
                }

                writeMessage(sendForm);
                break;

        case MSG_QUIT:
                qDebug()<<"\t Remote Address:  closed ";//<<inet_ntoa(client_addr.sin_addr)<< " closed connection\n";
                close(clientList[i].sockNum);
                //should clear allset!!
                //FD_CLR(sockNum->sock, &allset);
                //initClientInfo(i);
                break;
    }

}

void ServerSrc::receiveMsg(int i, MsgStr* sendMsg, MsgStr* rcvMsg){
    int n = 0;
    int bytes_to_read = sizeof(MsgStr);
    while ((n = recv(clientList[i].sockNum, (MsgStr *)rcvMsg, bytes_to_read, 0)) < 0)
    {
        printf("n is %d\n", n);
        rcvMsg+=n;
        bytes_to_read -= n;
    }

    qDebug("RECV SUCCESS\n");
    qDebug("TEST type %d : name - %s   : update %s\n",rcvMsg->type, rcvMsg->name, rcvMsg->msgTxt);

    switch(rcvMsg->type){
        case MSG_CONN:
            strcpy(clientList[i].name, rcvMsg->name);
            //sendList(clientList[i].sockNum);
            createMsg(sendMsg, MSG_CONN, i, rcvMsg->msgTxt);
            qDebug()<< "TEST MSG_CONN :  name - "<< rcvMsg->name<< "   : update "<<clientList[i].name<<"\n";
            //printf("TEST MSG_CONN : name - %s   : update %s\n", rcvMsg->name, clientList[i].name);
            break;
        case MSG_NAME:
            strcpy(clientList[i].name, rcvMsg->name);
            break;
        case MSG_MESG:
            qDebug() << "the recv message type: "<< rcvMsg->type  <<" context " << rcvMsg->msgTxt<<"\n";
            //printf("the recv message type: %d  context%s\n", rcvMsg->type, rcvMsg->msgTxt);
            createMsg(sendMsg, MSG_MESG, i, rcvMsg->msgTxt);

            break;
    }
    qDebug()<< "RESULT Of SEND: Type %d, Name "<<sendMsg->type<<"\n\tText: "<<sendMsg->msgTxt<<"\n";
   // printf("RESULT Of SEND: Type %d, Name %s\n\tText: %s\n", sendMsg->type, sendMsg->name, sendMsg->msgTxt);
}
*/
void ServerSrc::createMsg(MsgStr* fullMsg, int type, int i, const char *msgText){
    fullMsg->type = type;
    strcpy(fullMsg->name, clientList[i].name);
    strcpy(fullMsg->msgTxt, msgText);
    printf("CreateMsg - type: %d, client: %s, txt: %s\n", fullMsg->type, fullMsg->name, fullMsg->msgTxt);
}

void ServerSrc::writeMessage(MsgStr *sendMsg){
    MsgStr message;
    message.type = sendMsg->type;
    strcpy(message.name, sendMsg->name);
    strcpy(message.msgTxt, sendMsg->msgTxt);

    for(int i = 0; i <=maxi; i++){
            if(clientList[i].sockNum < 0)
                continue;
        write(clientList[i].sockNum, (void *)sendMsg, sizeof(MsgStr));
        qDebug()<<"write to client["<<i<<"]: "<<clientList[i].sockNum<<"\n";
    }

//    pthread_create(&threadid, NULL, writeMsg, this);
}


