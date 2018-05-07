#include "queues.h"

key_t sKey;
int sQid;
int stop=0;


int startServer();
int registering(struct message query, struct message *response);
void clean();
int stringToInt(int *dest, char *src);
void handleEnd(struct message query);
void handleTime(struct message query, struct message *response);
void handleCalc(struct message query, struct message *response, enum operationType type);
void handleMirror(struct message query, struct message *response);
void handleStop(struct message query);

int clients[MAX_CLIENT_NUMBER];
int clientCount=0;

int main (int argc, char **argv) { 
    startServer();

    int flag=0;
    printf ("Server: Hello, World!\n"); 
    while (1) { // read an incoming message 
        struct message query, response;
        stop=0;
        if (msgrcv (sQid, &query, MAX_LINE_SIZE, 0, flag) == -1) { 
            printf ("no msg\n"); 
             
        } 
        printf("Got query: %s - msg,\t %d - id,\t %d - type\n", query.text == NULL ? "null" : query.text, query.id, query.message_type);
        switch(query.message_type){
            case REG:
                registering(query, &response);
                break;
            case MIRROR:
                handleMirror(query, &response);
                break;
            case ADD:
                handleCalc(query, &response, ADD);
                break;
            case SUB:
                handleCalc(query, &response, SUB);
                break;
            case MUL:
                handleCalc(query, &response, MUL);
                break;
            case DIV:
                handleCalc(query, &response, DIV);
                break;
            case TIME:
                handleTime(query, &response);
                break;
            case STOP:
                handleStop(query);
                break;
            case END:
                printf("quit\n");
                handleEnd(query);
                break;
            
        }
        
        if(!stop){
            response.message_type=query.message_type;
            msgsnd(clients[clientCount-1], &response, MAX_LINE_SIZE, 0);
            printf("repsonse sent to:%d\n",response.id);
        }
    }
}

int startServer(){
    atexit(clean);
    sKey=ftok(getenv("HOME"), SERVER_ID);
    sQid=msgget(sKey, MSGPERM | IPC_CREAT);
    return 1;
}

int registering(struct message query, struct message *response){
    if((clients[clientCount]=msgget(query.key, MSGPERM))==-1){
        printf("%s\n", strerror(errno));
        return -1;
    }
    printf("queue opened\n");
    response->message_type = REG;
    response->id = clientCount;
    clientCount++;
    return 1;
}

void handleMirror(struct message query, struct message *response){
    size_t len = strlen(query.text);
    for(int i=0;i<len;i++){
        response->text[i]=query.text[len-i-1];
    }
    response->text[len]='\0';
}

void handleCalc(struct message query, struct message *response, enum operationType type){
    int arg1, arg2;
    //errorCode = stringToInt(&arg1, strtok(query.text, "\n\t"));
    //printf("%d\n",errorCode);
    //errorCode |= stringToInt(&arg2, strtok(NULL, "\n\t"));
    //    printf("%d %d %d\n",arg1, arg2, errorCode);
    char *a=strtok(query.text, " \n\t");
    int er=stringToInt(&arg1, a);
    char *b=strtok(NULL, " \n\t");
    er |=stringToInt(&arg2, b);
    
    if(er!=0)
        sprintf(response->text, "wrong command\n");
    else
        sprintf(response ->text, "%d", 
            (type==ADD) ? arg1 + arg2:
            (type==SUB)? arg1-arg2:
            (type==MUL) ? arg1* arg2:
            arg1/arg2
        );
        
}

void handleTime(struct message query, struct message *response){
    struct timeval t;
    struct tm *formatedTime;
    gettimeofday(&t, NULL);
    formatedTime=localtime(&t.tv_sec);
    strftime(response->text, sizeof(response->text), "%Y-%m-%d %H:%M:%S", formatedTime);
}

void handleStop(struct message query){
    for(int i=query.id;i<clientCount;i++){
        clients[i]=clients[i+1];
    }
    clientCount--;
    stop=1;
    printf("removed id %d from clients list\n", query.id);
}

void handleEnd(struct message query){
    for(int i=0;i<sizeof(clients);i++){
        msgctl(clients[i], IPC_RMID, NULL);
    }
}

void clean(){
    msgctl(sQid, IPC_RMID, (struct msqid_ds *) NULL);
}

int stringToInt(int *dest, char *src){
    char *dump;
    *dest = (int)strtol(src, &dump, 10);
    if(*dump != '\0') return -1;
    return 0;
}