#include <stdio.h>
#include "calc.h"

int main(void) {
    Calculator calculator;
    calculator_init(&calculator);

    if (!calculator_add_operation(&calculator, "Сложение", "+", calc_add) ||
        !calculator_add_operation(&calculator, "Вычитание", "-", calc_sub) ||
        !calculator_add_operation(&calculator, "Умножение", "*", calc_mul) ||
        !calculator_add_operation(&calculator, "Деление", "/", calc_div)) {
        puts("Не удалось создать список операций.");
        calculator_free(&calculator);
        return 1;
    }

    int choice;
    do {
        puts("\nОперации:");
        for (int i = 0; i < calculator.size; i++)
            printf("%d. %s (%s)\n", i + 1, calculator.items[i].name,
                   calculator.items[i].symbol);
        printf("0. Выход\nВыбор: ");

        if (scanf("%d", &choice) != 1) break;
        if (choice == 0) break;
        if (choice < 1 || choice > calculator.size) {
            puts("Неизвестная операция.");
            continue;
        }

        double a, b, result;
        printf("Введите два числа: ");
        if (scanf("%lf %lf", &a, &b) != 2) {
            puts("Ошибка ввода.");
            calculator_free(&calculator);
            return 1;
        }

        Operation *op = &calculator.items[choice - 1];
        if (op->function(a, b, &result))
            printf("Результат: %.10g\n", result);
        else
            puts("Ошибка: деление на ноль.");
    } while (choice != 0);

    calculator_free(&calculator);
    return 0;
}
