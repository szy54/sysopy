#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char* argv[]){
    srand(time(NULL));
    int fd;
    int N=atoi(argv[2]);
    char arr1[512];
    printf("%d\n", getpid());
    for(int i=0;i<N;i++){
        fd=open(argv[1], O_WRONLY);
        FILE* ps_input = popen("date", "r");
        //fscanf(ps_input, "%s", arr1);
        fgets(arr1, sizeof(arr1), ps_input);
        pclose(ps_input);
        write(fd, arr1, strlen(arr1)+1);
        close(fd);
        sleep(1+rand()%5);
    }

    return 0;
}