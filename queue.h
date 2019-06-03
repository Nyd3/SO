// includes

#include <stdio.h>
#include <stdbool.h>
#include "PCB.h"

// Q Blueprint

struct Queue
{
    int front, rear, size;
    unsigned int capacity; // unsigned int porque tem de ser sempre >=0
    struct PCB *array[];
};

/*Constructor
 Q is initialized with a size of 0 */

struct Queue *newQueue(unsigned int capacity)
{
    struct Queue *queue = malloc(sizeof(struct Queue));
    struct PCB *vazio = malloc(sizeof(struct PCB));

    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array[capacity] = vazio;

    return queue;
}

// Q full?

bool isFull(struct Queue *queue)
{
    return (queue->size == queue->capacity);
}

// Q empty?

bool isEmpty(struct Queue *queue)
{
    return (queue->size == 0);
}

// Function to add an item to the queue.

void enqueue(struct Queue *queue, struct PCB *item)
{
    if (isFull(queue) == 1)
    {
        printf(" no more space\n");
    }
    else
    {
        queue->rear = (queue->rear + 1) % queue->capacity;
        queue->array[queue->rear] = item;
        queue->size += 1;
    }
}

// Function to remove an item from queue.

struct PCB *dequeue(struct Queue *queue)
{
    if (isEmpty(queue) == 1)
    {
        printf("empty\n");
    }
    else
    {
        struct PCB *item = malloc(sizeof(struct Queue));
        item = queue->array[queue->front];
        queue->front = (queue->front + 1) % queue->capacity;
        queue->size -= 1;
        return item;
    }
    return NULL;
}

// Function to get front of queue

struct PCB *front(struct Queue *queue)
{

    return queue->array[queue->front];
}

// Function to get rear of queue

struct PCB *rear(struct Queue *queue)
{

    return queue->array[queue->rear];
}
