// 1
#include "complex.h"

// 2
complex conjugate(complex z) {
    return (complex) {
        z.real,
        -z.imag
    };
}

// 3
complex add(complex z1, complex z2) {
    return (complex) {
        z1.real + z2.real,
        z1.imag + z2.imag
    };
}

complex subtract(complex z1, complex z2) {
    return (complex) {
        z1.real - z2.real,
        z1.imag - z2.imag
    };
}

complex multiply(complex z1, complex z2) {
    return (complex) {
        z1.real * z2.real - z1.imag * z2.imag,
        z1.real * z2.imag + z1.imag * z2.real
    };
}

complex divide(complex z1, complex z2) {
    double denominator = z2.real * z2.real + z2.imag * z2.imag;
    return (complex) {
        (z1.real * z2.real + z1.imag * z2.imag) / denominator,
        (z1.imag * z2.real - z1.real * z2.imag) / denominator
    };
}

// 4
complex reciprocal(complex z) {
    double denominator = z.real * z.real + z.imag * z.imag;
    return (complex) {
        z.real / denominator,
        -z.imag / denominator
    };
}

// 5
double magnitude(complex z) {
    return sqrt(z.real * z.real + z.imag * z.imag);
}