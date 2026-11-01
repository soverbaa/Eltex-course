#include "../plugin_api.h"
int calc_operation(double a, double b, double *result)
{
    if (result == 0) return 0;
    *result = a * b;
    return 1;
}
