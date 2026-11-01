#include <stdio.h>
#include <assert.h>
#include "calc.h"

int main(void) {
    Calculator c;
    calculator_init(&c);
    assert(calculator_add_operation(&c, "Сложение", "+", calc_add));
    assert(calculator_add_operation(&c, "Деление", "/", calc_div));
    assert(c.size == 2);

    double result;
    assert(c.items[0].function(2, 3, &result) && result == 5);
    assert(!c.items[1].function(2, 0, &result));

    calculator_free(&c);
    assert(c.size == 0);
    puts("All tests passed!");
    return 0;
}
