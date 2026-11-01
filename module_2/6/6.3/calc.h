#ifndef CALC_H
#define CALC_H

#include <stddef.h>

typedef int (*CalcFunc)(double, double, double *);

typedef struct {
    char name[32];
    char symbol[8];
    CalcFunc function;
    void *library_handle;
} Operation;

typedef struct {
    Operation *items;
    size_t size;
    size_t capacity;
} Calculator;

void calculator_init(Calculator *calculator);
void calculator_free(Calculator *calculator);
int calculator_load_plugins(Calculator *calculator, const char *directory);
int calculator_add_operation(Calculator *calculator, const char *name,
                             const char *symbol, CalcFunc function,
                             void *library_handle);

#endif
