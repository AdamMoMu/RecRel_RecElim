#include "header.hpp"


int fib(int n) {
    if (n <= 1) return 1;
    return fib(n - 2) + fib(n - 1);
}


int process_top(int n) {
    return fib(n);
}
