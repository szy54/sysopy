//compile with -D_POSIX_C_SOURCE=199309L
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>


int sigusrsReceived=0, M, N, sigrtReceived=0, childrenTerminated=0;
pid_t *childrenAwaiting;
pid_t *pids;
int *ack;




void sigactionHandler(int sig, siginfo_t *info, void *uncontext){
    //stack says check si_code ==SI_USER or SI_QUEUE which is why signal was sent: SI_USER for kill() and SI_QUEUE for sigqueue()
    
    pid_t senderPid=info->si_pid;
    printf("got request no.%d for permission from child %d\n", sigusrsReceived, senderPid);

    int i;
    for(i=0;i<N;i++){
        if(pids[i]==senderPid && ack[i]==0) {
            ack[i]=1;
            childrenAwaiting[sigusrsReceived]=senderPid;
            sigusrsReceived++;
        }
    }
    
    if(sigusrsReceived==M){
        int i;
        for(i=0;i<sigusrsReceived;i++){
            kill(childrenAwaiting[i], SIGUSR2);
            printf("give permission to waiting child %d for SIGRT\n", childrenAwaiting[i]);

        }
    }
    else if(sigusrsReceived>M){
        kill(senderPid, SIGUSR2);
        printf("give permission to child %d for SIGRT\n", senderPid);

    }

    
}

void sigintHandler(int sig_no){
    if(sig_no==SIGINT){
        for(int i=0;i<N;i++){
            kill(pids[i], SIGINT);
        }
    }
}

void sigChildAccepted(int sig_no){
    if(sig_no==SIGUSR2){
        printf("permission received: child %d\n", getpid());

        int rtsig_no = SIGRTMIN+rand()%(SIGRTMAX-SIGRTMIN+1);
        kill(getppid(), rtsig_no);
        printf("sending sigrt %d from child %d\n", (rtsig_no-SIGRTMIN), getpid());
    }
}

void sigrtHandler(int sig_no, siginfo_t *info, void *uncontext){
    pid_t senderPid=info->si_pid;
    printf("RTsig no %d received from %d\n", (sig_no-SIGRTMIN), senderPid);

    sigrtReceived++;
}

int main(int argc, char * argv[]){

    srand(time(NULL));
    N = atoi(argv[1]);
    M = atoi(argv[2]);
    printf("N: %d M: %d\n", N,M);

    pids=calloc(N, sizeof(pid_t));
    childrenAwaiting=calloc(M, sizeof(pid_t));
    ack=calloc(N, sizeof(int));

    struct sigaction act;
    act.sa_flags=SA_SIGINFO;
    act.sa_sigaction=sigactionHandler;
    sigemptyset(&act.sa_mask);
    sigaction(SIGUSR1, &act, NULL);
    signal(SIGINT, sigintHandler);
    
    sigset_t signals;
    sigfillset(&signals);
    sigdelset(&signals, SIGUSR1);
    sigdelset(&signals, SIGINT);

    sigset_t rtsignals;
    sigfillset(&rtsignals);
    sigdelset(&rtsignals, SIGINT);
    sigdelset(&rtsignals, SIGUSR1);

    int j;
    for(j=SIGRTMIN;j<=SIGRTMAX;j++){
        struct sigaction act2;
        act2.sa_flags=SA_SIGINFO;
        act2.sa_sigaction=sigrtHandler;
        sigaction(j, &act2, NULL);
        sigdelset(&rtsignals, SIGRTMIN+j);
    }

    int i;
    for(i=0;i<N;i++){
        if((pids[i]=fork())==-1){
            printf("error forking\n");

            exit(EXIT_FAILURE);
        }
        else if(pids[i]==0){
            srand((unsigned int)getpid());
            int secs=rand()%4;
            //int secs=4;
            sleep(secs);
            
            
            signal(SIGUSR2, sigChildAccepted);
            sigset_t sig1;
            sigfillset(&sig1);
            ///sigdelset(&sig1, SIGUSR1);
            sigdelset(&sig1, SIGUSR2);
            sigdelset(&sig1, SIGINT);
            
            
                kill(getppid(), SIGUSR1);
                
            
            printf("child %d sent request for permission with SIGUSR1\n", getpid());
            sigsuspend(&sig1);
            return secs;
        }
        else{
            printf("created child pid %d\n", pids[i]);

        }
    }


   int stat;
    while(childrenTerminated<N){
        printf("waiting for child termination\n");
        pid_t terminatedChild = wait(&stat);
        if(terminatedChild!=-1){
            childrenTerminated++;
        printf("%d. %d terminated with %d\n",childrenTerminated, terminatedChild, WEXITSTATUS(stat));

        }
        

    }

    return 0;
}