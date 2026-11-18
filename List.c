#include <stdio.h>
#include "listlib.h"

int main()
{
    list lst;
    ListInit(&lst, 16);
    ListGraphicDump(&lst, "graph.dot");
    int a = InsertHead(&lst, 5);
    a = InsertHead(&lst, 5);
    a = InsertHead(&lst, 5);
    a = InsertHead(&lst, 5);
    a = InsertHead(&lst, 5);
    a = InsertHead(&lst, 5);
    a = InsertHead(&lst, 5);
    a = InsertHead(&lst, 5);
    a = InsertHead(&lst, 5);
    //ListGraphicDump(&lst, "graph.dot");
    ListPrint(&lst);
    RemoveListHead(&lst);
    ListGraphicDump(&lst, "graph.dot");
    ListVerify(&lst);
    ListDestroy(&lst);
    return 0;
}