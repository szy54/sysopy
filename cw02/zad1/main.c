#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

void generate (char* fileName, int recordsNum, int recordLength){
    srand(time(NULL));
    int f = open(fileName, O_WRONLY|O_CREAT|O_TRUNC);
    unsigned char *buffer =malloc(recordLength);
    int i,j;
    for(i=0;i<recordsNum;i++){
        for(j=0;j<recordLength;j++){
            buffer[j]=65+rand()%26;    
        }
        printf("%s\n", buffer);
        write(f, buffer, recordLength);
        write(f, "\n", 1);
    }
    close(f);
}

void makeActions(int argc, char* argv[]){
    generate (argv[1], atoi(argv[2]), atoi(argv[3]));
    
}



int main(int argc, char* argv[]){
    makeActions(argc, argv);

    return 0;
}

