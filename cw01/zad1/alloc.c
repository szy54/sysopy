#include <stdio.h>
#include <stdlib.h>

#include "alloc.h"
#define arrSize 10000


char** allocateTable(int size1, int size2){
    int i,j;
    //time_t tt;
    //int r=time(&tt);
    //srand(r);
    char **ptr = (char **) calloc(size1, sizeof(char *));
    for(i=0;i<size1;i++){
        //ptr[i]=(char*) calloc(size2, sizeof(char));
        ptr[i]=allocateBlock(size2);
        for(j=0;j<size2;j++){
            ptr[i][j]=49+rand()%32;
            //printf("cos");
        }
    }
    return ptr;
}

void printTable(char **ptr, int size1, int size2){
    int i,j;
    for(i=0;i<size1;i++){
        for(j=0;j<size2;j++){
            printf("%d, ", ptr[i][j]);
        }
        printf("\n");
    }
}

void deallocate(char **ptr, int size1){
    int i;
    for(i=0;i<size1;i++){
        //free(ptr[i]);
        deallocateBlock(ptr, i);
    }
    free(ptr);
}

char* allocateBlock(int size){
    char *block = (char*) calloc(size, sizeof(char));
    return block;
}

void deallocateBlock(char **ptr, int blockIndex){
    free(ptr[blockIndex]);
}

int findBlock(char **ptr, int size1, int size2, int blockIndex){
    int i,j,sum1=0, sum2=0, index;
    
    for(j=0;j<size2;j++){
        sum1=sum1+ptr[blockIndex][j];
    }
    for(i=0;i<size1;i++){
        int sum3=0;
        for(j=0;j<size2;j++){
            if(i!=blockIndex){
                sum3=sum3+ptr[i][j];
            }
        }
        if(abs(sum3-sum1)<abs(sum2-sum1)){
            sum2=sum3;
            index=i;
        }
    }
    return index;
}

void allocateStaticBlock(char arr[][arrSize], int size2, int i){
    //time_t tt;
    //int r=time(&tt)+12;
    //srand(time(NULL));
    int j;
    for(j=0;j<size2;j++){
        arr[i][j]=rand()%32 + 49;
    }
}

void allocateStaticTable(char arr[][arrSize], int size1, int size2){
    //to ensure different values in each row I decided to 
    //make allocation of whole table in one function instead
    //of calling allocateStaticBlock many times
    //time_t tt;
    //int r=time(&tt);
    //srand(time(NULL));
    int i,j;
    for(i=0;i<size1;i++){
        for(j=0;j<size2;j++){
            arr[i][j]=49+rand()%32;
        }
    }
}

void deallocateStaticBlock(char arr[][arrSize], int size2, int i){
    int j;
    for(j=0;j<size2;j++){
        arr[i][j]=0;
    }
}

void deallocateStaticTable(char arr[][arrSize], int size1, int size2){
    int i;
    for(i=0;i<size1;i++){
        deallocateStaticBlock(arr, size2, i);
    }
}

int findStaticBlock(char arr[][arrSize], int size1, int size2, int blockIndex){
    int i,j,sum1=0,sum2=0,index;
    for(j=0;j<size2;j++){
            sum1=sum1+arr[blockIndex][j];
        }
        for(i=0;i<size1;i++){
            int sum3=0;
            for(j=0;j<size2;j++){
                if(i!=blockIndex){
                    sum3=sum3+arr[i][j];
                }
            }
            if(abs(sum3-sum1)<abs(sum2-sum1)){
                sum2=sum3;
                index=i;
            }
        }
    return index;
}

void printStaticTable(char arr[][arrSize], int size1, int size2){
    int i,j;
    for(i=0;i<size1;i++){
        for(j=0;j<size2;j++){
            printf("%d, ", arr[i][j]);
        }
        printf("\n");
    }
}