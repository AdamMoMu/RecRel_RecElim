

#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <complex>
#include <cmath>

// Simple FFT implementation without hls_recurse
using complex_t = std::complex<float>;

int process_top(int log2n, const complex_t* pIn, complex_t* pOut);

#endif
