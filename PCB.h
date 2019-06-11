// include
#include <stdlib.h>

// define

#define NEW 0
#define WAIT 1
#define RUN 2
#define BLOCK 3
#define EXIT 4
#define NOTNEW 5
#define REALEXIT 6

// PCB Blueprint

struct PCB
{

    int id, pc, n_instructions, state, runtime, block_counter, entry, count, iniMem, endMem, instSize;
    char inst[300];
};

int count_instructions(char arr[])
{
    int i = 0;
    int a = 0;

    while (arr[i] != '\0')
    {
        if (arr[i] == ' ')
            a++;
        i++;
    }
    return a / 3;
}

int string_size(char arr[])
{
    int i = 0;

    while (arr[i] != '\0')
    {
        i++;
    }
    return i;
}

// Bob o Constructor

struct PCB *newPCB(int id, char array[], int entry)
{
    struct PCB *PCB = malloc(sizeof(struct PCB));
    PCB->count = 0;
    PCB->id = id;
    PCB->pc = 0;
    PCB->runtime = 0;
    PCB->entry = entry;
    PCB->block_counter = -1;
    strcpy(PCB->inst, array);
    //  um novo process começa sempre a NEW
    PCB->state = NOTNEW;
    PCB->instSize = string_size(array);

    int a = 0;
    char i = array[a];

    PCB->n_instructions = count_instructions(array);

    return PCB;
}

struct PCB *copyPCB(struct PCB *pcb)
{
    struct PCB *son = malloc(sizeof(struct PCB));

    son->count = pcb->count + 3;
    son->id = pcb->id;
    son->pc = pcb->pc;
    son->runtime = 0;
    son->entry = pcb->entry;
    son->block_counter = -1;
    strcpy(son->inst, pcb->inst);
    son->state = WAIT;
    son->instSize = pcb->instSize;
    son->n_instructions = pcb->n_instructions - 1;

    return son;
}