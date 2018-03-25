#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAXARGC 50
#define WHITE " "

int parsecommand(char * cmdstr)
{
    char *cmdstrdup = strdup(cmdstr);
    if(cmdstrdup == NULL)
      //insuficient memory, do some errorhandling. 
      return 0; 
    char *saveptr;
    char *ptr;
    char *argv[MAXARGC]; 
    int argc;

    if((ptr = strtok_r(cmdstrdup, WHITE, &saveptr)) == NULL)
    {
        printf("%s\n", "no args given");
        return 0;
    } 

    argv[argc = 0] = cmdstrdup;
    while(ptr != NULL) {
        //printf("%d\n", argc);
        ptr = strtok_r(NULL, WHITE, &saveptr);
        if(++argc >= MAXARGC-1) // -1 for room for NULL at the end
            break;
        argv[argc] = ptr;
    }
    /*
    int i;
    for(i=0;i<argc;i++){
        printf("%s\n", argv[i]);
    }
    */
    int exitState;
    pid_t childPid = fork();
    if(childPid==0){
        execvp(argv[0], argv);
        exit(17);
    }
    else{
        //printf("%d %d", childPid, exitState);
        //waitpid(childPid, &exitState, 0);
        waitpid(childPid, &exitState, 0);
        //printf("The status is %d\n", exitState);
        if(WIFEXITED(exitState) && WEXITSTATUS(exitState)==0){
            printf("\n");
            free(cmdstrdup);
            return 1;
        } 
        else{
            printf("Error: process did not exited with 0!\n"); //maybe I should put some status macro here to get the err msg
            printf("Process terminated by a signal: %d\n", WIFSIGNALED(exitState));
            if(WIFSIGNALED(exitState)) printf("Terminated by signal number: %d\n", WTERMSIG(exitState));
            #ifdef WCOREDUMP
                if(WIFSIGNALED(exitState)) printf("core dumped: %d\n", WCOREDUMP(exitState));
            #endif
            printf("Process stopped: %d\n", WIFSTOPPED(exitState));
            if(WIFSTOPPED(exitState)) printf("Stopsig: %d\n", WSTOPSIG(exitState));
            free(cmdstrdup);
            return 0;
        }
    }
    //printf("%d", argc);
}

void linesReading(char *fileName){
    FILE *fp;
    char *line = NULL;
    size_t len =0;
    ssize_t read;

    fp=fopen(fileName, "r");
    if(fp==NULL)
        exit(EXIT_FAILURE);
    while((read=getline(&line, &len, fp))!=-1){
        //printf("retrieved line of length %zu:\n", read);
        //printf("%s", line);
        if(line[read-1]=='\n')line[read-1]=NULL;
        if(parsecommand(line)!=1)
            break;

    }
    free(line);
    fclose(fp);
}


int main(int argc, char* argv[]){
    linesReading(argv[1]);
    return 0;
}