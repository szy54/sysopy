#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>

#define MAXARGC 50  //max # of arguments in one line
#define WHITE " "   //space delimiter for strtok
struct rlimit cpuLim, memLim;   
unsigned long long int timeLimit, dataLimit;

int parsecommand(char * cmdstr)
{
    //some workaround because strtok needs static char[]
    char *cmdstrdup = strdup(cmdstr);
    if(cmdstrdup == NULL) 
      return 0; 

    //variables needed by strtok
    char *saveptr;
    char *ptr;

    //variables for result
    char *argv[MAXARGC]; 
    int argc;

    //splitting string with WHITE delimiter
    if((ptr = strtok_r(cmdstrdup, WHITE, &saveptr)) == NULL)
    {
        printf("%s\n", "no args given");
        return 0;
    } 
    argv[argc = 0] = cmdstrdup;

    while(ptr != NULL) {
            ptr = strtok_r(NULL, WHITE, &saveptr);
        if(++argc >= MAXARGC-1) // -1 for room for NULL at the end
            break;
        argv[argc] = ptr;
    }

    //getting the "before" struct and preparing the "after" one
    struct rusage before_usage, after_usage;
    struct timeval ru_utime;
    struct timeval ru_stime;
    getrusage(RUSAGE_CHILDREN, &before_usage);

    int exitState;
    pid_t childPid = fork();
    if(childPid==0){
        if(setrlimit(RLIMIT_CPU, &cpuLim)!=0)
            printf("cannot set cpu limit\n");
        if(setrlimit(RLIMIT_DATA, &memLim)!=0)
            printf("cannot set mem limit");
        execvp(argv[0], argv);
        exit(0);
    }
    else{
        //printf("%d %d", childPid, exitState);
        //waitpid(childPid, &exitState, 0);
        waitpid(childPid, &exitState, 0);
        //printf("The status is %d\n", exitState);
        if(WIFEXITED(exitState) && WEXITSTATUS(exitState)==0){
            printf("\n");
            free(cmdstrdup);
            getrusage(RUSAGE_CHILDREN, &after_usage);
            timersub(&after_usage.ru_utime, &before_usage.ru_utime, &ru_utime);
            timersub(&after_usage.ru_stime, &before_usage.ru_stime, &ru_stime);
            printf("User CPU time used: %d.%d seconds,  system CPU time used: %d.%d seconds\n\n", (int) ru_utime.tv_sec, (int) ru_utime.tv_usec, (int) ru_stime.tv_sec, (int) ru_stime.tv_usec);
            return 1;
        } 
        else{
            printf("Error: process did not exited with 0!\n"); //maybe I should put some status macro here to get the err msg
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
    timeLimit = strtol(argv[2], NULL, 10);
    dataLimit = strtol(argv[3], NULL, 10);
    cpuLim.rlim_cur=(rlim_t) timeLimit;
    cpuLim.rlim_max=(rlim_t) timeLimit;
    memLim.rlim_cur=(rlim_t) 1024 * 1024 * dataLimit;
    memLim.rlim_max=(rlim_t) 1024 * 1024 * dataLimit;
    linesReading(argv[1]);
    return 0;
}