#ifndef PERMISSIONS_H
#define PERMISSIONS_H

#include <sys/types.h>
#include <sys/stat.h>

// Преобразование прав из цифрового или буквенного формата
int parse_permissions(const char *input, mode_t *permissions);

// Вывод прав в трех форматах
void print_permissions(mode_t permissions);

// Применение символьной команды, аналогичной chmod
int modify_permissions(mode_t *permissions, const char *command);

#endif
