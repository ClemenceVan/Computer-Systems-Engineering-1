#include "complex.h"

// 2
complex conjugate(complex z) {
    return (complex){
        z.real,
        -z.imag
    };
}

// 3
complex add(complex z1, complex z2) {
    return (complex){
        z1.real + z2.real,
        z1.imag + z2.imag
    };
}

complex subtract(complex z1, complex z2) {
    return (complex){
        z1.real - z2.real,
        z1.imag - z2.imag
    };
}

complex multiply(complex z1, complex z2) {
    return (complex){
        z1.real * z2.real - z1.imag * z2.imag,
        z1.real * z2.imag + z1.imag * z2.real
    };
}

complex divide(complex z1, complex z2) {
    double denominator = z2.real * z2.real + z2.imag * z2.imag;
    return (complex){
        (z1.real * z2.real + z1.imag * z2.imag) / denominator,
        (z1.imag * z2.real - z1.real * z2.imag) / denominator
    };
}

// 4
complex reciprocal(complex z) {
    double denominator = z.real * z.real + z.imag * z.imag;
    return (complex){
        z.real / denominator,
        -z.imag / denominator
    };
}

// 5
double magnitude(complex z) {
    return sqrt(z.real * z.real + z.imag * z.imag);
}

// 6
// int main() {
//     complex z1, z2;

//     // Read two complex numbers from the user
//     printf("Enter the real and imaginary parts of z1 (e.g., 3 4): ");
//     scanf("%lf %lf", &z1.real, &z1.imag);

//     printf("Enter the real and imaginary parts of z2 (e.g., 1 -2): ");
//     scanf("%lf %lf", &z2.real, &z2.imag);

//     printf("You entered z1 = %.2lf + %.2lf and z2 = %.2f + %.2lfi\n", z1.real, z1.imag, z2.real, z2.imag);

//     complex sum = add(z1, z2);
//     printf("Sum (z1 + z2): %.2f + %.2lfi\n", sum.real, sum.imag);

//     complex diff = subtract(z1, z2);
//     printf("Difference (z1 - z2): %.2f + %.2lfi\n", diff.real, diff.imag);

//     complex product = multiply(z1, z2);
//     printf("Product (z1 * z2): %.2f + %.2lfi\n", product.real, product.imag);

//     complex quotient = divide(z1, z2);
//     printf("Quotient (z1 / z2): %.2f + %.2lfi\n", quotient.real, quotient.imag);

//     double mag = magnitude(z1);
//     printf("Magnitude of z1: %.2f\n", mag);
    
//     complex rec = reciprocal(z1);
//     printf("Reciprocal of z1: %.2f + %.2lfi\n", rec.real, rec.imag);

//     return 0;
// }