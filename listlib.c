#include <stdio.h>
#include <stdlib.h>

#include "listlib.h"

// TODO: удаление элемента DONE
// NOTE*: проверка удаления свободного элемента

int ListInit(list* lst, int capacity)
{
    if (capacity < 0)
    {
        fprintf(stderr, "Can't init list with capacity < 0\n");
        return ERROR_OUTPUT;
    }
    //capacity += 1;
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
        lst->data[i] = POISON;
        if (i != lst->capacity - 1) lst->next[i] = i + 1;
        lst->prev[i] = i - 1;
    }
    lst->next_free = 1;
    lst->next[lst->capacity - 1] = NULL_ADDRESS;

    #ifndef NDEBUG
        ListVerify(lst);
    #endif

    return NORMAL_OUTPUT;
}

void ListPrint(list* lst)
{
    for (int i = 0;i < 50;i++) printf("-");
    printf("\n");

    printf("size : %d\tcapacity : %d\n", lst->size, lst->capacity);
    printf("tail : %d\thead : %d\n", lst->tail, lst->head);
    printf("next_free : %d\n", lst->next_free);

    printf("#\tdata\t\tnext\t\tprev\n");

    printf("0\t%d\t%d\t\t%d\n", lst->data[0], lst->next[0], lst->prev[0]);

    
    
    for (int i = 1;i < lst->capacity;i++)
    {
        if (lst->data[i] != POISON) printf("%d\t%d\t\t%d\t\t%d\n", i, lst->data[i], lst->next[i], lst->prev[i]);
        if (lst->data[i] == POISON) printf("%d\t%d\t%d\t\t%d\n", i, lst->data[i], lst->next[i], lst->prev[i]);
    }

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
    #ifndef NDEBUG
        ListVerify(lst);
    #endif

    // TODO*: realloc
    if (lst->next_free != NULL_ADDRESS)
    {
        int save_next_free = lst->next_free;
        lst->next_free = lst->next[lst->next_free];
        return save_next_free;
    }

    #ifndef NDEBUG
        ListVerify(lst);
    #endif

    return NULL_ADDRESS;
}

int InsertTail(list* lst, int value)
{
    #ifndef NDEBUG
        ListVerify(lst);
    #endif

    int free_cell = FindFreeCell(lst);
    if (free_cell == NULL_ADDRESS)
    {
        fprintf(stderr, "InsertHead error: can't find free cell\n");
        return NULL_ADDRESS;
    }
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

    #ifndef NDEBUG
        ListVerify(lst);
    #endif

    return free_cell;
}

int InsertHead(list* lst, int value)
{
    #ifndef NDEBUG
        ListVerify(lst);
    #endif

    int free_cell = FindFreeCell(lst);
    if (free_cell == NULL_ADDRESS)
    {
        fprintf(stderr, "InsertHead error: can't find free cell\n");
        return NULL_ADDRESS;
    }
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

    #ifndef NDEBUG
        ListVerify(lst);
    #endif

    return free_cell;
}

int InsertAfterAddress(list* lst, int address, int value)
{
    #ifndef NDEBUG
        ListVerify(lst);
    #endif

    if (address == lst->tail)
    {
        return InsertTail(lst, value);
    }
    else
    {
        int free_cell = FindFreeCell(lst);
        if (free_cell == NULL_ADDRESS)
        {
            fprintf(stderr, "InsertHead error: can't find free cell\n");
            return NULL_ADDRESS;
        }
        lst->next[free_cell] = lst->next[address];
        lst->prev[free_cell] = address;
        lst->prev[lst->next[address]] = free_cell;
        lst->next[address] = free_cell;
        lst->data[free_cell] = value;
        lst->size++;
        return free_cell;
    }

    #ifndef NDEBUG
        ListVerify(lst);
    #endif
}

int InsertBeforeAddress(list* lst, int address, int value)
{
    #ifndef NDEBUG
        ListVerify(lst);
    #endif

    if (address == 0)
    {
        return InsertHead(lst, value);
    }
    else
    {
        int free_cell = FindFreeCell(lst);
        if (free_cell == NULL_ADDRESS)
        {
            fprintf(stderr, "InsertHead error: can't find free cell\n");
            return NULL_ADDRESS;
        }
        lst->next[lst->prev[address]] = free_cell;
        lst->prev[free_cell] = lst->prev[address];
        lst->prev[address] = free_cell;
        lst->next[free_cell] = address;
        lst->data[free_cell] = value;
        lst->size++;
        return free_cell;
    }

    #ifndef NDEBUG
        ListVerify(lst);
    #endif
}

