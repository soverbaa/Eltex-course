#include "permissions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PERMISSION_MASK 0777

// проверяет и преобразует цифровую или буквенную запись прав
int parse_permissions(const char *input, mode_t *permissions)
{
    if (input == NULL || permissions == NULL || input[0] == '\0') {
        return 0;
    }

    // обработка буквенной записи (rwxr-xr--)
    if (strlen(input) == 9) {
        const char expected[3] = {'r', 'w', 'x'};
        mode_t result = 0;

        for (int i = 0; i < 9; i++) {
            char expected_char = expected[i % 3]; //ожидаемые символы повторяются, 0, 1 или 2 (r, w, x)

            if (input[i] == expected_char) { // символ совпал - устанавливаем бит в result
                result |= (mode_t)1 << (8 - i);
            } else if (input[i] != '-') {
                return 0;
            }
        }

        *permissions = result;
        return 1;
    }

    // цифровая запись (755 и тд)
    if (strlen(input) < 3 || strlen(input) > 4) {
        return 0;
    }

    for (size_t i = 0; input[i] != '\0'; i++) {
        if (input[i] < '0' || input[i] > '7') {
            return 0;
        }
    }

    char *end = NULL; // проверка на то что это число
    unsigned long value = strtoul(input, &end, 8); // преобразуем строку в число (используя основание 8)

    if (*end != '\0' || value > PERMISSION_MASK) { // проверка что строка преобразовна полностью 
        return 0;
    }

    *permissions = (mode_t)value;
    return 1;
}

void print_permissions(mode_t permissions)
{
    permissions &= PERMISSION_MASK;

    const char symbols[] = {'r', 'w', 'x'}; 
    char letters[10]; //для буквенного представления
    char bits[10];

    for (int i = 0; i < 9; i++) {
        int bit_is_set = (permissions >> (8 - i)) & 1;//Сдвигаем нужный бит на младшую позицию и получаем его значение с помощью побитового И

        bits[i] = bit_is_set ? '1' : '0'; 
        letters[i] = bit_is_set ? symbols[i % 3] : '-';
    }

    letters[9] = '\0';
    bits[9] = '\0';

    printf("Буквенное представление: %s\n", letters);
    printf("Цифровое представление: %03o\n",
           (unsigned int)permissions);
    printf("Битовое представление:  %s\n", bits);
}

// применяет одну команду к правам
static int apply_single_command(mode_t *permissions,
                                const char *command)
{
    const char *p = command;
    int who = 0;
    int selected = 0;

    // определяем субъекты: u, g, o, a
    while (*p != '\0' &&
           (*p == 'u' || *p == 'g' ||
            *p == 'o' || *p == 'a')) {
        switch (*p) {
            case 'u':
                who |= 4;
                break;
            case 'g':
                who |= 2;
                break;
            case 'o':
                who |= 1;
                break;
            case 'a':
                who |= 7;
                break;
        }

        selected = 1;
        p++;
    }

    //енсли субъект не указан, считаем, что выбраны все
    if (!selected) {
        who = 7;
    }

    char operation = *p;

    if (operation != '+' && operation != '-' &&
        operation != '=') {
        return 0;
    }

    p++;

    // для операций '+' и '-' обязательно должно быть указано право. операция = может быть без прав, например g=
    if (*p == '\0' && operation != '=') {
        return 0;
    }

    mode_t mask = 0;

    // формируем маску прав r, w, x
    while (*p != '\0') {
        mode_t permission_bit;

        switch (*p) {
            case 'r':
                permission_bit = 4;
                break;
            case 'w':
                permission_bit = 2;
                break;
            case 'x':
                permission_bit = 1;
                break;
            default:
                return 0;
        }

        if (who & 4) { // если выбрали владельца, переносим право в старшие три бита: позиции 6–8.
            mask |= permission_bit << 6;
        }
        if (who & 2) { //группа - средние три бита(3-5)
            mask |= permission_bit << 3;
        }
        if (who & 1) { //остальные младшие три бита (0-2)
            mask |= permission_bit;
        }

        p++;
    }

    // для операции '=' также очищаем выбранные разряды
    mode_t affected = 0;

    if (who & 4) {
        affected |= 0700;
    }
    if (who & 2) {
        affected |= 0070;
    }
    if (who & 1) {
        affected |= 0007;
    }

    switch (operation) {
        case '+':
            *permissions |= mask;
            break;
        case '-':
            *permissions &= ~mask;
            break;
        case '=':
            *permissions = (*permissions & ~affected) | mask;
            break;
    }

    *permissions &= PERMISSION_MASK;//оставляем только девять битов обычных прав доступа
    return 1;
}

// обрабатывает несколько команд через запятую
int modify_permissions(mode_t *permissions, const char *command)
{
    if (permissions == NULL || command == NULL ||
        command[0] == '\0') {
        return 0;
    }

    char *copy = malloc(strlen(command) + 1);

    if (copy == NULL) {
        return 0;
    }

    strcpy(copy, command);// копируем исходную строку в выделенную память

    mode_t result = *permissions;
    char *start = copy;
    char *comma;

    do {
        comma = strchr(start, ',');

        if (comma != NULL) {
            *comma = '\0';
        }

        if (!apply_single_command(&result, start)) { //применяем текущую команду к временным правам
            free(copy);
            return 0;
        }

        if (comma != NULL) {
            start = comma + 1;

            if (*start == '\0') {
                free(copy);
                return 0;
            }
        }
    } while (comma != NULL);

    free(copy);
    *permissions = result;
    return 1;
}
