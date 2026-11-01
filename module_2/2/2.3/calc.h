#ifndef CALC_H
#define CALC_H

typedef int (*CalcFunc)(double, double, double *); // тип указателя на функцию

typedef struct {
    char name[20];
    char symbol[8];
    CalcFunc function; //указатель на функцию, которая выполняет эту операцию
} Operation;

typedef struct {
    Operation *items; //указатель на массив операций
    int size; // сколько операций хранится
    int capacity;
} Calculator;

void calculator_init(Calculator *c);
void calculator_free(Calculator *c);

int calculator_add_operation(
    Calculator *c, 
    const char *name,        
    const char *symbol, 
    CalcFunc function
);

int calc_add(double a, double b, double *result);
int calc_sub(double a, double b, double *result);
int calc_mul(double a, double b, double *result);
int calc_div(double a, double b, double *result);

#endif
