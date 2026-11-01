#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "calc.h"

int main(void) {
    double r;
    assert(calc_add(2, 3, &r) && r == 5);
    assert(calc_sub(7, 2, &r) && r == 5);
    assert(calc_mul(3, 4, &r) && r == 12);
    assert(calc_div(8, 2, &r) && r == 4);
    assert(!calc_div(8, 0, &r));
    puts("All tests passed!");
    return 0;
}
