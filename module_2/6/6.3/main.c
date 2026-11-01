#include "calc.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    const char *plugin_directory = argc > 1 ? argv[1] : "./plugins";
    Calculator calculator;
    calculator_init(&calculator);

    printf("Загрузка библиотек из каталога: %s\n", plugin_directory);
    int loaded = calculator_load_plugins(&calculator, plugin_directory);
    if (loaded == 0) {
        puts("Не найдено ни одной подходящей операции.");
        calculator_free(&calculator);
        return 1;
    }

    int choice = -1;
    while (choice != 0) {
        puts("\nОперации:");
        for (size_t i = 0; i < calculator.size; ++i) {
            printf("%zu. %s (%s)\n", i + 1, calculator.items[i].name,
                   calculator.items[i].symbol);
        }
        printf("0. Выход\nВыбор: ");

        if (scanf("%d", &choice) != 1) break;
        if (choice == 0) break;
        if (choice < 1 || (size_t)choice > calculator.size) {
            puts("Неизвестная операция.");
            continue;
        }

        double a, b, result;
        printf("Введите два числа: ");
        if (scanf("%lf %lf", &a, &b) != 2) {
            puts("Ошибка ввода.");
            break;
        }

        Operation *operation = &calculator.items[(size_t)choice - 1];
        if (operation->function(a, b, &result)) {
            printf("Результат: %.10g\n", result);
        } else {
            puts("Операция не может быть выполнена (например, деление на ноль).");
        }
    }

    calculator_free(&calculator);
    return 0;
}
