#include "header.hpp"


int sum_list(int* arr, int n) {
    if (n <= 1) return arr[0];
    return arr[n-1] + sum_list(arr, n - 1);
}


int process_top(int* arr, int n) {
    #pragma HLS INTERFACE ap_memory port=arr depth=16
    return sum_list(arr, n);
}
