#ifndef QUEUE_H
#define QUEUE_H

#include <stddef.h>

#define MESSAGE_TEXT 100

typedef struct QueueNode {
    char message[MESSAGE_TEXT];
    unsigned int priority;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *head;
    size_t size;
} PriorityQueue;

void queue_init(PriorityQueue *queue);
void queue_free(PriorityQueue *queue);
int queue_enqueue(PriorityQueue *queue, const char *message, unsigned int priority);
int queue_dequeue_first(PriorityQueue *queue, char *message, size_t message_size,
                        unsigned int *priority);
int queue_dequeue_priority(PriorityQueue *queue, unsigned int wanted_priority,
                           char *message, size_t message_size);
int queue_dequeue_at_least(PriorityQueue *queue, unsigned int min_priority,
                           char *message, size_t message_size,
                           unsigned int *priority);
void queue_print(const PriorityQueue *queue);

#endif
