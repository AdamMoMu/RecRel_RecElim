#include "header.hpp"

int fib(int n, int m) {
    if (n <= 1 || m <= 1) {
        return 1;
    }

    return fib(n /2, m-1) + fib(n / 4, m-2);
}

int process_top(int n, int m) {
    return fib(n, m);
}