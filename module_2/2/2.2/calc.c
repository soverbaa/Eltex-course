#include "calc.h"

int calc_add(double a, double b, double *result) {
    *result = a + b;
    return 1;
}

int calc_sub(double a, double b, double *result) {
    *result = a - b;
    return 1;
}

int calc_mul(double a, double b, double *result) {
    *result = a * b;
    return 1;
}

int calc_div(double a, double b, double *result) {
    if (b == 0) return 0;
    *result = a / b;
    return 1;
}
