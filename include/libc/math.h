#ifndef LIBC_MATH_H
#define LIBC_MATH_H

// Types

typedef float float_t;
typedef double double_t;

// Constants

#define HUGE_VALF (__builtin_huge_valf())
#define HUGE_VAL (__builtin_huge_val())
#define HUGE_VALL (__builtin_huge_vall())
#define INFINITY (__builtin_inff())
#define NAN (__builtin_nanf(""))

#define FP_NAN 0
#define FP_INFINITE 1
#define FP_ZERO 2
#define FP_SUBNORMAL 3
#define FP_NORMAL 4

#define M_PI 3.14159265358979323846
#define M_PIf 3.14159265358979323846f

// Functions

#define fpclassify(x) __builtin_fpclassify(FP_NAN, FP_INFINITE, FP_NORMAL, FP_SUBNORMAL, FP_ZERO, x)
#define fabsf(f) __builtin_fabsf((float)(f))

#endif
