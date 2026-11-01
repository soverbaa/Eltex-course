#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void calculator_init(Calculator *c) {
    c->items = NULL;
    c->size = 0;
    c->capacity = 0;
}

void calculator_free(Calculator *c) {
    free(c->items);
    calculator_init(c);
}

int calculator_add_operation(Calculator *c, const char *name,
                             const char *symbol, CalcFunc function) {
    if (c->size == c->capacity) {
        int capacity = c->capacity ? c->capacity * 2 : 4;
        Operation *p = realloc(c->items, (size_t)capacity * sizeof(*p));
        if (!p) return 0;
        c->items = p;
        c->capacity = capacity;
    }

    Operation *op = &c->items[c->size++];
    snprintf(op->name, sizeof(op->name), "%s", name);
    snprintf(op->symbol, sizeof(op->symbol), "%s", symbol);
    op->function = function;
    return 1;
}
