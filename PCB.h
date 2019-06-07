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

    int id, pc, n_instructions, state, runtime, block_counter, entry, count, iniMem, endMem;
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

// Bob o Constructor

struct PCB *newPCB(int id, char array[], int entry)
{
    struct PCB *PCB = malloc(sizeof(struct PCB));
    PCB->count = 1;
    PCB->id = id;
    PCB->pc = 0;
    PCB->runtime = 0;
    PCB->entry = entry;
    PCB->block_counter = -1;
    strcpy(PCB->inst, array);
    //  um novo process começa sempre a NEW
    PCB->state = NOTNEW;

    int a = 0;
    char i = array[a];

    PCB->n_instructions = count_instructions(array);

    return PCB;
}