#include <stdio.h>
#include "calc.h"

int main(void) {
    int choice;
    double a, b, result;

    do {
        printf("\n1. Сложение\n2. Вычитание\n3. Умножение\n4. Деление\n0. Выход\nВыбор: ");
        if (scanf("%d", &choice) != 1) return 1;
        if (choice == 0) break;
        if (choice < 1 || choice > 4) {
            puts("Неизвестная операция.");
            continue;
        }

        printf("Введите два числа: ");
        if (scanf("%lf %lf", &a, &b) != 2) {
            puts("Ошибка ввода.");
            return 1;
        }

        int ok = 0;
        switch (choice) {
            case 1: ok = calc_add(a, b, &result); break;
            case 2: ok = calc_sub(a, b, &result); break;
            case 3: ok = calc_mul(a, b, &result); break;
            case 4: ok = calc_div(a, b, &result); break;
        }

        if (ok) printf("Результат: %.10g\n", result);
        else puts("Ошибка: деление на ноль.");
    } while (choice != 0);

    return 0;
}
