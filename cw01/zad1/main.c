#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "alloc.h"
#define SIZE 12
#define SIZE2 13

char arr[10000][10000];

int main(int argc, char *argv[]){
     srand(time(NULL));

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

    return 0;
}