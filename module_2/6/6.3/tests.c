#include "calc.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    const char *directory = argc > 1 ? argv[1] : "./plugins";
    Calculator calculator;
    calculator_init(&calculator);

    int loaded = calculator_load_plugins(&calculator, directory);
    assert(loaded == 4);
    assert(calculator.size == 4);

    double result = 0.0;
    int found_add = 0, found_sub = 0, found_mul = 0, found_div = 0;
    for (size_t i = 0; i < calculator.size; ++i) {
        Operation *operation = &calculator.items[i];
        if (strcmp(operation->symbol, "+") == 0) {
            assert(operation->function(7.0, 5.0, &result) && result == 12.0);
            found_add = 1;
        } else if (strcmp(operation->symbol, "-") == 0) {
            assert(operation->function(7.0, 5.0, &result) && result == 2.0);
            found_sub = 1;
        } else if (strcmp(operation->symbol, "*") == 0) {
            assert(operation->function(7.0, 5.0, &result) && result == 35.0);
            found_mul = 1;
        } else if (strcmp(operation->symbol, "/") == 0) {
            assert(operation->function(8.0, 2.0, &result) && result == 4.0);
            assert(!operation->function(8.0, 0.0, &result));
            found_div = 1;
        }
    }

    assert(found_add && found_sub && found_mul && found_div);
    calculator_free(&calculator);
    assert(calculator.size == 0);
    puts("All dynamic plugin tests passed!");
    return 0;
}
