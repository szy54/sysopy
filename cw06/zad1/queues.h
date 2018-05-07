#ifndef QUEUES_H
#define QUEUES_H

#include <sys/msg.h> 
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

#define MAX_LINE_SIZE 128
#define MAX_CLIENT_NUMBER 1128
#define MSGPERM 0600
#define SERVER_ID '7'


struct message { 
    int message_type;
    int id;
    key_t key; 
    char text[MAX_LINE_SIZE]; 
}; 


enum operationType{
    REG=1
    ,MIRROR
    ,ADD
    ,SUB
    ,MUL
    ,DIV
    ,TIME
    ,STOP
    ,END
};

int errorCode=0;
#endif