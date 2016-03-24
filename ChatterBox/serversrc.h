#ifndef SERVERSRC_H
#define SERVERSRC_H
#include <QDebug>
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
#include "Value.h"

extern clientStr clientList[];
class ServerSrc
{
public:
    ServerSrc();
    int sd , port;
    struct hostent	*hp;
    struct sockaddr_in server;
    char  *host;
    pthread_t p_thread[2];
    int thr_id;
    void *thread_result;
  //  static clientStr clientList[MAXUSER];
    int maxi;
  //  int writeBuf, readBuf;
  //  int socketBuffer[BUFLEN];

    int start();
    int open_socket(int port);
    void receiveMsg(int i, MsgStr* sendMsg, MsgStr* rcvMsg);
    void createMsg(MsgStr* fullMsg, int type, int i, const char *msgText);
    void writeMessage(MsgStr*);
    void* rcvWriteMsg(void *param);

};

#endif // SERVERSRC_H
