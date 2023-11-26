#include <math.h>
#include <stdio.h>

// 1
typedef struct complex_t {
    double real;
    double imag;
} complex;

// 2
complex conjugate(complex z);

// 3
complex add(complex z1, complex z2);

complex subtract(complex z1, complex z2);

complex multiply(complex z1, complex z2);

complex divide(complex z1, complex z2);

// 4
complex reciprocal(complex z);

// 5
double magnitude(complex z);