#ifndef SERVER_H
#define SERVER_H

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
#include <fstream>
#include "Value.h"

using namespace std;


int client[MAXUSER];
int maxi;
Clients clientList[MAXUSER];

void receiveMsg(int, MsgStr*, MsgStr*);
int open_socket(int port);
void receiveMsg(int, MsgStr* , MsgStr*);
void initClientInfo(int);
void createMsg(MsgStr*, int, int, const char *);
void sendList(int );
void *writeMsg(void *);
void writeOne(int, MsgStr *);
void client_display(int);
static void SystemFatal(const char*);

#endif
