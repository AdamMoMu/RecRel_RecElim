#include "header.hpp"

int factorial(int n)
{
    if (n == 0 || n - 1 == 0)
        return 1;
    return n * factorial(n - 1);
}


int process_top(int n) {
    return factorial(n);
}