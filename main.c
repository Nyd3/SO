// includes
#include <string.h>
#include "queue.h"

// Global variables

#define MEMSIZE 300
#define QUANTUM 4
#define EMPTY -1
#define FILE_NAME "input1.txt"

bool next_best = false; //false->next, true->best
int disk;
int nextFit = -1;
int listaindex = 1;
int numPc = 0;

// MEM
static int MEM[MEMSIZE];
// funçoes transição

void fillMem()
{
    for (int i = 0; i < MEMSIZE; i++)
        MEM[i] = EMPTY;
}

void print_MEM()
{
    for (int i = 0; i < MEMSIZE; i++)
    {
        if (MEM[i] != EMPTY)
            printf("%d", MEM[i]);
    }
}

void setMem(struct PCB *pcb)
{
    int a = pcb->iniMem;
    for (int i = 0; i < 10; i++)
    {
        MEM[a] = 0;
        a++;
    }
    for (int i = 0; i <= pcb->instSize; i++)
    {
        switch (pcb->inst[i])
        {
        case '0':
            MEM[a] = 0;
            a++;
            break;

        case '1':
            if (pcb->inst[i + 1] == ' ')
            {
                MEM[a] = 1;
                a++;
                break;
            }
            else if (pcb->inst[i + 1] == '0')
            {
                MEM[a] = 10;
                a++;
                i++;
                break;
            }
            else if (pcb->inst[i + 1] == '1')
            {
                MEM[a] = 11;
                a++;
                i++;
                break;
            }

        case '2':
            MEM[a] = 2;
            a++;
            break;
        case '3':
            MEM[a] = 3;
            a++;
            break;

        case '4':
            MEM[a] = 4;
            a++;
            break;

        case '5':
            MEM[a] = 5;
            a++;
            break;

        case '6':
            MEM[a] = 6;
            a++;
            break;

        case '7':
            MEM[a] = 7;
            a++;
            break;

        case '8':
            MEM[a] = 8;
            a++;
            break;

        case '9':
            MEM[a] = 9;
            a++;
            break;

        default:
            break;
        }
    }
}

bool toMem(struct PCB *pcb)
{
    int b = 0;
    int a = 0;
    int space[MEMSIZE];
    for (int i = 0; i < MEMSIZE; i++)
    {
        space[i] = 0;
    }
    for (int i = 0; i <= MEMSIZE; i++)
    {
        if (MEM[i] == EMPTY)
        {
            a++;
            //printf("\n %d", a);
        }
        else if (a != 0 && MEM[i] != EMPTY)
        {
            space[b] = a;
            space[b + 1] = i;
            b++;
            a = 0;
        }
    }
    //printf("HEllo, %d, %d\n", space[0], space[1]);

    if (next_best)
    {
        //best
        int min = 400;
        int minini;
        for (int i = 0; i < MEMSIZE; i = i + 2)
        {
            if (space[i] > pcb->n_instructions * 3 + 10 && space[i] < min)
            {
                min = space[i];
                //printf("min=%d\n",min);
                minini = space[i + 1];
                //printf("minini=%d\n",minini);
            }
        }
        if (min != 400)
        {
            pcb->iniMem = minini - min;
            //printf("%d\n", pcb->iniMem);
            pcb->endMem = pcb->iniMem + pcb->n_instructions * 3 + 10;
            //printf("%d\n", pcb->endMem);
            setMem(pcb);
            //print_MEM();
            printf("\n");
            return true;
        }
        printf("\n");
        return false;
    }
    else
    {
        //next
        bool done = false;
        for (int ii = 0; ii < 2; ii++)
        {
            for (int i = 0; i < MEMSIZE; i++)
            {
                if (space[i + 1] > nextFit && space[i] > pcb->n_instructions * 3 + 10)
                {
                    pcb->iniMem = space[i + 1] - space[i];
                    pcb->endMem = pcb->iniMem + pcb->n_instructions * 3 + 10;
                    setMem(pcb);
                    //print_MEM();
                    nextFit = pcb->endMem;
                    done = true;
                    //printf("%d", nextFit);
                    printf("\n");
                    return true;
                }
            }
            if (done)
            {
                break;
            }
            nextFit = -1;
        }
    }
    printf("\n");
    return false;
}

void leave_X(struct PCB *pcb)
{
    for (int i = pcb->iniMem; i < pcb->endMem + 3; i++)
    {
        MEM[i] = EMPTY;
    }
    printf("exit");
}

void set_X(int x1, int x2, struct PCB *pcb)
{
    int beg = pcb->iniMem;
    MEM[beg + x1 - 1] = MEM[beg + x2 - 1];
}

void set_N(int x, int n, struct PCB *pcb)
{
    MEM[pcb->iniMem + x - 1] = n;
    printf("setN\n");
}

void inc_X(int x, struct PCB *pcb)
{
    int beg = pcb->iniMem;
    MEM[beg + x - 1] = MEM[beg + x - 1] + 1;
}

void dec_X(int x, struct PCB *pcb)
{
    int beg = pcb->iniMem;
    MEM[beg + x - 1] = MEM[beg + x - 1] - 1;
    printf("DEC\n");
}

