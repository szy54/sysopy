#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <sys/times.h>
#include <unistd.h>
#include "alloc.h"
#define MAX_OPS 6

char arr[10000][10000];


int isNumeric(char *string);
void fPrintTime(FILE *f, struct tms *stop, struct tms *start, struct timespec *start_real, struct timespec *stop_real){
    fprintf(f, "Real: %lds %ldns\n User: %ld\n Sys: %ld\n", (stop_real->tv_sec-start_real->tv_sec), (stop_real->tv_nsec-start_real->tv_nsec), stop->tms_stime-start->tms_stime, stop->tms_utime-start->tms_utime);
}
double timeDiffInSeconds(clock_t start, clock_t end) {
  return (double)(end - start) / sysconf(_SC_CLK_TCK);
}

int main(int argc, char *argv[]){
    srand(time(NULL));
    //0 - filename
    //1 - size1
    //2 - size2
    //3 - memory allocation type :  's'== static, 'd'== dynamic
    //4 - operation #1 :            'e'==table allocation (must be the first one)
    //[5 - operation #2]:           'a'==block search, 
    //                              'b'==delete n blocks then allocate n blocks
    //                              'c'==delete and allocate n blocks  
    //[6 - operation #2 argument]   int
    //[7 - operation #3]:           same as operation #2
    //[8 - operation #3 argument]   int
    
    //printf("argc: %d", argc);
    //int i;
    //for(i=0;i<argc;i++){
    //    printf("\n arg%d ",i);
    //    printf("%s",argv[i]);
    //}
    
    if(argc<5) printf("not enough arguments");
    else{
        int size1, size2;
        int dynamic;
        char **p;
        
        clock_t * realTimeClocks = (clock_t*)malloc(4 * sizeof(clock_t));
        struct tms **tmsTimes = (struct tms**)malloc(4 * sizeof(struct tms *));

        for (int i = 0; i < 4; ++i) {
            tmsTimes[i] = (struct tms *)malloc(sizeof(struct tms *));
        }
        realTimeClocks[0] = times(tmsTimes[0]);

        if(isNumeric(argv[1]) && isNumeric(argv[2])){   //check whether table and block size are numeric
            size1=atoi(argv[1]);    //convert them to integers
            size2=atoi(argv[2]);

            
            if(!strcmp(argv[3],"s")){    //if the allocation type is static
                allocateStaticTable(arr, size1, size2);
                dynamic=0;

            }
            else if(!strcmp(argv[3],"d")){   //or dynamic
                p=allocateTable(size1, size2);
                dynamic=1;
            }
            else{
                printf("wrong static/dynamic argument");
            }
            realTimeClocks[1] = times(tmsTimes[1]);
            
            
            if(!strcmp(argv[4], "e")){
                
                if((argc==7 && isNumeric(argv[6])) || (argc==9 && isNumeric(argv[6]) && isNumeric(argv[8]))){
                    int j=0;
                    for(j=0;j<=2;j+=2){
                        char *op=argv[5+j];
                        int arg=atoi(argv[6+j]);
                        if(!strcmp(op,"a") && arg>=0 && arg<size1){
                            printf("entered a \n");
                            int blockIndex;
                            if(dynamic) blockIndex=findBlock(p, size1, size2, arg);
                            else blockIndex=findStaticBlock(arr, size1, size2, arg);
                            printf("%d\n", blockIndex);
                        }
                        else if(!strcmp(op,"b") && arg>0 && arg<=size1){
                            printf("entered b \n");
                            int i;
                            if(dynamic){
                                for(i=0;i<arg;i++){
                                    deallocateBlock(p, i);
                                }
                                for(i=0;i<arg;i++){
                                    p[i]=allocateBlock(size2);
                                }
                            }
                            else{
                                for(i=0;i<arg;i++){
                                    deallocateStaticBlock(arr, size2, i);
                                }
                                for(i=0;i<arg;i++){
                                    allocateStaticBlock(arr, size2, i);
                                }
                            }
                        }
                        else if(!strcmp(op,"c") && arg>0 && arg<=size1){
                            printf("entered c \n");
                            int i;
                            if(dynamic){
                                for(i=0;i<arg;i++){
                                    deallocateBlock(p,i);
                                    p[i]=allocateBlock(size2);
                                }
                            }
                            else{
                                for(i=0;i<arg;i++){
                                    deallocateStaticBlock(arr,size2,i);
                                    allocateStaticBlock(arr,size2,i);
                                }
                            } 
                        }
                        else{
                            printf("wrong operation parameters");
                        }
                        realTimeClocks[2+j/2] = times(tmsTimes[2+j/2]);
                    }
                }
                
                
                }
            else{
                printf("table allocation must be the first operation, found %c", argv[4][0]);
            }
            
            if(dynamic){
                //deallocate(p, size1);
                int k;
                for(k=0;k<size1;k++)
                    free(p[k]);
                //free(p); //throws error when I try to free it
            } 
        }
        else{
            printf("wrong block or table size");
        }
        FILE *f;
        f = fopen("raport2.txt", "a");
        if (f == NULL) {
            printf("Error opening file!\n");
            exit(1);
        }
        char *alloc;
        if(dynamic) alloc="dynamic";
        
        else alloc="static";
        fprintf(f, "table %d elements, %d blocks, %s allocation\n", size1, size2, alloc);
        fprintf(f, "%-14s\t%-11s\t%-11s\t%-11s\n", "", "User", "System", "Real");
        fprintf(f, "%-14s\t%-10.8fs\t%-10.8fs\t%-10.8fs\n",
            "Init & fill",
            timeDiffInSeconds(tmsTimes[0]->tms_stime, tmsTimes[1]->tms_stime)/size2,
            timeDiffInSeconds(tmsTimes[0]->tms_utime, tmsTimes[1]->tms_utime)/size2,
            timeDiffInSeconds(realTimeClocks[0], realTimeClocks[1])/size2);

        fprintf(f, "%-14s\t%-10.8fs\t%-10.8fs\t%-10.8fs\n",
            "First operation",
            timeDiffInSeconds(tmsTimes[1]->tms_stime, tmsTimes[2]->tms_stime)/size2,
            timeDiffInSeconds(tmsTimes[1]->tms_utime, tmsTimes[2]->tms_utime)/size2,
            timeDiffInSeconds(realTimeClocks[1], realTimeClocks[2])/size2);
  
        fprintf(f, "%-14s\t%-10.8fs\t%-10.8fs\t%-10.8fs\n\n",
            "Second operation",
            timeDiffInSeconds(tmsTimes[2]->tms_stime, tmsTimes[3]->tms_stime)/size2/2.0,
            timeDiffInSeconds(tmsTimes[2]->tms_utime, tmsTimes[3]->tms_utime)/size2/2.0,
            timeDiffInSeconds(realTimeClocks[2], realTimeClocks[3])/size2/2.0);
            
        fclose(f);
    }

    /*
    char **p = allocateTable(SIZE, SIZE2);
    //for(i=0;i<SIZE;i++) printf("%c\n", p[i]);
    printTable(p, SIZE, SIZE2);
    int i=findBlock(p,SIZE,SIZE2,2);
    printf("%d\n", i);
    deallocate(p, SIZE);
    allocateStaticTable(arr, SIZE, SIZE2);
    //deallocateStaticBlock(arr, SIZE2, 2);
    
    allocateStaticBlock(arr,SIZE2,4);
    allocateStaticBlock(arr,SIZE2,5);
    printStaticTable(arr, SIZE, SIZE2);
    int j=findStaticBlock(arr, SIZE, SIZE2, 2);
    printf("%d\n", j);
    deallocateStaticTable(arr, SIZE, SIZE2);
    */
    return 0;
}

int isNumeric(char *string){
    int i=strlen(string);
    for(i=i-1;i>=0;i--){
        if(!isdigit(string[i])) return 0;
    }
    return 1;
}
