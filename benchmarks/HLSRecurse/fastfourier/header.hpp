

#ifndef HEADER
#define HEADER

#include <stdio.h>

#include "../include/hls_recurse/state_machine_self_recursion.hpp"
#include "../include/hls_recurse/complex.hpp"

// Simple FFT implementation without hls_recurse

int process_top(int log2n, const complex_t* pIn, complex_t* pOut);

#endif
