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
    clientSrc(QWidget *parent = 0);
    void clientStart();
    void createMsg(MsgStr *mesgForm, int typeNum, const char *message);
    void sendPersonalInfo();
    void writeMsg(int type, std::string message);
    void recvList();

    Clients list[MAXUSER];

    QWidget *parent;
};

#endif // CLIENTSRC_H

