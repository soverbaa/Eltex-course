#include "calc.h"

#include <dirent.h>
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>

static int has_library_extension(const char *filename)
{
    size_t length = strlen(filename);
#ifdef __APPLE__
    return length > 6 && strcmp(filename + length - 6, ".dylib") == 0;
#else
    return length > 3 && strcmp(filename + length - 3, ".so") == 0;
#endif
}

static void operation_details(const char *filename, char *name, size_t name_size,
                              char *symbol, size_t symbol_size)
{
    if (strstr(filename, "add") != NULL) {
        snprintf(name, name_size, "Сложение");
        snprintf(symbol, symbol_size, "+");
    } else if (strstr(filename, "sub") != NULL) {
        snprintf(name, name_size, "Вычитание");
        snprintf(symbol, symbol_size, "-");
    } else if (strstr(filename, "mul") != NULL) {
        snprintf(name, name_size, "Умножение");
        snprintf(symbol, symbol_size, "*");
    } else if (strstr(filename, "div") != NULL) {
        snprintf(name, name_size, "Деление");
        snprintf(symbol, symbol_size, "/");
    } else {
        snprintf(name, name_size, "%s", filename);
        snprintf(symbol, symbol_size, "?");
    }
}

int calculator_load_plugins(Calculator *calculator, const char *directory)
{
    if (calculator == NULL || directory == NULL) return 0;

    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("Не удалось открыть каталог библиотек");
        return 0;
    }

    int loaded = 0;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (!has_library_extension(entry->d_name)) continue;

        char path[512];
        int written = snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
        if (written < 0 || (size_t)written >= sizeof(path)) {
            fprintf(stderr, "Слишком длинный путь: %s\n", entry->d_name);
            continue;
        }

        void *handle = dlopen(path, RTLD_NOW);
        if (handle == NULL) {
            fprintf(stderr, "Не удалось загрузить %s: %s\n", path, dlerror());
            continue;
        }

        dlerror();
        void *address = dlsym(handle, "calc_operation");
        const char *error = dlerror();
        if (error != NULL || address == NULL) {
            fprintf(stderr, "В %s нет функции calc_operation: %s\n", path,
                    error != NULL ? error : "символ не найден");
            dlclose(handle);
            continue;
        }

        CalcFunc function = NULL;
        /* POSIX гарантирует возможность получить адрес функции через dlsym.
           memcpy позволяет избежать непереносимого прямого приведения типов в C. */
        memcpy(&function, &address, sizeof(function));

        char name[32], symbol[8];
        operation_details(entry->d_name, name, sizeof(name), symbol, sizeof(symbol));
        if (!calculator_add_operation(calculator, name, symbol, function, handle)) {
            fprintf(stderr, "Не удалось добавить операцию из %s\n", path);
            dlclose(handle);
            continue;
        }
        printf("Загружена операция: %s (%s)\n", name, symbol);
        loaded++;
    }

    closedir(dir);
    return loaded;
}
