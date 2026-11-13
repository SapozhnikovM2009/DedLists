#include <stdio.h>
#include <stdlib.h>

#include "listlib.h"

// TODO: enum for errors DONE

// TODO: вставка в хвост, голову и после элемента DONE
// TODO: graphviz DONE
// TODO: двухсвязный, вставка перед элементом DONE
// TODO : переделать dump, чтобы выводило в каком порядке в памяти (адресов), а не в списке, сделать выравнивание
// TODO: удаление элемента
// TODO: список свободных

int ListInit(list* lst, int capacity)
{
    if (capacity < 0)
    {
        fprintf(stderr, "Can't init list with capacity < 0\n");
        return -1;
    }
    lst->data = calloc(capacity, sizeof(int));
    if (lst->data == NULL)
    {
        perror("calloc");
        return ERROR_OUTPUT;
    }

    lst->next = calloc(capacity, sizeof(int));
    if (lst->next == NULL)
    {
        perror("calloc");
        return ERROR_OUTPUT;
    }
    lst->prev = calloc(capacity, sizeof(int));
    if (lst->prev == NULL)
    {
        perror("calloc");
        return ERROR_OUTPUT;
    }

    lst->size = 0;
    lst->capacity = capacity;
    lst->data[0] = POISON;
    lst->next[0] = 0;
    lst->prev[0] = 0;
    lst->tail = 0;
    lst->head = 0;
    // NOTE: почитать про memset
    for (int i = 1;i < lst->capacity;i++)
    {
        lst->next[i] = -1;
        lst->prev[i] = -1;
    }
    return NORMAL_OUTPUT;
}

void ListPrint(list* lst)
{
    for (int i = 0;i < 50;i++) printf("-");
    printf("\n");

    printf("size : %d\tcapacity : %d\n", lst->size, lst->capacity);
    printf("tail : %d\thead : %d\n", lst->tail, lst->head);

    printf("#\tdata\t\tnext\t\tprev\n");

    printf("0\t%d\t%d\t\t%d\n", lst->data[0], lst->next[0], lst->prev[0]);

    
    
    for (int i = 1;i < lst->capacity;i++)
    {
        printf("%d\t%d\t\t%d\t\t%d\n", i, lst->data[i], lst->next[i], lst->prev[i]);
    }
    for (int i = 0;i < 50;i++) printf(".");
    printf("\n");

    int ind = lst->head;
    while (ind != 0)
    {
        printf("%d ", lst->data[ind]);
        ind = lst->next[ind];
    }
    printf("\n");
    for (int i = 0;i < 50;i++) printf("-");
    printf("\n");
}

void ListDestroy(list* lst)
{
    free(lst->next);
    free(lst->prev);
    free(lst->data);
}

int FindFreeCell(list* lst)
{
    for (int i = 1; i < lst->capacity; i++)
    {
        if (lst->next[i] == -1)
        {
            return i;
        }
    }
}

int InsertTail(list* lst, int value)
{
    int free_cell = FindFreeCell(lst);
    if (lst->size == 0)
    {
        lst->next[free_cell] = 0;
        lst->prev[free_cell] = 0;
        lst->prev[0] = free_cell;
        lst->tail = free_cell;
        lst->head = free_cell;
        lst->data[free_cell] = value;
        lst->size++;
    }
    else
    {
        lst->next[free_cell] = 0;
        lst->prev[free_cell] = lst->tail;
        lst->prev[0] = free_cell;
        lst->next[lst->tail] = free_cell;
        lst->tail = free_cell;
        lst->data[free_cell] = value;
        lst->size++;
    }
    return free_cell;
}

int InsertHead(list* lst, int value)
{
    int free_cell = FindFreeCell(lst);
    if (lst->size == 0)
    {
        lst->next[free_cell] = 0;
        lst->prev[free_cell] = 0;
        lst->prev[0] = free_cell;
        lst->head = free_cell;
        lst->tail = free_cell;
        lst->data[free_cell] = value;
        lst->size++;
    }
    else
    {
        lst->next[free_cell] = lst->head;
        lst->prev[free_cell] = 0;
        lst->prev[lst->head] = free_cell;
        lst->head = free_cell;
        lst->data[free_cell] = value;
        lst->size++;
    }
    return free_cell;
}

int InsertAfterAddress(list* lst, int address, int value)
{
    int free_cell = FindFreeCell(lst);
    lst->next[free_cell] = lst->next[address];
    lst->prev[free_cell] = address;
    lst->prev[lst->next[address]] = free_cell;
    lst->next[address] = free_cell;
    lst->data[free_cell] = value;
    lst->size++;
    return free_cell;
}

int InsertBeforeAddress(list* lst, int address, int value)
{
    if (address == 0)
    {
        return InsertHead(lst, value);
    }
    else
    {
        int free_cell = FindFreeCell(lst);
        lst->next[lst->prev[address]] = free_cell;
        lst->prev[free_cell] = lst->prev[address];
        lst->prev[address] = free_cell;
        lst->next[free_cell] = address;
        lst->data[free_cell] = value;
        lst->size++;
        return free_cell;
    }
}

void ListGraphicDump(list* lst, char* file_name)
{
    FILE* out_file = fopen(file_name, "w+");
    fprintf(out_file, "digraph structs\n");
    fprintf(out_file, "{\n");

    fprintf(out_file, "\trankdir = LR;\n");

    int ind = lst->head;
    int num = 0;
    while (ind != 0 || num != lst->size + 1)
    {
        if (ind != lst->tail && ind != lst->head)
        {
            fprintf(out_file, "\t%d[shape = record, fillcolor = \"lightgreen\", color = \"green\", style = \"filled, rounded\", ", num);
            fprintf(out_file, "label = \" value : %d | {address : %d} | {next : %d | prev : %d}\"];\n", lst->data[ind], ind, lst->next[ind], lst->prev[ind]);
        }
        if (ind == lst->tail)
        {
            fprintf(out_file, "\t%d[shape = record, fillcolor = \"pink\", color = \"red\", style = \"filled, rounded\", ", num);
            fprintf(out_file, "label = \" value : %d | {address : %d} | {next : %d | prev : %d}\"];\n", lst->data[ind], ind, lst->next[ind], lst->prev[ind]);
        }
        if (ind == lst->head)
        {
            fprintf(out_file, "\t%d[shape = record, fillcolor = \"lightblue\", color = \"blue\", style = \"filled, rounded\", ", num);
            fprintf(out_file, "label = \" value : %d | {address : %d} | {next : %d | prev : %d}\"];\n", lst->data[ind], ind, lst->next[ind], lst->prev[ind]);
        }
        if (num < lst->size) fprintf(out_file, "\t%d->%d;\n\n", num, num + 1);
        if (num < lst->size - 1) fprintf(out_file, "\t%d->%d;\n\n", num + 1, num);
        if (num == lst->size - 1)
        { 
            fprintf(out_file, "\t%d->%d;\n\n", num + 1, 0);
            fprintf(out_file, "\t%d->%d;\n\n", 0, num + 1);
        }
        num++;
        //fprintf(out_file, "%d ", lst->data[ind]);
        ind = lst->next[ind];
    }

    fprintf(out_file, "}\n");
    fclose(out_file);
}
/*
{
		edge[color=white];
		head->tail;
	}
	{rank = same; 0; head;}
	{rank = same; 5; tail;}
/*
digraph structs {
  rankdir = LR;
  1[shape=record,fillcolor="lightgreen",color="green",style="filled, rounded",label=" value 5 | {next 3 | prev 4}" ];
  2[shape=record,style="rounded",label=" value 8 | {next 1 | prev 2}" ];
  1->2;



}*/