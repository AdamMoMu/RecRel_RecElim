#include "header.hpp"

#include "../include/hls_recurse/state_machine_self_recursion.hpp"
using namespace hls_recurse;

void heap_sum(int *f, int start, int end)
{
    run_function_old<void>(
        IfElse([&](){ return end - start <= 1; },
            Return(),
            Sequence(
                Recurse([&](){ return make_hls_state_tuple(f, start, start + (end-start) / 2); }),
                Recurse([&](){ return make_hls_state_tuple(f, start + (end-start) / 2, end); }),
                [&](){  f[start] += f[start + (end-start) / 2];  }
            )
        ),
        f, start, end
    );
}


int process_top(int *arr, int n) 
{
    #pragma HLS INTERFACE ap_memory port=arr depth=16
    heap_sum(arr, 0, n);
    return 0;
}