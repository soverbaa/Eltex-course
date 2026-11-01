#include "ipv4.h"

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

// преобразует строку IPv4-адреса в 32-битное число
int parse_ipv4(const char *text, uint32_t *address)
{
    if (text == NULL || address == NULL) {
        return 0;
    }

    uint32_t result = 0;
    int octet = 0;
    int digits = 0;

    for (const char *p = text; ; p++) {
        if (*p >= '0' && *p <= '9') {
            // добавляем цифру к октету
            octet = octet * 10 + (*p - '0'); //при добавлении цифры мы сдвигаем старое число на один разряд
            digits++;

            if (octet > 255 || digits > 3) {
                return 0;
            }
        } else if (*p == '.' || *p == '\0') {
            if (digits == 0) { // октет не может быть пустым
                return 0;
            }
            
            result = (result << 8) | (uint32_t)octet;// овобождаем 8 бит для нового октета сдвигом

            if (*p == '\0') {
                break;
            }

            octet = 0;
            digits = 0;
        } else {
            return 0;
        }
    }

    int dots = 0;

    for (const char *p = text; *p != '\0'; p++) { // проверяем, что адрес состоит ровно из четырёх октетов
        if (*p == '.') {
            dots++;
        }
    }

    if (dots != 3) {
        return 0;
    }

    *address = result;
    return 1;
}

int is_valid_mask(uint32_t mask)
{
    uint32_t inverted = ~mask;

    return (inverted & (inverted + 1U)) == 0; //идут ли единицы в маске непрерывно слева направо, а затем нули
}

int same_subnet(uint32_t address1, uint32_t address2,
                uint32_t mask)
{
    return (address1 & mask) == (address2 & mask);
}

int format_ipv4(uint32_t address, char *buffer, // преобразуем 32-битное число в строку IPv4-адреса
                size_t buffer_size)
{
    if (buffer == NULL || buffer_size < 16) {
        return 0;
    }

    unsigned int octet1 = (address >> 24) & 255U; //одно 32 битное число у нас делится на 4 октета (первый октет в конец)
    unsigned int octet2 = (address >> 16) & 255U; //второй в конец
    unsigned int octet3 = (address >> 8) & 255U;
    unsigned int octet4 = address & 255U;

    int written = snprintf(buffer, buffer_size,
                           "%u.%u.%u.%u",
                           octet1, octet2, octet3, octet4);

    return written >= 0 && (size_t)written < buffer_size;
}
