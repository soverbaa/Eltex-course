#include "queue.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    PriorityQueue queue;
    char message[MESSAGE_TEXT];
    unsigned int priority;
    queue_init(&queue);

    assert(queue_enqueue(&queue, "low", 20));
    assert(queue_enqueue(&queue, "high-first", 100));
    assert(queue_enqueue(&queue, "middle", 50));
    assert(queue_enqueue(&queue, "high-second", 100));
    assert(!queue_enqueue(&queue, "invalid", 256));
    assert(queue.size == 4);

    assert(queue_dequeue_first(&queue, message, sizeof(message), &priority));
    assert(priority == 100 && strcmp(message, "high-first") == 0);
    assert(queue_dequeue_priority(&queue, 50, message, sizeof(message)));
    assert(strcmp(message, "middle") == 0);
    assert(queue_dequeue_at_least(&queue, 80, message, sizeof(message), &priority));
    assert(priority == 100 && strcmp(message, "high-second") == 0);
    assert(queue_dequeue_first(&queue, message, sizeof(message), &priority));
    assert(priority == 20 && strcmp(message, "low") == 0);
    assert(!queue_dequeue_first(&queue, message, sizeof(message), &priority));
    assert(queue.size == 0);

    srand(42);
    for (int i = 0; i < 8; ++i) {
        char generated[MESSAGE_TEXT];
        unsigned int generated_priority = (unsigned int)(rand() % 256);
        snprintf(generated, sizeof(generated), "Сообщение №%d", i + 1);
        assert(queue_enqueue(&queue, generated, generated_priority));
    }
    assert(queue_enqueue(&queue, "Контрольное сообщение", 128));
    puts("\nСгенерированная очередь:");
    queue_print(&queue);

    assert(queue_dequeue_first(&queue, message, sizeof(message), &priority));
    printf("\nИзвлечение первого: [%u] %s\n", priority, message);
    assert(queue_dequeue_priority(&queue, 128, message, sizeof(message)));
    printf("Извлечение с приоритетом 128: %s\n", message);
    assert(queue_dequeue_at_least(&queue, 0, message, sizeof(message), &priority));
    printf("Извлечение с приоритетом не ниже 0: [%u] %s\n", priority, message);

    queue_free(&queue);
    puts("\nAll queue tests passed!");
    return 0;
}
