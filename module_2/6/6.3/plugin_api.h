#ifndef PLUGIN_API_H
#define PLUGIN_API_H

/* Каждая динамическая библиотека экспортирует одну функцию с таким интерфейсом. */
int calc_operation(double a, double b, double *result);

#endif
