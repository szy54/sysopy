#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/resource.h>
#include <time.h>
#include <string.h>


void generate (char* fileName, int recordsNum, int recordLength){
    srand(time(NULL));
    int f = open(fileName, O_WRONLY|O_CREAT|O_TRUNC);
    unsigned char *buffer =malloc(recordLength);
    int i,j;
    for(i=0;i<recordsNum-1;i++){
        for(j=0;j<recordLength;j++){
            buffer[j]=65+rand()%26;    
        }
        buffer[recordLength-1]='\n';
        //printf("%s\n", buffer);
        write(f, buffer, recordLength);
        //write(f, "\n", 1);
    }
    close(f);
}

int bufferCompare(char *buff1, char *buff2){
    unsigned char a = buff1[0];
    unsigned char b = buff2[0];
    if(a<b) return -1;
    else if(a==b) return 0;
    else return 1;
}

void sort(char *fileName, int recordsNum, int recordLength, int sys){
    char *buffer1 = malloc(recordLength);
    char *buffer2 = malloc(recordLength);
    if(sys==1){
        int f=open(fileName, O_RDWR);
        int i,j;
        for(i=1;i<recordsNum;i++){
            lseek(f, i*recordLength, SEEK_SET);
            read(f, buffer1, recordLength);
            j=i-1;
            while(j>=0){
                //printf("%d ", j);
                lseek(f, j*recordLength, SEEK_SET);
                read(f, buffer2, recordLength);
                if(bufferCompare(buffer2, buffer1)==1){
                    lseek(f, (j+1)*recordLength, SEEK_SET);
                    write(f, buffer2, recordLength);
                    j=j-1;
                    lseek(f, (j+1)*recordLength, SEEK_SET);
                    write(f, buffer1, recordLength);
                }
                else{
                    break;
                }
            }
            //printf("%d sorted\n", i);
        }
        close(f);
    }
    else{
        FILE *f=fopen(fileName, "r+");
        int i,j;
        for(i=1;i<recordsNum;i++){
            fseek(f, i*recordLength, 0);
            fread(buffer1, sizeof(char), recordLength, f);
            j=i-1;
            while(j>=0){
                fseek(f, j*recordLength, 0);
                fread(buffer2, sizeof(char), recordLength, f);
                if(bufferCompare(buffer2, buffer1)==1){
                    fseek(f, (j+1)*recordLength, 0);
                    fwrite(buffer2, sizeof(char), recordLength, f);
                    j=j-1;
                    fseek(f, (j+1)*recordLength, 0);
                    fwrite(buffer1, sizeof(char), recordLength, f);
                }
                else{
                    break;
                }

            }
            //printf("%d sorted\n", i);
        }
        fclose(f);
    }
    
}

void copy (char *srcFile, char *destFile, int recordsNum, int recordLength, int sys){  //sys=1 -> system functions, otherwise lib functions
    int bytes;
    char *buffer = malloc(recordLength);

    if(sys==1){
        int fIn=open(srcFile, O_RDONLY);
        int fOut=open(destFile, O_WRONLY|O_CREAT|O_TRUNC);
        while((bytes=read(fIn, buffer, recordLength))>0){
           write(fOut, buffer, bytes);
        }
        close(fIn);
        close(fOut);
    }
    else{
        FILE *fIn=fopen(srcFile, "r");
        FILE *fOut=fopen(destFile, "w");
        while((bytes=fread(buffer, sizeof(char), recordLength, fIn))>0){
            fwrite(buffer, sizeof(char), bytes, fOut);
        }
        fclose(fIn);
        fclose(fOut);
    }
}

void makeActions(char* argv[]){
    printf("%s %s %s %s %s %s\n",argv[0], argv[1], argv[2], argv[3], argv[4], argv[5]);
    if(strcmp(argv[1], "generate")==0){

        generate(argv[2], atoi(argv[3]), atoi(argv[4]));
    }
    else if(strcmp(argv[1], "copy")==0){

        copy(argv[2], argv[3], atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
    }
    else if(strcmp(argv[1], "sort")==0){

        sort(argv[2], atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    }
    //generate (argv[1], atoi(argv[2]), atoi(argv[3]));
    //copy(argv[1], argv[2], atoi(argv[3]), atoi(argv[4]), atoi(argv[5]));
    //sort(argv[1], atoi(argv[2]), atoi(argv[3]), atoi(argv[4]));
}

double timevalToSec(struct timeval *tv) {
  return (double)(tv->tv_sec) + ((double)tv->tv_usec / 1000000.0);
}

int main(int argc, char* argv[]){
    struct rusage ruStart;
    struct rusage ruEnd;
    memset(&ruStart, 0, sizeof(struct rusage));
    memset(&ruEnd, 0, sizeof(struct rusage));
    getrusage(RUSAGE_SELF, &ruStart);
    makeActions(argv);
    getrusage(RUSAGE_SELF, &ruEnd);
    //printf("%ld",tStop->tms_stime);
    double userTime = timevalToSec(&ruEnd.ru_utime)-timevalToSec(&ruStart.ru_utime);
    double sysTime = timevalToSec(&ruEnd.ru_stime)-timevalToSec(&ruStart.ru_stime);
    int i;
    for(i=1;i<argc;i++)
        printf("%s ", argv[i]);
    printf("\nusr: %f\nsys: %f\n", userTime,sysTime);
    return 0;
}