void back_N(int n, struct PCB *pcb)
{
    if (count_instructions(pcb->inst) >= pcb->n_instructions + n)
    {
        pcb->pc -= n;
        pcb->count -= 3 * n;
        pcb->n_instructions += n;
    }
    else
    {
        leave_X(pcb);
        printf("MEMORY ACCESS VIOLATION\n");
    }
    printf("backwards\n");
}

void forward_N(int n, struct PCB *pcb)
{
    if ((pcb->pc + n) <= count_instructions(pcb->inst))
    {
        pcb->pc += (n-1);
        pcb->count += 3 * (n-1);
        pcb->n_instructions -= (n-1);
    }
    else
    {
        leave_X(pcb);
        printf("MEMORY ACCESS VIOLATION\n");
    }
    printf("forward\n");
}

void if_X_N(int x, int n, struct PCB *pcb)
{
    if (MEM[pcb->iniMem + x - 1] == 0)
        forward_N(n, pcb);

    printf("if\n");
}

void fork_X(int x, struct PCB *pcb, struct Queue *queue, struct PCB *list[])
{
    struct PCB *son = copyPCB(pcb);
    son->id = (68 + 1) * x;
    bool created = toMem(son);

    if (!created)
        pcb->id = -1;
    else
    {
        enqueue(queue, son);
        list[listaindex] = son;
        listaindex++;
        numPc++;

        for (int i = 0; i < 10; i++)
        {
            MEM[son->iniMem + i] = MEM[pcb->iniMem + i];
        }

        MEM[pcb->iniMem + x - 1] = son->id;
        MEM[son->iniMem + x - 1] = 0;
        printf("fork");
    }
}

void disk_save_X(int x)
{
    disk = x;
    printf("save");
}

void disk_load_X(int x, struct PCB *pcb)
{
    MEM[pcb->iniMem + x - 1] = disk;
}

void print_X(int x, struct PCB *pcb)
{
    printf("x = %d\n", MEM[pcb->iniMem + x - 1]);
}

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
        toMem(pcb);
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
        pcb->runtime = 0;
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
        pcb->runtime = 0;
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

short exit_pcb(struct Queue *exit)
{
    if (!isEmpty(exit))
    {

        dequeue(exit)->state = REALEXIT;
        numPc--;
    }

    return numPc;
}

int run_core(struct Queue *queue, struct Queue *wait, struct PCB *list[])
{
    int begi = front(queue)->iniMem + front(queue)->count + 10;
    struct PCB *pcb = front(queue);

    if (!isEmpty(queue))
    {

        switch (MEM[begi])
        {
        case 0:
            set_X(MEM[begi + 1], MEM[begi + 2], pcb);
            break;
        case 1:
            set_N(MEM[begi + 1], MEM[begi + 2], pcb);
            break;
        case 2:
            inc_X(MEM[begi + 1], pcb);
            break;
        case 3:
            dec_X(MEM[begi + 1], pcb);
            break;
        case 4:
            back_N(MEM[begi + 1], pcb);
            pcb->count -= 3;
            pcb->n_instructions++;
            pcb->pc--;
            break;
        case 5:
            forward_N(MEM[begi + 1], pcb);
            pcb->count -= 3;
            pcb->n_instructions++;
            pcb->pc--;
            break;
        case 6:
            if_X_N(MEM[begi + 1], MEM[begi + 2], pcb);
            break;
        case 7:
            fork_X(MEM[begi + 1], pcb, wait, list);
            break;
        case 8:
            front(queue)->inst[front(queue)->block_counter = -1];
            disk_save_X(MEM[begi + 1]);
            break;
        case 9:
            front(queue)->inst[front(queue)->block_counter = -1];
            disk_load_X(MEM[begi + 1], pcb);
            break;
        case 10:
            print_X(MEM[begi + 1], pcb);
            break;
        case 11:
            leave_X(pcb);
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
    short max_entry = 0;

    struct Queue *New = newQueue(30);
    struct Queue *Wait = newQueue(30);
    struct Queue *Run = newQueue(1);
    struct Queue *Block = newQueue(30);
    struct Queue *Exit = newQueue(5);
    struct Queue *notNew = newQueue(30);
    struct PCB *list[30] = {NULL};
    fillMem();

    FILE *f = fopen("scheduler_simples.out", "w+");

    while (scanf("%d", &entry) != EOF)
    {
        scanf("%[^\n]c", line);
        if (line != NULL)
        {
            struct PCB *pcb = newPCB(listaindex, line, entry);
            enqueue(notNew, pcb);
            list[listaindex] = pcb;
        }
        listaindex++;
        numPc++;
    }

    // ciclo
    while (numPc != 0)
    {
        exit_pcb(Exit);

        if (!isEmpty(Run))
        {
            if (front(Run)->n_instructions == 0)
            {
                run_to_exit(Run, Exit);
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
            if ((front(Run)->n_instructions > 0) && (front(Run)->runtime == QUANTUM))
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

            run_core(Run, Wait, list);
            front(Run)->count += 3;
            front(Run)->n_instructions--;
            front(Run)->runtime++;
            front(Run)->pc++;
        }

        new_to_wait(New, Wait);
        notNew_to_New(notNew, New, c);

        if (!isEmpty(Block))
        {
            block_increment(Block);
        }

        fprintf(f, "%.2d ", c);
        print_state(list, listaindex, f);
        c++;
        //print_MEM();
        printf("\n");
    }
    fclose(f);
    return 0;
}
