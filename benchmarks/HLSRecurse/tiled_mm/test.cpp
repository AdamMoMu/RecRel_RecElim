
#include "header.hpp"
#include <iostream>

int main() {
    int n = 4;
    float a[n * n], b[n * n], dst[n * n];
    for (int i = 0; i <= n*n; i++) {
        a[i] = i;
        b[i] = i;
        dst[i] = 0;
    }

    process_top(n, n, dst, a, b);
	
    for (int i = 0; i < n*n; i++) {
        std::cout << dst[i] << " ";
        if ((i+1) % n == 0) {
            std::cout << std::endl;
        }
    }

    return 0;
}