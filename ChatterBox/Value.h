#ifndef VALUE_H
#define VALUE_H

#define DEFAULT_PORT		7000	// Default port
#define BUFLEN		  255  	// Buffer length
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
    char msgTxt[BUFLEN];
} MsgStr;
typedef struct clientStr{
    int sockNum;
    char name[NAMESIZE];
} Clients;

#endif // VALUE_H
