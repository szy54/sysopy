#include "queues.h"


int sQid, cQid, myID=-1;
key_t sKey, cKey;

int startClient();
void clean();
void sigintH(int signo);



int main (int argc, char **argv) {
    startClient();
    
    struct message msg1;
    struct message rsp1;
    msg1.message_type=REG;
    msg1.key=cKey;
    if((msgsnd(sQid, &msg1, MAX_LINE_SIZE, 0))==-1){
        printf("%s\n", strerror(errno));
        return -1;
    }
    printf("sent  key\n");
    printf("waiting for response\n");
    if((msgrcv(cQid, &rsp1, MAX_LINE_SIZE, 0, 0))==-1){
        printf("%s\n", strerror(errno));
        return -1;
    }
    myID = rsp1.id;
    printf("got response with id:%d\n", myID);


    FILE * inputFile = fopen(argv[1], "r");
    if(inputFile == NULL){
        printf("%s 7\n", strerror(errno));
        return 1;
    }
    char line[MAX_LINE_SIZE];
    char* operations[] = {"MIRROR", "ADD", "SUB", "MUL", "DIV", "TIME", "END"};
    
    while(fgets(line, MAX_LINE_SIZE, inputFile)) {
        int isProperCmd = 0;
        char * ptr = strtok(line, " \n\t");
        for (int i = 0; i < sizeof(operations) / sizeof(char *); i++) {
            if (strcmp(operations[i], ptr) == 0) {
                isProperCmd = 1;

                msg1.message_type = MIRROR + i;
                msg1.id = myID;
                ptr = strtok(NULL, "\n");
                if(ptr != NULL) {
                    strcpy(msg1.text, ptr);

                }
                else
                    msg1.text[0] = '\0';
                if(msg1.text[0] != NULL && (msg1.message_type == TIME || msg1.message_type == STOP)){
                    printf("Syntax of %s is incorrect\n", operations[i]);
                    continue;
                }
                printf("sending %d %s\n", msg1.message_type, msg1.text);
                errorCode = msgsnd(sQid, &msg1, MAX_LINE_SIZE, 0);
                if (errorCode == -1 && errno) {
                    printf("%s 8\n", strerror(errno));
                    return 1;
                }
                break;
            }
        }
        if (!isProperCmd) {
            printf("Given command \'%s\' is incorrect", line);
            return 1;
        }
    }

    while(1){
        errorCode = (int)msgrcv(cQid, &rsp1, MAX_LINE_SIZE, 0, 0);
        if(errorCode == -1) {
            printf("%s 9\n", strerror(errno));
            return 1;
        }
        printf("Received msg from server: %s\noutput of query: %s\n\n",
               rsp1.text,
               operations[rsp1.message_type - MIRROR]
        );

    }
    
    printf ("Client: bye\n"); 

    exit (0); 
} 

int startClient(){
    signal(SIGINT, sigintH);
    atexit(clean);
    printf("client start\n");
    sKey = ftok(getenv("HOME"), SERVER_ID);
    if(sKey==-1)
        return -1;
    sQid=msgget(sKey, MSGPERM);
    if(sQid==-1){
        printf("%s\n", strerror(errno));
        return -1;
    }
    cKey = ftok(getenv("HOME"), getpid());
    if(cKey==-1)
        return -1;
    cQid=msgget(cKey, MSGPERM | IPC_CREAT);
    if(cQid==-1){
        printf("%s\n", strerror(errno));
        return -1;
    }
    return 0;
}

void sigintH(int signo){
    msgctl(cQid, IPC_RMID, NULL);
    struct message query;
    query.message_type = STOP;
    query.id=myID;
    msgsnd(sQid, &query, MAX_LINE_SIZE, 0);
    _exit(1);
}

void clean(){
    msgctl(cQid, IPC_RMID, NULL);

    struct message query;
    query.message_type = QUIT;

    msgsnd(sQid, &query, MAX_LINE_SIZE, 0);
}

