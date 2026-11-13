#ifndef LISLIB_H
#define LISTLIB_H

#define POISON -24012009

enum
{
    ERROR_OUTPUT = -1,
    NULL_ADDRESS = 0,
    NORMAL_OUTPUT = 1
};

typedef struct
{
    int* data;
    int* next;
    int* prev;
    int tail;
    int head;
    int capacity;
    int size;
}list;

int ListInit(list* lst, int capacity);
void ListPrint(list* lst);
void ListDestroy(list* lst);
int FindFreeCell(list* lst);
int InsertTail(list* lst, int value);
int InsertHead(list* lst, int value);
int InsertAfterAddress(list* lst, int address, int value);
void ListGraphicDump(list* lst, char* file_name);
int InsertBeforeAddress(list* lst, int address, int value);

#endif