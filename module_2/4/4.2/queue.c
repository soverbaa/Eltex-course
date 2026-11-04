#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void queue_init(PriorityQueue *queue)
{
    if (queue != NULL) {
        queue->head = NULL;
        queue->size = 0;
    }
}

void queue_free(PriorityQueue *queue)
{
    if (queue == NULL) return;

    QueueNode *current = queue->head;
    while (current != NULL) {
        QueueNode *next = current->next;
        free(current);
        current = next;
    }
    queue_init(queue);
}

int queue_enqueue(PriorityQueue *queue, const char *message, unsigned int priority)
{
    if (queue == NULL || message == NULL || priority > 255U || message[0] == '\0') {
        return 0;
    }

    QueueNode *node = malloc(sizeof(*node));
    if (node == NULL) return 0;

    snprintf(node->message, sizeof(node->message), "%s", message);
    node->priority = priority;
    node->next = NULL;

    QueueNode **position = &queue->head;
    while (*position != NULL && (*position)->priority >= priority) {
        position = &(*position)->next;
    }
    node->next = *position;
    *position = node;
    queue->size++;
    return 1;
}

static int remove_node(PriorityQueue *queue, QueueNode **position,
                       char *message, size_t message_size,
                       unsigned int *priority)
{
    if (queue == NULL || position == NULL || *position == NULL ||
        message == NULL || message_size == 0) {
        return 0;
    }

    QueueNode *node = *position;
    snprintf(message, message_size, "%s", node->message);
    if (priority != NULL) *priority = node->priority;
    *position = node->next;
    free(node);
    queue->size--;
    return 1;
}

int queue_dequeue_first(PriorityQueue *queue, char *message, size_t message_size,
                        unsigned int *priority)
{
    if (queue == NULL) return 0;
    return remove_node(queue, &queue->head, message, message_size, priority);
}

int queue_dequeue_priority(PriorityQueue *queue, unsigned int wanted_priority,
                           char *message, size_t message_size)
{
    if (queue == NULL || wanted_priority > 255U) return 0;

    QueueNode **position = &queue->head;
    while (*position != NULL && (*position)->priority != wanted_priority) {
        position = &(*position)->next;
    }
    return remove_node(queue, position, message, message_size, NULL);
}

int queue_dequeue_at_least(PriorityQueue *queue, unsigned int min_priority,
                           char *message, size_t message_size,
                           unsigned int *priority)
{
    if (queue == NULL || min_priority > 255U) return 0;

    QueueNode **position = &queue->head;
    while (*position != NULL && (*position)->priority < min_priority) {
        position = &(*position)->next;
    }
    return remove_node(queue, position, message, message_size, priority);
}

void queue_print(const PriorityQueue *queue)
{
    if (queue == NULL || queue->head == NULL) {
        puts("Очередь пуста.");
        return;
    }

    size_t number = 1;
    for (const QueueNode *node = queue->head; node != NULL; node = node->next) {
        printf("%zu. [приоритет %u] %s\n", number++, node->priority, node->message);
    }
}
