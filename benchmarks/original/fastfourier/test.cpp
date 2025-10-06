
#include "header.hpp"
#include <iostream>

#include <complex>
#include <cmath>
#include <vector>

using complex_t = std::complex<float>;

// Example driver
int main() {
    int log2n = 3;               // 2^3 = 8 points
    int n = 1 << log2n;

    // Prepare input and output buffers
    std::vector<complex_t> input(n), output(n);

    // Fill input with sample data (e.g., real ramp)
    for (int i = 0; i < n; ++i) {
        input[i] = complex_t(static_cast<float>(i), 0.0f);
    }

    // Run FFT
    process_top(log2n, input.data(), output.data());
    // Or for HLS: process_top(log2n, input.data(), output.data());

    // Print results
    std::cout << "FFT output (real, imag):\n";
    for (int i = 0; i < n; ++i) {
        std::cout << "[" << i << "] = ("
                  << output[i].real() << ", "
                  << output[i].imag() << ")\n";
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