#ifndef CLIENTSRC_H
#define CLIENTSRC_H
#include <stdio.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <stdlib.h>
#include <sstream>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include "client.h"
#include "mainwindow.h"
#include "Value.h"

extern int sockNum;
class clientSrc
{
public:
    clientSrc(QWidget *parent = 0, QWidget *grandparent = 0);//std::string, int);
    void clientStart();
   // void recvStart();
    void createMsg(MsgStr *mesgForm, int typeNum, const char *message);
    void sendPersonalInfo();
    void writeMsg(std::string message);
    void recvList();


    Clients list[MAXUSER];

    QWidget *parent;
    /*
    Clients list[MAXUSER];
    std::string host;
    int port;
    int sockNum;



    void displayList();
    void recvList(int sock);
 //   void *writeMsg(void *sock);
    void clientStart();
    void recvStart(int sv_socket);
    void writeMsg(int type, char* sendMessage);
    */
};

#endif // CLIENTSRC_H


