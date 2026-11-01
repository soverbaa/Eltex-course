#include "calc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

void calculator_init(Calculator *calculator)
{
    if (calculator == NULL) return;
    calculator->items = NULL;
    calculator->size = 0;
    calculator->capacity = 0;
}

void calculator_free(Calculator *calculator)
{
    if (calculator == NULL) return;
    for (size_t i = 0; i < calculator->size; ++i) {
        if (calculator->items[i].library_handle != NULL) {
            /* Библиотека закрывается только после прекращения вызовов её функции. */
            (void)dlclose(calculator->items[i].library_handle);
        }
    }
    free(calculator->items);
    calculator_init(calculator);
}

int calculator_add_operation(Calculator *calculator, const char *name,
                             const char *symbol, CalcFunc function,
                             void *library_handle)
{
    if (calculator == NULL || name == NULL || symbol == NULL || function == NULL) return 0;

    if (calculator->size == calculator->capacity) {
        size_t new_capacity = calculator->capacity == 0 ? 4 : calculator->capacity * 2;
        Operation *new_items = realloc(calculator->items, new_capacity * sizeof(*new_items));
        if (new_items == NULL) return 0;
        calculator->items = new_items;
        calculator->capacity = new_capacity;
    }

    Operation *operation = &calculator->items[calculator->size];
    snprintf(operation->name, sizeof(operation->name), "%s", name);
    snprintf(operation->symbol, sizeof(operation->symbol), "%s", symbol);
    operation->function = function;
    operation->library_handle = library_handle;
    calculator->size++;
    return 1;
}
