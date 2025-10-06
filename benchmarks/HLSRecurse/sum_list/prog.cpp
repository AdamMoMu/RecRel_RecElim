#include "header.hpp"
#include "../include/hls_recurse/state_machine_self_recursion.hpp"
using namespace hls_recurse;

int sum_list(int *arr, int n) {
    int temp;
    return run_function_old<int>(
        IfElse([&](){ return n <= 1; },
            Return([&](){ return arr[0]; }),
            Sequence(
                RecurseWithResult(temp, [&](){ return make_hls_state_tuple(arr, n - 1); }),
                Return([&](){ return arr[n-1] + temp; })
            )
        ),
        arr, n
    );
}


int process_top(int* arr, int n) {
    #pragma HLS INTERFACE ap_memory port=arr depth=16
    return sum_list(arr, n);
}
