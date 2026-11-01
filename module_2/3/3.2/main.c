#include "ipv4.h"

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static uint32_t generate_ip(void)
{
    uint32_t address = 0;

    for (int i = 0; i < 4; i++) {
        address = (address << 8) |
                  (uint32_t)(rand() % 256);
    }

    return address;
}

int main(int argc, char *argv[])
{
    if (argc != 4) {
        fprintf(stderr,
                "Использование: %s <IP шлюза> <маска> <N>\n",
                argv[0]);
        return 1;
    }

    uint32_t gateway;
    uint32_t mask;

    if (!parse_ipv4(argv[1], &gateway)) {
        fprintf(stderr, "Некорректный IP-адрес шлюза.\n");
        return 1;
    }

    if (!parse_ipv4(argv[2], &mask) ||
        !is_valid_mask(mask)) {
        fprintf(stderr, "Некорректная маска подсети.\n");
        return 1;
    }

    errno = 0;
    char *end = NULL;
    unsigned long long count = strtoull(argv[3], &end, 10);

    if (errno != 0 || end == argv[3] ||
        *end != '\0' || count == 0) {
        fprintf(stderr,
                "Количество пакетов N должно быть положительным числом.\n");
        return 1;
    }

    srand((unsigned int)time(NULL));

    unsigned long long local_count = 0;
    unsigned long long external_count = 0;

    char ip_text[16];

    printf("Шлюз: %s\n", argv[1]);
    printf("Маска: %s\n", argv[2]);
    printf("Количество пакетов: %llu\n\n", count);

    for (unsigned long long i = 0; i < count; i++) {
        uint32_t destination = generate_ip();

        if (!format_ipv4(destination, ip_text,
                         sizeof(ip_text))) {
            fprintf(stderr, "Ошибка преобразования IP.\n");
            return 1;
        }

        if (same_subnet(gateway, destination, mask)) {
            local_count++;
            printf("Пакет %llu: %-15s — своя подсеть\n",
                   i + 1, ip_text);
        } else {
            external_count++;
            printf("Пакет %llu: %-15s — другая сеть\n",
                   i + 1, ip_text);
        }
    }

    double local_percent =
        100.0 * (double)local_count / (double)count;
    double external_percent =
        100.0 * (double)external_count / (double)count;

    printf("\n=== Статистика ===\n");
    printf("Своя подсеть:  %llu пакетов (%.2f%%)\n",
           local_count, local_percent);
    printf("Другие сети:   %llu пакетов (%.2f%%)\n",
           external_count, external_percent);
    printf("Всего:         %llu пакетов (100.00%%)\n",
           count);

    return 0;
}
