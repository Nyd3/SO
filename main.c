// includes
#include <string.h>
#include "queue.h"

// Global variables

#define MEMSIZE 300
#define QUANTUM 4
#define FILE_NAME "input1.txt"

// MEM
//static int MEM[MEMSIZE];

// funçoes transição

void notNew_to_New(struct Queue *notNew, struct Queue *New, int time)
{
    while (!isEmpty(notNew))
    {
        if (front(notNew)->entry == time)
        {
            front(notNew)->state = NEW;
            enqueue(New, dequeue(notNew));
        }
        else
        {
            break;
        }
    }
}

void new_to_wait(struct Queue *New, struct Queue *Wait)
{
    while (!isEmpty(New))
    {
        struct PCB *pcb = dequeue(New);
        pcb->state = WAIT;
        enqueue(Wait, pcb);
    }
}

void wait_to_run(struct Queue *Wait, struct Queue *Run)
{
    if (!isEmpty(Wait) && isEmpty(Run))
    {
        struct PCB *pcb = dequeue(Wait);
        pcb->state = RUN;
        enqueue(Run, pcb);
    }
}

void run_to_block(struct Queue *Run, struct Queue *Block)
{
    if (!isEmpty(Run))
    {
        struct PCB *pcb = dequeue(Run);
        pcb->block_counter = 0;
        pcb->state = BLOCK;
        pcb->pc = 0;
        enqueue(Block, pcb);
    }
}

void block_to_wait(struct Queue *Block, struct Queue *Wait)
{
    if (!isEmpty(Block))
    {
        if (front(Block)->block_counter == 3)
        {
            struct PCB *pcb = dequeue(Block);
            pcb->state = WAIT;
            pcb->block_counter = -1;
            enqueue(Wait, pcb);
        }
    }
}

void run_to_wait(struct Queue *Run, struct Queue *Wait)
{
    if (!isEmpty(Run))
    {
        struct PCB *pcb = dequeue(Run);
        pcb->state = WAIT;
        pcb->pc = 0;
        enqueue(Wait, pcb);
    }
}

void run_to_exit(struct Queue *Run, struct Queue *Exit)
{
    if (!isEmpty(Run))
    {
        struct PCB *pcb = dequeue(Run);
        pcb->state = EXIT;
        enqueue(Exit, pcb);
    }
}

void block_increment(struct Queue *queue)
{
    for (int i = 0; i < queue->size; i++)
    {
        if (queue->array[i] != NULL)
            queue->array[i]->block_counter++;
    }
}

short exit_pcb(struct Queue *exit, short num)
{
    if (!isEmpty(exit))
    {

        dequeue(exit)->state = REALEXIT;
        num--;
    }

    return num;
}

int function(struct Queue *queue)
{

    if (!isEmpty(queue))
    {

        switch (front(queue)->inst[front(queue)->count])
        {
        case '0':

            return 0;
            break;
        case '1':
            if (front(queue)->inst[front(queue)->count + 1] == ' ')
            {
                return 1;
            }
            else if (front(queue)->inst[front(queue)->count + 1] == '0')
            {
                front(queue)->count++;
                return 10;
            }
            else if (front(queue)->inst[front(queue)->count + 1] == '1')
            {
                front(queue)->count++;
                return 11;
            }

        case '2':

            return 2;
            break;
        case '3':

            return 3;
            break;
        case '4':

            return 4;
            break;
        case '5':

            return 5;
            break;
        case '6':

            return 6;
            break;
        case '7':

            return 7;
            break;
        case '8':

            front(queue)->inst[front(queue)->block_counter = 0];
            return 8;
            break;
        case '9':

            return 9;
            break;

        default:
            break;
        }
    }
}

bool PCB_complete(struct Queue *queue)
{
    return !isEmpty(queue);
}

void print_state(struct PCB *array[], int total, FILE *file)
{
    for (int i = 0; i <= total; i++)
    {
        if (array[i] != NULL)
        {
            int state = array[i]->state;
            switch (state)
            {
            case 0:
                fprintf(file, "|  NEW  ");
                break;
            case 1:
                fprintf(file, "|  WAIT ");
                break;
            case 2:
                fprintf(file, "|  RUN  ");
                break;
            case 3:
                fprintf(file, "| BLOCK ");
                break;
            case 4:
                fprintf(file, "|  EXIT ");
                break;
            default:
                fprintf(file, "|       ");
                break;
            }
        }
    }
    fprintf(file, "\n");
}

int main()
{
    int entry;
    int c = 0;
    char line[300];
    short i = 1;
    short num = 0;
    short max_entry = 0;

    struct Queue *New = newQueue(30);
    struct Queue *Wait = newQueue(30);
    struct Queue *Run = newQueue(1);
    struct Queue *Block = newQueue(30);
    struct Queue *Exit = newQueue(5);
    struct Queue *notNew = newQueue(30);
    struct PCB *list[30] = {NULL};

    FILE *f = fopen("scheduler_simples.out", "w+");

    while (scanf("%d", &entry) != EOF)
    {
        scanf("%[^\n]c", line);
        if (line != NULL)
        {
            struct PCB *pcb = newPCB(i, line, entry);
            enqueue(notNew, pcb);
            list[i] = pcb;
        }
        i++;
        num++;
    }

    // ciclo
    int e = 0;
    while (num != 0)
    {
        num = exit_pcb(Exit, num);

        if (!isEmpty(Run))
        {
            if (front(Run)->n_instructions == 0 || e == 11)
            {
                run_to_exit(Run, Exit);
                e = 0;
            }
        }
        if (!isEmpty(Run))
        {
            if (front(Run)->block_counter == 0)
            {
                run_to_block(Run, Block);
            }
        }

        if (!isEmpty(Run))
        {
            if ((front(Run)->n_instructions > 0) && (front(Run)->pc == QUANTUM))
            {
                run_to_wait(Run, Wait);
            }
        }

        if (isEmpty(Run))
        {
            wait_to_run(Wait, Run);
        }

        block_to_wait(Block, Wait);

        if (!isEmpty(Run))
        {
            e = function(Run);
            front(Run)->count += 6;
            front(Run)->n_instructions--;
            front(Run)->pc++;
        }

        new_to_wait(New, Wait);
        notNew_to_New(notNew, New, c);

        if (!isEmpty(Block))
        {
            block_increment(Block);
        }

        fprintf(f, "%.2d ", c);
        print_state(list, i, f);
        c++;
    }
    fclose(f);
    return 0;
}
