#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int read_line(const char *prompt, char *buffer, size_t size)
{
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) return 0;
    size_t length = strcspn(buffer, "\n");
    if (buffer[length] == '\n') buffer[length] = '\0';
    else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}
    }
    return 1;
}

static int read_priority(unsigned int *priority)
{
    char line[32], *end = NULL;
    if (!read_line("Приоритет (0-255): ", line, sizeof(line))) return 0;
    long value = strtol(line, &end, 10);
    if (end == line || *end != '\0' || value < 0 || value > 255) return 0;
    *priority = (unsigned int)value;
    return 1;
}

int main(void)
{
    PriorityQueue queue;
    queue_init(&queue);
    char message[MESSAGE_TEXT];
    int choice = -1;

    while (choice != 0) {
        puts("\nОчередь с приоритетом");
        puts("1. Добавить сообщение");
        puts("2. Извлечь первое сообщение");
        puts("3. Извлечь сообщение с заданным приоритетом");
        puts("4. Извлечь сообщение с приоритетом не ниже заданного");
        puts("5. Показать очередь");
        puts("0. Выход");
        printf("Выбор: ");

        char line[32], *end = NULL;
        if (!fgets(line, sizeof(line), stdin)) break;
        long parsed = strtol(line, &end, 10);
        if (end == line || (*end != '\n' && *end != '\0') || parsed < 0 || parsed > 5) {
            puts("Некорректный пункт меню.");
            continue;
        }
        choice = (int)parsed;
        if (choice == 0) break;

        unsigned int priority = 0;
        int success = 0;
        switch (choice) {
            case 1:
                if (!read_line("Текст сообщения: ", message, sizeof(message)) || message[0] == '\0') {
                    puts("Сообщение не должно быть пустым.");
                    break;
                }
                if (!read_priority(&priority)) {
                    puts("Приоритет должен быть целым числом от 0 до 255.");
                    break;
                }
                puts(queue_enqueue(&queue, message, priority) ? "Сообщение добавлено." : "Не удалось добавить сообщение.");
                break;
            case 2:
                success = queue_dequeue_first(&queue, message, sizeof(message), &priority);
                if (success) printf("Извлечено [приоритет %u]: %s\n", priority, message);
                else puts("Очередь пуста.");
                break;
            case 3:
                if (!read_priority(&priority)) { puts("Некорректный приоритет."); break; }
                success = queue_dequeue_priority(&queue, priority, message, sizeof(message));
                if (success) printf("Извлечено [приоритет %u]: %s\n", priority, message);
                else puts("Сообщений с таким приоритетом нет.");
                break;
            case 4: {
                unsigned int min_priority;
                if (!read_priority(&min_priority)) { puts("Некорректный приоритет."); break; }
                success = queue_dequeue_at_least(&queue, min_priority, message, sizeof(message), &priority);
                if (success) printf("Извлечено [приоритет %u]: %s\n", priority, message);
                else puts("Сообщений с таким или более высоким приоритетом нет.");
                break;
            }
            case 5:
                queue_print(&queue);
                break;
            default:
                break;
        }
    }

    queue_free(&queue);
    return 0;
}
