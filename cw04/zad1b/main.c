/*Napisz program wypisujący w pętli nieskończonej aktualną godzinę Po odebraniu sygnału SIGTSTP (CTRL+Z) program zatrzymuje się, wypisując komunikat "Oczekuję na CTRL+Z - kontynuacja albo CTR+C - zakonczenie programu". Po ponownym wysłaniu SIGTSTP program powraca do pierwotnego wypisywania.
Program powinien również obsługiwać sygnał SIGINT. Po jego odebraniu program wypisuje komunikat "Odebrano sygnał SIGINT" i kończy działanie. W kodzie programu, do przechwycenia sygnałów użyj zarówno funkcji signal, jak i sigaction (np. SIGINT odbierz za pomocą signal, a SIGTSTP za pomocą sigaction).
Zrealizuj powyższe zadanie, tworząc program potomny, który będzie wywoływał jedną z funkcji z rodziny exec skrypt shellowy zawierający zapętlone systemowe polecenie date. Proces macierzysty będzie przychwytywał powyższe sygnały i przekazywał je do procesu potomnego, tj po otrzymaniu SIGTSTP kończy proces potomka, a jeśli ten został wcześniej zakończony, tworzy nowy, wznawiając działanie skryptu, a po otrzymaniu SIGINT kończy działanie potomka (jeśli ten jeszcze pracuje) oraz programu.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

int sigstpReceived=0;
int killProcess=0;
pid_t childPid=1;
void obslugaINT(int signum){
    printf("Odebrano SIGINT\n");
    kill(childPid, SIGINT);
    exit(0);
}

void spawn(){
    childPid=fork();
    if(childPid==0){
        execlp("./loopScript", (char *)NULL);
        exit(0);
    }
}

void obslugaTSTP(int signum){
    printf("oczekuje na CTRL+Z - kontynuacja lub CTRL+C - zakonczenie programu\n");
    if(sigstpReceived==0) sigstpReceived=1;
    else sigstpReceived=0;
}



int main(int argc, char* argv[]){
    struct sigaction sigact;
    sigact.sa_handler=obslugaTSTP;
    sigaction(SIGTSTP, &sigact, NULL);
    signal(SIGINT, obslugaINT);
    spawn();

    while(1){
        if(sigstpReceived==0){
            if(killProcess){
                killProcess=0;
                spawn();
            }
        }
        else{
            if(killProcess==0){
                kill(childPid, SIGTSTP);
                killProcess=1;
            }
        }
    }
    return 0;
}