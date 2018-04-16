#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
    int fd;
    mkfifo(argv[1], 0666);
    char arr1[512];
    while(1){
        fd=open(argv[1], O_RDONLY);
        read(fd, arr1, sizeof(arr1));
        printf("%s\n", arr1);
        close(fd);
    }
    return 0;
}