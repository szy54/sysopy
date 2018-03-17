#define arrSize 10000

char** allocateTable(int size1, int size2);
void printTable(char **ptr, int size1, int size2);
void deallocate(char **ptr, int size1);
char* allocateBlock(int size);
void deallocateBlock(char **ptr, int blockIndex);
int findBlock(char **ptr, int size1, int size2, int blockIndex);
void allocateStaticBlock(char arr[][arrSize], int size2, int i);
void allocateStaticTable(char arr[][arrSize], int size1, int size2);
void deallocateStaticBlock(char arr[][arrSize], int size2, int i);
void deallocateStaticTable(char arr[][arrSize], int size1, int size2);
int findStaticBlock(char arr[][arrSize], int size1, int size2, int blockIndex);
void printStaticTable(char arr[][arrSize], int size1, int size2);