void RemoveListElem(list* lst, int address)
{
    #ifndef NDEBUG
        ListVerify(lst);
    #endif

    if (address != lst->head)
    {
        lst->next[lst->prev[address]] = lst->next[address];
    }
    if (address == lst->head)
    {
        lst->head = lst->next[lst->head];
    }
    if (address == lst->tail)
    {
        lst->tail = lst->prev[lst->tail];
    }
    
    lst->prev[lst->next[address]] = lst->prev[address];
    lst->next[address] = lst->next_free;
    //
    lst->prev[address] = lst->prev[lst->next_free];
    lst->data[address] = POISON;
    lst->prev[lst->next_free] = address;
    lst->next_free = address;
    lst->size -= 1;

    #ifndef NDEBUG
        ListVerify(lst);
    #endif
}

void ListGraphicDump(list* lst, char* file_name)
{
    FILE* out_file = fopen(file_name, "w");
    if (out_file == NULL)
    {
        fprintf(stderr, "Can't open file %s\n", file_name);
        return;
    }
    fprintf(out_file, "digraph structs\n");
    fprintf(out_file, "{\n");

    fprintf(out_file, "\trankdir = LR;\n\tsplines = ortho;\n");

    int ind = 0;
    while (ind < lst->capacity)
    {
        if (ind != lst->tail && ind != lst->head && ind != 0 && lst->data[ind] != POISON)
        {
            fprintf(out_file, "\t%d[shape = record, fillcolor = \"lightgreen\", color = \"green\", style = \"filled, rounded\", ", ind);
            fprintf(out_file, "label = \" value : %d | {address : %d} | {next : %d | prev : %d}\"];\n", lst->data[ind], ind, lst->next[ind], lst->prev[ind]);
        }
        if (ind == lst->tail)
        {
            fprintf(out_file, "\t%d[shape = record, fillcolor = \"pink\", color = \"red\", style = \"filled, rounded\", ", ind);
            fprintf(out_file, "label = \" value : %d | {address : %d} | {next : %d | prev : %d}\"];\n", lst->data[ind], ind, lst->next[ind], lst->prev[ind]);
        }
        if (ind == lst->head)
        {
            fprintf(out_file, "\t%d[shape = record, fillcolor = \"lightblue\", color = \"blue\", style = \"filled, rounded\", ", ind);
            fprintf(out_file, "label = \" value : %d | {address : %d} | {next : %d | prev : %d}\"];\n", lst->data[ind], ind, lst->next[ind], lst->prev[ind]);
        }
        if (ind == 0)
        {
            fprintf(out_file, "\t%d[shape = record, fillcolor = \"#ffbdf3\", color = \"purple\", style = \"filled, rounded\", ", ind);
            fprintf(out_file, "label = \" value : %d | {address : %d} | {next : %d | prev : %d}\"];\n", lst->data[ind], ind, lst->next[ind], lst->prev[ind]);
        }
        if (ind != 0 && lst->data[ind] == POISON)
        {
            fprintf(out_file, "\t%d[shape = record, fillcolor = \"#f2ae87\", color = \"brown\", style = \"filled, rounded\", ", ind);
            fprintf(out_file, "label = \" value : %d | {address : %d} | {next : %d | prev : %d}\"];\n", lst->data[ind], ind, lst->next[ind], lst->prev[ind]);
        }

        if (ind < lst->capacity - 1) fprintf(out_file, "\t%d->%d [color = transparent; constraint=true; weight=100];\n\n", ind, ind + 1);
        ind++;
    }
    
    ind = 0;
    while (ind < lst->capacity)
    {
        fprintf(out_file, "\t%d->%d[weight = 1; constraint=false];\n", ind, lst->next[ind]);
        fprintf(out_file, "\t%d->%d[style = dashed; weight=1, constraint=false];\n", ind, lst->prev[ind]);
        ind++;
    }

    fprintf(out_file, "}\n");
    fclose(out_file);
}

