#include <stdio.h>
#include "listlib.h"

int main()
{
    list lst;
    ListInit(&lst, 16);
    int a = InsertHead(&lst, 4);
    int b = InsertHead(&lst, 5);
    int c = InsertHead(&lst, 6);
    int d = InsertHead(&lst, 7);
    ListPrint(&lst);
    int f = InsertBeforeAddress(&lst, 0, 9);
    //int d = InsertAfterAddress(&lst, c, 13);
    //int e = InsertAfterAddress(&lst, c, 42342);
    //a = InsertHead(&lst, 10);
    //a = InsertHead(&lst, 11);
    InsertAfterAddress(&lst, a, 0xDED);
    ListGraphicDump(&lst, "graph.dot");
    ListPrint(&lst);
    ListDestroy(&lst);
    return 0;
}