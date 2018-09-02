# physics\_h

Header files for some common (and not so common) linear algebra operations.

## `fft.h`

An FFT implementation in C++, using complex numbers and trigonometric functions.

The function `fft` takes an `std::vector` of `std::complex<long double>` and returns the coefficients of the complex representation of the covolution.

Pass `inv = 1` to preform the covolution and `inv = -1` to preform an inverse FFT.

## `matrix.h`

Contains a matrix class, which defines:

- Addition
- Subtraction
- Multiplication
- Inverse
- Determinant
- Transpose

## `gauss.h`

Solves a systems of linear equations.

## `vector.h`

Contains a vector class, which defines:

- Addition
- Subtraction
- Mulitplication
- Cross Product
- Dot Product