void ListVerify(list* lst)
{
    if (lst->size < 0 || lst->capacity < 0 || lst->size > lst->capacity)
    {
        fprintf(stderr, "Wrong size or capacity size: %d, capacity: %d\n", lst->size, lst->capacity);
        ListPrint(lst);
        return;
    }
    if (lst->data[0] != POISON)
    {
        fprintf(stderr, "NULL elem of list is wrong! His value is %d\n", lst->data[0]);
        ListPrint(lst);
        return;
    }
    if (lst->next[0] != 0)
    {
        fprintf(stderr, "Next of NULL elem is %d, not 0\n", lst->next[0]);
        ListPrint(lst);
        return;
    }
    if (lst->size == 0)
    {
        if (lst->head !=0 || lst->tail != 0)
        {
            fprintf(stderr, "Empty list error: head: %d or tail: %d isn't 0\n", lst->head, lst->tail);
            ListPrint(lst);
            return;
        }
        return;
    }
    if (lst->head > lst->capacity || lst->head <= 0)
    {
        fprintf(stderr, "List head is %d (> capacity or <= 0)\n", lst->head);
        ListPrint(lst);
        return;
    }
    if (lst->tail > lst->capacity || lst->tail <= 0)
    {
        fprintf(stderr, "List tail is %d (> capacity or <= 0)\n", lst->tail);
        ListPrint(lst);
        return;
    }
    if (lst->prev[lst->head] != 0)
    {
        fprintf(stderr, "Prev of head isn't 0, is %d\n", lst->prev[lst->head]);
        ListPrint(lst);
        return;
    }
    if (lst->next[lst->tail] != 0)
    {
        fprintf(stderr, "Next of tail isn't 0, is %d\n", lst->next[lst->tail]);
        ListPrint(lst);
        return;
    }
    
    int ind = lst->head;
    for (int i = 0; i < lst->size; i++)
    {
        if (ind == 0)
        {
            fprintf(stderr, "You went from head to 0 elem for %d < %d (size) steps\n", i, lst->size);
            ListPrint(lst);
            return;
        }
        ind = lst->next[ind];
        if (ind > lst->capacity || ind < 0)
        {
            fprintf(stderr, "Error ind in next: %d, size: %d\n", ind, lst->size);
            ListPrint(lst);
            return;
        }   
    }
    if (ind != 0)
    {
        fprintf(stderr, "You can't go from head to 0 for %d (size) steps, you went to %d\n", lst->size, ind);
        ListPrint(lst);
        return;
    }

    ind = lst->tail;
    for (int i = 0; i < lst->size; i++)
    {
        if (ind == 0)
        {
            fprintf(stderr, "You went from tail to 0 for %d < %d steps\n", i, lst->size);
            ListPrint(lst);
            return;
        }
        ind = lst->prev[ind];
        if (ind > lst->capacity || ind < 0)
        {
            fprintf(stderr, "Error ind in prev: %d, size: %d\n", ind, lst->size);
            ListPrint(lst);
            return;
        }
    }
    if (ind != 0)
    {
        fprintf(stderr, "You can't go from tail to 0 for %d (size) steps, you went to %d\n", lst->size, ind);
        ListPrint(lst);
        return;
    }

    ind = lst->next_free;
    for (int i = 0; i < lst->capacity - lst->size - 1; i++)
    {
        if (ind == 0)
        {
            fprintf(stderr, "You went from next_free to 0 for %d < %d steps (free elem amount)\n", i, lst->capacity - lst->size - 1);
            ListPrint(lst);
            return;
        }
        ind = lst->next[ind];
        if (ind > lst->capacity || ind < 0)
        {
            fprintf(stderr, "Error ind in next: %d, size: %d (free list)\n", ind, lst->size);
            ListPrint(lst);
            return;
        }
    }
    if (ind != 0)
    {
        fprintf(stderr, "You can't go from next_free to 0 for %d (free elem amount) steps, you went to %d\n", lst->capacity - lst->size - 1, ind);
        ListPrint(lst);
        return;
    }
}

void RemoveListHead(list* lst)
{
    #ifndef NDEBUG
        ListVerify(lst);
    #endif

    int address = lst->head;
    if (address != lst->head)
    {
        lst->next[lst->prev[address]] = lst->next[address];
    }
    if (address == lst->head)
    {
        lst->head = lst->next[lst->head];
    }
    if (address == lst->tail)
    {
        lst->tail = lst->prev[lst->tail];
    }
    
    lst->prev[lst->next[address]] = lst->prev[address];
    lst->next[address] = lst->next_free;
    //
    lst->prev[address] = lst->prev[lst->next_free];
    lst->data[address] = POISON;
    lst->prev[lst->next_free] = address;
    lst->next_free = address;
    lst->size -= 1;

    #ifndef NDEBUG
        ListVerify(lst);
    #endif
}

void RemoveListTail(list* lst)
{
    #ifndef NDEBUG
        ListVerify(lst);
    #endif

    int address = lst->tail;
    if (address != lst->head)
    {
        lst->next[lst->prev[address]] = lst->next[address];
    }
    if (address == lst->head)
    {
        lst->head = lst->next[lst->head];
    }
    if (address == lst->tail)
    {
        lst->tail = lst->prev[lst->tail];
    }
    
    lst->prev[lst->next[address]] = lst->prev[address];
    lst->next[address] = lst->next_free;
    //
    lst->prev[address] = lst->prev[lst->next_free];
    lst->data[address] = POISON;
    lst->prev[lst->next_free] = address;
    lst->next_free = address;
    lst->size -= 1;

    #ifndef NDEBUG
        ListVerify(lst);
    #endif
}
