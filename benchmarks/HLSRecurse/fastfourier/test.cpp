
#include "header.hpp"
#include <iostream>

#include <complex>
#include <cmath>
#include <vector>

// Example driver
int main() {
    int log2n = 3;               // 2^3 = 8 points
    int n = 1 << log2n;

    // Prepare input and output buffers
    complex_t in[n], out[n];

    // Fill input with sample data (e.g., real ramp)
    for(int i=0; i<512; i++){
      in[i]=complex_t::from_int(i,0);
    }

    // Run FFT
    process_top(log2n, &in[0], &out[0]);

    // Print results
    std::cout << "FFT output (real, imag):\n";
    for (int i = 0; i < n; ++i) {
        std::cout << "[" << i << "] = ("
                  << out[i].re_int() << ", "
                  << out[i].im_int() << ")\n";
    }

    return 0;
}


// Expected Output:
//
// FFT output (real, imag):
// [0] = (28, 0)
// [1] = (-4, 9.65685)
// [2] = (-4, 4)
// [3] = (-4, 1.65685)
// [4] = (-4, 0)
// [5] = (-4, -1.65685)
// [6] = (-4, -4)
// [7] = (-4, -9.65685)